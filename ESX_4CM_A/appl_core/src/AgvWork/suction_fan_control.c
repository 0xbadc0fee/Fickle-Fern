//-----------------------------------------------------------------------------
/**
 * \file       suction_fan_control.c
 * \brief      AgvWork - Suction Fan Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup SuctionFanControl Suction Fan Control
 *
 * The Suction Fan Control Module shall regulate the PWM-controlled fan
 * output using operator speed requests and current suction fan RPM to ensure
 * consistent airflow and debris removal.
 *
 * @par Project
 * Flory_8772-4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Feb 24, 2026 Tiffany Gohnert
 *
 * @{
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
#include "engine_starter_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

// -- Defines ------------------------------------------------------------------------------------------------------
#define PROGRAM_START_DEB_MS (3000u) //!< Startup debounce delay in milliseconds (3 seconds)
// -- Types --------------------------------------------------------------------------------------------------------
// -- Module Global Function Prototypes ----------------------------------------------------------------------------
sint16 calc_sfSpeed(void); //!< Calculates the SF speed
// -- Module Global Variables --------------------------------------------------------------------------------------
static T_SuctionFanControl mt_suction_fan; //!<  Module-local instance of the suction fan control state structure.

// -- Implementation ------------------------------------------------------------------------------------------------

/** \brief Initialize Suction Fan Control
 *
 *  This function initializes the Suction Fan Control Logic.
 *
 * \param _can_devs Pointer to the project's UI Structure
 * \param _nvmSuctionFan Pointer to Suction Fan NVM
 * \param _chkSuctionFan Pointer to Suction Fan checkpoints
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_suctionFanControl(T_CANDevices     *_can_devs,
                              T_Config_SFan    *_nvmSuctionFan,
                              T_ChkPoints_SFan *_chkSuctionFan)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_devs == NULL) || (_nvmSuctionFan == NULL) || (_chkSuctionFan == NULL))
    {
        return C_WARN;
    }
    //populate local copy of RX ui elements
    mt_suction_fan.pu8_enable_cmd      = &_can_devs->t_joystick.u8_b4_state;
    mt_suction_fan.pu16_speed_req_rpm  = &_can_devs->t_display.u16_sf_speed_req;

    //populate local copy of TX ui elements
    mt_suction_fan.pu8_enable_status     = &_can_devs->t_display.u8_suction_fan_enable_status;
    mt_suction_fan.pu16_speed_status_rpm = &_can_devs->t_display.u16_suction_fan_speed_status_rpm;

    mt_suction_fan.pt_nvm = _nvmSuctionFan;

    mt_suction_fan.pt_cp_sfan = _chkSuctionFan;

    mt_suction_fan.u8_enable_latched     = SUCTION_FAN_DISABLED;

    //Initialize toggle button helper
    s16_error += toggleButton_init(&mt_suction_fan.t_btn_enable,
                                   &mt_suction_fan.u8_enable_latched,
                                   250u,
                                   SUCTION_FAN_DISABLED);

    //Initialize Moving Average Filter helper
    movingFltInit(&mt_suction_fan.t_speed_filter,
                  mt_suction_fan.af32_speed_buf,
                  sizeof(mt_suction_fan.af32_speed_buf)/sizeof(float32),
                  SUCTION_FAN_SAFE_OUTPUT,
                  sizeof(mt_suction_fan.af32_speed_buf)/sizeof(float32),
                  50);

    //Initialize Ramp helper
    s16_error += rampInit(&mt_suction_fan.t_speed_ramp,
                           SUCTION_FAN_RAMP_RATE,
                           SUCTION_FAN_CMD_MIN,
                           SUCTION_FAN_CMD_MAX,
                           SUCTION_FAN_SAFE_SPEED_RPM);

    //PID init
    mt_suction_fan.t_pid_state.f32_prev_error = 0.0F;
    mt_suction_fan.t_pid_state.f32_error_accum = 0.0F;
    mt_suction_fan.t_pid_state.f32_output = SUCTION_FAN_SAFE_OUTPUT;
    mt_suction_fan.t_pid_state.u64_last_time = 0u;

    // PID output is PWM duty cycle percent (0-10000)
    mt_suction_fan.t_pid_coeff.f32_kp = 5.0F;
    mt_suction_fan.t_pid_coeff.f32_ki = 2.0F;
    mt_suction_fan.t_pid_coeff.f32_kd = 0.0F;
    mt_suction_fan.t_pid_coeff.s32_min_output = 0;
    mt_suction_fan.t_pid_coeff.s32_max_output = 10000;

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
    uint8 u8_speed_fault = FALSE;
    uint8 u8_output_fault = FALSE;
    uint8 u8_btn_reset = FALSE;

    uint8 u8_engine_status = ENGINE_OFF;
    uint32 u32_engine_runtime = 0;

    uint8 u8_startup_deb_complete = FALSE;

    float32 f32_door_value = DOOR_CLOSED;
    float32 f32_speed_req_rpm = SUCTION_FAN_SAFE_SPEED_RPM;
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

    //Program start / Engine Run debounce Timing
    get_engineRuntime(&u32_engine_runtime);
    get_engineStatus(&u8_engine_status);

    if ((u8_engine_status == ENGINE_RUNNING) && (u32_engine_runtime >= PROGRAM_START_DEB_MS))
    {
        u8_startup_deb_complete = TRUE;
    }

    // FR-5.4 Read speed input and convert frequency to RPM
    s16_error += get_inputFaultStatus("FAN_SPEED", &u8_speed_fault);
    if (u8_speed_fault == TRUE)
    {
        mt_suction_fan.f32_shaft_rpm = SUCTION_FAN_SAFE_OUTPUT;
    }
    else
    {
        s16_error += calc_sfSpeed();
    }

    // FR-5.3 Force disable/reset when interlocks are not satisfied
    if ((u8_door_fault_status == TRUE) ||
    (f32_door_value != DOOR_CLOSED) ||
    (u8_engine_status == ENGINE_OFF) ||
    (u8_startup_deb_complete == FALSE) ||
    (u8_speed_fault == TRUE) ||
    (u8_btn_reset == TRUE))
    {
        u8_enable_reset = TRUE;
    }

    // FR-5.2 Apply latching and reset logic to enable command
    s16_error += toggleButton(&mt_suction_fan.t_btn_enable, u8_enable_cmd, u8_enable_reset);

    //FR-5.7 Force suction fan speed to zero when disabled
    if (mt_suction_fan.u8_enable_latched != SUCTION_FAN_ENABLED)
    {
        f32_speed_req_rpm = SUCTION_FAN_SAFE_OUTPUT;
    }

    //set ramp rate based on if target is higher or lower than current target
    if(f32_speed_req_rpm > mt_suction_fan.f32_prev_req_rpm)
    {
        set_rampRate(&mt_suction_fan.t_speed_ramp, (float32)mt_suction_fan.pt_nvm->u8_fan_inc_time);
    }
    else if (f32_speed_req_rpm <= mt_suction_fan.f32_prev_req_rpm)
    {
        set_rampRate(&mt_suction_fan.t_speed_ramp, (float32)mt_suction_fan.pt_nvm->u8_fan_dec_time);
    }


    s16_error += rampCalc(f32_speed_req_rpm, &mt_suction_fan.t_speed_ramp);

    // FR-5.9 Closed-loop PID control
    if ((u8_btn_reset == FALSE) &&
        (mt_suction_fan.u8_enable_latched == SUCTION_FAN_ENABLED)&&
        (u8_speed_fault == FALSE))
    {
        s16_error += PidOutput(mt_suction_fan.t_speed_ramp.f32_output,
                               mt_suction_fan.f32_shaft_rpm,
                               &mt_suction_fan.t_pid_state,
                               &mt_suction_fan.t_pid_coeff);

        f32_pwm_cmd = mt_suction_fan.t_pid_state.f32_output;
    }
    else if ((mt_suction_fan.u8_enable_latched == SUCTION_FAN_ENABLED)&&
            (u8_speed_fault == TRUE))
    {
        //open loop control?
        //f32_pwm_cmd = mt_suction_fan.t_speed_ramp.f32_output * 100.0f;
        f32_pwm_cmd = SUCTION_FAN_SAFE_OUTPUT;
    }

    else
    {
        f32_pwm_cmd = SUCTION_FAN_SAFE_OUTPUT;
    }

    //Inhibit on logic/output fault
    s16_error += get_outputFaultStatus("FAN_HYDRO_FWD", &u8_output_fault);
    if (u8_output_fault == FALSE)
    {
        s16_error += set_outputValue("FAN_HYDRO_FWD", f32_pwm_cmd);
    }

    //FR-5.6 Update CAN/display status
    if ((mt_suction_fan.pu8_enable_status != NULL) &&
    (mt_suction_fan.pu16_speed_status_rpm != NULL))
    {
        *(mt_suction_fan.pu8_enable_status) = mt_suction_fan.u8_enable_latched;
        *(mt_suction_fan.pu16_speed_status_rpm) = (uint16)mt_suction_fan.f32_shaft_rpm;
    }

    // Checkpoints
    if (mt_suction_fan.pt_cp_sfan != NULL)
    {
        mt_suction_fan.pt_cp_sfan->u8_suctionFanOn =  mt_suction_fan.u8_enable_latched;
        mt_suction_fan.pt_cp_sfan->u16_pwmStatus = (uint16)f32_pwm_cmd;
    }

    mt_suction_fan.f32_prev_req_rpm = f32_speed_req_rpm;

    return s16_error;
}

/**
 * \brief       Calculates the suction fan shaft speed in RPM.
 *
 * \details     This function determines the suction fan drive ratio based on
 * the NVM configuration (e.g., whether the 6.7L configuration is
 * enabled). It reads the raw fan frequency, converts it to motor
 * RPM using the defined Pulses Per Revolution (SF_PPR), applies
 * a moving average filter to smooth the reading, and finally
 * calculates the actual shaft RPM by applying the drive ratio.
 *
 * \return      sint16 Cumulative error status (C_NO_ERR if successful)
 */
