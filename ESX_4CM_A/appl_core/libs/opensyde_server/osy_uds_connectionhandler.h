//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       UDS connection handler
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_UDS_CONNECTIONHANDLERH
#define OSY_UDS_CONNECTIONHANDLERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_uds_globals.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// Wrapper for UDS connections initialization
// Call once
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: macro parameter used more than once*/
/*Reason: there are more variables to create depending on the same parameter*/
/*Potential risks: possible side effects*/
/*Prevention of risks:  no unexpected side effects, parameter intentionally used more than once*/
/*PRQA S 3456 ++*/
#define OSY_UDS_CONNECTIONHANDLER_INIT(STRUCT_NAME_CONNECTION_DATA, STRUCT_NAME_CONNECTIONS_CHANNELS, \
                                       MAX_NUMBER_OF_CONNECTIONS, MAX_NUMBER_OF_CHANNELS) \
   static T_osy_uds_ch_connection_data(STRUCT_NAME_CONNECTION_DATA)[(MAX_NUMBER_OF_CONNECTIONS)]; \
   static const T_osy_uds_ch_connection_data * (STRUCT_NAME_CONNECTIONS_CHANNELS)[(MAX_NUMBER_OF_CONNECTIONS) *( \
                                                                                     MAX_NUMBER_OF_CHANNELS)];
/*PRQA S 3456 --*/

/* -- Types --------------------------------------------------------------------------------------------------------- */
///connection type
#define OSY_UDS_CH_CHANNEL_TYPE_CAN      (0U) ///< connection via CAN-TP
#define OSY_UDS_CH_CHANNEL_TYPE_ETHERNET (1U) ///< connection via Ethernet-TP
#define OSY_UDS_CH_CHANNEL_NUM_TYPES     (2U)

///describes one active connection
typedef struct
{
   uint8 u8_ChannelType;  ///< type of TP to use for this connection (OSY_UDS_CH_CHANNEL_TYPE_*)
   uint8 u8_ChannelIndex; ///< channel index for defined TP (not necessarily identical to interface number)
} T_osy_uds_ch_channel_identifier;

///connection with assigned node address
typedef struct
{
   T_osy_uds_node_identifier t_AssignedAddress;  ///< client address occupying this connection
   T_osy_uds_ch_channel_identifier t_Connection; ///< information about associated communication interface
   uint8 u8_CurrentSession;

   //security engine status:
   uint8 u8_CurrentSALevel;
   uint8 u8_LastRequestedSA; ///< for seed/key mechanism; received key service must match preceding seed service
   uint64 u64_LastSeed;
   uint32 u32_LastTimeStampLastKey;

   uint32 u32_LastTimeStamp; ///< timestamp of last activity on this connection
   uint8 u8_RoutingActive;   ///< flag if routing was configured and used by this connection.

   void * pv_ApplicationSpecificData; ///< BLOB; ignored by this layer; just pass-through for data management
} T_osy_uds_ch_connection_data;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_uds_ch_init(const uint8 ou8_MaxNumConnections, T_osy_uds_ch_connection_data * const opt_Buffer,
                              const T_osy_uds_ch_connection_data ** const oppt_ConnectionsOfChannels,
                              const uint8 ou8_MaxNumCanChannels, const uint8 ou8_MaxNumEthernetChannels);
extern T_osy_uds_ch_connection_data * osy_uds_ch_get_connection(
   const T_osy_uds_node_identifier * const opt_SenderAddress);
extern const T_osy_uds_ch_connection_data ** osy_uds_ch_get_connections(
   const T_osy_uds_ch_channel_identifier * const opt_Channel, uint8 * const opu8_NumConnections);
extern sint16 osy_uds_ch_add_connection(const T_osy_uds_node_identifier * const opt_ClientAddress,
                                        const T_osy_uds_ch_channel_identifier * const opt_ChannelId,
                                        T_osy_uds_ch_connection_data ** const oppt_Connection);
extern sint16 osy_uds_ch_remove_connection(const T_osy_uds_node_identifier * const opt_ConnectedNode);
extern void osy_uds_ch_check_timeout(const T_osy_uds_ch_channel_identifier * const opt_Channel,
                                     const uint8 ou8_DisconnectDetected);
extern sint16 osy_uds_ch_refresh_timer(const T_osy_uds_node_identifier * const opt_SenderAddress);
extern sint16 osy_uds_ch_set_routing_active(const T_osy_uds_node_identifier * const opt_SenderAddress,
                                            const uint8 ou8_Active);
extern void osy_uds_ch_reset_routing_timer(void);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
