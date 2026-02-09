//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool handler data set functions.
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPH_DATA_SET_HANDLERH
#define OSY_DPH_DATA_SET_HANDLERH

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
extern sint16 osy_dph_apply_data_set(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                     const uint16 ou16_DataSetIndex);

extern sint16 osy_dph_get_data_set_value(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                         const uint16 ou16_ElementIndex, const uint16 ou16_DataSetIndex,
                                         const uint8 ou8_ElementType, const uint16 ou16_ElementSize,
                                         const void ** const oppv_DataSetValue);
/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
