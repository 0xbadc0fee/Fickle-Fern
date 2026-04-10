//-----------------------------------------------------------------------------
/**
 * \file       front_sweeps_control.c
 * \brief      AgvWork - Front Sweeps Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup FrontSweepsControl Front Sweeps Control
 * This module manages the Front Sweeps control logic. It processes operator
 * Drum V-Sweep commands to regulate PWM outputs, controlling the speed
 * and trajectory of product delivery into the pick-up belt throat.
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
static T_FrontSweepsControl mt_front_sweeps;  //!<Global persistent state for Front Sweeps Control.

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize Front Sweeps Control
 *
 *  \param _can_dev Pointer to UI structure
 *  \param _chkFrontSweeps Pointer to checkpoints
 *
 *  \return s16_error Error code
 */
sint16 init_frontSweepsControl(T_CANDevices *_can_dev, T_ChkPoints_FSweeps *_chkFrontSweeps)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_dev == NULL) || (_chkFrontSweeps == NULL))
    {
        return C_WARN;
    }

    //populate local RX pointers
    mt_front_sweeps.pu8_drum_speed_request   = &_can_dev->t_display.u8_drum_speed_request;
    mt_front_sweeps.pu8_drum_speed_enable   = &_can_dev->t_display.u8_drum_speed_enable;

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
        f32_req = (float32)(*(mt_front_sweeps.pu8_drum_speed_request))* 100.0f;
    }

    get_shaftDriveStatus(&u8_shaft_drive);

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
    mt_front_sweeps.pt_cp_frontsweeps->u8_speed_cmd = *(mt_front_sweeps.pu8_drum_speed_request);

    // FR-3.5 Ramp toward target
    s16_error += rampCalc(f32_target_cmd_pct, &mt_front_sweeps.t_sweeps_ramp);

    // Output fault handling
    s16_error += get_outputFaultStatus("DV_SWEEP", &u8_output_fault);
    if(u8_output_fault == TRUE)
    {
        s16_error += set_outputValue("DV_SWEEP", FRONT_SWEEPS_PWM_SAFE_STATE);
    }
    else
    {
        // Output command
        s16_error += set_outputValue("DV_SWEEP", mt_front_sweeps.t_sweeps_ramp.f32_output);
    }

    return s16_error;
}
