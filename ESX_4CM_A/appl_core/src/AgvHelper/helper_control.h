
/*! \file       helper_control.h
    \brief      <description>


    \implementation
    project     Flory_8772_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 7, 2026 Tiffany.Gohnert
    \endimplementation
 */

#ifndef APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_
#define APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_


/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x)))
/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Engine Start Control
 *
 * This structure represents all checkpoints that are relevant
 * to engine start control
 */
typedef struct
{
        uint8 u8_chkP;

}T_ChkPoints_Helper;


/** \brief Control Structure - Engine Start Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for elevator control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //Control Checkpoints
        T_ChkPoints_Helper *pt_chk_helper;   //!<Helper Checkpoints Structure


}T_HelperControl;

/** \brief Structure containing PID coefficients (Kp, Ki, Kd, Max/Min output).*/
typedef struct
   {
      float32 s32_max_output;
      float32 s32_min_output;
      float32 f32_kp;
      float32 f32_kd;
      float32 f32_ki;
   } T_PID_coeff;

//define terms that monitor PID state
/** \brief Structure containing all relevant PID information for each PID loop*/
typedef struct
   {
      float32 f32_prev_error;
      float32 f32_error_accum;
      uint64 u64_last_time;
      float32 f32_output;

   } T_PID_state;

typedef struct
{
        float32 f32_output;  //!<Current Ramped Output
        uint8 u8_faulted; //!<Latched invalid-input fault
}T_RampState;

typedef struct
{
        float32 f32_dt_s; //!<Execution period [s]
        float32 f32_ramp_rate; //!<Ramp Rate[unit/s]
        float32 f32_min_limit; //!<MIN Output Limit
        float32 f32_max_limit; //!<MAX Output Limit
        float32 f32_safe_state; //!<Safe State Output
} T_RampParams;

typedef struct
{
       uint8 *p_btn_state;
       uint32 u32_hold_ms;
       uint8 u8_btn_set;
}T_ToggleBtn;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
//sint16 initHelperControl(T_UserInterface *_ui, T_ChkPoints_Helper *_chkp);
sint16 PidOutput(float32 f32_command, float32 f32_feedback,T_PID_state *t_pid_state, T_PID_coeff *t_PID_coeff);
sint16 rampCalc(float32 f32_target, const T_RampParams *pt_params, T_RampState *pt_state);
sint16 movingAdvFlt(void);
sint16 lowPassFlt(void);
sint16 toggleButton(T_ToggleBtn * pt_btn, uint8 u8_raw_btn, uint32 u32_dt_ms, uint32 _u32_deb_ms, uint8 u8_faulted, uint8 u8_safe_state);
//sint16 calcNeuStatus(uint8 *pu8_neu_status);


#endif /* APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_ */
