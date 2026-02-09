//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to data pool access init function.
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPH_INITH
#define OSY_DPH_INITH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_dpa_data_pool.h"

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_dph_init_data_pools(const T_osy_dpa_data_pool * const * const oppt_DataPools,
                                      const uint8 ou8_NumDataPools);
extern sint16 osy_dph_init_public_access(const uint8 ou8_DataPoolIndex);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
