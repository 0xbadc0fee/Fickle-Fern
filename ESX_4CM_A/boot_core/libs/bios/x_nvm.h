//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      functions for user-nvm access

   The non volatile memory (nvm) attached to the system shall be separated into regions allocated for user- and
   system-usage ("user-nvm" and "system-nvm" respectively). NVM regions are addressed separately with logical addresses
   in the range [0, x-nvm size).

   ! This header declares the public API for the user-nvm. !

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_NVM_H___
#define X_NVM_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "stwerrors.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/*! \brief  nvm status of Pages*/
typedef struct
{
   uint32 u32_OpenPages; ///< Number of modified pages (wrt. to physically stored data) inside the range of pages
   ///< touched by the specified user-nvm block.
   uint32 u32_FaultyPages; ///< Number of faulty pages inside the range of pages touched by the specified user-nvm
   ///< memory block
} T_x_nvm_status;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_nvm_read(uint32 const ou32_Address, uint32 const ou32_Count, uint8 * const opu8_Data);
sint16 x_nvm_write(uint32 const ou32_Address, uint32 const ou32_Count, uint8 const * const opu8_Data);
sint16 x_nvm_flush(void);
uint32 x_nvm_get_size(void);
uint32 x_nvm_get_page_size(void);
sint16 x_nvm_get_status(uint32 const ou32_Address, uint32 const ou32_Size, T_x_nvm_status * const opt_Status);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_NVM_H___
