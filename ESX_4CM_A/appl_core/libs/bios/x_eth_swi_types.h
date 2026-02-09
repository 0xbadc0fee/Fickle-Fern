//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Types and constants for Ethernet switch API
               Header defines types and constants for Ethernet switch API
   \copyright  Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_SWI_TYPES_H
#define X_ETH_SWI_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_eth_phy_types.h"
#include "x_eth_swi_defs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/*! \brief  Ethernet switch PHY status data */
typedef struct
{
   uint8 u8_LinkStatus; //!< Link status (X_ETH_LINK_UP, X_ETH_LINK_DOWN)
   uint16 u16_LinkMode; //!< Requested link speed and duplex mode (X_ETH_LINK_MODE_...)
} T_x_eth_swi_phy_status;

//!< Ethernet switch PHY status of callback
typedef void (* PR_x_eth_swi_phy_status_callback)(void * const opv_CallbackParameter,
                                                  const T_x_eth_swi_phy_status * const opt_PhyStatus);

/*! \brief  Ethernet switch PHY configuration data */
typedef struct
{
   uint16 u16_PortId;                                  //!< Port Id (combined switch/port number, X_ETH_SWI_ETHx_PORTx)
   uint8 u8_AutoNeg;                                   //!< Auto-negotiation ON/OFF (X_ON, X_OFF)
   uint16 u16_LinkMode;                                //!< Requested link speed and duplex mode (X_ETH_LINK_MODE_)
   uint32 u32_Feature;                                 //!< Additional configuration option
   PR_x_eth_swi_phy_status_callback pr_StatusCallback; //!< PHY status callback
   void * pv_StatusCallbackParameter;                  //!< Callback parameter for PHY status callback
} T_x_eth_swi_phy_config;

/*! \brief  Static configuration data of a physical switch */
typedef struct
{
   const uint32 * pu32_Data; //!< Configuration data array
   uint32 u32_Size;          //!< Size of configuration data (uint32 word count)
} T_x_eth_swi_static_config_data;

/*! \brief  Static configuration data for all physical switches */
typedef struct
{
   //!< Switch configuration data for all physical switches of the considered logical switch
   const T_x_eth_swi_static_config_data ** ppt_SwitchConfigData;
   uint8 u8_ConfigCount; //!< Number of switch configurations
} T_x_eth_swi_static_config;

/*! \brief  Ethernet switch configuration data */
typedef struct
{
   T_x_eth_swi_static_config * pt_SwitchConfig;                  //!< Switch configuration
   T_x_eth_swi_phy_config at_PhyConfig[X_ETH_SWI_PHY_COUNT_MAX]; //!< PHY configuration data
} T_x_eth_swi_config;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_SWI_TYPES_H
