//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       CAN-TP encoder / decoder
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_CTP_MESSAGEPROCESSORH
#define OSY_CTP_MESSAGEPROCESSORH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_ctp_trg_if.h" //we need the target specific initialization definition
#include "osy_uds_globals.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// Wrapper for CAN TP Channel initialization
// Call first for each channel
// The macro should be invoked within a function to reduce the scope of the elements and to result in correct scope
//  prefixes

/*STW Deviation Coding Rules v4*/
/*Violated Rule: 20.10*/
/*Circumstances: using glue operator ##*/
/*Reason: creating different variable names combined with the same parameter*/
/*Potential risks: none*/
/*Prevention of risks:  nothing to do*/
/*PRQA S 0342 ++*/
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: using more than one ## operator*/
/*Reason: there are more variables to create*/
/*Potential risks: invalid preprocessing token because of different evaluation order*/
/*Prevention of risks: use of only a single ## operator for each variable to create*/
/*PRQA S 0881 ++*/
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: macro parameter used more than once*/
/*Reason: there are more variables to create depending on the same parameter*/
/*Potential risks: possible side effects*/
/*Prevention of risks:  no unexpected side effects, parameter intentionally used more than once*/
/*PRQA S 3456 ++*/
#define OSY_CTP_MESSAGEPROCESSSOR_CH_INIT(CHANNEL, STRUCT_NAME, NUMBER_OF_RX_INSTANCES, NUMBER_OF_TX_INSTANCES, \
                                          BUFFERSIZE_PER_RX_INSTANCE, BUFFERSIZE_PER_TX_INSTANCE, SIZE_OF_TX_FIFO) \
   static uint8 hau8_CanRxMessageBuffer ## CHANNEL[(NUMBER_OF_RX_INSTANCES) *(BUFFERSIZE_PER_RX_INSTANCE)]; \
   static uint8 hau8_CanTxMessageBuffer ## CHANNEL[(NUMBER_OF_TX_INSTANCES) *(BUFFERSIZE_PER_TX_INSTANCE)]; \
   static T_osy_ctp_mp_transmission_instance hat_CanRxInstance ## CHANNEL[(NUMBER_OF_RX_INSTANCES)]; \
   static T_osy_ctp_mp_transmission_instance hat_CanTxInstance ## CHANNEL[(NUMBER_OF_TX_INSTANCES)]; \
   static T_osy_ctp_trg_can_message hat_TxFifo ## CHANNEL[(SIZE_OF_TX_FIFO)]; \
   static const T_osy_ctp_mp_can_init_configuration(STRUCT_NAME) = \
   { \
      (NUMBER_OF_TX_INSTANCES), \
      (NUMBER_OF_TX_INSTANCES) *(BUFFERSIZE_PER_TX_INSTANCE), \
      &hau8_CanTxMessageBuffer ## CHANNEL[0], \
      &hat_CanTxInstance ## CHANNEL[0], \
      (NUMBER_OF_RX_INSTANCES), \
      (NUMBER_OF_RX_INSTANCES) *(BUFFERSIZE_PER_RX_INSTANCE), \
      &hau8_CanRxMessageBuffer ## CHANNEL[0], \
      &hat_CanRxInstance ## CHANNEL[0], \
      (SIZE_OF_TX_FIFO), \
      &hat_TxFifo ## CHANNEL[0] \
   };
/*PRQA S 0342,0881,3456 --*/

// single and multi frame message types to be used for sending
#define OSY_CTP_MESSAGE_TYPE_ISO15765                         (0x0U) // ISO15765 conform message type
#define OSY_CTP_MESSAGE_TYPE_OSF_EVENT_DRIVEN_DP_SINGLE_FRAME (0x1U) // openSYDE single frame without response SID=0xFA
#define OSY_CTP_MESSAGE_TYPE_OMF_MULTI_FRAME_NO_FLOWCONTROL   (0x2U) // openSYDE multi frame without first frame and
                                                                     // flow control
#define OSY_CTP_MESSAGE_TYPE_OSF_SERIAL_NUMBER_POS            (0x3U) // Serial number startup message in POS format
#define OSY_CTP_MESSAGE_TYPE_OSF_SERIAL_NUMBER_FSN            (0x4U) // Serial number startup message in FSN format

// status of transmission buffer
#define OSY_CTP_BUFFER_STATUS_FREE (0U)
#define OSY_CTP_BUFFER_STATUS_USED (1U)
#define OSY_CTP_BUFFER_STATUS_FULL (2U)

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
   uint8 u8_FlowControlState; ///< multi frame flow control state
   uint8 u8_BlockSize;        ///< number of consecutive frames without FC
   uint32 u32_TimerStart;     ///< timer start value for state wait
} T_ctp_flow_control;

