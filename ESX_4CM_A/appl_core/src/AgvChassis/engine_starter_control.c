//-----------------------------------------------------------------------------
/**
 * \file       engine_starter_control.c
 * \brief      AgvChassis - Engine Starter Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup EngineStarterControl Engine Starter Control
 *
 * This module manages the operation of the vehicle's engine starter motor,
 * cranking sequence, and associated safety interlocks.
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
//-----------------------------------------------------------------------------
// -- Includes ------------------------------------------------------------------------------------------------------
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "engine_starter_control.h"
#include "cleaning_chains_control.h"
#include "suction_fan_control.h"
#include "propulsion_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "can_engine.h"

// -- Function Prototypes --------------------------------------------------------------------------------------
void check_engineStatus(void);

// -- Module Global Variables --------------------------------------------------------------------------------------
static T_EngineControl mt_engine;//!< Internal state instance for managing engine control operations.

// -- Implementation ------------------------------------------------------------------------------------------------

/** \brief Initialize Engine Starter Control
 *
 * This function initializes the Engine Starter Control Logic.
 *
 * \param _can_devs Pointer to the CAN devices structure
 * \param _chkEngineStarter Pointer to Engine Starter checkpoints
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_engineStarterControl(T_CANDevices *_can_devs, T_ChkPoints_EngineStarter *_chkEngineStarter)
{
    if((_can_devs == NULL) || (_chkEngineStarter == NULL))
    {
        return C_WARN;
    }

    //populate local copy of RX ui elements
    mt_engine.pu16_engine_speed = &_can_devs->t_engine.u16_engine_speed;

    //populate local copy of TX ui elements
    mt_engine.pu8_neutral_safe_status = &_can_devs->t_display.u8_neutral_safe_status;

    mt_engine.pt_chk = _chkEngineStarter;

    //Initialize Local Variables
    mt_engine.u32_engine_start_time = 0;
    mt_engine.u8_engine_start_cmd = FALSE;
    mt_engine.u8_engine_status = ENGINE_OFF;
    mt_engine.u8_prev_engine_status = ENGINE_OFF;

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
    uint8 u8_joystick_neutral = FALSE;
    uint8 u8_suction_fan_status = FALSE;
    uint8 u8_shaft_drive_status = FALSE;
    uint8 u8_neutral_safe = NEUTRAL_SAFE_FALSE;


    // FR-12.1 Read ignition start hardware input
    s16_error += get_inputFaultStatus("IGNITION_SWITCH", &u8_ign_fault_status);

    // IR-12.1 Any invalid or fault reading on operator Ignition Start request
    // shall result in Engine Start output set to OFF
    if(u8_ign_fault_status == FALSE)
    {
        s16_error += get_inputValue("IGNITION_SWITCH", &f32_ign_value);
        u8_start_req = (f32_ign_value != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_start_req = FALSE;
    }

    // FR-12.1 Read additional inputs from internal control modules
    check_engineStatus();

    get_joystickNeutralStatus(&u8_joystick_neutral);
    get_suctionFanStatus(&u8_suction_fan_status);
    get_shaftDriveStatus(&u8_shaft_drive_status);

    // Checkpoints
    if(mt_engine.pt_chk != NULL)
    {
        mt_engine.pt_chk->u8_eng_status = mt_engine.u8_engine_status;
        mt_engine.pt_chk->u8_start_key = u8_start_req;
        mt_engine.pt_chk->u8_suction_fan_status = u8_suction_fan_status;
        mt_engine.pt_chk->u8_shaft_drive_status = u8_shaft_drive_status;
        mt_engine.pt_chk->u8_js_neutral = u8_joystick_neutral;
    }

    // FR-12.4 Compute Neutral Safe
    if((mt_engine.u8_engine_status == ENGINE_OFF) &&
    (u8_joystick_neutral == TRUE) &&
    (u8_suction_fan_status == X_OFF) &&
    (u8_shaft_drive_status == X_OFF))
    {
        u8_neutral_safe = NEUTRAL_SAFE_TRUE;
    }
    else
    {
        u8_neutral_safe = NEUTRAL_SAFE_FALSE;
    }

    // FR-12.3 Output Engine Start Signal only when all permissive are valid
    if((u8_start_req == TRUE) && (u8_neutral_safe == NEUTRAL_SAFE_TRUE))
    {
        mt_engine.u8_engine_start_cmd = ENGINE_START_CMD_ON;

    }
    else
    {
        mt_engine.u8_engine_start_cmd = ENGINE_START_CMD_OFF;
    }

    s16_error += set_outputValue("STARTER_RELAY",mt_engine.u8_engine_start_cmd );

    // FR-12.5 Transmit Neutral Safe status to display via CAN
    if(mt_engine.pu8_neutral_safe_status != NULL)
    {
        *(mt_engine.pu8_neutral_safe_status) = u8_neutral_safe;
    }

    mt_engine.u8_prev_engine_status = mt_engine.u8_engine_status;

    return s16_error;
}

/** \brief Get AgChassis - Engine Status
 *
 *  This function accesses the engine status - running or off
 *
 *  \ref ENGINE_RUNNING
 *  \ref ENGINE_OFF
 *
 * *  \param pu8_engine_start_status Pointer to the Engine start status
 *
 *   \return void
 */
void get_engineStatus(uint8 *pu8_engine_status)
{
    if(pu8_engine_status != NULL)
    {
        *pu8_engine_status = mt_engine.u8_engine_status;
    }
}

/** \brief Get AgChassis - Engine Run Time
 *
 *  This function accesses how long the engine has been running since controller startup
 *
 * *  \param pu32_engine_start_status Pointer variable to hold engine run time
 *
 *   \return void
 */
void get_engineRuntime(uint32 *pu32_engine_runtime)
{
    if(pu32_engine_runtime != NULL)
    {
        if(mt_engine.u8_engine_status != ENGINE_RUNNING)
            *pu32_engine_runtime = get_system_time_ms() - mt_engine.u32_engine_start_time;
        else
            *pu32_engine_runtime = 0;
    }
}

/** \brief Get AgChassis - Engine Status
 *
 *  This function Engine Start Status
 *
 * *  \param pu8_engine_off_status Pointer to the Engine ON/OFF Status
 *
 *  \return
 */
void check_engineStatus(void)
{
    float32 f32_engine_speed;

    if((mt_engine.pu16_engine_speed != NULL))
    {
        f32_engine_speed = *(mt_engine.pu16_engine_speed) * 0.125 ;

        if(f32_engine_speed < 450u)
        {
            mt_engine.u8_engine_status = ENGINE_OFF;
        }
        else
        {
            mt_engine.u8_engine_status = ENGINE_RUNNING;
            if(mt_engine.u8_prev_engine_status == ENGINE_OFF)
                mt_engine.u32_engine_start_time = get_system_time_ms();
        }
    }
}
//EOF
