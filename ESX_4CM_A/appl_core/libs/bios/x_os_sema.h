//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      RTOS API - Counting semaphore functions
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_SEMA_H
#define X_OS_SEMA_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_os_sema_init(const uint16 ou16_Count, const uint16 ou16_SafetyLevel, T_x_os_sema ** const oppt_Semaphore);
sint16 x_os_sema_reinit(T_x_os_sema * const opt_Semaphore, const uint16 ou16_Count);
sint16 x_os_sema_get(T_x_os_sema * const opt_Semaphore, const uint32 ou32_Timeout);
sint16 x_os_sema_release(T_x_os_sema * const opt_Semaphore);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
