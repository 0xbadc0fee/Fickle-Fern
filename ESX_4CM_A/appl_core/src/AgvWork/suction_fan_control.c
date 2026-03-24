//-----------------------------------------------------------------------------
/*! \file       suction_fan_control.c
    \brief      <description>

    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 24, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
// -- Includes ------------------------------------------------------------------------------------------------------
//STD
#include <stdint.h>
#include "x_stdtypes.h"
#include "math.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "suction_fan_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

// -- Defines ------------------------------------------------------------------------------------------------------
#define PROGRAM_START_DEB_MS (3000u) //3 seconds
// -- Types --------------------------------------------------------------------------------------------------------
// -- Module Global Function Prototypes ----------------------------------------------------------------------------
// -- Module Global Variables --------------------------------------------------------------------------------------
static T_SuctionFanControl mt_suction_fan;

// -- Implementation ------------------------------------------------------------------------------------------------

/** \brief Initialize Suction Fan Control
 *
 *  This function initializes the Suction Fan Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _nvmSuctionFan Pointer to Suction Fan NVM
 * \param _chkSuctionFan Pointer to Suction Fan checkpoints
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_suctionFanControl(T_UserInterface *_ui, T_Config_SFan *_nvmSuctionFan, T_ChkPoints_SFan *_chkSuctionFan)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_min_ramp_limit = 0.0F;
    float32 f32_max_ramp_limit= 0.0F;

    if((_ui == NULL) || (_nvmSuctionFan == NULL) || (_chkSuctionFan == NULL))
    {
        return C_WARN;
    }

    mt_suction_fan.pu8_enable_cmd      = &_ui->t_joystick.u8_b4_state;
    mt_suction_fan.pu16_speed_req_rpm  = &_ui->t_display.u16_suction_fan_speed_req_spd;

    mt_suction_fan.pu8_enable_status     = &_ui->t_display.u8_suction_fan_enable_status;
    mt_suction_fan.pu16_speed_status_rpm = &_ui->t_display.u16_suction_fan_speed_status_rpm;

    mt_suction_fan.pt_nvm = _nvmSuctionFan;

    mt_suction_fan.pt_cp_sfan = _chkSuctionFan;

    mt_suction_fan.u8_enable_latched     = SUCTION_FAN_DISABLED;
    mt_suction_fan.u8_prev_ign_on        = FALSE;
    mt_suction_fan.u32_ign_start_time_ms = 0u;

    //Initialize toggle button helper
    s16_error += toggleButton_init(&mt_suction_fan.t_btn_enable,
    &mt_suction_fan.u8_enable_latched,
    250u,
    SUCTION_FAN_DISABLED);

    //Initialize Moving Average Filter helper
    s16_error += movingFltInit(&mt_suction_fan.t_speed_filter,
    mt_suction_fan.af32_speed_buf,
    SUCTION_FAN_FILTER_BUF_LEN,
    SUCTION_FAN_SAFE_OUTPUT,
    5u,
    100u);

    //Initialize Ramp helper
    if(mt_suction_fan.pt_nvm == NULL)
    {
        f32_max_ramp_limit = SUCTION_FAN_PWM_MAX;
        f32_min_ramp_limit = SUCTION_FAN_PWM_MIN;
    }
    else
    {
        // FR-5.8 Apply ramping using configured increase/decrease times and bounds
        f32_max_ramp_limit = mt_suction_fan.pt_nvm->f32_fan_inc_time;
        f32_min_ramp_limit = mt_suction_fan.pt_nvm->f32_fan_dec_time;
    }
    s16_error += rampInit(&mt_suction_fan.t_speed_ramp,
    SUCTION_FAN_RAMP_RATE,
    f32_min_ramp_limit,
    f32_max_ramp_limit,
    SUCTION_FAN_SAFE_SPEED_RPM);

    //PID init
    mt_suction_fan.t_pid_state.f32_prev_error = 0.0F;
    mt_suction_fan.t_pid_state.f32_error_accum = 0.0F;
    mt_suction_fan.t_pid_state.f32_output = SUCTION_FAN_SAFE_OUTPUT;
    mt_suction_fan.t_pid_state.u64_last_time = 0u;

    // PID output is PWM duty cycle percent
    mt_suction_fan.t_pid_coeff.f32_kp = 500.0F;
    mt_suction_fan.t_pid_coeff.f32_ki = 5.0F;
    mt_suction_fan.t_pid_coeff.f32_kd = 0.0F;
    mt_suction_fan.t_pid_coeff.s32_min_output = -1000;
    mt_suction_fan.t_pid_coeff.s32_max_output = 1000;

    return s16_error;
}

/** \brief Update Suction Fan Control
 *
 *  Reads enable and speed request, filters measured RPM, applies ramping and
 *  PID control, and outputs the suction fan PWM duty-cycle command.
 *
 *  \return s16_error Error code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_suctionFanControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_enable_cmd = FALSE;
    uint8 u8_enable_reset = FALSE;

    uint8 u8_door_fault_status = FALSE;
    uint8 u8_ign_fault_status = FALSE;
    uint8 u8_speed_fault = FALSE;
    uint8 u8_output_fault = FALSE;
    uint8 u8_btn_reset = FALSE;

    uint8 u8_ign_on = FALSE;
    uint8 u8_startup_deb_complete = FALSE;

    uint32 u32_now_ms = get_system_time_ms();

    float32 f32_door_value = DOOR_CLOSED;
    float32 f32_ign_value = IGN_OFF;
    float32 f32_speed_req_rpm = SUCTION_FAN_SAFE_SPEED_RPM;
    float32 f32_speed_sensor = 0.0F;
    float32 f32_meas_speed_rpm_adj = 0.0F;
    float32 f32_pwm_cmd = SUCTION_FAN_SAFE_OUTPUT;

    //Validate required pointers
    if ((mt_suction_fan.pu8_enable_cmd == NULL) ||
    (mt_suction_fan.pu16_speed_req_rpm == NULL) ||
    (mt_suction_fan.pu8_enable_status == NULL) ||
    (mt_suction_fan.pu16_speed_status_rpm == NULL))
    {
        u8_btn_reset = TRUE;
    }
    else
    {
        //FR-5.1 Read operator inputs
        u8_enable_cmd = (*(mt_suction_fan.pu8_enable_cmd) != FALSE) ? TRUE : FALSE;
        f32_speed_req_rpm = (float32)(*(mt_suction_fan.pu16_speed_req_rpm));
    }

    // Read required interlock inputs
    s16_error += get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    if (u8_door_fault_status == FALSE)
    {
        s16_error += get_inputValue("CAB_DOOR", &f32_door_value);
    }

    s16_error += get_inputFaultStatus("IGNITION_SWITCH", &u8_ign_fault_status);
    if (u8_ign_fault_status == FALSE)
    {
        s16_error += get_inputValue("IGNITION_SWITCH", &f32_ign_value);
    }

    //Program Start debounce timing
    u8_ign_on = ((u8_ign_fault_status == FALSE) && (f32_ign_value != IGN_OFF)) ? TRUE : FALSE;

    if ((u8_ign_on == TRUE) && (mt_suction_fan.u8_prev_ign_on == FALSE))
    {
        mt_suction_fan.u32_ign_start_time_ms = u32_now_ms;
    }
    else if (u8_ign_on == FALSE)
    {
        mt_suction_fan.u32_ign_start_time_ms = 0u;
    }

    if ((u8_ign_on == TRUE) &&
    ((u32_now_ms - mt_suction_fan.u32_ign_start_time_ms) >= PROGRAM_START_DEB_MS))
    {
        u8_startup_deb_complete = TRUE;
    }

    // FR-5.4 Read speed input and convert frequency to RPM
    s16_error += get_inputFaultStatus("SUCTION_FAN_SPEED", &u8_speed_fault);
    if (u8_speed_fault == TRUE)
    {
        f32_meas_speed_rpm_adj = 0.0F;
    }
    else
    {
        s16_error += get_inputValue("SUCTION_FAN_SPEED", &f32_speed_sensor);

        //FR-5.5 Apply average filter and drive ratio
        s16_error += movingAdvFlt(&mt_suction_fan.t_speed_filter, f32_speed_sensor);

        if (mt_suction_fan.pt_nvm != NULL)
        {
            f32_meas_speed_rpm_adj =
            mt_suction_fan.t_speed_filter.f32_out * mt_suction_fan.pt_nvm->f32_drive_ratio;
        }
        else
        {
            f32_meas_speed_rpm_adj = mt_suction_fan.t_speed_filter.f32_out;
        }
    }

    // FR-5.3 Force disable/reset when interlocks are not satisfied
    if ((u8_door_fault_status == TRUE) ||
    (f32_door_value != DOOR_CLOSED) ||
    (u8_ign_fault_status == TRUE) ||
    (f32_ign_value == IGN_OFF) ||
    (u8_startup_deb_complete == FALSE) ||
    (u8_speed_fault == TRUE) ||
    (u8_btn_reset == TRUE))
    {
        u8_enable_reset = TRUE;
    }

    // FR-5.2 Apply latching and reset logic to enable command
    s16_error += toggleButton(&mt_suction_fan.t_btn_enable,
    u8_enable_cmd,
    u8_enable_reset);

    //FR-5.7 Force suction fan speed to zero when disabled
    if (mt_suction_fan.u8_enable_latched != SUCTION_FAN_ENABLED)
    {
        mt_suction_fan.t_speed_ramp.f32_output = 0.0F;
        mt_suction_fan.t_pid_state.f32_prev_error = 0.0F;
        mt_suction_fan.t_pid_state.f32_error_accum = 0.0F;
        mt_suction_fan.t_pid_state.u64_last_time = 0u;
        mt_suction_fan.t_pid_state.f32_output = SUCTION_FAN_SAFE_OUTPUT;
    }

    s16_error += rampCalc(f32_speed_req_rpm, &mt_suction_fan.t_speed_ramp);

    // FR-5.9 Closed-loop PID control
    if ((u8_btn_reset == FALSE) &&
    (mt_suction_fan.u8_enable_latched == SUCTION_FAN_ENABLED))
    {
        s16_error += PidOutput(mt_suction_fan.t_speed_ramp.f32_output,
        f32_meas_speed_rpm_adj,
        &mt_suction_fan.t_pid_state,
        &mt_suction_fan.t_pid_coeff);

        // FR-5.10 Convert final command into bounded proportional PWM output
        f32_pwm_cmd = CLAMP_F32(mt_suction_fan.t_pid_state.f32_output, SUCTION_FAN_PWM_MIN,SUCTION_FAN_PWM_MAX);
    }
    else
    {
        f32_pwm_cmd = SUCTION_FAN_SAFE_OUTPUT;
    }

    //Inhibit on logic/output fault
    s16_error += get_outputFaultStatus("FAN_HYDRO_FWD", &u8_output_fault);
    if ((u8_enable_reset == TRUE) || (u8_output_fault == TRUE))
    {
        mt_suction_fan.u8_enable_latched = SUCTION_FAN_DISABLED;
        f32_pwm_cmd = SUCTION_FAN_SAFE_OUTPUT;
    }

    s16_error += set_outputValue("FAN_HYDRO_FWD", f32_pwm_cmd);

    //FR-5.6 Update CAN/display status
    if ((mt_suction_fan.pu8_enable_status != NULL) &&
    (mt_suction_fan.pu16_speed_status_rpm != NULL))
    {
        *(mt_suction_fan.pu8_enable_status) = mt_suction_fan.u8_enable_latched;
        *(mt_suction_fan.pu16_speed_status_rpm) = (uint16)f32_meas_speed_rpm_adj;
    }

    // Checkpoints
    if (mt_suction_fan.pt_cp_sfan != NULL)
    {
        mt_suction_fan.pt_cp_sfan->u8_suctionFanOn =  mt_suction_fan.u8_enable_latched;
        mt_suction_fan.pt_cp_sfan->u16_pwmStatus =f32_pwm_cmd;
    }

    mt_suction_fan.u8_prev_ign_on = u8_ign_on;

    return s16_error;
}

/** \brief Get AgvWork - Shaft Drive Status
 *
 *  This function
 *
 *  Primary logic for this function is
 *
 * *  \param pu8_shaft_drive_status Pointer to the Shaft Drive ON/OFF Status
 *
 *  \return boolean
 */
void getSuctionFanStatus(uint8 *pu8_sfan_status)
{
    if(mt_suction_fan.pu8_enable_status != NULL && pu8_sfan_status != NULL)
    {
        *pu8_sfan_status = mt_suction_fan.u8_enable_latched;
    }
}

