//-----------------------------------------------------------------------------
/*! \file       elevator_control.h
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 STW Technic
 */

#ifndef APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_

#include "stwtypes.h"
#include "can_device_definition.h"
#include "toggle_button.h"
#include "ramp_calc.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define ELEVATOR_ON  (1u)
#define ELEVATOR_OFF (0u)

#define ELEVATOR_MIN_CURRENT_MA  (300.0F)
#define ELEVATOR_MAX_CURRENT_MA  (1000.0F)

#define ELEVATOR_RAMP_RATE (250.0f) //250mA / Second max rate of change

#define ELEVATOR_SAFE_STATE (0.0F)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Elevator Control */
typedef struct
{
        uint8 u8_status;                    //!<Checkpoint #1
        float32 f32_speed_cmd;                    //!<Checkpoint #2
        float32 f32_output;                         //!<Checkpoint #3
}T_ChkPoints_Elevator;


typedef struct
{
        float32 f32_max_current;
}T_Config_Elevator;

typedef struct
{
        //Local Control Variables
        uint8 u8_onOffCommand;                  //!<Local On Off Command Variable
        uint8 u8_speedCommand;                  //!<Local Speed Command Variable
        uint8 u8_speedFeedback;                 //!<Local Speed Feedback Variable
        //Local Control Variables
        uint8 u8_elevator_enabled;

        //TX CAN Variables
        uint8 *pu8_elevatorStatus;              //!<On/Off Status of Elevator (To Display)
        uint8 *pu8_elevatorButtonColor;         //!<Button color code for the Elevator On/Off button (To Button Panel)

        //RX CAN Variables
        uint8 *pu8_requestedSpeed;              //!<Elevator Requested Speed (From Display)
        uint8 *pu8_onOffCommand;                //!<Elevator On/Off Command (From Joystick)

        //Ramp Control
        T_RampState  t_ramp_state;

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
