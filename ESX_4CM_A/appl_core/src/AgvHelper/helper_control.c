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
void PidOutput(float32 f32_command, float32 f32_feedback,T_PID_state *t_pid_state, T_PID_coeff *t_PID_coeff)
{
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
}


/** \brief Initialize AgvHelper - Helper Control
 *
 *  This function initializes
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 rampCalc(void)
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




//EOF
