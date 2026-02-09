//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      AURIX target-specific Ethernet switch API configuration
   \copyright  Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_SWI_DEFS_H
#define X_ETH_SWI_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_eth_swi_port.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_ETH_SWI_ETH1_SWI1            (uint16)(0u) // Channel number of logical switch
#define X_ETH_SWI_COUNT                (uint16)(1u) // Number of logical switches

// PortId: Unique identifier for each port (internal and external) of a logical switch.
// (This defines represent the PortId used in the x_eth_swi API)
// u16_PortId[15:12] = Magic number
// u16_PortId[11:8] = reserved (0)
// u16_PortId[7:4] = Logical switch number (X_ETH_SWI_ETHx_SWIx)
// u16_PortId[3:0] = Port index (0..x)
// The port index is a up-counting number starting at zero that identifies each port (internal or external) of a
// logical switch. It is the index for the port map structure in the driver configuration (T_switch_drv_cfg->pt_PortMap)
// Internal ports shall be located at the end of the list.
//                               Magic                     Switch Channel                Port Index
#define X_ETH_SWI_ETH1_PORT1     (X_ETH_SWI_PORTID_MAGIC | (uint16)(X_ETH_SWI_ETH1_SWI1 << 4u) | 0u) // TJA1102
#define X_ETH_SWI_ETH1_PORT2     (X_ETH_SWI_PORTID_MAGIC | (uint16)(X_ETH_SWI_ETH1_SWI1 << 4u) | 1u) // TJA1102
#define X_ETH_SWI_ETH1_PORT3     (X_ETH_SWI_PORTID_MAGIC | (uint16)(X_ETH_SWI_ETH1_SWI1 << 4u) | 2u) // DP83822
#define X_ETH_SWI_ETH1_PORT4     (X_ETH_SWI_PORTID_MAGIC | (uint16)(X_ETH_SWI_ETH1_SWI1 << 4u) | 3u) // DP83822

#define X_ETH_SWI_PHY_COUNT (4u) // Number of switch ports at the ECU's connector (= number of PHYs connected to switch)

// Internal ports:
// Connection to AURIX (switch chip number 0 / port 0)
#define X_ETH_SWI_ETH1_PORT_ARX  (X_ETH_SWI_PORTID_MAGIC | (uint16)(X_ETH_SWI_ETH1_SWI1 << 4u) | 4u)

// PRQA S 3410 ++
#define X_ETH_SWI_ETH1_DECLARE_PORT_LIST(VARIABLE_NAME) \
   static const uint16 VARIABLE_NAME[X_ETH_SWI_PHY_COUNT] = \
   { \
      X_ETH_SWI_ETH1_PORT1, \
      X_ETH_SWI_ETH1_PORT2, \
      X_ETH_SWI_ETH1_PORT3, \
      X_ETH_SWI_ETH1_PORT4, \
   }
// PRQA S 3410 --

#define X_ETH_SWI_PHY_COUNT_MAX  X_ETH_SWI_PHY_COUNT // If multiple logical switches are available use the highest
                                                     // PHY count per switch

// Hardware configurations
#define X_ETH_SWI_HW_CFG_ETH1_DEFAULT    (0u) //!< Default hardware configuration

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_SWI_DEFS_H
