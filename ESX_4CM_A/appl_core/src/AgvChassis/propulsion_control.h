/*! \file       propulsion_control.h
    \brief      <description>


   	\implementation
   	project     Flory_8772-4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Mar 13, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_AGVCHASSIS_PROPULSION_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_PROPULSION_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "helper_control.h"
#include "input_handler_lib.h"
#include "output_handler_lib.h"
#include "can_engine.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define JOYSTICK_LIMIT_PERCENT 47.5f  //!<Percentage limit of the Joystick Y Axis When Speed Limit is enabled.
#define WHEEL_DIAMETER         31.5f  //!<Wheel Diameter in Inches
#define WHEEL_PPR              44.0f  //!<Pulses per wheel revolution
#define GEAR_RATIO             2.517f //!<Flory Wheel Gear Ratio

#define HIGH_SPEED_GEAR        1
#define LOW_SPEED_GEAR         0

#define EDC_STARTUP_DELAY      4000     //!<40000ms EDC enable startup delay.

#define NEUTRAL_DEADBAND      250.0f    //!<Joystick deadband of +/- 250 units
#define SPEED_LIMIT_PER      4750.0f    //!<when Speed enviro is selected - limit joystick Y pos to 47.5%

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef enum
{
    E_JOYSTICK_FWD = 0,
    E_JOYSTICK_REV,
    E_JOYSTICK_NEU,
    E_NUM_JOYSTICK_STATES
}E_JoystickStates;

typedef enum
{
    E_NO_RAMP = 0,
    E_ACCEL_RAMP,
    E_DECCEL_RAMP,
    E_CHANGE_DIR_RAMP,
    E_MAX_DECCEL_RAMP,
    E_NUM_RAMPS
}E_RampTypes;

/** \brief Checkpoints Structure - Elevator Control
 *
 * This structure represents all checkpoints that are relevant
 * to elevator control
 */
typedef struct
{
    float32 f32_wheel_rpm;                    //!<Wheel RPM Checkpoint
    float32 f32_wheel_speed_10;               //!<Wheel Speed MPH x 10 Checkpoint

    uint8 u8_edc_enable;                      //!<EDC Enable/Disable Status Checkpoint

}T_ChkPoints_Propulsion;


/** \brief Control Structure - Propulsion Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for propulsion control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
    //Local Control Variables


    T_ToggleBtn t_active_gear;              //!<Active Gear Toggle Variable
    T_ToggleBtn t_speed_limit_enable;       //!<Speed Limit Enable Toggle Variable
    T_ToggleBtn t_cc_enable;                //!< Cruise Control Enable Toggle Varialbe

    float32 f32_wheel_frequency;             //!<Wheel speed in mHz
    float32 f32_wheel_speed_mph;            //!<Wheel Speed in MPH

    uint8 u8_edc_enable;                    //!<True/False variable when EDC drive is enabled or disabled
    uint8 u8_reverse_ind;                   //!<True/False variable when Joystick is detected to be in Reverse

    uint8 u8_joystick_state;
    uint8 u8_prev_joystick_state;
    uint8 u8_speed_ramp_type;                //!<Tracker for what type of accel/deccel ramp will be used
    uint8 u8_neutral_ind;

    uint8 u8_speed_enable;
    sint16  s16_yPos;                       //!<Local Variable for Joystick Y Position
    float32 f32_raw_output;                 //!<Raw/ unramped output value
    float32 f32_ramped_output;              //!<Ramped output value to valves

    uint8 u8_cc_active;
    uint8 u8_cc_prev_active;
    uint16 u16_cc_max_speed;


    //TX CAN Variables
    uint8 *pu8_neutral_state;               //!<Pointer to the Neutral State to Display
    uint8 *pu8_wheel_speed_10;              //!<Wheel Speed (MPH x 10) to Display
    uint8 *pu8_speed_limit_set;

    //RX CAN Variables
    uint8  *pu8_gear_selector;              //!<Local variable to hold the gear selector command from Display
    sint16 *ps16_joy_y_pos;                 //!<Pointer to Joystick Y Position
    uint8  *pu8_speed_limit_enable;         //!<Pointer to Speed Limit Enable Button from Display
    uint8  *pu8_max_speed_set;              //!<Pointer to Max Speed Set Button from Display

    //Engine Variable Pointer
    uint8  *pu8_engine_status;

    //Control Checkpoints
    T_ChkPoints_Propulsion *pt_chkProp;     //!<Propulsion Control Checkpoints Structure


}T_PropulsionControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVCHASSIS_PROPULSION_CONTROL_H_ */

