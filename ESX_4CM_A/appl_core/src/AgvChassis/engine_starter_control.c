//-----------------------------------------------------------------------------
/*! \file       engine_starter_control.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------
// -- Includes ------------------------------------------------------------------------------------------------------
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "engine_starter_control.h"
#include "cleaning_chains_control.h"
#include "suction_fan_control.h"
//#include "propulsion_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

// -- Module Global Variables --------------------------------------------------------------------------------------
static T_EngineStarterControl mt_engine_starter;

// -- Implementation ------------------------------------------------------------------------------------------------

/** \brief Initialize Engine Starter Control
 *
 *  This function initializes the Engine Starter Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkEngineStarter Pointer to Engine Starter checkpoints
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_engineStarterControl(T_UserInterface *_ui,T_ChkPoints_EngineStarter *_chkEngineStarter)
{
    if((_ui == NULL) || (_chkEngineStarter == NULL))
    {
        return C_WARN;
    }

    //populate local copy of RX ui elements
    mt_engine_starter.pu8_engine_running_status = &_ui->t_engine->u8_engineStatus;

    //populate local copy of TX ui elements
    mt_engine_starter.pu8_neutral_safe_status = &_ui->t_display.u8_neutral_safe_status;

    mt_engine_starter.pt_chk = _chkEngineStarter;

    return C_NO_ERR;
}

/** \brief Update Engine Starter Control
 *
 *  This function reads ignition request and internal permissives, computes
 *  Neutral Safe status, and controls the Engine Start output.
 *
 *  \return s16_error Error code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_engineStarterControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_ign_fault_status = FALSE;
    float32 f32_ign_value = 0.0F;

    uint8 u8_start_req = FALSE;
    uint8 u8_engine_off = FALSE;
    uint8 u8_joystick_neutral = FALSE;
    uint8 u8_suction_fan_enabled = FALSE;
    uint8 u8_shaft_drive_enabled = FALSE;

    uint8 u8_neutral_safe = NEUTRAL_SAFE_FALSE;
    uint8 u8_engine_start_cmd = ENGINE_START_CMD_OFF;

    // FR-12.1 Read ignition start hardware input
    s16_error += get_inputFaultStatus("IGNITION_START", &u8_ign_fault_status);

    // IR-12.1 Any invalid or fault reading on operator Ignition Start request
    // shall result in Engine Start output set to OFF
    if(u8_ign_fault_status == FALSE)
    {
        s16_error += get_inputValue("IGNITION_START", &f32_ign_value);
        u8_start_req = (f32_ign_value != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_start_req = FALSE;
    }

    // FR-12.1 Read additional inputs from internal control modules
    if(mt_engine_starter.pu8_engine_running_status != NULL)
    {
        u8_engine_off =
        (*(mt_engine_starter.pu8_engine_running_status) == FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_engine_off = FALSE;
    }

    // FR-12.2 Read Internal Inputs
    //get_joystickNeutralStatus(&u8_joystick_neutral); //TODO_STW add getter propulsion neutral
    getSuctionFanStatus(&u8_suction_fan_enabled);
    getShaftDriveStatus(&u8_shaft_drive_enabled);

    // Checkpoints
    if(mt_engine_starter.pt_chk != NULL)
    {
        mt_engine_starter.pt_chk->u8_eng_off = u8_engine_off;
        mt_engine_starter.pt_chk->u8_start_key = u8_start_req;
        mt_engine_starter.pt_chk->u8_start_suction_fan_off = u8_suction_fan_enabled;
        mt_engine_starter.pt_chk->u8_start_shaft_drive_off = u8_shaft_drive_enabled;
        mt_engine_starter.pt_chk->u8_start_neutral = u8_joystick_neutral;
    }

    // FR-12.4 Compute Neutral Safe
    if((u8_engine_off == TRUE) &&
    (u8_joystick_neutral == TRUE) &&
    (u8_suction_fan_enabled == FALSE) &&
    (u8_shaft_drive_enabled == FALSE))
    {
        u8_neutral_safe = NEUTRAL_SAFE_TRUE;
    }
    else
    {
        u8_neutral_safe = NEUTRAL_SAFE_FALSE;
    }

    // FR-12.3 Output Engine Start Signal only when all permissive are valid
    if((u8_start_req == TRUE) &&
    (u8_engine_off == TRUE) &&
    (u8_joystick_neutral == TRUE) &&
    (u8_suction_fan_enabled == FALSE) &&
    (u8_shaft_drive_enabled == FALSE))
    {
        u8_engine_start_cmd = ENGINE_START_CMD_ON;
    }
    else
    {
        u8_engine_start_cmd = ENGINE_START_CMD_OFF;
    }

    s16_error += set_outputValue("ENGINE_START_SIGNAL",u8_engine_start_cmd );

    // FR-12.5 Transmit Neutral Safe status to display via CAN
    if(mt_engine_starter.pu8_neutral_safe_status != NULL)
    {
        *(mt_engine_starter.pu8_neutral_safe_status) = u8_neutral_safe;
    }

    return s16_error;
}

//EOF
