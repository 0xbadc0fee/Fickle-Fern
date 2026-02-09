//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data flash module functions
   \copyright   Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_DFLASH_H
#define X_DFLASH_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_dflash_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_DFLASH_MAX_NUM_FLASH_AREAS    5u //!< max. number of different sector areas

//! \brief flash status
#define X_DFLASH_STATUS_READY       (0uL)        //!< flash device is ready, access possible
#define X_DFLASH_STATUS_BUSY        (1uL << 0uL) //!< flash device is busy, no access possible
#define X_DFLASH_STATUS_LOCK        (1uL << 1uL) //!< flash device is locked, no access possible
#define X_DFLASH_STATUS_WRITE_ERR   (1uL << 2uL) //!< writing flash device failed

/* -- Types --------------------------------------------------------------------------------------------------------- */
//! \brief defines area of same sized sectors in series
typedef struct
{
   uint32 u32_SectorCount; //!< count of same sized sectors
   uint32 u32_SectorSize;  //!< sector size in bytes
} T_x_dflash_sector_area;

//! \brief flash layout definition
typedef struct
{
   uint32 u32_TotalSize;                                                //!< flash device size in byte
   uint32 u32_PageSize;                                                 //!< max. amount of memory to be written by page
                                                                        // program operation in byte
   uint32 u32_TotalNumberOfSectors;                                     //!< sum of sector count of all areas
   uint16 u16_SectorAreas;                                              //!< number of available sector areas (<=
                                                                        // X_DFLASH_MAX_NUM_FLASH_AREAS)
   T_x_dflash_sector_area at_SectorAreas[X_DFLASH_MAX_NUM_FLASH_AREAS]; //!< sector layout definition
} T_x_dflash_layout;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 x_dflash_init(const uint16 ou16_Device, T_x_dflash_layout * const opt_SectorLayout);
sint16 x_dflash_read(const uint16 ou16_Device, const uint32 ou32_Address, const uint32 ou32_Count,
                     uint8 * const opu8_Data);
sint16 x_dflash_erase_sector_nowait(const uint16 ou16_Device, const uint32 ou32_SectorNumber);
sint16 x_dflash_write_nowait(const uint16 ou16_Device, const uint32 ou32_Address, const uint32 ou32_Count,
                             const uint8 * const opu8_Data);
sint16 x_dflash_get_status(const uint16 ou16_Device, uint32 * const opu32_Status);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
