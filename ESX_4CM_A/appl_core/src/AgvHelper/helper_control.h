
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
        //Local Control Variables
        //uint8 u8_engineOffStatus;                  //!<Local On Off Command Variable

        //RX CAN Variables
       // uint8 *pu8_reqEngineOffStatus;              //!<Requested Engine Status (From Engine)

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

/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
//sint16 initHelperControl(T_UserInterface *_ui, T_ChkPoints_Helper *_chkp);
void PidOutput(float32 f32_command, float32 f32_feedback,T_PID_state *t_pid_state, T_PID_coeff *t_PID_coeff);
sint16 rampCalc(void);
sint16 movingAdvFlt(void);
sint16 lowPassFlt(void);
//sint16 getNeuStatus(uint8 *pu8_neu_status);


#endif /* APPL_CORE_SRC_AGVHELPER_HELPER_CONTROL_H_ */
