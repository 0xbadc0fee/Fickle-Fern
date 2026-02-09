//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief   Memory allocator API

   \copyright   Copyright 2022 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_MEM_INFO_H___
#define X_MEM_INFO_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "x_mem_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! Heap size information
typedef struct
{
   uint32 u32_Size;     //!< Size of heap
   uint32 u32_UsedSize; //!< Size of used heap
   uint32 u32_FreeSize; //!< Size of free heap
} T_x_mem_info_heap_stats;

//! Handle statistics
typedef struct
{
   uint32 u32_Capacity;  //!< Maximum number of handles as configured by X_MEM_MAKE_RESOURCES(...)
   uint32 u32_Allocated; //!< Currently allocated number of handles
} T_x_mem_info_handle_stats;

//! Safety informations
typedef struct
{
   uint8 u8_SafetyLevel;         //!< Safety level of memory one of X_MEM_SAFETY_LEVEL_ defines
   uint16 u16_Flags;             //!< Caching state, one of X_MEM_FLAGS_ defines
   uint32 u32_OwnerMask;         //!< Owners of memory, combination of X_MEM_OWNER_CORE defines
   const charn * pc_Description; //!< Description of memory
} T_x_mem_safety_info;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_mem_info_get_system_heap_stats(const uint8 ou8_MemIdx, T_x_mem_info_heap_stats * const opt_Stats);

void x_mem_info_get_handle_stats(T_x_mem_info_handle_stats * const opt_Stats);

sint16 x_mem_info_get_safety_info(const uint32 ou32_Address, const uint32 ou32_Size,
                                  T_x_mem_safety_info * const opt_SafetyInfo);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_MEM_ALLOCATOR_H___
