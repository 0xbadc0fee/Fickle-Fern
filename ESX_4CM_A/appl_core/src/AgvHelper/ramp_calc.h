//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 *
 * \file       ramp_calc.h
 * \brief      Interface for Ramp Calculation Module.
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup RampCalc Ramp Calculation
 * @{
 */

#ifndef APPL_CORE_SRC_AGVHELPER_RAMP_CALC_H_
#define APPL_CORE_SRC_AGVHELPER_RAMP_CALC_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x)))  //!<Restricts a 32-bit floating point value to a specified range [lo, hi].

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_RampState
 * \brief  Structure containing all relevant RAMP output information.
 */
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

#endif /* APPL_CORE_SRC_AGVHELPER_RAMP_CALC_H_ */
