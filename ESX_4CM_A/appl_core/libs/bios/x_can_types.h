//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Generic x_can types used in all software layers
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_CAN_TYPES_H
#define X_CAN_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_CAN_BUS_PROTOCOL_CAN      (0u) //!< Classic CAN protocol
#define X_CAN_BUS_PROTOCOL_CANFD    (1u) //!< CANFD protocol (supports Classic CAN too)
#define X_CAN_BUS_PROTOCOL_ISOCANFD (2u) //!< ISO CANFD protocol (supports Classic CAN too)

#define X_CAN_BUS_FAILURE_NONE      (0x0000u) //!< CAN bus is active (no error)
#define X_CAN_BUS_FAILURE_ACTIVE    (0x0000u) //!< CAN bus is active (no error)
#define X_CAN_BUS_FAILURE_BUSWARN   (0x0001u) //!< Buswarn error flag
#define X_CAN_BUS_FAILURE_BUSOFF    (0x0002u) //!< Busoff error flag

#define X_CAN_OBJ_TYPE_NULL         (0u) /*!< Message object, that "swallows" incoming data frames (without issuing a
                                              subsequent data processing like interupts etc.) */
#define X_CAN_OBJ_TYPE_RX           (1u) /*!< Message object, that can receive data frames */
#define X_CAN_OBJ_TYPE_TX           (2u) /*!< Message object, that can send data frames */
#define X_CAN_OBJ_TYPE_TX_RTR       (3u) /*!< Message object, that can receive RTR frames (requests) AND transmit
                                              response (data frame) automatically */
#define X_CAN_OBJ_TYPE_RTR_SENDER   (4u) /*!< This message object can only send RTR frames.
                                              It may be used to send RTR requests */
#define X_CAN_OBJ_TYPE_RTR_RECEIVER (5u) /*!< This message object is only able to receive RTR frames (RTR requests).
                                              Data frames can not be received. */
#define X_CAN_OBJ_TYPE_GATEWAY      (6u) /*!< Message object that can route messages of a CAN node to another one
                                              without any CPU interaction (gateway).
                                              (Delay time between reception on the source node and transmission to
                                              the destination node is round about 10us) */
#define X_CAN_OBJ_TYPE_GATEWAY_RTR  (7u) /*!< Same functionality as #X_CAN_OBJ_TYPE_GATEWAY, but additionally
                                              Remote Transfer Requests (RTR) can be handled.
                                              Use X_CAN_GATEWAY_CONTROL_RTR_xx to configure the behaviour in case of
                                              reception of a RTR frame */

#define X_CAN_DIRECTION_RX          (0u) //!< Callback function: direction flag (a message was received)
#define X_CAN_DIRECTION_TX          (1u) //!< Callback function: direction flag (a message was sent)

#define X_CAN_GATEWAY_CONTROL_AUTO_TX   (1 << 8u)  /*!< Transmit flag of gateway destination object is
                                                        automatically set. Hence the transmission of the destination
                                                        object starts immediately. */
#define X_CAN_GATEWAY_CONTROL_COPY_ID   (1 << 9u)  /*!< Identifier of the received CAN frame is copied to the gateway
                                                        destination object */
#define X_CAN_GATEWAY_CONTROL_COPY_DLC  (1 << 10u) /*!< Data length code of the received CAN frame is copied to the
                                                        gateway destination object */
#define X_CAN_GATEWAY_CONTROL_COPY_DATA (1 << 11u) /*!< Data of the received object is CAN frame to the gateway
                                                        destination object */
/*! If a RTR frame was received at the destination node the response to this RTR will be send on the same node
    (T_x_can_obj_gateway_config::u16_DestChannel).
    Content of this response is defined by T_x_can_obj_gateway_config::t_DefaultMessage.
    This behaviour is the default one.
    Note: This property is only valid if the message type is #X_CAN_OBJ_TYPE_GATEWAY_RTR */
#define X_CAN_GATEWAY_CONTROL_RTR_DEST  (0u)
/*! This setting will pass the received RTR frame to the source node (was set by x_can_bus_init).
    Note: This property is only valid if message type is #X_CAN_OBJ_TYPE_GATEWAY_RTR */
#define X_CAN_GATEWAY_CONTROL_RTR_PASS  (1 << 20u)

/* -- Types --------------------------------------------------------------------------------------------------------- */
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: No definition for structure*/
/*Reason: Dir 4.8 The pointer to the structure is never dereferenced, thus implementation of the object is hidden*/
/*Potential risks: none*/
/*Prevention of risks:  opaque type, nothing to do*/
/*PRQA S 3313 1*/
typedef struct T_x_can_obj_struct T_x_can_obj; //!< Opaque declaration (forward declaration) of CAN object structure

