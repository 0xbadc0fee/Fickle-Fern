//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to data pool handler NVM functions.
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPH_NVM_HANDLERH
#define OSY_DPH_NVM_HANDLERH

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
extern sint16 osy_dph_nvm_read_list(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex);
extern sint16 osy_dph_nvm_read_list_safe(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex);
extern sint16 osy_dph_nvm_write_list(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex);
extern sint16 osy_dph_nvm_write_list_safe(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
