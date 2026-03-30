//-----------------------------------------------------------------------------
/*! \file       cooling_fan_control.c
    \brief      The Cooling Fan Control Module controls two output valves (Speed Control Valve
    and Fan Direction Valve) to control the fan speed and direction.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "cooling_fan_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS (3500u) //3.5 seconds
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
void coolingFanTempToPwm(float32 f32_temp_input,
float32 f32_temp_min_c,
float32 f32_temp_max_c,
float32 *pf32_pwm_cmd);
void update_coolingFanReversal(float32 f32_dir_cmd_target_pct);
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_CoolingFanControl mt_cooling_fan;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */


/** \brief Initialize AgvWork - coolingFanTempToPwm Cooling Fan Control
 *
 *  This function is for the cooling fan temperature to PWM AgvWork - Cooling Fan Control Logic.
 *
 *  \param f32_temp_input
 *
 */
void coolingFanTempToPwm(float32 f32_temp_input,
float32 f32_temp_min_c,
float32 f32_temp_max_c,
float32 *pf32_pwm_cmd)
{
    float32 f32_pwm = COOLING_FAN_PWM_HIGH_LIMIT;

    if(pf32_pwm_cmd == NULL)
    {
        return;
    }

    if(f32_temp_max_c <= f32_temp_min_c)
    {
        *pf32_pwm_cmd = COOLING_FAN_PWM_HIGH_LIMIT;
        return;
    }

    f32_pwm =
    ((f32_temp_input - f32_temp_min_c) *
    (COOLING_FAN_PWM_LOW_LIMIT - COOLING_FAN_PWM_HIGH_LIMIT)) /
    (f32_temp_max_c - f32_temp_min_c) +
    COOLING_FAN_PWM_HIGH_LIMIT;

    *pf32_pwm_cmd = CLAMP_F32(f32_pwm,
    COOLING_FAN_PWM_LOW_LIMIT,
    COOLING_FAN_PWM_HIGH_LIMIT);
}

/** \brief Initialize AgvWork - update_coolingFanReversal Cooling Fan Control
 *
 *  This function is for the cooling fan REVERSE to PWM AgvWork - Cooling Fan Control Logic.
 *
 *  \param f32_forward_speed_target_pwm
 *
 */