/// The structure is for handling the Rx/Tx instances
typedef struct
{
   uint16 u16_BufferSize;                    ///< size of buffer for this instance
   uint8 * pu8_BufferStart;                  ///< remember buffer address
   T_osy_uds_node_identifier t_NodeIdSource; ///< node identifier of sender
   T_osy_uds_node_identifier t_NodeIdTarget; ///< node identifier of receiver
   uint32 u32_CanIdentifier;                 ///< CAN address
   uint16 u16_MessageLength;                 ///< length of entire message
   uint16 u16_ProcessedBytes;                ///< number of actually sent/retrieved bytes
   uint8 u8_SequenceNumber;                  ///< consecutive retrieve sequence number
   uint8 u8_BufferStatus;                    ///< buffer empty, used, full
   uint8 u8_UdsRouting;                      ///< retrieved message must be handled by UDS routing
   T_ctp_flow_control t_FlowControl;         ///< flow control for ISO15765 conform multi-frame messages
   uint8 u8_MessageType;                     ///< ISO15765 conform or openSYDE specific message type
                                             // 0: ISO15765 conform message type
                                             // 2: openSYDE specific multi-frame message without flow control
} T_osy_ctp_mp_transmission_instance;        ///< Rx/Tx instance buffer administration

/// The structure is placed in the header to allow the application to provide
/// configuration for the CAN objects.
typedef struct
{
   uint8 u8_NumberOfTxInstances;                       ///< number of instances for Tx
   uint32 u32_TxBufferSize;                            ///< total Tx buffer size (shared by all instances)
   uint8 * pu8_TxBuffer;                               ///< pointer to common Tx buffer (i.e. u8_NumberOfTxInstances *
                                                       // 4095)
   T_osy_ctp_mp_transmission_instance * pt_TxInstance; ///< static array with u8_NumberOfTxInstances entries
   uint8 u8_NumberOfRxInstances;                       ///< number of instances for Rx
   uint32 u32_RxBufferSize;                            ///< total Rx buffer size (shared by all instances)
   uint8 * pu8_RxBuffer;                               ///< pointer to common Rx buffer (i.e. u8_NumberOfRxInstances *
                                                       // 4095)
   T_osy_ctp_mp_transmission_instance * pt_RxInstance; ///< static array with u8_NumberOfRxInstances entries
   uint16 u16_TxFifoSize;                              ///< size of TX CAN message FIFO
   T_osy_ctp_trg_can_message * pt_TxFifo;              ///< pointer to TX CAN message FIFO
} T_osy_ctp_mp_can_init_configuration;                 ///< CAN initial set-up per channel

/// callback for checking whether routing is installed
typedef sint16 (* PR_osy_ctp_mp_is_can_routing_active)(const uint8 ou8_SourceCanChannel, const uint8 ou8_SourceBusId,
                                                       const uint8 ou8_TargetBusId, uint8 * const opu8_RoutingType,
                                                       uint8 * const opu8_TargetBusNumber);

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_ctp_mp_init(const uint8 ou8_ChannelIndex, const uint8 ou8_BusNumber,
                              const T_osy_uds_node_identifier * const opt_NodeId,
                              const T_osy_ctp_mp_can_init_configuration * const opt_CanInitConf,
                              const T_osy_ctp_trg_can_init_params * const opt_TargetInitParameters,
                              const PR_osy_ctp_mp_is_can_routing_active opr_IsRoutingActiveFunction);
extern sint16 osy_ctp_mp_send_message(const uint8 ou8_ChannelIndex, const uint8 ou8_MessageType,
                                      const T_osy_uds_node_identifier * const opt_TargetNodeId,
                                      const uint8 * const opu8_Data, const uint16 ou16_Size);
extern sint16 osy_ctp_mp_send_message_routing(const uint8 ou8_ChannelIndex, const uint8 ou8_MessageType,
                                              const T_osy_uds_node_identifier * const opt_SourceNodeId,
                                              const T_osy_uds_node_identifier * const opt_TargetNodeId,
                                              const uint8 * const opu8_Data, const uint16 ou16_Size);
extern sint16 osy_ctp_mp_retrieve_message(const uint8 ou8_ChannelIndex,
                                          T_osy_uds_node_identifier * const opt_SourceNodeId,
                                          T_osy_uds_node_identifier * const opt_TargetNodeId,
                                          uint8 * const opu8_UdsRouting, uint8 ** const oppu8_Data,
                                          uint16 * const opu16_Size);
extern sint16 osy_ctp_mp_handle_tx_communication(const uint8 ou8_ChannelIndex, uint8 * const opu8_BufferStatus);
extern sint16 osy_ctp_mp_handle_rx_communication(const uint8 ou8_ChannelIndex, uint8 * const opu8_BufferStatus);
extern sint16 osy_ctp_mp_close_connection(const uint8 ou8_ChannelIndex,
                                          const T_osy_uds_node_identifier * const opt_NodeId);
extern uint8 osy_ctp_mp_get_bus_by_tp_index(const uint8 ou8_ChannelIndex);
extern uint8 osy_ctp_mp_get_tp_index_by_bus(const uint8 ou8_BusNumber);
extern sint16 osy_ctp_mp_push_tx_message(const uint8 ou8_BusNumber,
                                         const T_osy_ctp_trg_can_message * const opt_CanMessage);
extern sint16 osy_ctp_mp_send_tx_messages(const uint8 ou8_BusNumber);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
