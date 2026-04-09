//-----------------------------------------------------------------------------
/**
 * \file       ramp_calc.c
 * \brief      AgvHelper - Ramp Calculation Utility
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup RampCalc Ramp Calculation
 *
 * The Ramp Value Calculation Module shall be used to control how
 * quickly an actuator command is allowed to change over a specified time.
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
 * March 6, 2026 STW Technic
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
#include "ramp_calc.h"
#include "system.h"
#include "math.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize Ramp State
 *
 *  Initializes the ramp state prior to cyclic ramp calculation.
 *
 *  \param pt_state Pointer to ramp state structure
 *  \param f32_ramp_rate_set Ramp Set Rate
 *  \param f32_min_set Min Value Set
 *  \param f32_max_set Max Value Set
 *  \param f32_safe_state_set Safe State
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 rampInit(T_RampState *pt_state, float32 f32_ramp_rate_set, float32 f32_min_set, float32 f32_max_set, float32 f32_safe_state_set)
{
    sint16 s16_error = C_NO_ERR;

    if(pt_state == NULL)
    {
        return C_WARN;
    }

    /* Initialize ramp output */
    pt_state->f32_output = f32_safe_state_set;

    /* Reset time base (first call handled in rampCalc) */
    pt_state->u32_last_time_ms = 0u;

    //Init Ramp Params
    pt_state->f32_ramp_rate = f32_ramp_rate_set;
    pt_state->f32_min_limit = f32_min_set;
    pt_state->f32_max_limit =f32_max_set;
    pt_state->f32_safe_state =f32_safe_state_set;

    return s16_error;
}

/** \brief Ramp Calculation AgvHelper - Helper Control
 *
 *  This function reduces the rate of change between a new target value and the current value over a configurable period of time (linear)
 *
 *  \param f32_target Ramp Target
 *  \param pt_state Ramp State Pointer
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 rampCalc(float32 f32_target, T_RampState *pt_state)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_current = 0.0F;
    float32 f32_step_max =  0.0F;
    float32 f32_next =  0.0F;
    float32 f32_dt_s =  0.0F;
    uint32 u32_dt_ms = 0u;
    uint32 u32_now_ms = get_system_time_ms();

    //IR-18 Fault Handling
    if (pt_state == NULL)
    {
        return C_WARN;
    }

    if(isnan(pt_state->f32_safe_state))
    {
        return C_WARN;
    }

    //IR-18.1-18.3 Param Validation
    if(isnan(f32_target) ||
    isnan(pt_state->f32_ramp_rate) || (pt_state->f32_ramp_rate < 0.0f) ||
    isnan(pt_state->f32_min_limit) || isnan(pt_state->f32_max_limit) ||
    (pt_state->f32_min_limit > pt_state->f32_max_limit))
    {
        pt_state->f32_output = CLAMP_F32(pt_state->f32_safe_state, pt_state->f32_min_limit, pt_state->f32_max_limit); //Force safe-state
        return C_WARN;
    }

    //First call: Init time base
    if(pt_state->u32_last_time_ms == 0u)
    {
        pt_state->u32_last_time_ms = u32_now_ms;
        pt_state->f32_output = CLAMP_F32(pt_state->f32_output, pt_state->f32_min_limit, pt_state->f32_max_limit);
        return s16_error;
    }

    //Calculate elapsed time
    u32_dt_ms = u32_now_ms - pt_state->u32_last_time_ms;
    pt_state->u32_last_time_ms = u32_now_ms;

    f32_dt_s = ((float32)u32_dt_ms/1000.0F);

    //FR-18.5 Clamp current output before use
    pt_state->f32_output = CLAMP_F32(pt_state->f32_output, pt_state->f32_min_limit, pt_state->f32_max_limit);
    f32_current = pt_state->f32_output;

    f32_target = CLAMP_F32(f32_target, pt_state->f32_min_limit, pt_state->f32_max_limit);

    //FR-18.1/FR-18.3 Limit Change per cycle
    f32_step_max = pt_state->f32_ramp_rate * f32_dt_s; //Limit change per cycle

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
    pt_state->f32_output = CLAMP_F32(f32_next, pt_state->f32_min_limit, pt_state->f32_max_limit);

    return s16_error;
}

/** \brief Set Ramp Helper Rate - Helper Control
 *
 *  This function sets the rate of change of the ramp function
 *
 *  \param pt_state Ramp State Pointer
 *  \param _rate    Target Ramp Rate (units / second)
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 *  \retval C_RANGE Requested Ramp outside of acceptable bounds
 */
sint16 set_rampRate(T_RampState *pt_state, float32 _rate)
{
    sint16 s16_error = C_NO_ERR;

    if(_rate >= 0.01f || _rate <= 100000.0f)
        pt_state->f32_ramp_rate = _rate;
    else
        s16_error = C_RANGE;

    return s16_error;
}

//EOF