void update_coolingFanReversal(float32 f32_forward_speed_target_pwm)
{
    uint8 u8_manual_purge_cmd = FALSE;
    uint8 u8_machine_in_motion = FALSE;
    uint8 u8_start_cleanout = FALSE;
    float32 f32_wheel_speed_rpm = 0.0F;
    float32 f32_speed_ramp_target_pwm = f32_forward_speed_target_pwm;
    uint32 u32_now_ms = get_system_time_ms();
    uint32 u32_elapsed_ms = 0u;

    //getWheelSpeed(&f32_wheel_speed_rpm); //TODO_STW GET FROM PROPULSION

    u8_manual_purge_cmd = (uint8)(*mt_cooling_fan.pu8_manual_purge_req);

    u8_machine_in_motion =
    (f32_wheel_speed_rpm > COOLING_FAN_MACHINE_IN_MOTION_RPM) ? TRUE : FALSE;

    // Default: fan moving forward at X speed
    mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;

    // FR-7.5 manual purge request initiates cleanout cycle
    if((u8_manual_purge_cmd == TRUE) &&
    (mt_cooling_fan.u8_manual_purge_latched == FALSE) &&
    (mt_cooling_fan.e_rev_state == CF_REV_IDLE_FORWARD))
    {
        u8_start_cleanout = TRUE;
        mt_cooling_fan.u8_manual_purge_latched = TRUE;
    }

    // FR-7.7 block repeated purge signals during active cleanout
    if(u8_manual_purge_cmd == FALSE)
    {
        mt_cooling_fan.u8_manual_purge_latched = FALSE;
    }

    // FR-7.6 automatic cleanout after preset forward time while machine is in motion
    if((u8_start_cleanout == FALSE) &&
    (mt_cooling_fan.e_rev_state == CF_REV_IDLE_FORWARD) &&
    (u8_machine_in_motion == TRUE) &&
    ((u32_now_ms - mt_cooling_fan.u32_forward_run_start_ms) >= COOLING_FAN_AUTO_CLEANOUT_DELAY_MS))
    {
        u8_start_cleanout = TRUE;
    }

    // FR-7.10 prevent entry if already active
    if(u8_start_cleanout == TRUE)
    {
        mt_cooling_fan.e_rev_state = CF_REV_RAMP_DOWN_TO_REV;
        mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
        mt_cooling_fan.u8_cleanout_active = TRUE;
    }

    u32_elapsed_ms = u32_now_ms - mt_cooling_fan.u32_rev_state_start_ms;

    switch(mt_cooling_fan.e_rev_state)
    {
        case CF_REV_IDLE_FORWARD:
            // Fan moving forward at X speed
            mt_cooling_fan.u8_cleanout_active = FALSE;
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
            f32_speed_ramp_target_pwm = f32_forward_speed_target_pwm;
            break;

        case CF_REV_RAMP_DOWN_TO_REV:
            // Fan slows down to 0 over 2.5s
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
            f32_speed_ramp_target_pwm = 0.0F;

            if(u32_elapsed_ms >= COOLING_FAN_RAMP_DOWN_MS)
            {
                // Fan direction valve triggered to reverse after fan stops
                mt_cooling_fan.e_rev_state = CF_REV_STOP_BEFORE_REV;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
                mt_cooling_fan.u8_fan_direction = COOLING_FAN_REVERSE;
            }
            break;

        case CF_REV_STOP_BEFORE_REV:
            // Fan stays stopped for 1.5s
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_REVERSE;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_REVERSE_CMD_PCT;
            f32_speed_ramp_target_pwm = 0.0F;

            if(u32_elapsed_ms >= COOLING_FAN_DIR_STOP_DELAY_MS)
            {
                mt_cooling_fan.e_rev_state = CF_REV_RAMP_UP_REV;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
            }
            break;

        case CF_REV_RAMP_UP_REV:
            // Fan ramps to target speed Y over 2.5s in reverse
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_REVERSE;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_REVERSE_CMD_PCT;
            f32_speed_ramp_target_pwm = f32_forward_speed_target_pwm;

            if(u32_elapsed_ms >= COOLING_FAN_RAMP_UP_MS)
            {
                mt_cooling_fan.e_rev_state = CF_REV_RUN_REV;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
            }
            break;

        case CF_REV_RUN_REV:
            // Fan stays at target speed Y for 10 seconds
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_REVERSE;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_REVERSE_CMD_PCT;
            f32_speed_ramp_target_pwm = f32_forward_speed_target_pwm;

            if(u32_elapsed_ms >= COOLING_FAN_REVERSE_RUN_MS)
            {
                mt_cooling_fan.e_rev_state = CF_REV_RAMP_DOWN_TO_FWD;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
            }
            break;

        case CF_REV_RAMP_DOWN_TO_FWD:
            // Fan slows down to 0 over 2.5s
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_REVERSE;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_REVERSE_CMD_PCT;
            f32_speed_ramp_target_pwm = 0.0F;

            if(u32_elapsed_ms >= COOLING_FAN_RAMP_DOWN_MS)
            {
                // Fan direction valve triggered forward after fan stops
                mt_cooling_fan.e_rev_state = CF_REV_STOP_BEFORE_FWD;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
                mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
            }
            break;

        case CF_REV_STOP_BEFORE_FWD:
            // Fan stays stopped for 1.5s
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
            f32_speed_ramp_target_pwm = 0.0F;

            if(u32_elapsed_ms >= COOLING_FAN_DIR_STOP_DELAY_MS)
            {
                mt_cooling_fan.e_rev_state = CF_REV_RAMP_UP_FWD;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
            }
            break;

        case CF_REV_RAMP_UP_FWD:
            // Fan ramps to target speed X over 2.5s in forward
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
            f32_speed_ramp_target_pwm = f32_forward_speed_target_pwm;

            if(u32_elapsed_ms >= COOLING_FAN_RAMP_UP_MS)
            {
                // FR-7.9 return to forward operation and restart forward timing
                mt_cooling_fan.e_rev_state = CF_REV_IDLE_FORWARD;
                mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
                mt_cooling_fan.u32_forward_run_start_ms = u32_now_ms;
                mt_cooling_fan.u8_cleanout_active = FALSE;
            }
            break;

        default:
            // IR-7.2 invalid sequence -> safe forward state
            mt_cooling_fan.e_rev_state = CF_REV_IDLE_FORWARD;
            mt_cooling_fan.u32_rev_state_start_ms = u32_now_ms;
            mt_cooling_fan.u8_cleanout_active = FALSE;
            mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
            mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
            f32_speed_ramp_target_pwm = f32_forward_speed_target_pwm;
            break;
    }

    // FR-7.8 ramp cooling demand during reversal sequence
    rampCalc(f32_speed_ramp_target_pwm, &mt_cooling_fan.t_speed_ramp);
}

