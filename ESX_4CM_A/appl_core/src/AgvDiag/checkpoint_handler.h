/*! \file       checkpoint_handler.h.h
    \brief      <description>

   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Jan 7, 2026 kyle.boch
*/
#ifndef APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_
#define APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "elevator_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_ChkPoints_Elevator gt_elevatorCheckpoints;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 update_checkpointHandler(void);

#endif /* APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_ */

