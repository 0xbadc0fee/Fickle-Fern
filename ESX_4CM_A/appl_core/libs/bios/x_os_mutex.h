//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      RTOS API - Mutex functions

   Module provides the implementation of the mutex functions of the RTOS API layer.

   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_MUTEX_H
#define X_OS_MUTEX_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_os_mutex_init(const uint16 ou16_SafetyLevel, T_x_os_mutex ** const oppt_Mutex);
sint16 x_os_mutex_get(T_x_os_mutex * const opt_Mutex, const uint32 ou32_TimeOut);
sint16 x_os_mutex_release(T_x_os_mutex * const opt_Mutex);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
