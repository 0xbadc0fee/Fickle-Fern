//-----------------------------------------------------------------------------
/**
 * \file       propulsion_control.c
 * \brief      AgvChassis - Propulsion Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup PropulsionControl Propulsion Control
 *
 * The Propulsion Control Module manages the core movement and drive systems
 * of the machine. It processes operator inputs to safely control vehicle speed,
 * direction, and acceleration, while monitoring drive train parameters and
 * handling motion-related safety interlocks.
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
 * Mar 13, 2026 STW Technic
 *
 * @{
 */
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "propulsion_control.h"
#include "stwerrors.h"
#include "stwtypes.h"
#include "x_stdtypes.h"
#include <stdlib.h>

#include "system.h"
#include "can_device_definition.h"

#include "engine_starter_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
sint16 calc_wheelSpeed(void);
sint16 check_edcInterlocks();
sint16 output_edcValves(void);
sint16 check_ccLimits(void);
sint16 ramp_targetSpeedCommand(E_RampTypes _rampType);
sint16 calc_rampType(void);

sint16 check_joystickInterlocks(void);
sint16 calc_joystickSpeedCommand(void);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_PropulsionControl mt_prop_control; //!< Instance of the propulsion control state structure.

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvChassis - Propulsion Control
 *
 *  This function initializes the AgvChassis - Propulsion Control Logic.
 *
 *  \param _can_dev Pointer to the project's UI Structure
 *  \param _chkPropulsion Pointer to the global Propulsion Checkpoints Structure
 *  \param _nvmPropControl Pointer to the global Propulsion NVM Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_propulsionControl(T_CANDevices *_can_dev, T_ChkPoints_Propulsion *_chkProp, T_Config_Propulsion *_configProp)
{
    sint16 s16_error = C_NO_ERR;

    //populate local copy of TX ui elements
    mt_prop_control.pu8_gear_selector      = &_can_dev->t_display.u8_gear_select;
    mt_prop_control.pu16_joy_y_pos         = &_can_dev->t_joystick.u16_yPos;
    mt_prop_control.pu8_joy_fwd            = &_can_dev->t_joystick.u8_fwd_status;
    mt_prop_control.pu8_joy_rev            = &_can_dev->t_joystick.u8_rev_status;
    mt_prop_control.pu8_speed_limit_enable = &_can_dev->t_display.u8_speed_limit_enable;
    mt_prop_control.pu8_max_speed_set      = &_can_dev->t_display.u8_max_speed_set;

    //populate local copy of RX ui elements
    mt_prop_control.pu8_neutral_state   = &_can_dev->t_display.u8_neutral_state;
    mt_prop_control.pu8_wheel_speed_10  = &_can_dev->t_display.u8_wheel_speed_10;
    mt_prop_control.pu8_speed_limit_set = &_can_dev->t_display.u8_speed_limit_set;

    //populate local copy of NVM elements
    mt_prop_control.pt_chkProp = _chkProp;

    mt_prop_control.pt_config = _configProp;

    //iniitalize helpers
    toggleButton_init(&mt_prop_control.t_active_gear, &mt_prop_control.u8_active_gear, 250, FALSE);
    toggleButton_init(&mt_prop_control.t_speed_limit_enable, &mt_prop_control.u8_speed_limit_enable, 250, FALSE);
    toggleButton_init(&mt_prop_control.t_cc_enable, &mt_prop_control.u8_cc_enable, 250, FALSE);

    rampInit(&mt_prop_control.t_js_command, ACCEL_RATE, 0, 1000, JS_NEUTRAL_POS);

    movingFltInit(&mt_prop_control.t_filter_wheel_speed,
                  mt_prop_control.af32_ws_buf,
                  sizeof(mt_prop_control.af32_ws_buf)/sizeof(float32),
                  0.0f,
                  sizeof(mt_prop_control.af32_ws_buf)/sizeof(float32),
                  50);


    return s16_error;
}

