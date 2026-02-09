//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Diagnostic protocol driver structs

   Definition of the configuration and data structs of the diagnostic protocol driver

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPD_DATAH
#define OSY_DPD_DATAH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_udc_global_init.h"
#include "osy_dpd_eventdriventransmissionhandler.h"

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */

///Common dpd data
///Instance data of Diagnostic Protocol Driver. Contains data for one client connection.
typedef struct
{
   const T_osy_dpd_ed_transmissions t_Transmissions;      ///< Configuration of event transmissions
   T_osy_uds_ch_connection_data * pt_UdsChConnectionData; ///< Pointer to the associated UDS connection
} T_osy_dpd_connection_instance;

//----------------------------------------------------------------------------------------------------------------------

///Aggregator for all Information for all communication stack purposes.
typedef struct
{
   ///Maximum number of active client connections
   const uint8 u8_MaxNumDpdConnectionInstances;
   ///Pointer to array with connection configurations
   T_osy_dpd_connection_instance * const * const ppt_DpdConnectionInstances;
   ///Number of used CAN-TP instances
   const uint8 u8_NumCanInstances;
   ///Number of used ETH-TP instances
   const uint8 u8_NumEthernetInstances;
   ///UDS Common initialization configuration
   const T_osy_udc_global_uds_init_configuration * const pt_UdcInitConfiguration;
   ///CAN configuration
   const T_osy_udc_global_cantp_init_configuration * const * const ppt_UdcGlobalCantpInitConfigurations;
   ///Ethernet configuration
   const T_osy_udc_global_ethertp_init_configuration * const * const ppt_UdcGlobalEthertpInitConfigurations;
} T_osy_dpd_data;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
