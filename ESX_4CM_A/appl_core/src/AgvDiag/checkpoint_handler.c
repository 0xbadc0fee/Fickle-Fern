//-----------------------------------------------------------------------------
/*! \file       checkpoint_handler.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "stwtypes.h"
#include "stwerrors.h"

//Include OSY Diagnostic Datapool headers
#include "checkpoints_data_pool.h"

//Include Controls that have checkpoints
#include "elevator_control.h"

//Include SPNS (current location for DP Assignment MACRO)
#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_ChkPoints_Elevator gt_elevatorCheckpoints;    //!<structure that holds all AgvWork - Elevator Control Checkpoints

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
sint16 update_checkpointHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_STW - Safe Datapool Lock

    //Expand out the Checkpoint Mapping File
    #define CNTRL2DP(name, DP_LVALUE, SRC_RVALUE) DP_ASSIGN((DP_LVALUE), (SRC_RVALUE));
    #include "checkpoint_map.def"
    #undef CNTRL2DP

    //TODO_STW - Safe Datapool Release

    return s16_error;
}




//EOF
