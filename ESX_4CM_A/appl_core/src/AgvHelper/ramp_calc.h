//-----------------------------------------------------------------------------
/*! \file       ramp_calc.h
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------

#ifndef APPL_CORE_SRC_AGVHELPER_RAMP_CALC_H_
#define APPL_CORE_SRC_AGVHELPER_RAMP_CALC_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x))) //!<Clamp F32 Macro

/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Structure containing all relevant RAMP output information*/
typedef struct
{
        float32 f32_output;  //!<Current Ramped Output
        uint32 u32_last_time_ms;

        float32 f32_ramp_rate; //!<Ramp Rate[unit/s]
        float32 f32_min_limit; //!<MIN Output Limit
        float32 f32_max_limit; //!<MAX Output Limit
        float32 f32_safe_state; //!<Safe State
}T_RampState;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 rampInit(T_RampState *pt_state, float32 f32_ramp_rate_set, float32 f32_min_set, float32 f32_max_set, float32 f32_safe_state_set);
sint16 rampCalc(float32 f32_target, T_RampState *pt_state);
sint16 set_rampRate(T_RampState *pt_state, float32 f32_rate);

#endif /* APPL_CORE_SRC_AGVHELPER_RAMP_CALC_H_ */
