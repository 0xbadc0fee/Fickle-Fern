//-----------------------------------------------------------------------------
/**
 * \file       elevator_control.h
 * \brief      AgvWork - Elevator Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup ElevatorControl Elevator Control
 *
 * This module manages the operation of the vehicle's elevator system. It
 * processes operator inputs to control elevator engagement, speed adjustments,
 * and ensures safe operation through integrated interlock logic and state
 * monitoring.
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

#ifndef APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_

#include "stwtypes.h"
#include "can_device_definition.h"
#include "toggle_button.h"
#include "ramp_calc.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define ELEVATOR_ON              (1u)         //!< Elevator enabled/on state
#define ELEVATOR_OFF             (0u)         //!< Elevator disabled/off state

#define ELEVATOR_MIN_CURRENT_MA  (300.0F)     //!< Minimum operational current for the elevator in mA
#define ELEVATOR_MAX_CURRENT_MA  (1000.0F)    //!< Maximum operational current for the elevator in mA

#define ELEVATOR_RAMP_RATE       (250.0f)     //!< Maximum rate of change for elevator current (250mA / Second)

#define ELEVATOR_SAFE_STATE      (0.0F)       //!< Safe default current or state for the elevator upon failure

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_ChkPoints_Elevator
 * \brief Checkpoints Structure - Elevator Control */
typedef struct
{
        uint8 u8_status;                    //!<Checkpoint #1
        float32 f32_speed_cmd;              //!<Checkpoint #2
        float32 f32_output;                 //!<Checkpoint #3
}T_ChkPoints_Elevator;

/**
 * \struct T_Config_Elevator
 * \brief NVM Configuration Structure - Elevator Control
 *
 * This structure holds persistent configuration parameters for the elevator limits
 * and hardware enables.
 */
typedef struct
{
        float32 f32_max_current;
        uint8 u8_vl3512_enable;
        uint8 u8_vl3514_enable;
}T_Config_Elevator;

/**
 * \struct T_ElevatorControl
 * \brief Main Control Structure - Elevator
 *
 * Contains all local state variables, CAN variable pointers, configuration data,
 * and checkpoints necessary to operate the elevator subsystem.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_onOffCommand;                  //!<Local On Off Command Variable
        uint8 u8_speedCommand;                  //!<Local Speed Command Variable
        uint8 u8_speedFeedback;                 //!<Local Speed Feedback Variable
        uint8 u8_elevator_enabled;              //!< Active flag indicating if the elevator hardware is currently enabled

        //TX CAN Variables
        uint8 *pu8_elevatorStatus;              //!<On/Off Status of Elevator (To Display)
        uint8 *pu8_elevatorButtonColor;         //!<Button color code for the Elevator On/Off button (To Button Panel)

        //RX CAN Variables
        uint8 *pu8_requestedSpeed;              //!<Elevator Requested Speed (From Display)
        uint8 *pu8_onOffCommand;                //!<Elevator On/Off Command (From Joystick)

        //Ramp Control
        T_RampState  t_ramp_state;              //!< Ramp control state structure for smoothing elevator movements

        //NVM Configuration Parameters
        T_Config_Elevator *pt_nvmElevator;      //!<Elevator Control Configuration Structure

        //Control Checkpoints
        T_ChkPoints_Elevator *pt_chkElevator;   //!<Elevator Control Checkpoints Structure

        // Toggle Button
        T_ToggleBtn t_btn_enable;     //!< Toggle Button Control

}T_ElevatorControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_elevatorControl(T_CANDevices *_can_dev, T_ChkPoints_Elevator *_chkElevator, T_Config_Elevator *_nvmElevator);
sint16 update_elevatorControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_ */
