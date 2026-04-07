//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/**
 * \file       pid_output.c
 * \brief      AgvHelper - PID Output
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup PidOutput PID Output
 *
 * The PID Calculation Module determines a bounded actuator command by
 * summing proportional, integral, and derivative responses to the
 * difference between setpoint and measured feedback each control cycle.
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
#include "x_stdtypes.h"
#include <stdint.h>
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "pid_output.h"
#include "../HAL/STW_4CM_HAL/system.h"
#include "math.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/**
 * \brief Calculate PID Output
 *
 * This function calculates the bounded control effort for a PID loop by
 * processing the error between the desired setpoint (command) and actual
 * measurement (feedback). It updates the persistent state of the controller,
 * including integral accumulation and error history.
 *
 * \param[in]     f32_command  Desired target setpoint
 * \param[in]     f32_feedback Current measured system feedback
 * \param[in,out] t_pid_state  Pointer to persistent state (error/time history)
 * \param[in]     t_PID_coeff  Pointer to loop coefficients and output limits
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 PidOutput(float32 f32_command, float32 f32_feedback,T_PID_state *t_pid_state, T_PID_coeff *t_PID_coeff)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_error;
    float32 f32_pPart;
    float32 f32_iPart;
    float32 f32_dPart;

    uint64 u64_currentTime;

    float32 f32_kp;
    float32 f32_ki;
    float32 f32_kd;

    //IR-17.2 Param Check
    if((t_pid_state == NULL) || (t_PID_coeff == NULL))
    {
        return C_WARN;
    }

    //FR-17.1 Control Rate
    u64_currentTime = get_system_time_us();  //Calculate delta t, used in Ipart and Dpart

    //IR-17.1 Compute Control Error
    if(isnan(f32_command) || isnan(f32_feedback))
    {
        t_pid_state->f32_output = 0.0f;
        t_pid_state->f32_error_accum = 0.0f;
        t_pid_state->f32_prev_error = 0.0f;
        t_pid_state->u64_last_time = u64_currentTime;
        return C_WARN;
    }

    //FR-17.2 Compute Control Error
    f32_error = f32_command - f32_feedback;  //Calculate error between target and actual pressures

    if(t_pid_state->u64_last_time == 0u)
    {
        t_pid_state->u64_last_time = u64_currentTime;
        t_pid_state->f32_prev_error = f32_error;
        t_pid_state->f32_error_accum = 0.0F;
        t_pid_state->f32_output = 0.0F;
        return C_NO_ERR;
    }

    t_pid_state->f32_error_accum += (((f32_error+t_pid_state->f32_prev_error)*(u64_currentTime-t_pid_state->u64_last_time))/2E6f);

    //Error check PID errors for NAN.  If anything is NAN, then set the NAN value to 0
    if(isnan(f32_error))
    {
        f32_error = 0.0F;
    }
    if(isnan(t_pid_state->f32_error_accum))
    {
        t_pid_state->f32_error_accum = 0;
    }

    //FR-17.3 Read PID coEfficients
    f32_kp = t_PID_coeff->f32_kp;
    f32_ki = t_PID_coeff->f32_ki;
    f32_kd = t_PID_coeff->f32_kd;

    //IR-17.2 Invlid PID Params
    if(isnan((double)f32_kp) || isnan((double)f32_ki) || isnan((double)f32_kd))
    {
        t_pid_state->f32_output = 0.0f;
        t_pid_state->f32_error_accum = 0.0f;
        t_pid_state->f32_prev_error = 0.0f;
        t_pid_state->u64_last_time = u64_currentTime;
        return C_WARN;

    }

    //FR-17.3: Proportional and Integral Terms
    f32_pPart = f32_kp * f32_error;
    f32_iPart = f32_ki *(t_pid_state->f32_error_accum);

    if (f32_iPart>t_PID_coeff->s32_max_output)
    {
        t_pid_state->f32_error_accum = (t_PID_coeff->s32_max_output/t_PID_coeff->f32_ki);
    }
    else
    {
        if (f32_iPart<t_PID_coeff->s32_min_output)
        {
            t_pid_state->f32_error_accum = ((t_PID_coeff->s32_min_output)/t_PID_coeff->f32_ki);
        }
    }

    f32_dPart = f32_kd*(1E6f*(f32_error-t_pid_state->f32_prev_error)/(u64_currentTime-t_pid_state->u64_last_time));

    t_pid_state->f32_prev_error=f32_error;
    t_pid_state->u64_last_time=u64_currentTime;

    //FR-17.4 Sum proportional, integral, and derivative terms
    t_pid_state->f32_output = (float32)(f32_pPart+f32_iPart+f32_dPart);

    //FR-17.5: Limit control output within bounds
    if (t_pid_state->f32_output>t_PID_coeff->s32_max_output)
    {
        t_pid_state->f32_output=t_PID_coeff->s32_max_output;
    }
    else
    {
        if (t_pid_state->f32_output<t_PID_coeff->s32_min_output)
        {
            t_pid_state->f32_output=t_PID_coeff->s32_min_output;
        }
    }
    return s16_error;
}

//EOF
