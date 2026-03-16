//-----------------------------------------------------------------------------
/*! \file       propulsion_control.c
    \brief      <description>

    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Mar 13, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "propulsion_control.h"
#include "stwerrors.h"
#include "stwtypes.h"
#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_PropulsionControl mt_prop_control;

//Wheel Speed Moving Average Filter Parameters
T_MoveAvgFilter     mt_filter_wheel_speed;
static const T_MoveAvgCfg mt_wheel_filter_config = {
                            .u16_sample_time_ms = 100,
                            .u16_sample_no      = 10,
                            .f32_safe_output    = 0.0
                            };

//Speed Change Ramping Parameters
T_RampState         mt_speed_ramp;
T_RampParams        mt_accel_config;
T_RampParams        mt_deccel_config;
T_RampParams        mt_change_dir_config;
T_RampParams        mt_max_deccel_config;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvChassis - Propulsion Control
 *
 *  This function initializes the AgvChassis - Propulsion Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkPropulsion Pointer to the global Propulsion Checkpoints Structure
 *  \param _nvmPropControl Pointer to the global Propulsion NVM Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_propulsionControl(T_UserInterface *_ui, T_Engine *_engine, T_ChkPoints_Propulsion *_chkProp)
{
    sint16 s16_error = C_NO_ERR;

    //populate local copy of TX ui elements
    mt_prop_control.pu8_gear_selector      = &_ui->t_display.u8_gear_select;
    mt_prop_control.ps16_joy_y_pos         = &_ui->t_joystick.s16_yPos;
    mt_prop_control.pu8_speed_limit_enable = &_ui->t_display.u8_speed_limit_enable;
    mt_prop_control.pu8_max_speed_set      = &_ui->t_display.u8_max_speed_set;


    //populate local copy of RX ui elements
    mt_prop_control.pu8_neutral_state   = &_ui->t_display.u8_neutral_state;
    mt_prop_control.pu8_wheel_speed_10  = &_ui->t_display.u8_wheel_speed_10;
    mt_prop_control.pu8_speed_limit_set = &_ui->t_display.u8_speed_limit_set;

    //populate local copy of engine elements
    mt_prop_control.pu8_engine_status = &_engine->u8_engineStatus;

    //populate local copy of NVM elements
    mt_prop_control.pt_chkProp = _chkProp;

    //iniitalize command variables

    return s16_error;
}

/** \brief Update AgvChassis - Propulsion Control
 *
 *  This function contains the cyclical logic for AgvChassis - Propulsion Control.
 *
 *  Primary logic for this function is to execute the functionality defined in Function 13
 *  of the Functional Requirements - Propulsion Control.
 *
 *  This funcitonality includes (but is not limited to):
 *
 *  - Gear Selector Functionality
 *  - Wheel Speed Calculation
 *  - Propulsion Valves / Pump Control
 *  - Speed Control Functionality.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_propulsionControl(void)
{
    sint16 s16_error = C_NO_ERR;

    //FR-13.1 Calculate Wheel Speed
    s16_error += calc_wheelSpeed();

    //FR-13.2/4 High/Low Gear Selection
    if(t_active_gear.u8_state)
        set_outputValue("GEAR_SELECTOR", (float32)HIGH_SPEED_GEAR);
    else
        set_outputValue("GEAR_SELECTOR", (float32)LOW_SPEED_GEAR);


    //FR-13.5 & IR13.1 Check EDC Enable Interlocks
    s16_error += check_edcInterlocks(&mt_prop_control.u8_edc_enable);
    mt_prop_control.pt_chkProp->u8_edc_enable = mt_prop_control.u8_edc_enable;


    //FR-13.6 - EDC Disabled
    if(!mt_prop_control.u8_edc_enable)
    {
        //Ramp speed command to 0
        mt_prop_control.f32_raw_output = 0.0;
        s16_error += ramp_targetSpeedCommand(E_MAX_DECCEL_RAMP);
    }
    else
    {
        //IR-13.2/3
        s16_error += check_joystickInterlocks();

        if(mt_prop_control.u8_speed_enable)
        {

            //FR-13.7/8/12/13/14/15/16/17
            s16_error += calc_joystickSpeedCommand();

            //FR13.18
            s16_error += calc_rampType();
            s16_error += ramp_targetSpeedCommand();
        }
        else
        {
            mt_prop_control.f32_raw_output = 0.0;
            s16_error += ramp_targetSpeedCommand();
        }
    }

    //FR-13.9 Neutral Indicator to Display
    mt_prop_control->pu8_neutral_state = mt_prop_control.u8_neutral_ind;

    //FR-13.10 Reverse Indicator Output
    s16_error += set_outputValue("REV_IND", mt_prop_control.u8_reverse_ind);

    //Populate CAN and Checkpoints
    mt_prop_control.pt_chkProp->f32_wheel_speed_10 = mt_prop_control.f32_wheel_speed_mph * 10;
    mt_prop_control->pu8_wheel_speed_10 = mt_prop_control.f32_wheel_speed_mph * 10;


    //update hardware output values
    s16_error += output_edcValves();

    return s16_error;

}

sint16 check_joystickInterlocks()
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_door_state = FALSE;
    uint8 u8_door_status = FALSE;
    uint8 u8_pb_status = FALSE;

    s16_error += get_inputValue("DOOR_SWITCH", &(float32*)u8_door_state);
    s16_error += get_inputFaultStatus("DOOR_SWITCH", &u8_door_status);
    s16_error += get_inputFaultStatus("PARK_BRAKE", &u8_pb_status);

    if(u8_door_state || u8_door_status ||u8_pb_status || mt_prop_control.s16_yPos == JOYSTICK_FAULT)
        mt_prop_control.u8_speed_enable = FALSE;

    else
        mt_prop_control.u8_speed_enable = TRUE;


    return s16_error;
}

sint16 calc_joystickSpeedCommand()
{
    sint16 s16_error = C_NO_ERR;
    uint16 u16_command_magnitude;
    float32 f32_target_output = 0;

    u16_command_magnitude = abs(mt_prop_control.s16_yPos);

    //FR-13.16/17 - Speed Limit Functionality
    if(*(mt_prop_control.pu8_speed_limit_enable))
    {
        if(u16_command_magnitude >= SPEED_LIMIT_PER)
            u16_command_magnitude = SPEED_LIMIT_PER;
    }

    //FR-12/13/14/15 - Cruise Control Functionality
    if(mt_prop_control.u8_cc_active)
    {
        s16_error += check_ccLimits(u16_command_magnitude);
    }

    //FR13.7 Joystick Command Calculation
    if (u16_command_magnitude < 250.0)
        mt_prop_control.f32_raw_output = 0.0;

    else if (u16_command_magnitude <= 750.0)
        mt_prop_control.f32_raw_output = (2.0 * u16_command_magnitude) + 500.0;

    else if (u16_command_magnitude <= 1500.0)
        mt_prop_control.f32_raw_output = ((4.0 / 3.0) * u16_command_magnitude) + 1000.0;

    else if (u16_command_magnitude <= 9000.0)
        mt_prop_control.f32_raw_output = ((4.0 / 5.0) * u16_command_magnitude) + 1800.0;

    else
        mt_prop_control.f32_raw_output = 9000.0;




    //FR-13.8 Joystick State Calculation
    if(mt_prop_control.s16_yPos > 0 && u16_command_magnitude >= NEUTRAL_DEADBAND)
        mt_prop_control.u8_joystick_state = E_JOYSTICK_FWD;

    else if(mt_prop_control.s16_yPos < 0 && u16_command_magnitude >= NEUTRAL_DEADBAND)
        mt_prop_control.u8_joystick_state = E_JOYSTICK_REV;

    else if(u16_command_magnitude < NEUTRAL_DEADBAND)
        mt_prop_control.u8_joystick_state = E_JOYSTICK_NEU;

    switch(mt_prop_control.u8_joystick_state)
    {
        case E_JOYSTICK_FWD:
            mt_prop_control.u8_neutral_ind = FALSE;
            mt_prop_control.u8_reverse_ind = FALSE;
            break;

        case E_JOYSTICK_REV:
            mt_prop_control.u8_neutral_ind = FALSE;
            mt_prop_control.u8_reverse_ind = TRUE;
            break;

        case E_JOYSTICK_NEU:
            mt_prop_control.u8_neutral_ind = TRUE;
            mt_prop_control.u8_reverse_ind = FALSE;
            break;
    }

    return s16_error;
}

sint16 check_ccLimits(uint16 u16_command)
{

    if(mt_prop_control.t_cc_enable.pu_btn_state)
    {
        if()
        u16_command
    }

}

sint16 ramp_targetSpeedCommand(E_RampTypes _rampType)
{
    sint16 s16_error = C_NO_ERR;

    switch(_rampType)
    {
        case E_ACCEL_RAMP:
            rampCalc(mt_prop_control.f32_raw_output, &mt_accel_config, &mt_speed_ramp);
            break;

        case E_DECCEL_RAMP:
            rampCalc(mt_prop_control.f32_raw_output, &mt_deccel_config, &mt_speed_ramp);
            break;

        case E_MAX_DECCEL_RAMP:
            rampCalc(mt_prop_control.f32_raw_output, &mt_max_deccel_config, &mt_speed_ramp);
            break;

        case E_CHANGE_DIR_RAMP:
            rampCalc(mt_prop_control.f32_raw_output, &mt_change_dir_config, &mt_speed_ramp);
            break;

        default:
            rampCalc(mt_prop_control.f32_raw_output, &mt_max_deccel_config, &mt_speed_ramp);
            break;

    }

    mt_prop_control.f32_ramped_output = mt_speed_ramp.f32_output;

    return s16_error;

}


sint16 calc_wheelSpeed(void)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_rpm = 0.0;
    float32 f32_filtered_rpm = 0.0;

    s16_error += get_inputValue("WHEEL_SPEED", &mt_prop_control.f32_wheel_frequency);

    f32_rpm = mt_prop_control.f32_wheel_frequency * 60.0 / WHEEL_PPR;
    mt_prop_control.pt_chkProp->f32_wheel_rpm = f32_rpm;

    //TODO_STW: Alter this call after moving average filter rework is done.
    //s16_error += movingAvgFlt(&mt_filter_wheel_speed, &mt_wheel_filter_config, f32_rpm);

    mt_prop_control.f32_wheel_speed_mph = (mt_filter_wheel_speed.f32_out * WHEEL_DIAMETER) / (GEAR_RATIO * 336.0f);
    return s16_error;
}

sint16 check_edcInterlocks(uint8 *u8_edc_enable)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_park_brake = TRUE;
    uint8 u8_fwd_status = 0;
    uint8 u8_rev_status = 0;

    s16_error += get_inputValue("PARK_BRAKE", &f32_park_brake);
    s16_error += get_outputFaultStatus("EDC_FWD_A", &u8_fwd_status);
    s16_error += get_outputFaultStatus("EDC_FWD_B", &u8_rev_status);

    //FR-13.5 Interlock Logic
    if( *(mt_prop_control.pu8_engine_status) == ENGINE_RUNNING)
    {
        if(get_system_time_ms() < EDC_STARTUP_DELAY || u8_fwd_status || u8_rev_status || f32_park_brake)
            u8_edc_enable = FALSE;
        else
            u8_edc_enable = TRUE;
    }
    else
    {
        u8_edc_enable = FALSE;
    }

    return s16_error;
}

sint16 output_edcValves()
{
    sint16 s16_error = C_NO_ERR;

    switch(mt_prop_control.u8_joystick_state)
    {
        case E_JOYSTICK_FWD:
            s16_error += set_outputValue("EDC_FWD_A", mt_prop_control.f32_ramped_output);
            s16_error += set_outputValue("EDC_REV_B", 0.0);
            break;

        case E_JOYSTICK_REV:
            s16_error += set_outputValue("EDC_FWD_A", 0.0);
            s16_error += set_outputValue("EDC_REV_B", mt_prop_control.f32_ramped_output);
            break;

        case E_JOYSTICK_NEU:
            s16_error += set_outputValue("EDC_FWD_A", 0.0);
            s16_error += set_outputValue("EDC_REV_B", 0.0);
            break;

        default:
            s16_error += set_outputValue("EDC_FWD_A", 0.0);
            s16_error += set_outputValue("EDC_REV_B", 0.0);
            break;

    }

    return s16_error;
}


//EOF
