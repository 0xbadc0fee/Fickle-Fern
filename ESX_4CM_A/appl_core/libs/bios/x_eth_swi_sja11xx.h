//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      SJA11XX API
   \copyright  Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_SWI_SJA11XX_H
#define X_ETH_SWI_SJA11XX_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_eth_swi_sja11xx_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_eth_swi_sja11xx_sample_port_diagnostics(const uint16 ou16_PortId,
                                                 T_x_eth_swi_sja11xx_port_diagnostics * const opt_Diag);

sint16 x_eth_swi_sja11xx_read_registers(const uint16 ou16_PortId, const uint32 ou32_Address, const uint16 ou16_Count,
                                        uint32 * const opu32_Values);

sint16 x_eth_swi_sja11xx_write_registers(const uint16 ou16_PortId, const uint32 ou32_Address, const uint16 ou16_Count,
                                         const uint32 * const opu32_Values);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_ETH_SWI_SJA11XX_H
