//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool handler protector functions.
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPH_DATA_POOL_PROTECTORH
#define OSY_DPH_DATA_POOL_PROTECTORH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_dph_lock_data_pool(const uint8 ou8_DataPoolIndex);
extern sint16 osy_dph_try_lock_data_pool(const uint8 ou8_DataPoolIndex, const uint32 ou32_TimeoutUs);
extern sint16 osy_dph_unlock_data_pool(const uint8 ou8_DataPoolIndex);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
