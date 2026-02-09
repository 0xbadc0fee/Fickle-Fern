//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      API functions for FRAM support
   \copyright  Copyright 2020 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef X_FRAM_H
#define X_FRAM_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_fram_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 x_fram_init(const uint16 ou16_Device);
sint16 x_fram_read(const uint16 ou16_Device, const uint32 ou32_Address, const uint32 ou32_Count,
                   uint8 * const opu8_Data);
sint16 x_fram_write(const uint16 ou16_Device, const uint32 ou32_Address, const uint32 ou32_Count,
                    const uint8 * const opu8_Data);
sint16 x_fram_get_size(const uint16 ou16_Device, uint32 * const opu32_Size);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
