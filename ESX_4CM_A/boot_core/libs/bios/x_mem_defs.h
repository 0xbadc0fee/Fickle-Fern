//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Target-specific configuration for memory management
   \copyright   Copyright 2022 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_MEM_DEFS_H
#define X_MEM_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//! Map logical heap names to array index in mat_HeapAreas
#define X_MEM_HEAP_MALLOC         (0u) //!< Malloc heap in EMEM SRAM. This define is mandatory
#define X_MEM_HEAP_SYS_INTERN     (1u) //!< Internal System Heap in EMEM SRAM
#define X_MEM_HEAP_STACKS_DSPR    (2u) //!< Dynamic task stack in DSPR
#define X_MEM_HEAP_STACKS_EMEM    (3u) //!< Dynamic task stack in EMEM
#define X_MEM_HEAP_NVM            (4u) //!< Application defined HEAP in DSPR (used for task stack)
#define X_MEM_HEAP_COUNT          (5u) //!< number of defined heap areas

//! Define the different safety levels that are available in the system
#define X_MEM_SAFETY_LEVEL_QM      (0u) //!< QM resources
#define X_MEM_SAFETY_LEVEL_SAFE    (1u) //!< Safe memory resources, protected against access by QM tasks
#define X_MEM_SAFETY_LEVEL_COUNT   (2u) //!< Number of different safety levels in the system

// Deprecated safety level macros
/*PRQA S 2217  ++*/ /*STW_MD4_2217_FORMATTING_RULE_SUPPRESSION_BEGIN*/
#define X_MEM_SAFETY_LEVEL_NON_INTERFERING   _Pragma("GCC warning \"'X_MEM_SAFETY_LEVEL_NON_INTERFERING' macro is deprecated, use 'X_MEM_SAFETY_LEVEL_QM'\"") X_MEM_SAFETY_LEVEL_QM
#define X_MEM_SAFETY_LEVEL_SIL2              _Pragma("GCC warning \"'X_MEM_SAFETY_LEVEL_SIL2' macro is deprecated, use 'X_MEM_SAFETY_LEVEL_SAFE'\"") X_MEM_SAFETY_LEVEL_SAFE
/*PRQA S 2217  --*/ /*FORMATTING_RULE_SUPPRESSION_END*/

#define X_MEM_OWNER_CORE0        0x01u //!< Owner of memory address is core0
#define X_MEM_OWNER_CORE1        0x02u //!< Owner of memory address is core1
#define X_MEM_OWNER_CORE2        0x04u //!< Owner of memory address is core2

#define X_MEM_FLAGS_CACHABLE     0x0001u //!< Cacheable memory
#define X_MEM_FLAGS_CACHED       0x0002u //!< Cached memory
#define X_MEM_FLAGS_FLASH        0x0004u //!< Flash memory

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_MEM_DEFS_H