/*! \brief CAN ID structure */
typedef struct
{
   uint32 u32_Id; //!< CAN identifier 11/29bit
   uint8 u8_Xtd;  //!< bXtd == true: 29bit ID, bXtd == false: 11bit ID
} T_x_can_id;

/*! \brief Abstract base of a CAN message */
typedef struct
{
   T_x_can_id t_Id; //!< CAN message ID
   uint8 u8_Dlc;    //!< Data Length of message
   uint32 u32_Time; //!< RX timestamp (not used for TX messages)
   /*PRQA S 0642 1*/ /*STW_MD4_M_Rule-1.1_0642*/
   uint8 au8_Data[]; //!< Message data bytes (number of bytes given by u8_Dlc)
} T_x_can_msg_base;

/*! \brief CAN message */
typedef struct
{
   T_x_can_id t_Id;   //!< CAN message ID
   uint8 u8_Dlc;      //!< Data Length [0..8]
   uint32 u32_Time;   //!< RX timestamp (not used for TX messages)
   uint8 au8_Data[8]; //!< Message data bytes (aligned to 32bit boundary)
} T_x_can_msg;

/*! \brief CANFD message */
typedef struct
{
   T_x_can_id t_Id;    //!< CAN message ID
   uint8 u8_Dlc;       //!< Data Length [0..8, 12, 16, 20, 24, 32, 48, 64]
   uint32 u32_Time;    //!< RX timestamp (not used for TX messages)
   uint8 au8_Data[64]; //!< Message data bytes (aligned to 32bit boundary)
} T_x_can_msg_fd;

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Signature of the callback function to handle CAN-bus errors.

   CAN bus failure callback is intended to implement an application specific failure reaction.
   The callback is executed whenever the bus enters the bus warn or bus off state. \n
   It is executed with (ou16_BusState & #X_CAN_BUS_FAILURE_BUSWARN) != 0 if one of the error counters REC or TEC reaches
   the warnlevel (default warnlevel is 96). \n
   It is executed with (ou16_BusState & #X_CAN_BUS_FAILURE_BUSOFF) != 0 if CAN controller is in the bus-off state.

   \attention
   The callback is not executed when the bus is recovered into active state!

   \note
   The status of the can bus is bit coded. So there are other status flags available in parameter \p ou16_BusState
   (e.g. X_CAN_BUS_LOCKED).


   \param[in]        opv_CallbackParameter          A user defined callback parameter
   \param[in]        ou16_Channel                   CAN bus channel number [0 .. X_CAN_COUNT[
   \param[in]        ou16_BusState                  Bus state involved into the error
                                                    - #X_CAN_BUS_FAILURE_BUSWARN
                                                    - #X_CAN_BUS_FAILURE_BUSOFF
*/
//----------------------------------------------------------------------------------------------------------------------
typedef void (* PR_x_can_bus_FailureCallback)(void * const opv_CallbackParameter, const uint16 ou16_Channel,
                                              const uint16 ou16_BusState);

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Signature of the callback function called when a TX, RX or RTR event occurred the CAN object.

   \par TX object
   A TX message object may generate a TX interrupt callback after successful message transmission.
   The value of parameter \p ou16_Direction is #X_CAN_DIRECTION_TX.

   \par TX_RTR object
   A message object of type TX_RTR may generate also a TX interrupt callback after successful transmission
   of the prepared RTR response message (RTR frame). In this case the value of parameter \p ou16_Direction is
   also #X_CAN_DIRECTION_TX. \n
   In the same way a TX_RTR message object may generate a RX interrupt callback after successful reception
   of a RTR request. In this case the value of parameter \p ou16_Direction is #X_CAN_DIRECTION_RX.

   \note
   The CAN message (*opt_CanMsg) passed to The RX callback of a TX_RTR message object contains the ID, DLC and
   payload data of the prepared RTR frame if the auto response feature is enabled. \n
   If the auto response feature is disabled the CAN message contains the ID and DLC of the RTR request message
   but not the payload data of the RTR frame.

   \par RX object
   A RX message object may generate a RX interrupt callback after successful message reception.
   In this case the value of parameter \p ou16_Direction is #X_CAN_DIRECTION_RX. \n
   A RX message object can also be used to transmit a RTR request. In this case the RX message object may generate
   a TX interrupt callback after successful transmission of the request (the value of parameter \p ou16_Direction
   is #X_CAN_DIRECTION_TX).

   TODO Currently no callback functionality for gateway objects is availabel
   \par Gateway object
   For a gateway object the callback function is invoked with \p ou16_Direction = #X_CAN_DIRECTION_TX, if a message is
   passed from source to destination bus and successfully transmitted on the destination bus
   (Argument #X_CAN_GATEWAY_CONTROL_AUTO_TX should used). \n
   Furthermore the callback function is invoked with \p ou16_Direction = #X_CAN_DIRECTION_RX when a RTR frame is relayed
   from the destination bus to the source bus (only for u32_GatewayControl = #X_CAN_GATEWAY_CONTROL_RTR_PASS).

   \note
   In gateway mode the channel number passed to the callback function is always the gateway source channel independent
   of the direction of a message.

   \par Return Value
   The return value of the callback function can be used to control the handling of received messages: \n
   - If the message has already been processed by the callback and is not longer needed it can be dropped. \n
      For those purpose a zero has to be returned. This means that the message data should not be stored in the RX \n
      buffer.
   - If the message is not completely processed in the callback and should be stored in the object buffer for later
      processing a value of "not zero" has to be returned. This means that the message data is stored in the RX buffer.

   The return value has no effect on: \n
   - callbacks generated by TX interrupts at all message objects
   - on RX interrupts invocated by message object of type TX_RTR
   - on all interrupts generated by a gateway object

   \param[in]        opv_CallbackParameter           A user defined callback parameter
   \param[in]        ou16_Channel                    CAN bus channel number [0 .. X_CAN_COUNT[
   \param[in]        opt_Handle                      CAN object handle
   \param[in]        opt_CanMsg                      CAN message (depending on the object type,
                                                                  opt_CanMsg point to a CAN or a CAN-FD message)
   \param[in]        ou16_Direction                  Direction of the CAN message
                                                     - #X_CAN_DIRECTION_RX
                                                     - #X_CAN_DIRECTION_TX

   \retval  0        message data will not be stored in the RX buffer
   \retval  others   message data will be stored in the RX buffer
*/
//----------------------------------------------------------------------------------------------------------------------
typedef sint16 (* PR_x_can_obj_InterruptCallback)(void * const opv_CallbackParameter, const uint16 ou16_Channel,
                                                  T_x_can_obj * const opt_Handle, T_x_can_msg_base * const opt_CanMsg,
                                                  const uint16 ou16_Direction);

//! \brief Structure to configure the CAN node
/* TODO If the legacy structure "T_x_can_bus_status" will be removed, the CAN node settings like bitrate and so on
       can be replaced by T_x_can_bus_settings */
typedef struct
{
   uint32 u32_Bitrate;                              //!< Bitrate (bits/s)
   uint32 u32_DataBitrate;                          //!< Data bitrate (bits/s) (valid for CANFD only)
   uint16 u16_SamplePoint;                          //!< Sample point (0.1%)
   uint16 u16_DataSamplePoint;                      //!< Sample point (0.1%) (valid for CANFD only)
   uint16 u16_Protocol;                             //!< CAN bus protocol (X_CAN_BUS_PROTOCOL_...)
   PR_x_can_bus_FailureCallback pr_FailureCallback; //!< CAN bus failure callback
   void * pv_CallbackParameter;                     //!< Callback parameter for failure callback
} T_x_can_bus_config;

/*! \var T_x_can_bus_status::u16_BusState

   This variable contains bitcoded status information of the CAN bus. Each bit is represented by a define: \n
   - Bit0: #X_CAN_BUS_FAILURE_BUSWARN   indication for a buswarn error
   - Bit1: #X_CAN_BUS_FAILURE_BUSOFF    indication for busoff error
*/
/*! \brief Structure containing the current status of a CAN node
           (this type is legacy, please use T_x_can_bus_settings or T_x_can_bus_errors for future projects) */
typedef struct
{
   uint32 u32_Bitrate;         //!< Configured nominal bitrate (bits/s)
   uint32 u32_DataBitrate;     //!< Configured data bitrate (bits/s) (valid for CANFD only)
   uint16 u16_SamplePoint;     //!< Configured sample point (0.1%)
   uint16 u16_DataSamplePoint; //!< Configured sample point (0.1%) (valid for CANFD only)
   uint16 u16_Protocol;        //!< Configured CAN bus protocol (X_CAN_BUS_PROTOCOL_...)
   uint16 u16_BusState;        //!< Bus status (bit coded) see X_CAN_BUS_FAILURE_...
} T_x_can_bus_status;

//! \brief This type contains all necessary settings of a CAN node
typedef struct
{
   uint32 u32_Bitrate;         //!< Configured nominal bitrate (bits/s)
   uint32 u32_DataBitrate;     //!< Configured data bitrate (bits/s) (valid for CANFD only)
   uint16 u16_SamplePoint;     //!< Configured sample point (0.1%)
   uint16 u16_DataSamplePoint; //!< Configured sample point (0.1%) (valid for CANFD only)
   uint16 u16_Protocol;        //!< Configured CAN bus protocol (X_CAN_BUS_PROTOCOL_...)
} T_x_can_bus_settings;

/*! \var T_x_can_bus_errors::u8_LastErrorCode

   Coding of #u8_LastErrorCode / #u8_FastLastErrorCode as follows:
   - 0x00  No Error:
           No error was detected for the last (most recent) message on the CAN bus.

   - 0x01  Stuff Error:
           More than 5 equal bits in a sequence have occurred in a part of a received message where this is not allowed.

   - 0x02  Form Error:
           A fixed format part of a received frame has the wrong format.

   - 0x03  Ack Error:
           The transmitted message was not acknowledged by another node.

   - 0x04  Bit1 Error:
           During a message transmission, the CAN node tried to send a recessive level (1) outside the arbitration field
           and the acknowledge slot, but the monitored bus value was dominant.

   - 0x05  Bit0 Error:
           Two different conditions are signaled by this code:
           1. During transmission of a message (or acknowledge bit, active-error flag, overload flag), the CAN node
              tried to send a dominant level (0), but the monitored bus value was recessive.
           2. During bus-off recovery, this code is set each time a sequence of 11 recessive bits has been monitored.
              The CPU may use this code as indication that the bus is not continuously disturbed.

   - 0x06  CRC Error:
           The CRC checksum of the received message was incorrect.

   - 0x07  CPU write to LEC:
           Whenever the CPU writes the value 111 to LEC, it takes the value 111. Whenever the CPU writes another value
           to LEC, the written LEC value is ignored.

   - other values:  Reserved

*/
/*! \var T_x_can_bus_errors::u16_BusState

   This variable contains bitcoded status information of the CAN bus. Each bit is represented by a define: \n
   - Bit0: #X_CAN_BUS_FAILURE_BUSWARN   indication for a buswarn error
   - Bit1: #X_CAN_BUS_FAILURE_BUSOFF    indication for busoff error
*/
//! \brief Error counter and status of a CAN node
typedef struct
{
   uint16 u16_BusState;             //!< Bus status (bit coded) see X_CAN_BUS_FAILURE_...
   uint16 u16_TransmitErrorCounter; //!< Value of transmit error counter
   uint16 u16_ReceiveErrorCounter;  //!< Value of receive error counter
   uint8 u8_LastErrorDirection;     /*!< Direction of the last transfer error
                                         - 0     = Last error occurred while the node was receiver (REC was incremented)
                                         - other = Last error occurred while the node was transmitter
                                                   (TEC was incremented) */
   uint8 u8_LastErrorCode;          //!< Last error code occurred
   uint8 u8_FastLastErrorCode;      //!< Last error code occurred in data phase of a frame (valid for CANFD only)
} T_x_can_bus_errors;

//! \brief Contains settings necessary for gateway mode
typedef struct
{
   uint32 u32_GatewayControl;    /*!< Additional settings if gateway mode was selected.
                                      One or more (ORed) X_CAN_GATEWAY_CONTROL.. */
   uint16 u16_DestChannel;       //!< CAN channel to which the received message object shall be passed through
   T_x_can_msg t_DefaultMessage; /*!< This contains the default message of the destination object.
                                      It is important when not all message elements are copied from the source to
                                      the destination bus. E.g. X_CAN_GATEWAY_CONTROL_COPY_ID is not set in the
                                      gateway message object. Hence the received ID of the source
                                      object will not be transferred to the destination object and replaced by
                                      this default message value */
}T_x_can_obj_gateway_config;

/*! \var T_x_can_obj_config::u16_Pause

   Specifies a minimum pause time between the transmission of CAN messages. The time is specified as a multiple
   of 0.5ms. If u16_Pause = 0 the delay feature is off.

   \note
   This setting is only applicable for TX message objects.

   \attention
   The message object need a software FIFO buffer when using the pause functionality.
*/
/*! \var T_x_can_obj_config::u16_SoftBuffer

   Specifies the size of the additional software FIFO buffer used to store messages for transmission or the already
   received messages. \n
   The buffer is allocated dynamically from heap memory. It does not require any additional can hardware objects.
   Set the value to 0 if no software FIFO buffer shall be used.

   \note
   The maximal size of u16_SoftBuffer is limited by available heap memory.

   \attention
   For message objects of type #X_CAN_OBJ_TYPE_TX_RTR, #X_CAN_OBJ_TYPE_GATEWAY and #X_CAN_OBJ_TYPE_GATEWAY_RTR
   u16_SoftBuffer has to be set to 0, as no software buffer is allowed for those objects type!
*/
/*! \var T_x_can_obj_config::u16_HardBuffer

   Specifies the size of the additional hardware FIFO buffer used to store messages for transmission or
   received messages. \n
   The buffer is allocated dynamically from the available message objects (hardware).

   Additional hardware buffer allowes a "relaxed software intervention". So if your application run
   the risk of loosing CAN messages as the CPU is not fast enough to copy them into the software buffer before a
   new message is received, the size of hardware buffer should be increased. \n
   However each hardware buffer element reduce the number of available CAN message objects!

   Set the value to 0 if no hardware FIFO buffer shall be used. A value of 1 should be avoided, because this
   configuration allocates in total 2 message objects, but the FIFO size would be just 1 (reason for this is the
   hardware architecture of the HW-FIFO buffer)

   \note
   The maximal size of u16_HardwareBuffer is limited by the number of available CAN message objects and the
   available heap memory.

   \attention
   For message objects of type #X_CAN_OBJ_TYPE_TX_RTR, #X_CAN_OBJ_TYPE_TX and #X_CAN_OBJ_TYPE_GATEWAY_RTR
   u16_HardBuffer has to be set to 0, as no harware buffer is allowed for those objects type!
*/
/*! \var T_x_can_obj_config::t_Mask

   Acceptance filtering is done as follows:
   (ID of received frame XOR t_Id.u32_Id) AND t_Mask.u32_Id
   Hence the identifier of the received frame must match the identifier of the message object and is qualified by
   the acceptance mask t_Mask.u32_Id. This means that each bit of the received message object identifier is equal
   to t_Id.u32_Id, except those bits for which the corresponding acceptance mask bits in bit field t_Mask.u32_Id
   are cleared. These identifier bits are do not care for reception.
*/
//! \brief Contains initial settings of a message object
typedef struct
{
   T_x_can_id t_Id;                            //!< ID the object shall use
   T_x_can_id t_Mask;                          //!< Filter mask the object shall use
   uint8 u8_MsgType;                           //!< X_CAN_OBJ_TYPE_...
   uint16 u16_Pause;                           //!< TX pause in steps of 0.5ms
   uint16 u16_SoftBuffer;                      //!< CAN software FIFO size (0 = no FIFO) (not available in gateway mode)
   uint16 u16_HardBuffer;                      //!< CAN hardware FIFO size (0 = no FIFO)
   void * pv_CallbackParameter;                //!< Callback parameter (not available in gateway mode)
   PR_x_can_obj_InterruptCallback pr_Callback; //!< TX/RX interrupt callback (not available in gateway mode)
   T_x_can_obj_gateway_config t_GatewayConfig; /*!< Settings of the gateway mode.
                                                    Only necessary if #X_CAN_OBJ_TYPE_GATEWAY or
                                                    #X_CAN_OBJ_TYPE_GATEWAY_RTR was selected */
}T_x_can_obj_config;

//MHE, 21.04.17: the status information retrieved by an object are redundant/obsolete.
//               thus status is not available, for now.
// - NumberMsgs can be retrieved by x_can_obj_get_buf_count
// - Whether TX messages are dropped due to overflow is returned to the user via error code of x_can_obj_send_msg
// - In Aurix, the option to lock an object was removed. Thus it is always "IN_USE".
//   Further more, for not initialized objects, it is not possible to call x_can_obj_get_status.
/*! \brief  Status information about the can object. */
typedef struct
{
   //   uint32    u32_LastInterruptTime;   //!< last RX/TX event
   //   uint16    u16_NumberMsgs;          //!< number of messages in FIFO buffer
   //   uint8     u8_Usage;                //!< X_CAN_OBJ_LOCKED, X_CAN_OBJ_IN_USE, X_CAN_OBJ_NO_INIT
   uint8 u8_Locked;      //!< message object locked (does not participate in communication)
   uint8 u8_MsgLostFlag; //!< at least one RX message was lost
   //   uint8     u8_OverflowCount;        //!< number of lost messages, will be reset by reading it out
}T_x_can_obj_status;

#endif /* _X_CAN_TYPES_H */
