//-----------------------------------------------------------------------------
/**
 * \file     elevator_control.c
 * \brief    AgvWork - Elevator Control
 *
 * This module manages the vertical lift and positioning logic for the
 * elevator system. It ensures synchronized movement and safety monitoring
 * during material transport and unloading operations.
 *
 * \project   FloryTemplate_4CM
 * \copyright STW Technic (c) 2026
 * \license   use only under terms of contract / confidential
 *
 * \created   Jan 6, 2026 STW Technic
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup ElevatorControl Elevator Control
 * @{
 *
 * @{
 */
#ifndef APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define ELEVATOR_ON  (1u)
#define ELEVATOR_OFF (0u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * @struct T_ChkPoints_Elevator
 * \brief Checkpoints Structure - Elevator Control
 *
 * Encapsulates the state-tracking checkpoints required for Elevator
 * control logic. These members are utilized to validate operational
 * sequences, safety interlocks, and positioning status.
 */
typedef struct
{
    uint8   u8_chkPoint1;                    //!<Checkpoint #1
    sint16  s16_chkPoint2;                    //!<Checkpoint #2
    float32 f32_chkPoint3;                         //!<Checkpoint #3

}T_ChkPoints_Elevator;

/**
 * @struct T_Config_Elevator
 * \brief Configuration Structure - Elevator Control
 *
 * Encapsulates all non-volatile memory (NVM) configuration variables
 * relevant to the elevator control system. These parameters define
 * calibrated limits, drive settings, and operational thresholds.
 */
typedef struct
{
    uint8 u8_minSpeed;                      //!<Configuration Parameter 1
    uint8 u8_maxSpeed;                      //!<Configuration Parameter 2

}T_Config_Elevator;

/**
 * @struct T_ElevatorControl
 * \brief Control Structure - Elevator Control
 *
 * Encapsulates all persistent state variables and pointers required for
 * the Elevator control logic. This context is maintained across cyclic
 * execution to facilitate vertical positioning and drive monitoring.
 *
 * \note This structure is reserved for persistent data only;
 * transient/temporary variables are excluded.
 */
typedef struct
{
    //Local Control Variables
    uint8 u8_onOffCommand;                  //!<Local On Off Command Variable
    uint8 u8_speedCommand;                  //!<Local Speed Command Variable
    uint8 u8_speedFeedback;                 //!<Local Speed Feedback Variable

    //TX CAN Variables
    uint8 *pu8_elevatorStatus;              //!<On/Off Status of Elevator (To Display)
    uint8 *pu8_elevatorButtonColor;         //!<Button color code for the Elevator On/Off button (To Button Panel)

    //RX CAN Variables
    uint8 *pu8_requestedSpeed;              //!<Elevator Requested Speed (From Display)
    uint8 *pu8_onOffCommand;                //!<Elevator On/Off Command (From Joystick)

    //NVM Configuration Parameters
    T_Config_Elevator *pt_nvmElevator;      //!<Elevator Control Configuration Structure

    //Control Checkpoints
    T_ChkPoints_Elevator *pt_chkElevator;   //!<Elevator Control Checkpoints Structure


}T_ElevatorControl;
/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_elevatorControl(T_UserInterface *_ui, T_ChkPoints_Elevator *_chkElevator, T_Config_Elevator *_nvmElevator);
sint16 update_elevatorControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_ELEVATOR_CONTROL_H_ */

