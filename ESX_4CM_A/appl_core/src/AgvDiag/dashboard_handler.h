//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Jan 6, 2026 STW Technic
 *
 * \file       checkpoint_handler.h
 * \brief      Interface for Checkpoint Handler.
 *
 * \addtogroup AgvDiag
 * @{
 * \addtogroup CheckpointHandler Checkpoint Handler
 * @{
 */
#ifndef APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_
#define APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "elevator_control.h"
#include "header_lift_control.h"
#include "cleaning_chains_control.h"
#include "front_sweeps_control.h"
#include "rotary_trap_control.h"
#include "propulsion_control.h"
#include "engine_starter_control.h"
#include "suction_fan_control.h"
#include "throttle_control.h"
#include "cooling_fan_control.h"
#include "misc_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_ChkPoints_Elevator gt_elevatorCheckpoints;
extern T_ChkPoints_Header  gt_headerCheckpoints;
extern T_ChkPoints_CChains gt_cleaningShaftCheckpoints;
extern T_ChkPoints_FSweeps gt_frontSweepsCheckpoints;
extern T_ChkPoints_RTrap   gt_rotaryTrapCheckpoints;
extern T_ChkPoints_Propulsion gt_propCheckpoints;
extern T_ChkPoints_EngineStarter gt_engineStarterCheckpoints;
extern T_ChkPoints_SFan gt_suctionFanCheckpoints;
extern T_ChkPoints_Throttle gt_throttleCheckpoints;
extern T_ChkPoints_CoolingFan gt_coolingFanCheckpoints;
extern T_ChkPoints_Mis gt_miscCheckpoints;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_dashHandler(void);
sint16 update_dashHandler(void);

#endif /* APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_ */

