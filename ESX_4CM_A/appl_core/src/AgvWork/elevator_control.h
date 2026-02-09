/*! \file       elevator_control.h.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Jan 7, 2026 kyle.boch
   	\endimplementation
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

/** \brief Checkpoints Structure - Elevator Control
 *
 * This structure represents all checkpoints that are relevant
 * to elevator control
 */
typedef struct
{
    uint8 u8_chkPoint1;                    //!<Checkpoint #1
    uint8 u8_chkPoint2;                    //!<Checkpoint #2
    uint8 u8_chk3;                         //!<Checkpoint #3

}T_ChkPoints_Elevator;


/** \brief Configuration Structure - Elevator Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to elevator control
 */
typedef struct
{
    uint8 u8_minSpeed;                      //!<Configuration Parameter 1
    uint8 u8_maxSpeed;                      //!<Configuration Parameter 2

}T_Config_Elevator;

/** \brief Control Structure - Elevator Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for elevator control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
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