/** \brief Initialize AgvWork - Cooling Fan Control
 *
 *  This function initializes the AgvWork - Cooling Fan Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkCooling Fan Pointer to the global Cooling Fan Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_coolingFanControl(T_UserInterface *_ui, T_ChkPoints_CoolingFan *_chkCoolingFan)
{
    sint16 s16_error = C_NO_ERR;

    if((_ui == NULL) || (_chkCoolingFan == NULL))
    {
        return C_WARN;
    }

    //populate local RX/TX pointers
    /*TODO_STW
    mt_cooling_fan.pf32_engine_coolant_temp_degC =
    &_ui->t_engine.f32_engine_coolant_temp_degC;
    mt_cooling_fan.pf32_intake_manifold_temp_degC =
    &_ui->t_engine.f32_intake_manifold_temp_degC;
     */
    mt_cooling_fan.pu8_manual_purge_req =
    &_ui->t_display.u8_manual_purge_req;
    mt_cooling_fan.pu16_disp_hyd_oil_temp_degC =
    &_ui->t_display.u16_hyd_oil_temp_degC;
    mt_cooling_fan.pu8_disp_fan_reverse_ind =
    &_ui->t_display.u8_cooling_fan_reverse_ind;
    mt_cooling_fan.pu8_disp_cooling_system_fault =
    &_ui->t_display.u8_cooling_system_fault;
    mt_cooling_fan.pu8_disp_hyd_oil_overtemp =
    &_ui->t_display.u8_hyd_oil_overtemp;
    mt_cooling_fan.pu8_disp_intake_overtemp =
    &_ui->t_display.u8_intake_manifold_overtemp;
    mt_cooling_fan.pu8_disp_coolant_overtemp =
    &_ui->t_display.u8_engine_coolant_overtemp;

    //Populate local copy of checkpoints
    mt_cooling_fan.pt_cp_cooling = _chkCoolingFan;

    //Initialize local variables
    mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;

    mt_cooling_fan.u8_cleanout_active = FALSE;
    mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
    mt_cooling_fan.u8_manual_purge_latched = FALSE;
    mt_cooling_fan.u8_cooling_fault = FALSE;

    mt_cooling_fan.u32_last_update_time_ms = 0u;
    mt_cooling_fan.u32_forward_run_start_ms = 0u;
    mt_cooling_fan.u32_cleanout_start_ms = 0u;
    mt_cooling_fan.u32_ign_on_start_ms = 0u;
    mt_cooling_fan.u32_rev_state_start_ms = 0u;

    mt_cooling_fan.e_rev_state = CF_REV_IDLE_FORWARD;

    s16_error += rampInit(&mt_cooling_fan.t_speed_ramp,
    COOLING_FAN_SPEED_RAMP_RATE,
    COOLING_FAN_PWM_LOW_LIMIT,
    COOLING_FAN_PWM_HIGH_LIMIT,
    COOLING_FAN_PWM_OFF);

    s16_error += rampInit(&mt_cooling_fan.t_dir_ramp,
    COOLING_FAN_DIR_RAMP_RATE,
    COOLING_FAN_DIR_FORWARD_CMD_PCT,
    COOLING_FAN_DIR_REVERSE_CMD_PCT,
    COOLING_FAN_DIR_FORWARD_CMD_PCT);

    s16_error += movingFltInit(&mt_cooling_fan.t_hyd_oil_temp_filt,
    mt_cooling_fan.f32_hyd_buff,
    COOLING_FAN_HYD_BUF_LEN,
    COOLING_FAN_HYD_FILTER_SAFE_OUTPUT,
    COOLING_FAN_HYD_FILTER_SAMPLE_NO,
    COOLING_FAN_HYD_FILTER_SAMPLE_MS);

    return s16_error;
}

