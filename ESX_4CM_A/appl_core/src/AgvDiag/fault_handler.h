/*! \file       fault_handler.h
    \brief      <description>


   	\implementation
   	project     Flory_8772-4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 20, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_AGVDIAG_FAULT_HANDLER_H_
#define APPL_CORE_SRC_AGVDIAG_FAULT_HANDLER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "alarm_handler_lib.h"
#include "input_handler_lib.h"
#include "output_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_faultHandler(void);
sint16 update_faultHandler(void);
sint16 clear_machineFaults(void);

#endif /* APPL_CORE_SRC_AGVDIAG_FAULT_HANDLER_H_ */

