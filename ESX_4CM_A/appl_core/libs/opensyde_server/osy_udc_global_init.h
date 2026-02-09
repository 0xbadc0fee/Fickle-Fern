//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       UDS commons: global init functions for stack and transport protocol
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_UDC_GLOBAL_INITH
#define OSY_UDC_GLOBAL_INITH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_ctp_messageprocessor.h"
#include "osy_etp_messageprocessor.h"
#include "osy_ctp_router.h"
#include "osy_uds_connectionhandler.h"
#include "osy_uds_sessionengine.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// Wrapper for global TP and UDS initialization
// The macros should be invoked within a function to reduce the scope of the elements and to result in correct scope
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
#define OSY_UDC_GLOBAL_CAN_TP_INIT(STRUCT_NAME, BUS_INDEX, NUMBER_OF_RX_INSTANCES, NUMBER_OF_TX_INSTANCES, \
                                   BUFFERSIZE_PER_RX_INSTANCE, BUFFERSIZE_PER_TX_INSTANCE, SIZE_OF_RX_ROUTING_FIFO, \
                                   SIZE_OF_TX_FIFO) \
   OSY_CTP_MESSAGEPROCESSSOR_CH_INIT(BUS_INDEX, (ht_CanInitConfig ## BUS_INDEX), (NUMBER_OF_RX_INSTANCES), \
                                     (NUMBER_OF_TX_INSTANCES), (BUFFERSIZE_PER_RX_INSTANCE), \
                                     (BUFFERSIZE_PER_TX_INSTANCE), SIZE_OF_TX_FIFO) \
   OSY_CTP_CAN_ROUTER_INIT((ht_CanRouterInitConfig ## BUS_INDEX), BUS_INDEX, (SIZE_OF_RX_ROUTING_FIFO)) \
   static const T_osy_udc_global_cantp_init_configuration(STRUCT_NAME) = \
   { \
      (BUS_INDEX), \
      &ht_CanInitConfig ## BUS_INDEX, \
      &ht_CanRouterInitConfig ## BUS_INDEX \
   };

#define OSY_UDC_GLOBAL_ETHER_TP_INIT(STRUCT_NAME, BUS_INDEX, NUMBER_OF_RX_INSTANCES, NUMBER_OF_TX_INSTANCES, \
                                     BUFFERSIZE_PER_RX_INSTANCE, BUFFERSIZE_PER_TX_INSTANCE) \
   OSY_ETP_MESSAGEPROCESSSOR_CH_INIT(BUS_INDEX, (ht_EtherInitConfig ## BUS_INDEX), (NUMBER_OF_RX_INSTANCES), \
                                     (NUMBER_OF_TX_INSTANCES), (BUFFERSIZE_PER_RX_INSTANCE) +12U, \
                                     (BUFFERSIZE_PER_TX_INSTANCE) +12U) \
   static const T_osy_udc_global_ethertp_init_configuration(STRUCT_NAME) = \
   { \
      (BUS_INDEX), \
      &ht_EtherInitConfig ## BUS_INDEX \
   };

#define OSY_UDC_GLOBAL_UDS_INIT(STRUCT_NAME, MAX_NUMBER_OF_CONNECTIONS, MAX_NUMBER_OF_CAN_CHANNELS, \
                                MAX_NUMBER_OF_ETHERNET_CHANNELS) \
   OSY_UDS_CONNECTIONHANDLER_INIT(hat_UdsConnectionData, hapt_UdsConnectionsChannels, (MAX_NUMBER_OF_CONNECTIONS), \
                                  ((MAX_NUMBER_OF_CAN_CHANNELS) + (MAX_NUMBER_OF_ETHERNET_CHANNELS))) \
   static const T_osy_udc_global_uds_init_configuration(STRUCT_NAME) = \
   { \
      (MAX_NUMBER_OF_CONNECTIONS), \
      &hat_UdsConnectionData[0], \
      &hapt_UdsConnectionsChannels[0], \
      (MAX_NUMBER_OF_CAN_CHANNELS), \
      (MAX_NUMBER_OF_ETHERNET_CHANNELS) \
   };
/*PRQA S 0342,0881,3456 --*/

/* -- Types --------------------------------------------------------------------------------------------------------- */

///collected parameters from CAN TP layer
typedef struct
{
   const uint8 u8_BusNumber; ///< number of CAN bus (not necessarily identical to the CAN-TP instance index)
   const T_osy_ctp_mp_can_init_configuration * const pt_CanInitConfig;
   const T_osy_ctp_ro_init_configuration * const pt_CanRouterInitConfig;
} T_osy_udc_global_cantp_init_configuration;

///collected parameters from Ethernet TP layer
typedef struct
{
   const uint8 u8_BusNumber; ///< number of ETH bus (not necessarily identical to the ETH-TP instance index)
   const T_osy_etp_mp_eth_init_configuration * const pt_EthInitConfig;
} T_osy_udc_global_ethertp_init_configuration;

///collected parameter from UDS layer
typedef struct
{
   const uint8 u8_MaxNumConnections;
   T_osy_uds_ch_connection_data * const pt_Buffer;
   const T_osy_uds_ch_connection_data ** ppt_ConnectionsOfChannels;
   const uint8 u8_MaxNumCanChannels;
   const uint8 u8_MaxNumEthernetChannels;
} T_osy_udc_global_uds_init_configuration;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_udc_global_init_can_tp(const T_osy_udc_global_cantp_init_configuration * const opt_InitConfiguration,
                                         const T_osy_ctp_trg_can_init_params * const opt_TargetInitParameters,
                                         const uint8 ou8_TpIndex);
extern sint16 osy_udc_global_init_ether_tp(
   const T_osy_udc_global_ethertp_init_configuration * const opt_InitConfiguration,
   const T_osy_etp_trg_eth_init_params * const opt_TargetInitParameters, const uint8 ou8_TpIndex);
extern sint16 osy_udc_global_init_uds(const T_osy_udc_global_uds_init_configuration * const opt_InitConfiguration,
                                      const T_osy_uds_se_session_engine_configuration * const opt_SessionEngineConfig,
                                      const uint16 ou16_BufferSize);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
