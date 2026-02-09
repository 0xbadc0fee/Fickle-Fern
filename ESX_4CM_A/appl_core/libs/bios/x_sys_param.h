//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       API System Function Prototypes for ECU Informations
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SYS_PARAM_H
#define X_SYS_PARAM_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "stwerrors.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

void x_sys_param_get_ecu_serial_number(uint8 opu8_SerialNumber[6]);
void x_sys_param_get_ecu_hw_version(charn opcn_Version[17]); // (16 characters + zero-termination)
void x_sys_param_get_ecu_article_number(uint32 * const opu32_ArticleNumber);
void x_sys_param_get_device_name(charn opcn_DeviceName[29]); // (28 characters + zero-termination)
void x_sys_param_get_company_id(charn opcn_CompanyId[6]);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /* X_SYS_PARAM_H */
