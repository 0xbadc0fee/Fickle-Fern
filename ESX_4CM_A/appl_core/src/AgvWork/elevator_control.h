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

#define ELEVATOR_ON                     (1u)        //!< Elevator engagement state ON
#define ELEVATOR_OFF                    (0u)        //!< Elevator engagement state OFF
#define ELEVATOR_SAFE_STATE             (0.0F)      //!< Default safe state current value (mA)

#define ELEVATOR_MIN_CURRENT_MA         (300.0F)    //!< Minimum allowable current in mA
#define ELEVATOR_MAX_CURRENT_MA         (1000.0F)   //!< Maximum allowable current in mA
#define ELEVATOR_RAMP_RATE              (250.0F)    //!< Maximum rate of change in mA per second

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct ChkPoints_Elevator
 * \brief Checkpoints Structure - Elevator Control
 *
 * This structure represents all checkpoints that are relevant
 * to the operational monitoring of the elevator system.
 */
typedef struct
{
        uint8 u8_status;                    //!<Checkpoint #1
        float32 f32_speed_cmd;                    //!<Checkpoint #2
        float32 f32_output;                         //!<Checkpoint #3
}T_ChkPoints_Elevator;

/**
 * \struct Config_Elevator
 * \brief Configuration Structure - Elevator Control
 *
 * This structure contains the parameters stored in NVM that define
 * the hardware-specific limits and engagement logic for the elevator.
 */
typedef struct
{
        float32 f32_max_current;
        uint8 u8_vl3512_enable;
        uint8 u8_vl3514_enable;
}T_Config_Elevator;

/**
 * \struct ElevatorControl
 * \brief Control Structure - Elevator Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for elevator control that need to
 * persist through cyclic calls (static).
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_onOffCommand;                  //!<Local On Off Command Variable
        uint8 u8_speedCommand;                  //!<Local Speed Command Variable
        uint8 u8_speedFeedback;                 //!<Local Speed Feedback Variable

        //Local Control Variables
        uint8   u8_elevator_enabled;              //!< Master enable flag for elevator logic

        //TX CAN Variables
        uint8 *pu8_elevatorStatus;              //!<On/Off Status of Elevator (To Display)
        uint8 *pu8_elevatorButtonColor;         //!<Button color code for the Elevator On/Off button (To Button Panel)

        //RX CAN Variables
        uint8 *pu8_requestedSpeed;              //!<Elevator Requested Speed (From Display)
        uint8 *pu8_onOffCommand;                //!<Elevator On/Off Command (From Joystick)

        //Ramp Control
        T_RampState t_ramp_state;                 //!< Current state of the speed ramp calculation

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
