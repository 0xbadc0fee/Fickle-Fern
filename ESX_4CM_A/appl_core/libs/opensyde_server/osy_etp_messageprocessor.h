//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Ethernet-TP message processor
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_ETP_MESSAGEPROCESSORH
#define OSY_ETP_MESSAGEPROCESSORH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_etp_trg_if.h"
#include "osy_uds_globals.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// Wrapper for Ethernet TP Channel initialization
// Call first for each channel
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
#define OSY_ETP_MESSAGEPROCESSSOR_CH_INIT(CHANNEL, STRUCT_NAME, NUMBER_OF_RX_INSTANCES, NUMBER_OF_TX_INSTANCES, \
                                          BUFFERSIZE_PER_RX_INSTANCE, BUFFERSIZE_PER_TX_INSTANCE) \
   static uint8 hau8_EtherRxMessageBuffer ## CHANNEL[(NUMBER_OF_RX_INSTANCES) *(BUFFERSIZE_PER_RX_INSTANCE)]; \
   static uint8 hau8_EtherTxMessageBuffer ## CHANNEL[(BUFFERSIZE_PER_TX_INSTANCE)]; \
   static T_osy_etp_mp_transmission_instance hat_EtherRxInstance ## CHANNEL[(NUMBER_OF_RX_INSTANCES)]; \
   static const T_osy_etp_mp_eth_init_configuration(STRUCT_NAME) = \
   { \
      (BUFFERSIZE_PER_TX_INSTANCE), \
      &hau8_EtherTxMessageBuffer ## CHANNEL[0], \
      (NUMBER_OF_RX_INSTANCES), \
      (NUMBER_OF_RX_INSTANCES) *(BUFFERSIZE_PER_RX_INSTANCE), \
      &hau8_EtherRxMessageBuffer ## CHANNEL[0], \
      &hat_EtherRxInstance ## CHANNEL[0], \
   };
/*PRQA S 0342,0881,3456 --*/

// special procedure in case of ETH->CAN routing
#define OSY_ETP_HANDLE_CTP_ROUTING_WITH_MULTI_FRAME_NO_FLOWCONTROL (2U) // use openSYDE specific multi-frame
// protocol instead of ISO15765-2 without
// flow control; (1U) means routing in general

/* -- Types --------------------------------------------------------------------------------------------------------- */
/// The structure is for handling the Rx/Tx instances
typedef struct
{
   uint8 u8_NumberOfInstances;               ///< given number of buffers per channel
   uint16 u16_BufferSize;                    ///< size of buffer for this instance
   uint8 * pu8_BufferStart;                  ///< message buffer address
   T_osy_uds_node_identifier t_NodeIdSource; ///< node identifier of sender
   T_osy_uds_node_identifier t_NodeIdTarget; ///< node identifier of receiver
   uint8 u8_UdsRouting;                      ///< retrieved message must be handled by UDS routing
   uint16 u16_MessageLength;                 ///< length of entire message
   uint8 u8_BufferStatus;                    ///< buffer free, used
   uint16 u16_PayloadType;                   ///< type of transmission
   uint16 u16_PayloadLength;                 ///< size of UDS message
   uint8 * pu8_Payload;                      ///< UDS message content (max. 4095 according to UDS specification)
} T_osy_etp_mp_transmission_instance;        ///< Rx/Tx instance buffer administration

/// The structure is placed in the header to allow the application to provide
/// configuration for the ethernet message handling.
typedef struct
{
   uint16 u16_TxBufferSize; ///< Tx buffer size
   //technically we would only need one TX buffer (not one per interface); but that would break the existing API
   uint8 * pu8_TxBuffer;                               ///< pointer to Tx buffer
                                                       //(only one required as intermediate buffer)
   uint8 u8_NumberOfRxInstances;                       ///< number of instances for Rx
   uint32 u32_RxBufferSize;                            ///< total Rx buffer size (shared by all instances)
   uint8 * pu8_RxBuffer;                               ///< pointer to common Rx buffer (i.e. u8_NumberOfRxInstances *
                                                       // 4095)
   T_osy_etp_mp_transmission_instance * pt_RxInstance; ///< static array with u8_NumberOfRxInstances entries
} T_osy_etp_mp_eth_init_configuration;

//callback notification for pending ECU reset:
typedef void (* PR_osy_etp_h_ecu_reset_handler) (const uint8 ou8_ChannelIndex, const uint8 ou8_ResetType,
                                                 uint8 * const opu8_AbortReset);

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_etp_mp_init(const uint8 ou8_ChannelIndex, const uint8 ou8_BusNumber,
                              const T_osy_uds_node_identifier * const opt_NodeIdentifier,
                              const T_osy_etp_mp_eth_init_configuration * const opt_EthInitConf,
                              const T_osy_etp_trg_eth_init_params * const opt_TargetInitParameters);
extern sint16 osy_etp_mp_send_message(const uint8 ou8_ChannelIndex,
                                      const T_osy_uds_node_identifier * const opt_TargetNodeIdentifier,
                                      const uint8 * const opu8_Data, const uint16 ou16_Size);
extern sint16 osy_etp_mp_send_message_routing(const uint8 ou8_ChannelIndex,
                                              const T_osy_uds_node_identifier * const opt_SourceNodeIdentifier,
                                              const T_osy_uds_node_identifier * const opt_TargetNodeIdentifier,
                                              const uint8 * const opu8_Data, const uint16 ou16_Size);
extern sint16 osy_etp_mp_retrieve_message(const uint8 ou8_ChannelIndex,
                                          T_osy_uds_node_identifier * const opt_SourceNodeIdentifier,
                                          T_osy_uds_node_identifier * const opt_TargetNodeIdentifier,
                                          uint8 * const opu8_UdsRouting, uint8 ** const oppu8_Data,
                                          uint16 * const opu16_Size);
extern sint16 osy_etp_mp_handle_rx_communication(const uint8 ou8_ChannelIndex);
extern sint16 osy_etp_mp_close_connection(const uint8 ou8_ChannelIndex,
                                          const T_osy_uds_node_identifier * const opt_NodeIdentifier);
extern sint16 osy_etp_mp_handle_get_device_info_request(const uint8 ou8_ChannelIndex,
                                                        const T_osy_etp_trg_eth_sockaddr * const opt_UdpSockAddr);
extern sint16 osy_etp_mp_get_routing_status(const uint8 ou8_ChannelIndex, uint8 * const opu8_Status);
extern sint16 osy_etp_mp_setup_routing(const uint8 ou8_ChannelIndex, const uint8 ou8_TargetBusNumber,
                                       const uint8 opu8_TargetIpAddress[4]);
extern uint8 osy_etp_get_bus_by_tp_index(const uint8 ou8_ChannelIndex);
extern uint8 osy_etp_get_tp_index_by_bus(const uint8 ou8_BusNumber);
extern void osy_etp_install_ecu_reset_handler(const PR_osy_etp_h_ecu_reset_handler opr_Handler);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
