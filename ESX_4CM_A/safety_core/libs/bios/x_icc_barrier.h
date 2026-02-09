//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       ICC barrier function for system-wide task startup synchronisation.
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_ICC_BARRIER_H
#define  X_ICC_BARRIER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ICC_BARRIER_ID_MAX                 31u         //!< Maximum value for barrier id
#define X_ICC_BARRIER_TIMEOUT_NOWAIT         0xFFFFFFFFu //!< Don't wait until barrier id has been reached
#define X_ICC_BARRIER_TIMEOUT_INFINITE       0u          //!< No timeout, wait forever

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_icc_barrier_wait_for(const uint8 ou8_BarrierId, const uint32 ou32_TimeOut);

#ifdef __cplusplus
}
#endif

#endif
