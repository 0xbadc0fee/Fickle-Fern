
/*! \file       helper_control.h
    \brief      Helper Module for 8772 Harvester


    \implementation
    project     Flory_8772_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 Tiffany.Gohnert
    \endimplementation
 */

#ifndef APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_
#define APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x))) //!<Clamp F32 Macro

/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Structure containing PID coefficients (Kp, Ki, Kd, Max/Min output).*/
typedef struct
   {
      float32 s32_max_output;
      float32 s32_min_output;
      float32 f32_kp;
      float32 f32_kd;
      float32 f32_ki;
   } T_PID_coeff;

/** \brief Structure containing all relevant PID information for each PID loop*/
typedef struct
   {
      float32 f32_prev_error;
      float32 f32_error_accum;
      uint64 u64_last_time;
      float32 f32_output;
   } T_PID_state;

 /** \brief Structure containing all relevant RAMP output information*/
typedef struct
{
        float32 f32_output;  //!<Current Ramped Output
        uint32 u32_last_time_ms;
}T_RampState;

/** \brief Structure containing all relevant RAMP Parameters*/
typedef struct
{
        float32 f32_ramp_rate; //!<Ramp Rate[unit/s]
        float32 f32_min_limit; //!<MIN Output Limit
        float32 f32_max_limit; //!<MAX Output Limit
        float32 f32_safe_state; //!<Safe State Output
} T_RampParams;

/** \brief Structure containing all relevant Moving Average Configuration Init Parameters*/
typedef struct
{
        uint16 u16_sample_time_ms;
        uint16 u16_sample_no;
        float32 f32_safe_output;
} T_MoveAvgCfg;

/** \brief Structure containing all relevant Moving Average Parameters*/
typedef struct
{
        float32 * pf32_buf; //!<Caller buffer
        uint16 u16_buf_len;  //!<Buffer capacity
        uint16 u16_head; //!<Next write index
        uint16 u16_count; //!<Samples currently stored
        uint32 u32_accum_ms; //!<Accumulates dt until >= sample time ms
        float32 f32_sum; //!<Running sum of samples in window
        float32 f32_out; //!<Current filtered output
        uint8 u8_faulted; //!<1 = Faulted and forced safe
}T_MoveAvgFilter;

/** \brief Structure containing all relevant Low Pass Filter output information*/
typedef struct
{
        float32 f32_output; //!<Previous filtered output
} T_LowPassFilter;

/** \brief Structure containing all relevant Toggle Button Parameters*/
typedef struct
{
       uint8 *pu_btn_state; //!<Button ON/OFF state
       uint32 u32_hold_ms; //!<Hold Button MS
       uint8 u8_btn_set; //!<Button Armed State
}T_ToggleBtn;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 PidOutput(float32 f32_command, float32 f32_feedback,T_PID_state *t_pid_state, T_PID_coeff *t_PID_coeff);
sint16 rampCalc(float32 f32_target, const T_RampParams *pt_params, T_RampState *pt_state);
sint16 movingFltInit(T_MoveAvgFilter * const pt_mv_adv_flt,float32 * const pf32_buffer, uint16 u16_buf_len, float32 f32_safe_output);
sint16 movingAdvFlt(T_MoveAvgFilter * const pt_mv_adv_flt,const T_MoveAvgCfg *const pt_cfg, float32 f32_new_value);
sint16 lowpassFilter(T_LowPassFilter *pt_filter, float32 f32_input, float32 f32_alpha, uint8 u8_input_valid);
sint16 toggleButton(T_ToggleBtn * pt_btn, uint8 u8_raw_btn,uint32 _u32_deb_ms, uint8 u8_faulted, uint8 u8_safe_state);

#endif /* APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_ */
