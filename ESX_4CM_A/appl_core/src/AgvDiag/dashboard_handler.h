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
#include "propulsion_control.h"
#include "engine_starter_control.h"
#include "suction_fan_control.h"
#include "throttle_control.h"
#include "cooling_fan_control.h"
#include "misc_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_ChkPoints_Elevator      gt_elevatorCheckpoints;          //!< Global checkpoints for Elevator Control
extern T_ChkPoints_Header        gt_headerCheckpoints;            //!< Global checkpoints for Header Control
extern T_ChkPoints_CChains       gt_cleaningShaftCheckpoints;     //!< Global checkpoints for Cleaning Chains/Shaft
extern T_ChkPoints_FSweeps       gt_frontSweepsCheckpoints;       //!< Global checkpoints for Front Sweeps
extern T_ChkPoints_RTrap         gt_rotaryTrapCheckpoints;        //!< Global checkpoints for Rotary Trap
extern T_ChkPoints_Propulsion    gt_propCheckpoints;              //!< Global checkpoints for Propulsion Control
extern T_ChkPoints_EngineStarter gt_engineStarterCheckpoints;     //!< Global checkpoints for Engine Starter
extern T_ChkPoints_SFan          gt_suctionFanCheckpoints;        //!< Global checkpoints for Suction Fan
extern T_ChkPoints_Throttle      gt_throttleCheckpoints;          //!< Global checkpoints for Throttle Control
extern T_ChkPoints_CoolingFan    gt_coolingFanCheckpoints;        //!< Global checkpoints for Cooling Fan
extern T_ChkPoints_Mis           gt_miscCheckpoints;              //!< Global checkpoints for Miscellaneous functions

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_dashHandler(void);
sint16 update_dashHandler(void);

#endif /* APPL_CORE_SRC_AGVDIAG_CHECKPOINT_HANDLER_H_ */

