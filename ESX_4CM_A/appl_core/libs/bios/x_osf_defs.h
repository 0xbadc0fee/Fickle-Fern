//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Target-specific System Parameter API configuration
   \copyright  Copyright 2020 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef X_OSF_DEFS_H
#define X_OSF_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
// Position of ETH switch hardware configuration bits
#define X_OSF_TARGET_CFG_POS_ETH_SWI_HW_CFG   (0u)

// Length of ETH switch hardware configuration bits
#define X_OSF_TARGET_CFG_LEN_ETH_SWI_HW_CFG   (2u)

// Mask for ETH switch hardware configuration bits
#define X_OSF_TARGET_CFG_MSK_ETH_SWI_HW_CFG   (0x3uL << X_OSF_TARGET_CFG_POS_ETH_SWI_HW_CFG)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif
