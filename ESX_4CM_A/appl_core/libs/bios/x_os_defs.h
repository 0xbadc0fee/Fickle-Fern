//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Target specific RTOS definitions

   This module provides target specific parts of the RTOS API layer.

   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_DEFS_H
#define X_OS_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_mem_defs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//! \brief HEAP memory that can be selected by T_x_os_task_config.u32_MemClass for Task stack allocation
#define X_OS_STACKS_DSPR      X_MEM_HEAP_STACKS_DSPR //!< Select dynamic stack memory in DSPR
#define X_OS_STACKS_EMEM      X_MEM_HEAP_STACKS_EMEM //!< Select dynamic stack memory in EMEM

//! \brief RTOS safety levels (attention: not all available for use in T_x_os_task_config.u16_SafetyLevel)
#define X_OS_SAFETY_LEVEL_QM      X_MEM_SAFETY_LEVEL_QM    //!< QM Task: No write access to safety critical data
#define X_OS_SAFETY_LEVEL_SAFE    X_MEM_SAFETY_LEVEL_SAFE  //!< Safe Task: Write access to safety critical data
#define X_OS_SAFETY_LEVEL_COUNT   X_MEM_SAFETY_LEVEL_COUNT //!< Number of safety levels in the system

// deprecated, provided for backwards compatibility
#define X_OS_HEAP_ID_FAST                        X_OS_STACKS_DSPR
#define X_OS_HEAP_ID_SRAM                        X_OS_STACKS_EMEM
#define X_OS_TASK_SAFETY_LEVEL_NON_INTERFERING   X_OS_SAFETY_LEVEL_QM

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_OS_DEFS_H
