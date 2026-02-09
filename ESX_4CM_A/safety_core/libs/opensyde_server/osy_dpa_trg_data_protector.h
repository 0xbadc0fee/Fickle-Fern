//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       openSYDE server target layer: data protector definition

   Target specific definitions of thread and process lock objects for AURIX.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSDP_TARGET_DATA_PROTECTORH
#define OSDP_TARGET_DATA_PROTECTORH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <stdbool.h>
#include "stwtypes.h"
#include "x_os_types.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

///target specific data for locking against other thread (individual data for each core):
typedef struct
{
   T_x_os_mutex * pt_Mutex; //!< Pointer to mutex object (NULL if thread locking is done via critical section)
} T_osy_dpa_trg_dp_thread_lock;

///target specific data for locking against other cores (shared data from core with local linkage):
typedef struct
{
   _Bool q_UseLock;    //!< Defines wether a process lock is necessary or not
   uint8 u8_ProcessId; //!< Process ID of owner of the data pool to be accessed
} T_osy_dpa_trg_dp_process_lock;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif
