//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 *
 * \file       pid_output.h
 * \brief      Interface for PID Calculation Module.
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup PidOutput PID Output
 * @{
 */
#ifndef APPL_CORE_SRC_AGVHELPER_PID_OUTPUT_H_
#define APPL_CORE_SRC_AGVHELPER_PID_OUTPUT_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x))) //!<Restricts a 32-bit floating point value to a specified range [lo, hi].

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * @struct T_PID_coeff
 * \brief Structure containing PID coefficients and output constraints
 *
 * This structure defines the tuning parameters and physical limits
 * for a PID control loop, including gains and saturation bounds.
 */
typedef struct
{
        float32 s32_max_output;
        float32 s32_min_output;
        float32 f32_kp;
        float32 f32_kd;
        float32 f32_ki;
} T_PID_coeff;

/**
 * @struct T_PID_state
 * \brief Structure containing persistent state information for a PID loop
 *
 * This structure maintains the historical data and error accumulation
 * required to calculate the derivative and integral terms across
 * consecutive control cycles.
 */
typedef struct
{
        float32 f32_prev_error;
        float32 f32_error_accum;
        uint64 u64_last_time;
        float32 f32_output;
} T_PID_state;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 PidOutput(float32 f32_command, float32 f32_feedback,T_PID_state *t_pid_state, T_PID_coeff *t_PID_coeff);

#endif /* APPL_CORE_SRC_AGVHELPER_PID_OUTPUT_H_ */
