/*! \file       fault_handler_lib.h.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 5, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_HAL_FAULT_HANDLER_LIB_H_
#define APPL_CORE_SRC_HAL_FAULT_HANDLER_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "osy_dph_nvm_handler.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_faultHandler(void);
sint16 update_faultHandler(void);

#endif /* APPL_CORE_SRC_HAL_FAULT_HANDLER_LIB_H_ */

