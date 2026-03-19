//-----------------------------------------------------------------------------
/*! \file       front_sweeps_control.c
    \brief      The Front Sweeps Control Module shall read the operator Drum V-Sweep
    commands and convert the requested Drum V-Sweep Speed to a PWM output to control
    how far the product is thrown into the throat of the pick-up belt.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
#include "math.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "front_sweeps_control.h"
#include "cleaning_chains_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_FrontSweepsControl mt_front_sweeps;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize Front Sweeps Control
 *
 *  \param _ui Pointer to UI structure
 *  \param _nvmFrontSweepsControl Pointer to configuration
 *
 *  \return s16_error Error code
 */
sint16 init_frontSweepsControl(T_UserInterface *_ui, T_ChkPoints_FSweeps *_chkFrontSweeps)
{
    sint16 s16_error = C_NO_ERR;

    if((_ui == NULL) || (_chkFrontSweeps == NULL))
    {
        return C_WARN;
    }

    //populate local RX pointers
    mt_front_sweeps.pu8_drum_speed_request   = &_ui->t_display.u8_drum_speed_request;
    mt_front_sweeps.pu8_drum_speed_enable   = &_ui->t_display.u8_drum_speed_enable;

    //populate local copy of checkpoints
    mt_front_sweeps.pt_cp_frontsweeps = _chkFrontSweeps;

    s16_error += rampInit(&mt_front_sweeps.t_sweeps_ramp,
    FRONT_SWEEPS_RAMP_RATE,
    FRONT_SWEEPS_MIN,
    FRONT_SWEEPS_MAX,
    FRONT_SWEEPS_PWM_SAFE_STATE);

    return s16_error;
}

/** \brief Update Front Sweeps Control
 *
 *  \return s16_error Error code
 */
sint16 update_frontSweepsControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_enable = FALSE;
    uint8 u8_shaft_drive = FALSE;
    uint8 u8_output_fault = FALSE;

    float32 f32_req = FRONT_SWEEPS_PWM_SAFE_STATE;
    float32 f32_target_cmd_pct = FRONT_SWEEPS_DISABLED;
    float32 f32_pwm_output_pct = FRONT_SWEEPS_PWM_SAFE_STATE;

    // Validate pointers
    if((mt_front_sweeps.pu8_drum_speed_enable == NULL) ||
    (mt_front_sweeps.pu8_drum_speed_request == NULL))
    {
        u8_enable = FALSE;
        f32_req = FRONT_SWEEPS_PWM_SAFE_STATE;
        s16_error += C_WARN;
    }
    else
    {
        //FR-3.1, FR-3.2 Read inputs
        u8_enable = (*(mt_front_sweeps.pu8_drum_speed_enable) != FALSE) ? TRUE : FALSE;
        f32_req = (float32)(*(mt_front_sweeps.pu8_drum_speed_request));
    }

    getShaftDriveStatus(&u8_shaft_drive);

    // FR-3.4 Sweep Target Speed to zero when Shaft Drive Enable or Drum V-Sweep Enable is disabled.
    if((u8_enable == TRUE) && (u8_shaft_drive == TRUE))
    {
        f32_target_cmd_pct = CLAMP_F32(f32_req, FRONT_SWEEPS_MIN, FRONT_SWEEPS_MAX);
    }
    else
    {
        f32_target_cmd_pct = FRONT_SWEEPS_DISABLED;
    }

    //Publish checkpoints
    mt_front_sweeps.pt_cp_frontsweeps->u8_checkpoint1 = f32_target_cmd_pct;

    // FR-3.5 Ramp toward target
    s16_error += rampCalc(f32_target_cmd_pct, &mt_front_sweeps.t_sweeps_ramp);

    // FR-3.6 Convert final drum speed command to PWM
    if(mt_front_sweeps.t_sweeps_ramp.f32_output <= 0.0F)
    {
        f32_pwm_output_pct = FRONT_SWEEPS_PWM_SAFE_STATE;
    }
    else
    {
        f32_pwm_output_pct = FRONT_SWEEPS_PWM_THRESHOLD_CURRENT +
        ((mt_front_sweeps.t_sweeps_ramp.f32_output * 0.01F) *
        (FRONT_SWEEPS_PWM_END_CURRENT - FRONT_SWEEPS_PWM_THRESHOLD_CURRENT));
    }

    // Output fault handling
    s16_error += get_outputFaultStatus("DV_SWEEP", &u8_output_fault);
    if(u8_output_fault == TRUE)
    {
        s16_error += set_outputValue("DV_SWEEP", FRONT_SWEEPS_PWM_SAFE_STATE);
    }
    else
    {
        // Output command
        s16_error += set_outputValue("DV_SWEEP", f32_pwm_output_pct);
    }

    return s16_error;
}