sint16 calc_sfSpeed(void)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_mRPM = 0.0;
    float32 f32_ratio = 209.0;

    //determine what the suction fan drive ratio actually is
    if(mt_suction_fan.pt_nvm->u8_6_7_enable)
        f32_ratio = 209.0;
    else
        f32_ratio = 310.0;

    s16_error += get_inputValue("FAN_SPEED", &mt_suction_fan.f32_fan_frequency);

    f32_mRPM = (mt_suction_fan.f32_fan_frequency/1000.0f) * 60.0f / SF_PPR;

    s16_error += movingAdvFlt(&mt_suction_fan.t_speed_filter, f32_mRPM);

    mt_suction_fan.f32_shaft_rpm = mt_suction_fan.t_speed_filter.f32_out / f32_ratio;

    return s16_error;
}

/** \brief Get AgvWork - Suction Fan Status
 *
 *  This function
 *
 *  Primary logic for this function is
 *
 * *  \param pu8_shaft_drive_status Pointer to the Shaft Drive ON/OFF Status
 *
 *  \return boolean
 */
void get_suctionFanStatus(uint8 *pu8_sfan_status)
{
    if(mt_suction_fan.pu8_enable_status != NULL && pu8_sfan_status != NULL)
    {
        *pu8_sfan_status = mt_suction_fan.u8_enable_latched;
    }
}

