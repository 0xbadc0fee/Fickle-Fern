//-----------------------------------------------------------------------------
/*! \file       rotary_trap_control.c
    \brief      The Rotary Trap Control Module shall read the operator request
    commands and convert the Rotary Trap Speed to a proportional PWM output to
    control the speed of the windrow feed.

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
#include "math.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "rotary_trap_control.h"
#include "cleaning_chains_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "propulsion_control.h"

#include "checkpoints_data_pool.h"

// -- Defines ------------------------------------------------------------------------------------------------------

// -- Types --------------------------------------------------------------------------------------------------------
// -- Module Global Function Prototypes ----------------------------------------------------------------------------
// -- Module Global Variables --------------------------------------------------------------------------------------
static T_RotaryTrapControl mt_rotary_trap;

// -- Implementation  ----------------------------------------------------------------------------------------------

/** \brief Initialize Rotary Trap Control
 *
 *  \param _ui Pointer to UI structure
 *
 *  \return s16_error Error code
 */
sint16 init_rotaryTrapControl(T_CANDevices *_can_dev, T_ChkPoints_RTrap *_chkRotaryTrap)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_dev == NULL) || (_chkRotaryTrap == NULL))
       {
           return C_WARN;
       }

    // Populate local RX pointers
    mt_rotary_trap.pu8_trap_speed_increase = &_can_dev->t_joystick.u8_b3_state;
    mt_rotary_trap.pu8_trap_speed_range = &_can_dev->t_display.u8_trap_speed_range;

    //populate local copy of checkpoints
    mt_rotary_trap.pt_cp_rotarytrap = _chkRotaryTrap;

    s16_error += rampInit(&mt_rotary_trap.t_trap_ramp,
    ROTARY_TRAP_RAMP_RATE,
    ROTARY_TRAP_LOW_MIN,
    ROTARY_TRAP_LOW_MAX,
    ROTARY_TRAP_PWM_SAFE_STATE);

    return s16_error;
}

/** \brief Update Rotary Trap Control
 *
 *  \return s16_error Error code
 */
sint16 update_rotaryTrapControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_shaft_drive = FALSE;
    uint8 u8_trap_speed_increase = FALSE;
    uint8 u8_output_fault = FALSE;
    uint8 u8_range = ROTARY_TRAP_RANGE_LOW;

    float32 f32_wheel_speed_mph = 0.0F;
    float32 f32_target_cmd_pct = ROTARY_TRAP_DISABLED;
    float32 f32_final_cmd_pct = ROTARY_TRAP_DISABLED;
    float32 f32_range_min = ROTARY_TRAP_LOW_MIN;
    float32 f32_range_max = ROTARY_TRAP_LOW_MAX;

    // Validate pointers
    if((mt_rotary_trap.pu8_trap_speed_increase == NULL) ||
    (mt_rotary_trap.pu8_trap_speed_range == NULL))
    {
        (void)set_outputValue("ROTARY_TRAP", ROTARY_TRAP_PWM_SAFE_STATE);
        return C_WARN;
    }

    // FR-4.1 Shaft Drive
    getShaftDriveStatus(&u8_shaft_drive);

    // FR-4.2 Trap Speed Increase
    u8_trap_speed_increase = (*(mt_rotary_trap.pu8_trap_speed_increase) != FALSE) ? TRUE : FALSE;

    // FR-4.3 Wheel Speed
    get_wheelSpeed(&f32_wheel_speed_mph);

    // FR-4.4 Range
    u8_range = *(mt_rotary_trap.pu8_trap_speed_range);

    // Select range (IR-4.2 fallback = no change)
    switch(u8_range)
    {
        case ROTARY_TRAP_RANGE_LOW:
            f32_range_min = ROTARY_TRAP_LOW_MIN;
            f32_range_max = ROTARY_TRAP_LOW_MAX;
            break;

        case ROTARY_TRAP_RANGE_MED:
            f32_range_min = ROTARY_TRAP_MED_MIN;
            f32_range_max = ROTARY_TRAP_MED_MAX;
            break;

        case ROTARY_TRAP_RANGE_MAX:
            f32_range_min = ROTARY_TRAP_MAX_MIN;
            f32_range_max = ROTARY_TRAP_MAX_MAX;
            break;

        default:
            f32_target_cmd_pct = mt_rotary_trap.t_trap_ramp.f32_output;
            s16_error += C_WARN;
            break;

    }

    //set the trap speed ramping object limits
    mt_rotary_trap.t_trap_ramp.f32_min_limit = f32_range_min;
    mt_rotary_trap.t_trap_ramp.f32_max_limit = f32_range_max;

    // Valid range path
    if((u8_range == ROTARY_TRAP_RANGE_LOW) ||
    (u8_range == ROTARY_TRAP_RANGE_MED) ||
    (u8_range == ROTARY_TRAP_RANGE_MAX))
    {
        // FR-4.7 Shaft drive disabled
        if(u8_shaft_drive == FALSE)
        {
            f32_target_cmd_pct = ROTARY_TRAP_DISABLED;
        }
        // FR-4.6 Full speed override
        else if(u8_trap_speed_increase == TRUE)
        {
            f32_target_cmd_pct = f32_range_max;
        }
        // Normal scaling path
        else if(f32_wheel_speed_mph >= 0)
        {
            f32_wheel_speed_mph = CLAMP_F32(f32_wheel_speed_mph, MIN_FIELD_GS, MAX_FIELD_GS);
            f32_target_cmd_pct = (((f32_range_max - f32_range_min) / MAX_FIELD_GS)*f32_wheel_speed_mph) + f32_range_min;
        }
        else
        {
            // IR-4.1 no change
            f32_target_cmd_pct = mt_rotary_trap.t_trap_ramp.f32_output;
        }

        f32_target_cmd_pct = CLAMP_F32(f32_target_cmd_pct, ROTARY_TRAP_PWM_MIN,ROTARY_TRAP_PWM_MAX);
    }

    //Publish checkpoints
    mt_rotary_trap.pt_cp_rotarytrap->f32_trap_target_cmd = f32_target_cmd_pct;


    // FR-4.8 Ramp
    s16_error += rampCalc(f32_target_cmd_pct, &mt_rotary_trap.t_trap_ramp);
    f32_final_cmd_pct = mt_rotary_trap.t_trap_ramp.f32_output;

    // IR-4.3 Output fault
    if((get_outputFaultStatus("ROTARY_TRAP", &u8_output_fault) != C_NO_ERR) ||
    (u8_output_fault == TRUE))
    {
        s16_error += set_outputValue("ROTARY_TRAP", ROTARY_TRAP_PWM_SAFE_STATE);
        return C_WARN;
    }

    s16_error += set_outputValue("ROTARY_TRAP", f32_final_cmd_pct);

    return s16_error;
}
//EOF

