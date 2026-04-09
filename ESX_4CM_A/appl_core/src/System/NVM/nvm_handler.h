//-----------------------------------------------------------------------------
/**
 * \file       nvm_handler.h
 * \brief      AgvCore - NVM Handler Module
 *
 * \addtogroup System
 * @{
 * \addtogroup NvmHandler NVM Handler
 *
 * The NVM Handler module manages the application-specific reading, writing,
 * and storage mapping of non-volatile memory parameters. It interfaces
 * with the underlying NVM library to ensure machine settings, states,
 * and fault data are properly formatted and safely preserved across
 * power cycles.
 *
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Jan 7, 2026 STW Technic
 *
 * @{
 */
//-----------------------------------------------------------------------------
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
#include "power_assist_control.h"
#include "suction_fan_control.h"
#include "misc_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_Config_Elevator           gt_elevatorConfig;      //!< Global configuration for Elevator Control
extern T_Config_HeaderControl      gt_headerConfig;        //!< Global configuration for Header Control
extern T_Config_StickBoxControl    gt_stickBConfig;        //!< Global configuration for Stick Box Control
extern T_Config_PowerAssistControl gt_paConfig;            //!< Global configuration for Power Assist Control
extern T_Config_SFan               gt_suctionFanConfig;    //!< Global configuration for Suction Fan
extern T_Config_MiscrControl       gt_miscConfig;          //!< Global configuration for Miscellaneous functions

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_nvmParameters(void);
sint16 write_nvmParameters(void);
sint16 reset_nvmParameters(void);

#endif /* APPL_CORE_SRC_SYSTEM_NVM_NVM_HANDLER_H_ */

