//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Jan 7, 2026 STW Technic
 *
 * \file       nvm_handler.h
 * \brief      Interface for NVM Handler Module.
 *
 * \addtogroup System
 * @{
 * \addtogroup NvmHandler NVM Handler
 * @{
 */
#ifndef APPL_CORE_SRC_SYSTEM_NVM_NVM_HANDLER_H_
#define APPL_CORE_SRC_SYSTEM_NVM_NVM_HANDLER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "nvm_handler_lib.h"
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_stdtypes.h"
#include "x_nvm.h"

#include "elevator_control.h"
#include "header_lift_control.h"
#include "stick_box_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_Config_Elevator        gt_elevatorConfig; //!< Structure that holds the Elevator configuration
extern T_Config_HeaderControl   gt_headerConfig;   //!< Structure that holds the Header Control configuration
extern T_Config_StickBoxControl gt_stickBConfig;   //!< Structure that holds the Stick Box configuration

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_nvmParameters(void);
sint16 write_nvmParameters(void);
sint16 reset_nvmParameters(void);

#endif /* APPL_CORE_SRC_SYSTEM_NVM_NVM_HANDLER_H_ */

