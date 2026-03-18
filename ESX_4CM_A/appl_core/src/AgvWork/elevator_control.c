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


#include "hw_inputs.h"
#include "hw_outputs.h"
#include "fault_handler.h"
#include "system.h"



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
    uint8 u8_status = 0;
    float32 f32_door_state = 3;

    static uint32 u32_startTime = 0;

    static uint8 faultEnabled = FALSE;

    mt_elevator.pt_chkElevator->u8_chkPoint1 = faultEnabled;

    if((get_system_time_ms() - u32_startTime) >= 10000)
    {
        if(faultEnabled)
        {
            set_logicFaultStatus(520999, 6, FALSE);
            faultEnabled = FALSE;
            u32_startTime = get_system_time_ms();
        }
        else
        {
            set_logicFaultStatus(520999, 6, TRUE);
            faultEnabled = TRUE;
            u32_startTime = get_system_time_ms();
        }
    }

    s16_error = get_inputFaultStatus("CAB_DOOR", &u8_status);

    s16_error = get_inputValue("CAB_DOOR", &f32_door_state);

    s16_error = set_outputValue("STICKBOX_ON",f32_door_state);


    set_dm1Lamp(e_AMBER_WARN, TRUE);



    return s16_error;

}



//EOF
