//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      CRC algorithm API implementations
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_CRC_H
#define X_CRC_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

void x_crc_calc_16(const uint32 ou32_Count, const uint8 * const opu8_Data, uint16 * const opu16_Crc);
void x_crc_calc_16_stw(const uint32 ou32_Count, const uint8 * const opu8_Data, uint16 * const opu16_Crc);
void x_crc_calc_32(const uint32 ou32_Count, const uint8 * const opu8_Data, uint32 * const opu32_Crc);
void x_crc_calc_tricore_sw(const uint32 ou32_Count, const uint32 * const opu32_Data, uint32 * const opu32_Crc);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /* XB_CRC_H */
