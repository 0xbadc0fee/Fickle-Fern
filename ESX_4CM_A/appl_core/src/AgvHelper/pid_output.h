//-----------------------------------------------------------------------------
/**
 * \file       pid_output.h
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
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x))) //!<Clamp F32 Macro

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 *\struct T_PID_coeff
 * \brief Structure containing PID coefficients (Kp, Ki, Kd, Max/Min output).*/
typedef struct
{
        float32 s32_max_output;
        float32 s32_min_output;
        float32 f32_kp;
        float32 f32_kd;
        float32 f32_ki;
} T_PID_coeff;

/**
 * \struct T_PID_state
 * \brief Structure containing all relevant PID information for each PID loop*/
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
