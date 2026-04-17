//-----------------------------------------------------------------------------
/**
 * \file       dashboard_handler.c
 * \brief      AgvChassis - Dashboard Handler
 *
 * \addtogroup AgvDiag
 * @{
 * \addtogroup DashboardHandler Dashboard Handler
 *
 * This module is responsible for managing system "checkpoints"—critical state
 * variables and diagnostic data that need to be monitored or persisted.
 * It provides a centralized interface for updating, tracking, and validating
 * the operational health of various machine modules.
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
 * Jan 6, 2026 STW Technic
 *
 * @{
 */
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "stwtypes.h"
#include "stwerrors.h"

//Include OSY Diagnostic Datapool headers
#include "osy_dph_data_pool_protector.h"
#include "dashboard_data_pool.h"

//Include Controls that have checkpoints
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

#include "nvm_handler.h"

//Include SPNS (current location for DP Assignment MACRO)
#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_ChkPoints_Elevator gt_elevatorCheckpoints;      //!<Structure that holds all AgvWork - Elevator Control Checkpoints
T_ChkPoints_Header   gt_headerCheckpoints;        //!<Structure that holds all AgvWork - Header Control Checkpoints
T_ChkPoints_CChains  gt_cleaningShaftCheckpoints; //!<Structure that holds Cleaning Chains Checkpoints
T_ChkPoints_FSweeps  gt_frontSweepsCheckpoints;	  //!<Structure that holds Front Sweeps Checkpoints
T_ChkPoints_RTrap    gt_rotaryTrapCheckpoints;	  //!<Structure that holds Rotary Traps Checkpoints
T_ChkPoints_Propulsion gt_propCheckpoints;        //!<Structure that holds Propulsion Checkpoints
T_ChkPoints_EngineStarter gt_engineStarterCheckpoints; //!<Structure that holds the Engine Starter Checkpoints.
T_ChkPoints_SFan gt_suctionFanCheckpoints; //!<Structure that holds the Suction Fan Checkpoints.
T_ChkPoints_Throttle gt_throttleCheckpoints; //!<Structure that holds the Throttle Checkpoints.
T_ChkPoints_CoolingFan gt_coolingFanCheckpoints; //!<Structure that holds the Cooling Fan Checkpoints
T_ChkPoints_Mis gt_miscCheckpoints;              //!<Structure that holds theMiscellaneous Checkpoints

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/**
 * \brief Initializes control variables from the dashboard data pool.
 *
 * This function locks the dashboard data pool, maps the stored dashboard values
 * into their corresponding local control variables using the definition file,
 * and then unlocks the data pool.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_dashHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(DASHBOARD_DATA_POOL_INDEX);

    //Expand out the Checkpoint Mapping File
    #define CNTRL2DP(name, CNTRL_VALUE, DPL_VALUE) VAR_ASSIGN((CNTRL_VALUE), (DPL_VALUE));
    #include "dashboard_map.def"
    #undef CNTRL2DP

    (void)osy_dph_unlock_data_pool(DASHBOARD_DATA_POOL_INDEX);

    return s16_error;
}

/**
 * \brief Updates the dashboard data pool with the latest control values.
 *
 * This function locks the dashboard data pool, pushes the current live values
 * of the control variables back into the data pool using the definition file,
 * and then unlocks the data pool for external access.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 update_dashHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(DASHBOARD_DATA_POOL_INDEX);

    //Expand out the Checkpoint Mapping File
    #define CNTRL2DP(name, CNTRL_VALUE, DPL_VALUE) VAR_ASSIGN((DPL_VALUE), (CNTRL_VALUE));
    #include "dashboard_map.def"
    #undef CNTRL2DP

    (void)osy_dph_unlock_data_pool(DASHBOARD_DATA_POOL_INDEX);

    return s16_error;
}

//EOF
