//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Jan 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/**
 * \file       checkpoint_handler.c
 * \brief      AgvChassis - Checkpoint Handler
 *
 * \addtogroup AgvDiag
 * @{
 * \addtogroup CheckpointHandler Checkpoint Handler
 *
 * The Checkpoint Handler manages the recording and verification of system
 * checkpoints, ensuring data persistence and state recovery across
 * mission cycles or system restarts.
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
#include "checkpoints_data_pool.h"

//Include Controls that have checkpoints
#include "elevator_control.h"
#include "header_lift_control.h"
#include "cleaning_chains_control.h"
#include "front_sweeps_control.h"
#include "rotary_trap_control.h"

//Include SPNS (current location for DP Assignment MACRO)
#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_ChkPoints_Elevator gt_elevatorCheckpoints;      //!<structure that holds all AgvWork - Elevator Control Checkpoints
T_ChkPoints_Header   gt_headerCheckpoints;        //!<structure that holds all AgvWork - Header Control Checkpoints
T_ChkPoints_CChains  gt_cleaningShaftCheckpoints; //!<Structure that holds Cleaning Chains Checkpoints
T_ChkPoints_FSweeps  gt_frontSweepsCheckpoints;	  //!<Structure that holds Front Sweeps Checkpoints
T_ChkPoints_RTrap    gt_rotaryTrapCheckpoints;	  //!<Structure that holds Rotary Traps Checkpoints

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/**
 * \brief Updates the Checkpoint Handler state machine.
 *
 * This function processes the current system state, evaluates if a new
 * checkpoint needs to be written to non-volatile memory, and handles
 * the validation of existing checkpoint integrity.
 *
 * \return sint16
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 update_checkpointHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(CHECKPOINTS_DATA_POOL_INDEX);

    //Expand out the Checkpoint Mapping File
    #define CNTRL2DP(name, CNTRL_VALUE, DPL_VALUE) VAR_ASSIGN((DPL_VALUE), (CNTRL_VALUE));
    #include "checkpoint_map.def"
    #undef CNTRL2DP

    (void)osy_dph_unlock_data_pool(CHECKPOINTS_DATA_POOL_INDEX);

    return s16_error;
}

//EOF