/** \brief Update AgvChassis - Propulsion Control
 *
 *  This function contains the cyclical logic for AgvChassis - Propulsion Control.
 *
 *  Primary logic for this function is to execute the functionality defined in Function 13
 *  of the Functional Requirements - Propulsion Control.
 *
 *  This functionality includes (but is not limited to):
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
    toggleButton(&mt_prop_control.t_active_gear, *(mt_prop_control.pu8_gear_selector), FALSE);

    if(mt_prop_control.u8_active_gear)
        set_outputValue("SHIFT_COIL", (float32)HIGH_SPEED_GEAR);
    else
        set_outputValue("SHIFT_COIL", (float32)LOW_SPEED_GEAR);


    //FR-13.5 & IR13.1 Check EDC Enable Interlocks
    s16_error += check_edcInterlocks();
    mt_prop_control.pt_chkProp->u8_edc_enable = mt_prop_control.u8_edc_enable;

    //FR-13.7/8/12/13/14/15/16/17
    s16_error += calc_joystickSpeedCommand();

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
            //update max speed set (cc) and speed limit enable toggles.
            toggleButton(&mt_prop_control.t_cc_enable,          *(mt_prop_control.pu8_max_speed_set),      FALSE);
            toggleButton(&mt_prop_control.t_speed_limit_enable, *(mt_prop_control.pu8_speed_limit_enable), FALSE);

            //FR-13.7/8/12/13/14/15/16/17
            s16_error += calc_joystickSpeedCommand();

            //FR13.18
            s16_error += ramp_targetSpeedCommand(calc_rampType());
        }
        else
        {
            mt_prop_control.f32_raw_output = 0.0;
            s16_error += ramp_targetSpeedCommand(E_MAX_DECCEL_RAMP);
        }
    }

    //FR-13.9 Neutral Indicator to Display
    *(mt_prop_control.pu8_neutral_state) = mt_prop_control.u8_neutral_ind;

    //FR-13.10 Reverse Indicator Output
    s16_error += set_outputValue("BACKUP_ALARM", mt_prop_control.u8_reverse_ind);

    //Populate CAN and Checkpoints
    mt_prop_control.pt_chkProp->f32_wheel_speed_10 = mt_prop_control.f32_wheel_speed_mph * 10;
    *(mt_prop_control.pu8_wheel_speed_10) = mt_prop_control.f32_wheel_speed_mph * 10;

    mt_prop_control.u8_prev_joystick_state = mt_prop_control.u8_joystick_state;
    mt_prop_control.u16_prev_joystick_command = mt_prop_control.u16_joystick_command;


    //update hardware output values
    s16_error += output_edcValves();



    return s16_error;
}

/**
 * \brief Evaluates safety interlocks to determine if joystick speed commands can be enabled.
 *
 *  This function checks multiple safety parameters before allowing machine movement:
 * - Cab Door state (must be closed)
 * - Cab Door sensor fault status (must not be faulted)
 * - Parking Brake sensor fault status (must not be faulted)
 * - Joystick Y-axis position (must not be in a fault state)
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 check_joystickInterlocks(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_door_state = FALSE;
    uint8 u8_door_status = FALSE;
    uint8 u8_pb_status = FALSE;

    s16_error += get_inputValue("CAB_DOOR", &f32_door_state);
    s16_error += get_inputFaultStatus("CAB_DOOR", &u8_door_status);
    s16_error += get_inputFaultStatus("PARK_BRAKE", &u8_pb_status);

    if(f32_door_state || u8_door_status || u8_pb_status || *(mt_prop_control.pu16_joy_y_pos)  == JOYSTICK_FAULT)
        mt_prop_control.u8_speed_enable = FALSE;
    else
        mt_prop_control.u8_speed_enable = TRUE;

    return s16_error;
}

/**
 * \brief Translates raw joystick movement into a target speed and direction.
 *
 * This function takes the operator's joystick input, applies any active speed limits
 * or cruise control rules, calculates the appropriate output speed, and determines
 * if the machine should be in forward, reverse, or neutral.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 calc_joystickSpeedCommand(void)
{
    sint16 s16_error = C_NO_ERR;

    mt_prop_control.u16_joystick_command = *(mt_prop_control.pu16_joy_y_pos)*10;

    //FR-13.16/17 - Speed Limit Functionality
    if(mt_prop_control.u8_speed_limit_enable)
    {
        if(mt_prop_control.u16_joystick_command >= SPEED_LIMIT_PER)
            mt_prop_control.u16_joystick_command = SPEED_LIMIT_PER;
    }

    //FR-12/13/14/15 - Cruise Control Functionality
    s16_error += check_ccLimits();

    //FR13.7 Joystick Command Calculation
    if (mt_prop_control.u16_joystick_command < 250.0)
        mt_prop_control.f32_raw_output = 0.0;

    else if (mt_prop_control.u16_joystick_command <= 750.0)
        mt_prop_control.f32_raw_output = (2.0 * mt_prop_control.u16_joystick_command) + 500.0;

    else if (mt_prop_control.u16_joystick_command <= 1500.0)
        mt_prop_control.f32_raw_output = ((4.0 / 3.0) * mt_prop_control.u16_joystick_command) + 1000.0;

    else if (mt_prop_control.u16_joystick_command <= 9000.0)
        mt_prop_control.f32_raw_output = ((4.0 / 5.0) * mt_prop_control.u16_joystick_command) + 1800.0;

    else
        mt_prop_control.f32_raw_output = 9000.0;

    //FR-13.8 Joystick State Calculation
    if(*(mt_prop_control.pu8_joy_fwd) && mt_prop_control.u16_joystick_command >= NEUTRAL_DEADBAND)
        mt_prop_control.u8_joystick_state = E_JOYSTICK_FWD;

    else if(*(mt_prop_control.pu8_joy_rev) && mt_prop_control.u16_joystick_command >= NEUTRAL_DEADBAND)
        mt_prop_control.u8_joystick_state = E_JOYSTICK_REV;

    else if(mt_prop_control.u16_joystick_command < NEUTRAL_DEADBAND)
        mt_prop_control.u8_joystick_state = E_JOYSTICK_NEU;

    switch(mt_prop_control.u8_joystick_state)
    {
        case E_JOYSTICK_FWD:
            mt_prop_control.u8_neutral_ind = FALSE;
            mt_prop_control.u8_reverse_ind = FALSE;
            mt_prop_control.f32_raw_output = ((mt_prop_control.f32_raw_output / 10000.0f)
                                              * (mt_prop_control.pt_config->u16_max_curr_fwd - mt_prop_control.pt_config->u16_min_curr_fwd))
                                              + mt_prop_control.pt_config->u16_min_curr_fwd;

            mt_prop_control.t_js_command.f32_min_limit = mt_prop_control.pt_config->u16_min_curr_fwd;
            mt_prop_control.t_js_command.f32_max_limit = mt_prop_control.pt_config->u16_max_curr_fwd;
            break;

        case E_JOYSTICK_REV:
            mt_prop_control.u8_neutral_ind = FALSE;
            mt_prop_control.u8_reverse_ind = TRUE;
            mt_prop_control.f32_raw_output = ((mt_prop_control.f32_raw_output / 10000.0f)
                                              * (mt_prop_control.pt_config->u16_max_curr_rev - mt_prop_control.pt_config->u16_min_curr_rev))
                                              + mt_prop_control.pt_config->u16_min_curr_rev;

            mt_prop_control.t_js_command.f32_min_limit = mt_prop_control.pt_config->u16_min_curr_rev;
            mt_prop_control.t_js_command.f32_max_limit = mt_prop_control.pt_config->u16_max_curr_rev;
            break;

        case E_JOYSTICK_NEU:
            mt_prop_control.u8_neutral_ind = TRUE;
            mt_prop_control.u8_reverse_ind = FALSE;
            break;
    }

    return s16_error;
}

/**
 * \brief Evaluates cruise control limits and caps the speed command if active.
 *
 * This function manages the cruise control state. When cruise control is activated,
 * it latches the current joystick speed as the maximum limit and prevents the
 * operator's command from exceeding that saved speed.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 check_ccLimits(void)
{
    sint16 s16_error = C_NO_ERR;
    static uint8 u8_prev_active = FALSE;

    //set active or inactive flag
    if(mt_prop_control.u8_cc_enable)
        mt_prop_control.u8_cc_active = TRUE;
    else
    {
        if(u8_prev_active && (mt_prop_control.u16_joystick_command >= mt_prop_control.u16_cc_max_speed))
            mt_prop_control.u8_cc_active = TRUE;
        else
            mt_prop_control.u8_cc_active = FALSE;
    }

    //check if toggled active from inactive
    if(mt_prop_control.u8_cc_active && !u8_prev_active)
        mt_prop_control.u16_cc_max_speed = mt_prop_control.u16_joystick_command;

    //set limit
    if(mt_prop_control.u8_cc_active && (mt_prop_control.u16_joystick_command >= mt_prop_control.u16_cc_max_speed))
        mt_prop_control.u16_joystick_command = mt_prop_control.u16_cc_max_speed;

    //set previously active flag
    u8_prev_active = mt_prop_control.u8_cc_active;


    return s16_error;

}

/**
 * \brief Determines the appropriate speed ramp profile based on joystick movement.
 *
 * This function compares the current joystick position and directional state against
 * the previous cycle to decide if the machine should apply an acceleration,
 * deceleration, or direction-change ramp.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 calc_rampType(void)
{
    sint16 s16_error = C_NO_ERR;

    if(mt_prop_control.u8_prev_joystick_state != mt_prop_control.u8_joystick_state &&
       mt_prop_control.u8_joystick_state != E_JOYSTICK_NEU)
    {
        mt_prop_control.e_rampType = E_CHANGE_DIR_RAMP;
    }

    if(mt_prop_control.u16_joystick_command >= mt_prop_control.u16_prev_joystick_command)
    {
        mt_prop_control.e_rampType = E_ACCEL_RAMP;
    }

    if(mt_prop_control.u16_joystick_command < mt_prop_control.u16_prev_joystick_command)
    {
        mt_prop_control.e_rampType = E_DECCEL_RAMP;
    }

    return s16_error;;
}

/**
 * \brief Applies the selected ramp profile to smooth the target speed command.
 *
 * This function sets the appropriate ramping rate (acceleration, deceleration,
 * max deceleration, or direction change) based on the calculated ramp type,
 * and then computes the smoothed speed output to prevent sudden, jerky movements.
 *
 * \param [in] _rampType The specific ramp profile to apply to the speed command.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 ramp_targetSpeedCommand(E_RampTypes _rampType)
{
    sint16 s16_error = C_NO_ERR;

    switch(_rampType)
    {
        case E_ACCEL_RAMP:
            set_rampRate(&mt_prop_control.t_js_command, ACCEL_RATE);
            break;

        case E_DECCEL_RAMP:
            set_rampRate(&mt_prop_control.t_js_command, DECCEL_RATE);
            break;

        case E_MAX_DECCEL_RAMP:
            set_rampRate(&mt_prop_control.t_js_command, MAX_DECCEL_RATE);
            break;

        case E_CHANGE_DIR_RAMP:
            set_rampRate(&mt_prop_control.t_js_command, CHG_DIR_RATE);
            break;

        default:
            set_rampRate(&mt_prop_control.t_js_command, ACCEL_RATE);
            break;

    }

    rampCalc(mt_prop_control.f32_raw_output, &mt_prop_control.t_js_command);
    mt_prop_control.f32_ramped_output = mt_prop_control.t_js_command.f32_output;

    return s16_error;

}

/**
 * \brief Calculates the vehicle's wheel speed based on sensor frequency.
 *
 * This function reads the raw wheel speed sensor frequency, converts it to RPM,
 * applies a moving average filter to smooth the reading, and computes the
 * final ground speed in miles per hour (MPH) using the wheel diameter and gear ratio.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 calc_wheelSpeed(void)
{

    sint16 s16_error = C_NO_ERR;
    float32 f32_rpm = 0.0;

    s16_error += get_inputValue("WHEEL_SPEED", &mt_prop_control.f32_wheel_frequency);

    f32_rpm = (mt_prop_control.f32_wheel_frequency/1000.0f) * 60.0f / WHEEL_PPR;
    mt_prop_control.pt_chkProp->f32_wheel_rpm = f32_rpm;

    s16_error += movingAdvFlt(&mt_prop_control.t_filter_wheel_speed, f32_rpm);

    mt_prop_control.f32_wheel_speed_mph = (mt_prop_control.t_filter_wheel_speed.f32_out * mt_prop_control.pt_config->f32_tire_diameter) / (GEAR_RATIO * 336.0f);

    return s16_error;
}

/**
 * \brief Evaluates safety interlocks to determine if the EDC valves can be enabled.
 *
 * This function checks the engine status, startup delay, parking brake state,
 * and propel valve fault statuses. If all conditions are safe and the engine
 * is running, it sets the enable flag to TRUE; otherwise, it is set to FALSE.
 *
 * \param [out] u8_edc_enable Pointer to store the evaluated EDC enable flag.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 check_edcInterlocks(void)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_park_brake = TRUE;
    uint8 u8_fwd_status = 0;
    uint8 u8_rev_status = 0;
    uint8 u8_engine_status = ENGINE_RUNNING;

    s16_error += get_inputValue("PARK_BRAKE", &f32_park_brake);
    s16_error += get_outputFaultStatus("PROPEL_FWD", &u8_fwd_status);
    s16_error += get_outputFaultStatus("PROPEL_REV", &u8_rev_status);

    //FR-13.5 Interlock Logic
    get_engineStatus(&u8_engine_status);

    if(u8_engine_status == ENGINE_RUNNING)
    {
        if(get_system_time_ms() < EDC_STARTUP_DELAY || u8_fwd_status || u8_rev_status || f32_park_brake)
           mt_prop_control.u8_edc_enable = FALSE;
        else
            mt_prop_control.u8_edc_enable = TRUE;
    }
    else
    {
        mt_prop_control.u8_edc_enable = FALSE;
    }

    return s16_error;
}

/**
 * \brief Outputs the commanded speed to the appropriate EDC propel valves.
 *
 * This function routes the calculated, ramped output signal to either the forward
 * or reverse propel valve based on the current joystick state. It ensures that
 * the inactive direction (or both in neutral/default states) is commanded to zero.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 output_edcValves(void)
{
    sint16 s16_error = C_NO_ERR;

    switch(mt_prop_control.u8_joystick_state)
    {
        case E_JOYSTICK_FWD:
            s16_error += set_outputValue("PROPEL_FWD", mt_prop_control.f32_ramped_output);
            s16_error += set_outputValue("PROPEL_REV", 0.0);

            mt_prop_control.pt_chkProp->u16_edc_fwd_curr = (uint16)mt_prop_control.f32_ramped_output;
            mt_prop_control.pt_chkProp->u16_edc_rev_curr = 0;

            break;

        case E_JOYSTICK_REV:
            s16_error += set_outputValue("PROPEL_FWD", 0.0);
            s16_error += set_outputValue("PROPEL_REV", mt_prop_control.f32_ramped_output);

            mt_prop_control.pt_chkProp->u16_edc_fwd_curr = 0;
            mt_prop_control.pt_chkProp->u16_edc_rev_curr = (uint16)mt_prop_control.f32_ramped_output;
            break;

        case E_JOYSTICK_NEU:
            s16_error += set_outputValue("PROPEL_FWD", 0.0);
            s16_error += set_outputValue("PROPEL_REV", 0.0);

            mt_prop_control.pt_chkProp->u16_edc_fwd_curr = 0;
            mt_prop_control.pt_chkProp->u16_edc_rev_curr = 0;
            break;

        default:
            s16_error += set_outputValue("PROPEL_FWD", 0.0);
            s16_error += set_outputValue("PROPEL_REV", 0.0);

            mt_prop_control.pt_chkProp->u16_edc_fwd_curr = 0;
            mt_prop_control.pt_chkProp->u16_edc_rev_curr = 0;
            break;
    }

    return s16_error;
}

/**
 * \brief Retrieves the current calculated wheel speed.
 *
 * \param [out] _wheelSpeed Pointer to store the current wheel speed in MPH.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 get_wheelSpeed(float32 *_wheelSpeed)
{
    sint16 s16_error = C_NO_ERR;

    *(_wheelSpeed) = mt_prop_control.f32_wheel_speed_mph;

    return s16_error;
}

/**
 * \brief Retrieves the currently active gear selection.
 *
 * \param [out] _gear_selection Pointer to store the active gear state.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 get_gearSelection(uint8 *_gear_selection)
{
    sint16 s16_error = C_NO_ERR;

    *(_gear_selection) = mt_prop_control.u8_active_gear;

    return s16_error;
}

/**
 * \brief Retrieves the neutral status indicator of the joystick.
 *
 * \param [out] _neutral_status Pointer to store the joystick neutral status flag.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 get_joystickNeutralStatus(uint8 *_neutral_status)
{
    sint16 s16_error = 0;

    *(_neutral_status) = mt_prop_control.u8_neutral_ind;

    return s16_error;
}

//EOF
