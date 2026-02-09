//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to openSYDE J1939 DM2 module
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_J1939_DM2H
#define OSY_COM_J1939_DM2H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "osy_com_j1939_dm_generic.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// Error types
// Reported by pr_Dm2RxError
#define J1939_DM2_ERR_RX_TIMEOUT_INIT_RESPOND (0U) // Timeout detected after sending a DM2 request
#define J1939_DM2_ERR_RX_SEND_CTS             (1U) // Problem with sending a CTS message
#define J1939_DM2_ERR_RX_SEND_EACK            (2U) // Problem with sending the EACK message
#define J1939_DM2_ERR_RX_SEND_ABORT           (3U) // Problem with sending an abort message in RX process
// Reported by pr_Dm2TxError
#define J1939_DM2_ERR_TX_SEND_SINGLE_FRAME    (4U) // Problem with sending single frame message response
#define J1939_DM2_ERR_TX_SEND_RTS             (5U) // Problem with sending RTS response
#define J1939_DM2_ERR_TX_SEND_DATA            (6U) // Problem with sending a DT message
#define J1939_DM2_ERR_TX_SEND_ABORT           (7U) // Problem with sending an abort message in TX process

// Abort reasons
// When an abort message was sent, not additional error tpye is reported by the error callback
// Reported by pr_Dm2AbortTxEvent and pr_Dm2AbortRxEvent
#define J1939_CMDT_ABORT_RS_BUSY             (1U) // already one active connection on the channel
#define J1939_CMDT_ABORT_RS_NO_RESSOURCES    (2U) // needed system resource not available
#define J1939_CMDT_ABORT_RS_TIMEOUT          (3U) // timeout (any expected message was not received)
#define J1939_CMDT_ABORT_RS_UNEXPECTED_CTS   (4U) // CTS messages received when data transfer is in progress
#define J1939_CMDT_ABORT_RS_MAX_RETRANSMIT   (5U) // Maximum retransmit request limit reached
#define J1939_CMDT_ABORT_RS_UNEXPECTED_DT    (6U) // Unexpected data transfer packet
#define J1939_CMDT_ABORT_RS_WRONG_SEQ_NR     (7U) // Bad sequence number (is not the turn of the value)
#define J1939_CMDT_ABORT_RS_DUPLICATE_SEQ_NR (8U) // Duplicate sequence number (was already received)
#define J1939_CMDT_ABORT_RS_MSG_SIZE_TO_BIG  (9U) // "Total Message Size" is greater than 1785 bytes

// Abort message direction
#define J1939_CMDT_ABORT_MSG_RECEIVED     (0U) // The abort message was received by this node
#define J1939_CMDT_ABORT_MSG_SENT         (1U) // The abort message was sent by this node

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
// Generic callback types
typedef void (* PR_osy_com_j1939_dm2_abort_msg_event)(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                                      const uint8 ou8_DestinationAddress, const uint8 ou8_WasSent,
                                                      const uint8 ou8_AbortReason);

///Callback type initialization structure
typedef struct
{
   PR_osy_com_j1939_dm_rx_dtc_received pr_Dm2RxDtcReceived; // Callback when a DTC was received
   PR_osy_com_j1939_dm_error pr_Dm2RxError;                 // Error callback for the CMDT receiving process
   PR_osy_com_j1939_dm2_abort_msg_event pr_Dm2AbortRxEvent; // Callback if in the CMDT receiving process an abort
                                                            // message was sent or received
   PR_osy_com_j1939_dm_error pr_Dm2TxError;                 // Error callback for the CMDT transmission process
   PR_osy_com_j1939_dm2_abort_msg_event pr_Dm2AbortTxEvent; // Callback if in the CMDT transmission process an abort
                                                            // message was sent or received
} T_osy_com_j1939_dm2_callbacks;

///this is the parameter passed to the osy_com_j1939_dm2_init_tx function
typedef struct
{
   // Sending on one channel only for one device at the same time
   T_osy_com_j1939_dm_logic_node_tx_config * const pt_DmConfigs;
   const uint16 u16_NumConfigs;   // Number of configurations equals number of logical J1939 nodes
   const uint16 u16_TpBufferSize; // Size of the TP buffer
   uint8 * const pu8_TpBuffer;    // Internal one TP Buffer for all configs. The biggest DTC configuration must deliver
                                  // the size for the buffer
} T_osy_com_j1939_dm2_appl_tx_configs;

//----------------------------------------------------------------------------------------------------------------------
///this is the parameter passed to the osy_com_j1939_dm2_init_rx function
typedef struct
{
   // Receiving on one channel only from one device at the same time
   uint8 au8_TpBuffer[6U]; // Internal TP Buffer size for holding one DTC (4 byte) and the lamp information (2 byte)
} T_osy_com_j1939_dm2_appl_rx_configs;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

extern void osy_com_j1939_dm2_init_callbacks(const T_osy_com_j1939_dm2_callbacks * const opt_Callbacks);
extern sint16 osy_com_j1939_dm2_init_tx(const uint8 ou8_CanChannel,
                                        const T_osy_com_j1939_dm2_appl_tx_configs * const opt_DmTxConfigs);
extern sint16 osy_com_j1939_dm2_init_rx(const uint8 ou8_CanChannel,
                                        T_osy_com_j1939_dm2_appl_rx_configs * const opt_DmRxConfigs);

extern sint16 osy_com_j1939_dm2_send_request(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                             const uint8 ou8_DestinationAddress);

extern sint16 osy_com_j1939_dm2_lock_tx(const uint8 ou8_CanChannel, const uint16 ou16_Dm2TxConfigIndex);
extern sint16 osy_com_j1939_dm2_unlock_tx(const uint8 ou8_CanChannel, const uint16 ou16_Dm2TxConfigIndex);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
