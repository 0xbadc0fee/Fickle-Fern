//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Ethernet switch API
   \copyright  Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_SWI_H
#define X_ETH_SWI_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "x_eth_swi_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_eth_swi_select_hw_config(const uint16 ou16_SwitchCh, const uint32 ou32_HwConfig);

sint16 x_eth_swi_init(const uint16 ou16_SwitchCh, const T_x_eth_swi_config * const opt_Config);

sint16 x_eth_swi_phy_get_status(const uint16 ou16_PortId, T_x_eth_swi_phy_status * const opt_Status);

sint16 x_eth_swi_phy_dump_registers(const uint16 ou16_PortId, const uint16 ou16_StartAddr, const uint8 ou8_Count,
                                    uint16 * const opu16_Values);

sint16 x_eth_swi_phy_write_registers(const uint16 ou16_PortId, const uint16 ou16_StartAddr, const uint8 ou8_Count,
                                     const uint16 * const opu16_Values);

sint16 x_eth_swi_phy_dump_mmd_registers(const uint16 ou16_PortId, const uint16 ou16_MmdAddr,
                                        const uint16 ou16_StartAddr, const uint8 ou8_Count,
                                        uint16 * const opu16_Values);

sint16 x_eth_swi_phy_write_mmd_registers(const uint16 ou16_PortId, const uint16 ou16_MmdAddr,
                                         const uint16 ou16_StartAddr, const uint8 ou8_Count,
                                         uint16 * const opu16_Values);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_ETH_SWI_H
