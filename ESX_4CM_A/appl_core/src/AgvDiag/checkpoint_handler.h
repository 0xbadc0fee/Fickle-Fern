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
#include "header_lift_control.h"
#include "cleaning_chains_control.h"
#include "front_sweeps_control.h"
#include "rotary_trap_control.h"
#include "engine_starter_control.h"
#include "suction_fan_control.h"
#include "throttle_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_ChkPoints_Elevator gt_elevatorCheckpoints;
extern T_ChkPoints_Header  gt_headerCheckpoints;
extern T_ChkPoints_CChains gt_cleaningShaftCheckpoints;
extern T_ChkPoints_FSweeps gt_frontSweepsCheckpoints;
extern T_ChkPoints_RTrap   gt_rotaryTrapCheckpoints;
extern T_ChkPoints_EngineStarter gt_engineStarterCheckpoints;
extern T_ChkPoints_SFan gt_suctionFanCheckpoints;
extern T_ChkPoints_Throttle gt_throttleCheckpoints;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 update_checkpointHandler(void);

#endif /* APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_ */

