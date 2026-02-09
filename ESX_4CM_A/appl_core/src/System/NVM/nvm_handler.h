/*! \file       nvm_handler.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Jan 7, 2026 kyle.boch
   	\endimplementation
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

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_Config_Elevator gt_elevatorConfig;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_nvmParameters(void);

#endif /* APPL_CORE_SRC_SYSTEM_NVM_NVM_HANDLER_H_ */