/** \brief Update AgvWork - Cooling Fan Control
 *
 *  This function contains the cyclically logic for AgvWork - Cooling Fan Control.
 *
 *  Primary logic for this function
 *
 *  Additional interlocks are utilized throughout the logic.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_CoolingFanControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_t1_degC = *(mt_cooling_fan.pf32_engine_coolant_temp_degC);
    float32 f32_t2_degC = *(mt_cooling_fan.pf32_intake_manifold_temp_degC);
    float32 f32_t3_hyd_degC = 0.0F;
    uint8 u8_flt_hyd_degC = FALSE;

    float32 f32_speed_cmd_target_pwm = COOLING_FAN_PWM_OFF;
    float32 f32_speed_cmd_target_pwm_in1 =COOLING_FAN_PWM_OFF;
    float32 f32_speed_cmd_target_pwm_in2 = COOLING_FAN_PWM_OFF;

    float32 f32_t1_pwm = COOLING_FAN_PWM_HIGH_LIMIT;
    float32 f32_t2_pwm = COOLING_FAN_PWM_HIGH_LIMIT;
    float32 f32_t3_pwm = COOLING_FAN_PWM_HIGH_LIMIT;
    float32 f32_temp_min_pwm = COOLING_FAN_PWM_HIGH_LIMIT;

    float32 f32_cooling_demand_pct = 0.0F;
    float32 f32_ign_value = 0.0F;

    uint8 u8_ign_on = IGN_OFF;
    uint8 u8_ign_fault = FALSE;
    uint8 u8_temp_fault = FALSE;
    uint8 u8_speed_output_fault = FALSE;
    uint8 u8_dir_output_fault = FALSE;
    float32 f32_hyd_oil_temp_degC = 0.0F;

    uint32 u32_now_ms = get_system_time_ms();

    if(mt_cooling_fan.u32_last_update_time_ms == 0u)
    {
        mt_cooling_fan.u32_last_update_time_ms = u32_now_ms;
        mt_cooling_fan.u32_forward_run_start_ms = u32_now_ms;
    }

    mt_cooling_fan.u32_last_update_time_ms = u32_now_ms;

    get_inputFaultStatus("IGNITION_SWITCH", &u8_ign_fault);
    if(u8_ign_fault == FALSE)
    {
        get_inputValue("IGNITION_SWITCH", &f32_ign_value);
    }
    u8_ign_on = ((u8_ign_fault == FALSE) && (f32_ign_value == IGN_ON));

    // IR-7.3 output faults
    get_outputFaultStatus("COOL_FAN_SPEED", &u8_speed_output_fault);
    get_outputFaultStatus("COOL_FAN_DIRECTION", &u8_dir_output_fault);

    // FR-7.3 hydraulic oil plausibility / filtering
    get_inputFaultStatus("HYD_OIL_TEMP", &u8_flt_hyd_degC);
    if(u8_flt_hyd_degC == FALSE)
    {
        get_inputValue("HYD_OIL_TEMP", &f32_t3_hyd_degC);
    }
    s16_error += movingAdvFlt(&mt_cooling_fan.t_hyd_oil_temp_filt, f32_t3_hyd_degC);
    f32_hyd_oil_temp_degC = mt_cooling_fan.t_hyd_oil_temp_filt.f32_out;

    if((f32_t1_degC > COOLING_FAN_MAX_TEMP_DEGC) ||
    (f32_t2_degC > COOLING_FAN_MAX_TEMP_DEGC) ||
    (f32_hyd_oil_temp_degC > COOLING_FAN_MAX_TEMP_DEGC))
    {
        u8_temp_fault = TRUE;
    }
    else
    {
        // FR-7.4 temperature to PWM demand
        coolingFanTempToPwm(f32_t1_degC,
        COOLING_FAN_T1_MIN_C,
        COOLING_FAN_T1_MAX_C,
        &f32_t1_pwm);

        coolingFanTempToPwm(f32_t2_degC,
        COOLING_FAN_T2_MIN_C,
        COOLING_FAN_T2_MAX_C,
        &f32_t2_pwm);

        coolingFanTempToPwm(f32_hyd_oil_temp_degC,
        COOLING_FAN_T3_MIN_C,
        COOLING_FAN_T3_MAX_C,
        &f32_t3_pwm);

        f32_temp_min_pwm = f32_t1_pwm;

        if(f32_t2_pwm < f32_temp_min_pwm)
        {
            f32_temp_min_pwm = f32_t2_pwm;
        }

        if(f32_t3_pwm < f32_temp_min_pwm)
        {
            f32_temp_min_pwm = f32_t3_pwm;
        }

        mt_cooling_fan.pt_cp_cooling->u8_leadsensornumber = f32_temp_min_pwm;
        f32_speed_cmd_target_pwm_in1 = CLAMP_F32(f32_temp_min_pwm,
        COOLING_FAN_PWM_LOW_LIMIT,
        COOLING_FAN_PWM_HIGH_LIMIT);
    }

    mt_cooling_fan.u8_cooling_fault =
    ((u8_ign_fault == TRUE) ||
    (u8_temp_fault == TRUE) ||
    (u8_speed_output_fault == TRUE) ||
    (u8_dir_output_fault == TRUE)) ? TRUE : FALSE;

    // Fault override applies to IN1
    if(mt_cooling_fan.u8_cooling_fault == TRUE)
    {
        mt_cooling_fan.u8_cleanout_active = FALSE;
        mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
        mt_cooling_fan.e_rev_state = CF_REV_IDLE_FORWARD;
        mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
        f32_speed_cmd_target_pwm_in1 = COOLING_FAN_FAULT_SAFE_PWM;
    }
    else
    {
        update_coolingFanReversal(f32_speed_cmd_target_pwm_in1);
    }

    // Ignition ON timer drives IN2 directly
    if(u8_ign_on == IGN_ON)
    {
        if(mt_cooling_fan.u32_ign_on_start_ms == 0u)
        {
            mt_cooling_fan.u32_ign_on_start_ms = u32_now_ms;
        }

        if((u32_now_ms - mt_cooling_fan.u32_ign_on_start_ms) >= PROGRAM_START_DEB_MS)
        {
            f32_speed_cmd_target_pwm_in2 = COOLING_FAN_PWM_HIGH_LIMIT;
        }
        else
        {
            f32_speed_cmd_target_pwm_in2 = 0.0F;
        }
    }
    else
    {
        mt_cooling_fan.u32_ign_on_start_ms = 0u;
        f32_speed_cmd_target_pwm_in2 = 0.0F;
        mt_cooling_fan.u8_cleanout_active = FALSE;
        mt_cooling_fan.u8_fan_direction = COOLING_FAN_FORWARD;
        mt_cooling_fan.e_rev_state = CF_REV_IDLE_FORWARD;
        mt_cooling_fan.f32_dir_cmd_target_pct = COOLING_FAN_DIR_FORWARD_CMD_PCT;
    }

    //Scale to 10000
    f32_speed_cmd_target_pwm = (f32_speed_cmd_target_pwm_in1 +
    (COOLING_FAN_PWM_HIGH_LIMIT - f32_speed_cmd_target_pwm_in1) * (f32_speed_cmd_target_pwm_in2 / COOLING_FAN_PWM_HIGH_LIMIT));

    // Reversal owns speed ramp output during active sequence
    if(mt_cooling_fan.e_rev_state == CF_REV_IDLE_FORWARD)
    {
        rampCalc(f32_speed_cmd_target_pwm, &mt_cooling_fan.t_speed_ramp);
    }

    rampCalc(mt_cooling_fan.f32_dir_cmd_target_pct, &mt_cooling_fan.t_dir_ramp);

    // 4000 = 100%
    // 10000 = 0%
    f32_cooling_demand_pct =
    ((COOLING_FAN_PWM_HIGH_LIMIT -  mt_cooling_fan.t_speed_ramp.f32_output) * 100.0F) /
    (COOLING_FAN_PWM_HIGH_LIMIT - COOLING_FAN_PWM_LOW_LIMIT);

    f32_cooling_demand_pct = CLAMP_F32(f32_cooling_demand_pct, 0.0F, 100.0F);

    mt_cooling_fan.pt_cp_cooling->f32_cooling_demand_pct = f32_cooling_demand_pct;
    // Hardware outputs
    set_outputValue("COOL_FAN_SPEED",  mt_cooling_fan.t_speed_ramp.f32_output);
    set_outputValue("COOL_FAN_DIRECTION",  mt_cooling_fan.t_dir_ramp.f32_output);

    // FR-7.15 CAN/display outputs
    *(mt_cooling_fan.pu16_disp_hyd_oil_temp_degC) = f32_hyd_oil_temp_degC;
    *(mt_cooling_fan.pu8_disp_fan_reverse_ind) =
    (mt_cooling_fan.u8_fan_direction == COOLING_FAN_REVERSE) ? TRUE : FALSE;
    *(mt_cooling_fan.pu8_disp_cooling_system_fault) = mt_cooling_fan.u8_cooling_fault;
    *(mt_cooling_fan.pu8_disp_hyd_oil_overtemp) =
    (f32_hyd_oil_temp_degC >= COOLING_FAN_T3_OVERTEMP_C) ? TRUE : FALSE;
    *(mt_cooling_fan.pu8_disp_intake_overtemp) =
    (f32_t2_degC >= COOLING_FAN_T2_OVERTEMP_C) ? TRUE : FALSE;
    *(mt_cooling_fan.pu8_disp_coolant_overtemp) =
    (f32_t1_degC >= COOLING_FAN_T1_OVERTEMP_C) ? TRUE : FALSE;

    return s16_error;
}

//EOF
