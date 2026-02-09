//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       CAN-TP: target driver interface

   Interface to target specific CAN driver wrapper functions.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_CTP_TRG_IFH
#define OSY_CTP_TRG_IFH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_uds_globals.h"
#include "osy_ctp_trg.h" //we need the target specific initialization definition

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */
///container for one CAN message
typedef struct
{
   uint8 u8_Dlc;             ///< 0 .. 8
   uint8 u8_IdTypeXtd;       ///< 0 -> 11bit; 1 -> 29bit
   uint32 u32_Id;            ///< 11bit or 29bit CAN message ID
   uint8 au8_Data[8];        ///< message content
} T_osy_ctp_trg_can_message; ///< CAN message structure

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize communication channel

   Perform target specific communication initialization action.

   The function shall prepare the CAN driver for sending messages.
   All CAN messages are sent via this target layer.

   The function shall configure the following 29bit CAN identifier ranges to be received:

   Range 1:
   Range: 0x18DAxx00 to 0x18DAxx7F; xx is our node ID
   Purpose: "local" requests addressed to "us"

   Range 2:
   Range: 0x18DB7F00 to 0x18DB7F7F
   Purpose: "local" requests addressed to "all nodes"

   Range 3:
   Range: 0b1101111 0000 0000000 xxxx yyyyyyy  to  0b1101111 1111 1111111 xxxx yyyyyyy
      xxxx is our bus ID
      yyyyyyy is our node ID
   Purpose: "remote" requests addressed to "us"

   Range 4:
   Range: 0b1101111 0000 0000000 xxxx 1111111  to  0b1101111 1111 1111111 xxxx 1111111
      xxxx is our bus ID
   Purpose: "remote" requests addressed to "all nodes"

   Range 5:
   Range: 0b1101111 0000 0000000 0000 0000000  to 0b1101111 1111 1111111 1111 1111111
                                 xxxx                                    xxxx
   This range shall exclude the bus ID (xxxx) that matches our own bus ID
   Purpose: for routing CAN2CAN or CAN2ETH (only required if target supports routing)
   Reception for this range shall be preconfigured but not be activated (see function osy_ctp_trg_activate_routing)

   Notes on buffering:
   Transmission:
   * The openSYDE stack provides buffering of TX messages for all types of transfers
   * all CAN messages to send on one bus are buffered in a central TX FIFO
   ** routed ETH-TP  -> CAN-TP frames
   ** routed CAN-TP <-> CAN-TP frames
   ** CAN messages sent via the "SendCanMessage" service (e.g. tunneling of STW Flashloader protocol)
   * If the target layer send function "osy_ctp_trg_send_message()" reports a problem the messages stay in the TX FIFO
      until the next cycle
   * Even though the messages are buffered by the stack: adding target layer TX buffers will improve performance as
      multiple messages can be sent between calls to the cyclic stack function

   Reception:
   * The target layer must make sure that no messages get lost between calls to "osy_ctp_trg_receive_message()" resp.
      "osy_ctp_ro_trg_receive_message()".
   * If the cycle time is not fast enough the target layer needs to provide buffering mechanisms for all set up
      RX objects, including the object in the routing target layer (e.g. HW buffers, interrupt-driven SW FIFOs).
   * Notes:
   ** the "smallest" possible CAN message (11Bit-ID, DLC=0, no stuff bits) consists of 44bits
   *** this corresponds to 44us at 1MBit/s
   * the openSYDE stack can handle multiple CAN frames received between those calls
   ** in other words: when checking for new messages the stack reads all messages it can get (not just one)

   Notes on communication status:
   The openSYDE Server does not provide a generic mechanism to inform the application about low-level communication
    problems. If needed this information should be provided to the application by the target integration.
    See the openSYDE integration manual requirements for more information.

   Handling of opt_InitParameters:
   If the target integration requires information from this structure the function shall check whether the pointer
    is != NULL and return C_RANGE if it is.

   \param[in]   ou8_BusNumber          CAN bus number (0 = first supported bus, ...)
   \param[in]   opt_InitParameters     target specific initialization parameters (NULL if none passed by application)
   \param[in]   opt_OwnNodeIdentifier  our own node and bus identifier (to be used for filter calculations)

   \retval C_NO_ERR   initialization successful
   \retval C_RANGE    one of the parameters is out of range (or NULL)
   \retval C_NOACT    initialization failed
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_ctp_trg_init_communication(const uint8 ou8_BusNumber,
                                             const T_osy_ctp_trg_can_init_params * const opt_InitParameters,
                                             const T_osy_uds_node_identifier * const opt_OwnNodeIdentifier);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Read incoming CAN messages

   Return incoming CAN messages matching the sum of configured filters.
   Only CAN messages passing at least one filter may be reported.
   The function shall check all filters configured at initialization in the documented sequence and
    report the first incoming message.
   For details on buffering and performance requirements see the documentation of osy_ctp_trg_init().

   \param[in]   ou8_BusNumber  CAN bus number (0 = first supported bus, ...)
   \param[out]  opt_Message    read CAN message (caller guarantees valid pointer)

   \retval C_NO_ERR   message retrieved
   \retval C_RANGE    bus number is out of range
   \retval C_NOACT    no new message or error reading message
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_ctp_trg_retrieve_message(const uint8 ou8_BusNumber, T_osy_ctp_trg_can_message * const opt_Message);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Send one CAN message

   Send one CAN message on specified channel.
   The job of the function is to pass the CAN message to the system's CAN driver.
   It does not need to wait until the message was technically sent out on the bus.

   The function shall not block execution when writing to the CAN bus fails (e.g. due to high traffic)
   Specifically: If the function can not send a message due to full buffer it shall return with an error
    (and not block until it can send out the message)
   The openSYDE stack hat a TX buffer of its own; a message does not get lost if the target layer function reports a
    problem.
   For details on buffering see the documentation of osy_ctp_trg_init().

   Retransmission on bus conflicts:
   If multiple nodes send a message with the same CAN-ID but different payload at the same time only one of the messages
    will "win" and be sent on the bus. The "loser" will retry and its messages will be sent subsequently.
   The sender shall ensure such messages will be sent on the bus
    (sending shall not be aborted if collissions are detected).
   This behavior is relied on in the openSYDE Device Configuration procedure where multiple nodes have the same Node-ID
    and therefore send their serial numbers with the same CAN-ID.

   \param[in]   ou8_BusNumber  CAN bus number (0 = first supported bus, ...)
   \param[in]   opt_Message    CAN message to send (caller guarantees valid pointer)

   \retval C_NO_ERR   message successfully sent
   \retval C_RANGE    bus number is out of range
   \retval C_NOACT    could not send message
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_ctp_trg_send_message(const uint8 ou8_BusNumber, const T_osy_ctp_trg_can_message * const opt_Message);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Activate or deactivate reception of routing messages

   Configures the target driver to receive or block reception of CAN-TP routing messages.
   The implementation shall activate or deactivate reception of those messages
    (see last identifier range in osy_ctp_trg_init_communication()).
   The UDS stack will call this function whenever a routing is activated and after all routings are deactivated.
   So for multiple routings in parallel make sure the implementation can handle multiple "activate" calls.

   If the target does not support routing keep this function empty.

   \param[in]   ou8_BusNumber  CAN bus number (0 = first supported bus, ...);
   \param[in]   ou8_OnOff      0: deactivate reception
                               1: activate reception
*/
//----------------------------------------------------------------------------------------------------------------------
extern void osy_ctp_trg_activate_routing(const uint8 ou8_BusNumber, const uint8 ou8_OnOff);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
