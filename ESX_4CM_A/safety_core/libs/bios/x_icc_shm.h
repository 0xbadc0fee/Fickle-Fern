//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief     Shared memory Safety-API for Inter-Core-Communication

   This module defines the application interface for the Shared Memory of the Inter-Core-Communication
   for Safety Cores.

   \copyright   Copyright Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_ICC_SHM_H
#define  X_ICC_SHM_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_memtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ICC_SHM_TIMEOUT_INFINITE                 0u //!< Timeout value for infinite timeout
#define X_ICC_SHM_TIMEOUT_NOWAIT          0xFFFFFFFFu //!< Timeout value for non-waiting ( return immediately)

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! Shared memory handle structure (Opaque definition).
typedef struct T_x_icc_shm_obj_struct T_x_icc_shm_obj;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_icc_shm_create_static(const uint16 ou16_Id, const uint32 ou32_Size, void * const opv_Mem,
                               T_x_icc_shm_obj ** const oppt_Handle);

sint16 x_icc_shm_attach(const uint16 ou16_Id, uint32 * const opu32_Size, void ** const oppv_Mem,
                        T_x_icc_shm_obj ** const oppt_Handle);

sint16 x_icc_shm_lock(T_x_icc_shm_obj * const opt_Handle);

sint16 x_icc_shm_trylock(T_x_icc_shm_obj * const opt_Handle, const uint32 ou32_TimeoutUs);

sint16 x_icc_shm_unlock(const T_x_icc_shm_obj * const opt_Handle);

sint16 x_icc_shm_get_size(const T_x_icc_shm_obj * const opt_Handle, uint32 * const opu32_Size);

#ifdef __cplusplus
}
#endif

#endif
