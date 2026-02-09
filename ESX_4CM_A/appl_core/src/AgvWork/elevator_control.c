//-----------------------------------------------------------------------------
/*! \file       elevator_control.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "elevator_control.h"
#include "stwerrors.h"
#include "stwtypes.h"


/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_ElevatorControl mt_elevator;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Elevator Control
 *
 *  This function initializes the AgvWork - Elevator Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkElevator Pointer to the global Elevator Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_elevatorControl(T_UserInterface *_ui, T_ChkPoints_Elevator *_chkElevator, T_Config_Elevator *_nvmElevator)
{
    sint16 s16_error = C_NO_ERR;

    //populate local copy of RX ui elements
    mt_elevator.pu8_onOffCommand   = &_ui->t_joystick.u8_b1_state;
    mt_elevator.pu8_requestedSpeed = &_ui->t_display.u8_elevatorSpeedRequest;

    //populate local copy of TX ui elements
    mt_elevator.pu8_elevatorStatus = &_ui->t_display.u8_elevatorStatus;

    //populate local copy of checkpoints
    mt_elevator.pt_chkElevator = _chkElevator;

    //populate local copy of NVM elements
    mt_elevator.pt_nvmElevator = _nvmElevator;

    return s16_error;

}

/** \brief Update AgvWork - Elevator Control
 *
 *  This function contains the cyclical logic for AgvWork - Elevator Control.
 *
 *  Primary logic for this function is to set the speed of the elevator drive (cleaning shafts)
 *  based on CAN commands from the display and joystick.
 *
 *  Additional interlocks are utilized throughout the logic.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_elevatorControl(void)
{
    sint16 s16_error = C_NO_ERR;

    //get_inputStatus("THROTPOS", &u8_throttleSensorStatus);

    /*
    if(firstRun == TRUE)
    {
        start_delay_time = get_current_time - start time;
    }
    sint16 s16_error = C_NO_ERR;

    mt_elevator.pt_chkElevator->u8_chkPoint1 = 255;

    //if joystick button is faulted, set the joystick status to faulted
    check_jocyStickStatus(mt_elevator.pu8_onOffCommand, &u8_status);


    get_outputValue("DOOR_SWITCH", &door_state);

    if(door_state == OPENED)
    {

    }

    if(u8_status != FAULTED)
    {
        if(*(mt_elevator.pu8_onOffCommand) == (uint8)JS_BUTTON_PRESSED)
        {
            //do something
        }
    }
    else
    {

    }
    */




    return s16_error;

}



//EOF
