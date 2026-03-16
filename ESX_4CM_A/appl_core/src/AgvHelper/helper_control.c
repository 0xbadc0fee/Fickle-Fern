//-----------------------------------------------------------------------------
/*! \file       helper_control.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 STW Technic
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
#include "helper_control.h"
#include "math.h"
#include "x_sys.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief PID Output AgvHelper - Helper Control
 *
 *  This function calculates what the PID output should be for a given set of parameters and feedback.
 *
 *  \param f32_command
 *  \param f32_feedback
 *  \param t_pid_state
 *  \param _t_PID_coeff
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

    //IR-17.2 Param Check
    if((t_pid_state == NULL) || (t_PID_coeff == NULL))
    {
        return C_WARN;
    }

    //FR-17.1 Control Rate
    u64_currentTime = (uint64)x_sys_get_time_us();  //Calculate delta t, used in Ipart and Dpart

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


/** \brief Ramp Calculation AgvHelper - Helper Control
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

    //IR-18 Fault Handling
    if ((pt_state == NULL)|| (pt_params == NULL))
    {
        return C_WARN;
    }

    //IR-18.1-18.3 Param Validation
    if(isnan(f32_target) ||
    isnan(pt_params->f32_dt_s) || isnan(pt_params->f32_dt_s <= 0.0f) ||
    isnan(pt_params->f32_ramp_rate) || isnan(pt_params->f32_ramp_rate < 0.0f) ||
    isnan(pt_params->f32_min_limit) || isnan(pt_params->f32_max_limit) ||
    (pt_params->f32_min_limit > pt_params->f32_max_limit))
    {
        pt_state->f32_output = CLAMP_F32(pt_state->f32_output, pt_params->f32_min_limit, pt_params->f32_max_limit); //Force safe-state
        return C_WARN;
    }

    //FR-18.5 Clamp current output before use
    pt_state->f32_output = CLAMP_F32(pt_state->f32_output, pt_params->f32_min_limit, pt_params->f32_max_limit);
    f32_current = pt_state->f32_output;

    //FR-18.1/FR-18.3 Limit Change per cycle
    f32_step_max = pt_params->f32_ramp_rate * pt_params->f32_dt_s; //Limit change per cycle

    //FR-18.2/FR-18.4 Linear transition toward target; hold once reached.
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

    //FR-18.5 Clamp final output
    pt_state->f32_output = CLAMP_F32(f32_next, pt_params->f32_min_limit, pt_params->f32_max_limit);

    return s16_error;
}

/** \brief Moving Average Filter Init AgvHelper - Helper Control
 *
 *  The Moving Average Filter Module Init initialized movingAdvFlt.
 *
 *  \param pt_mvAdvFlt
 *  \param pf32_buffer
 *  \param u16_buf_len
 *  \param f32_safe_output
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 movingFltInit(T_MoveAvgFilter * const pt_mv_adv_flt, float32 * const pf32_buffer, uint16 u16_buf_len, float32 f32_safe_output)
{
    sint16 s16_error = C_NO_ERR;

    if((pt_mv_adv_flt == NULL) || (pf32_buffer == NULL) || (u16_buf_len == 0u))
    {
        return C_WARN;
    }

    pt_mv_adv_flt->pf32_buf = pf32_buffer;
    pt_mv_adv_flt->u16_buf_len = u16_buf_len;
    pt_mv_adv_flt->u16_head = 0u;
    pt_mv_adv_flt->u16_count = 0u;
    pt_mv_adv_flt->u32_accum_ms = 0u;
    pt_mv_adv_flt->f32_sum = 0.0f;
    pt_mv_adv_flt->f32_out = f32_safe_output;
    pt_mv_adv_flt->u8_faulted = FALSE;

    return s16_error;
}

/** \brief Moving Average Filter AgvHelper - Helper Control
 *
 *  The Moving Average Filter Module produces a smoothed output by averaging values within a configurable sample window and forces a safe output when required parameters or values are invalid.
 *
 *  \param pt_mv_adv_flt
 *  \param pt_cfg
 *  \param u32_dt_ms
 *  \param f32_new_value
 *  \param u8_value_valid
 *  \param pf32_output
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 movingAdvFlt(T_MoveAvgFilter * const pt_mv_adv_flt,const T_MoveAvgCfg *const pt_cfg, uint32 u32_dt_ms, float32 f32_new_value , uint8 u8_value_valid, float32 * const pf32_output)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_old = 0.0f;

    if((pt_mv_adv_flt == NULL) || (pt_cfg == NULL) || (pf32_output == NULL))
    {
        return C_WARN;
    }

    //IR-19.1 Invalid Params
    if((pt_mv_adv_flt->pf32_buf == NULL) ||
    (pt_mv_adv_flt->u16_buf_len == 0u)||
    (pt_cfg->u16_sample_time_ms == 0u) ||
    (pt_cfg->u16_sample_no == 0u) ||
    (pt_cfg->u16_sample_no > pt_mv_adv_flt->u16_buf_len))
    {
        pt_mv_adv_flt->f32_out = pt_cfg->f32_safe_output;
        pt_mv_adv_flt->u8_faulted = TRUE;
        *pf32_output = pt_mv_adv_flt->f32_out;
        return C_WARN;
    }

    //IR-19.2 Invalid sample value
    if(u8_value_valid == FALSE || isnan(f32_new_value))
    {
        pt_mv_adv_flt->f32_out = pt_cfg->f32_safe_output;
        pt_mv_adv_flt->u8_faulted = TRUE;
        *pf32_output = pt_mv_adv_flt->f32_out;
        return C_WARN;
    }

    //IR-1.3
    pt_mv_adv_flt->u8_faulted = FALSE;

    //FR-19.2/19.3 Filter
    if(pt_mv_adv_flt->u32_accum_ms <= (UINT32_MAX - u32_dt_ms))
    {
        pt_mv_adv_flt->u32_accum_ms += u32_dt_ms;
    }
    else
    {
        pt_mv_adv_flt->u32_accum_ms = UINT32_MAX;
    }

    if((pt_mv_adv_flt->u16_count == 0u) || (pt_mv_adv_flt->u32_accum_ms >= (uint32)pt_cfg->u16_sample_time_ms))
    {
        pt_mv_adv_flt->u32_accum_ms =0u;

        if(pt_mv_adv_flt->u16_count < pt_cfg->u16_sample_no)
        {
            pt_mv_adv_flt->pf32_buf[pt_mv_adv_flt->u16_head] = f32_new_value;
            pt_mv_adv_flt->f32_sum += f32_new_value;

            pt_mv_adv_flt->u16_head++;
            if(pt_mv_adv_flt->u16_head >= pt_cfg->u16_sample_no)
            {
                pt_mv_adv_flt->u16_head = 0u;
            }

            pt_mv_adv_flt->u16_count++; //FR-19.4
        }
    }
    else
    {
        f32_old = pt_mv_adv_flt->pf32_buf[pt_mv_adv_flt->u16_head];
        pt_mv_adv_flt->pf32_buf[pt_mv_adv_flt->u16_head] = f32_new_value;
        pt_mv_adv_flt->f32_sum += (f32_new_value = f32_old);

        pt_mv_adv_flt->u16_head++;
        if(pt_mv_adv_flt->u16_head >= pt_cfg->u16_sample_no)
        {
            pt_mv_adv_flt->u16_head = 0u;
        }

        pt_mv_adv_flt->f32_out = pt_mv_adv_flt->f32_sum / (float32)pt_mv_adv_flt->u16_count; //FR-19.1/19.4
    }

    *pf32_output = pt_mv_adv_flt->f32_out;


    return s16_error;

}


/** \brief Low Pass Filter AgvHelper - Helper Control
 *
 *  The Low Pass Filter Module produces a smoothed output by combining the new input value with the previous filtered value using a single filtering coefficient.
 *
 *  \param pt_filter Struct previous filtered output
 *  \param f32_input new raw input value
 *  \param f32_alpha filter coefficient
 *  \param u8_input_valid flag for input valid value
 *  \param pf32_output New filtered value written
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 lowpassFilter(T_LowPassFilter *pt_filter, float32 f32_input, float32 f32_alpha, uint8 u8_input_valid, float32 *pf32_output)
{
    sint16 s16_error = C_NO_ERR;

    if((pt_filter == NULL) || (pf32_output == NULL))
    {
        return C_WARN;
    }

    // IR-20.1 Invalid Coefficient
    if((f32_alpha < 0.0f) || (f32_alpha > 1.0f) || isnan(f32_alpha))
    {
        return C_WARN;
    }

    // IR-20.2 Invalid Input
    if ((u8_input_valid == FALSE) ||  isnan(f32_input))
    {
        return C_WARN;
    }

    if (f32_input != pt_filter->f32_output)
    {
        // FR-20.1 - FR 20.2
        pt_filter->f32_output = pt_filter->f32_output + f32_alpha * (f32_input - pt_filter->f32_output);
    }

    *pf32_output = pt_filter->f32_output;

    return s16_error;
}

/** \brief Initialize AgvHelper - Toggle Button
 *
 *  This function maintains a toggle button with a set debounce.
 *
 *  \param pt_btn Pointer to the toggle button structure
 *  \param u8_raw_btn Current raw button input value
 *  \param u32_dt_ms Cyclic execution period in milliseconds
 *  \param _u32_deb_ms Minimum press duration required to toggle
 *  \param u8_faulted Indicates interlock or fault condition active
 *  \param u8_safe_state Forced output state during fault
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 toggleButton(T_ToggleBtn *pt_btn, uint8 u8_raw_btn, uint32 u32_dt_ms, uint32 _u32_deb_ms, uint8 u8_faulted, uint8 u8_safe_state)
{
    sint16 s16_error = C_NO_ERR;

    if((pt_btn == NULL) || (pt_btn->pu_btn_state == NULL))
    {
        return C_WARN;
    }

    u8_raw_btn = (u8_raw_btn != FALSE) ? TRUE : FALSE;

    //IR-21.1 Fault/Interlock forces Safe State
    if(u8_faulted == TRUE)
    {
        *(pt_btn->pu_btn_state) = (u8_safe_state != FALSE) ? TRUE : FALSE;
        pt_btn->u32_hold_ms = 0u;
        //IR-21.2 Requires a new press after fault clears
        pt_btn->u8_btn_set = FALSE;
        return C_WARN;
    }

    //FR-21.2 Required release before accepting new press
    if(u8_raw_btn == FALSE)
    {
        pt_btn->u32_hold_ms = 0u;
        pt_btn->u8_btn_set = TRUE;
    }
    else
    {
        //FR-21.1 Measure continuous press time
        if(pt_btn->u32_hold_ms < (UINT32_MAX - u32_dt_ms))
        {
            pt_btn->u32_hold_ms += u32_dt_ms;
        }
        else
        {
            pt_btn->u32_hold_ms = UINT32_MAX;
        }
    
        //FR-21.1 & FR-21.2 Toggle once when press duration >= debounce, only once per press
        if( (pt_btn->u8_btn_set == TRUE) && (pt_btn->u32_hold_ms >= _u32_deb_ms))
        {
            *(pt_btn->pu_btn_state) = (*(pt_btn->pu_btn_state) == FALSE) ? TRUE : FALSE;
            pt_btn->u8_btn_set = FALSE;
        }
    }
  
    return s16_error;
}

//EOF
