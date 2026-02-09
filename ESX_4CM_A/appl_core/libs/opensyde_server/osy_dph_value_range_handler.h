//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool handler value range handler functions.
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPH_VALUE_RANGE_HANDLERH
#define OSY_DPH_VALUE_RANGE_HANDLERH

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
extern sint16 osy_dph_verify_range_list(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex);
extern sint16 osy_dph_verify_range_element(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                           const uint16 ou16_ElementIndex);
extern sint16 osy_dph_verify_range_application_element(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                                       const uint16 ou16_ElementIndex, const uint8 ou8_ElementType,
                                                       const uint16 ou16_ElementSize, const void * const opv_Value);
extern sint16 osy_dph_get_range_element(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                        const uint16 ou16_ElementIndex, const uint8 ou8_ElementType,
                                        const uint16 ou16_ElementSize, const void ** const oppv_MinimumValue,
                                        const void ** const oppv_MaximumValue);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
