//-----------------------------------------------------------------------------
/*! \file       helper_control.c
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
#include "helper_control.h"
#include "math.h"
#include "x_sys.h"
#include <stdint.h>

#include "stwerrors.h"
#include "stwtypes.h"


/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_PID_state gt_pid_autoGF;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvHelper - PID Output
 *
 *  This function calculates what the PID output should be for a given set of parameters and feedback.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
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

    u64_currentTime = (uint64)x_sys_get_time_us();  //Calculate delta t, used in Ipart and Dpart

    f32_error = f32_command-f32_feedback;  //Calculate error between target and actual pressures
    t_pid_state->f32_error_accum += (((f32_error+t_pid_state->f32_prev_error)*(u64_currentTime-t_pid_state->u64_last_time))/2E6f);

    //Error check PID errors for NAN.  If anything is NAN, then set the NAN value to 0
    if(isnan(f32_error))
    {
        f32_error = 0;
    }
    if(isnan(t_pid_state->f32_error_accum))
    {
        t_pid_state->f32_error_accum = 0;
    }

    f32_kp = t_PID_coeff->f32_kp;
    f32_ki = t_PID_coeff->f32_ki;
    f32_kd = t_PID_coeff->f32_kd;

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

    t_pid_state->f32_output = (float32)(f32_pPart+f32_iPart+f32_dPart);

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


/** \brief Initialize AgvHelper - Ramp Calculation
 *
 *  This function reduces the rate of change between a new target value and the current value over a configurable period of time (linear)
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 rampCalc(float32 f32_target, const T_RampParams *pt_params, T_RampState *pt_state)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_current = 0u;
    float32 f32_step_max = 0u;
    float32 f32_next = 0u;

    //Fault handling
    //todo check all params?
    if ((pt_state == (void*)0 || (pt_params == (void*)0)))
    {
        return C_WARN;
    }

    pt_state->f32_output = CLAMP_F32(pt_state->f32_output, pt_params->f32_min_limit, pt_params->f32_max_limit);

    f32_current = pt_state->f32_output;

    f32_step_max = pt_params->f32_ramp_rate * pt_params->f32_dt_s; //Limit change per cycle

    //Linear transition toward target; hold once reached.
    if(f32_target > f32_current)
    {
        const float32 diff =  f32_target - f32_current;
        const float32 step = (diff > f32_step_max) ? f32_step_max : diff;
        f32_next = f32_current + step;
    }
    else if(f32_target < f32_current)
    {
        const float32 diff = f32_current - f32_target;
        const float32 step = (diff > f32_step_max) ? f32_step_max : diff;
        f32_next = f32_current - step;
    }
    else
    {
        f32_next = f32_current; //hold at target
    }

    pt_state->f32_output = CLAMP_F32(f32_next, pt_params->f32_min_limit, pt_params->f32_max_limit);

    return s16_error;
}

/** \brief Initialize AgvHelper - Helper Control
 *
 *  This function initializes
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 movingAdvFlt(void)
{
    sint16 s16_error = C_NO_ERR;


    return s16_error;

}

/** \brief Initialize AgvHelper - Helper Control
 *
 *  This function initializes
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 lowPassFlt(void)
{
    sint16 s16_error = C_NO_ERR;


    return s16_error;

}

/** \brief Initialize AgvHelper - Toggle Button
 *
 *  This function maintains a toggle button with a set debounce.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 toggleButton(T_ToggleBtn * pt_btn, uint8 u8_raw_btn, uint32 u32_dt_ms, uint32 _u32_deb_ms, uint8 u8_faulted, uint8 u8_safe_state)
{
    sint16 s16_error = C_NO_ERR;

    u8_raw_btn = (u8_raw_btn != 0u) ? 1u : 0u;

    if(u8_faulted)
    {
        *(pt_btn->p_btn_state) = (u8_safe_state != 0u) ? 1u : 0u;
        pt_btn->u32_hold_ms =0u;
        pt_btn->u8_btn_set =1u;
        return C_WARN;
    }

    if(u8_raw_btn == 0u)
    {
        pt_btn->u32_hold_ms = 0u;
        pt_btn->u8_btn_set = 1u;
    }

    if(pt_btn->u32_hold_ms < (UINT32_MAX - u32_dt_ms))
    {
        pt_btn->u32_hold_ms += _u32_deb_ms;
    }

    if( (pt_btn->u8_btn_set == 1u) && (pt_btn->u32_hold_ms >= _u32_deb_ms))
    {
        *(pt_btn->p_btn_state) = (*(pt_btn->p_btn_state) == 0u) ? 1u : 0u;
        pt_btn->u8_btn_set = 0u;
    }

    return s16_error;
}

//EOF
