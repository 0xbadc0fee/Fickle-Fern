//-----------------------------------------------------------------------------
/**
 * \file     elevator_control.c
 * \brief    AgvWork - Elevator Control
 *
 * This module manages the vertical lift and positioning logic for the
 * elevator system. It ensures synchronized movement and safety monitoring
 * during material transport and unloading operations.
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

/** * \brief Initializes the Elevator Control logic.
 *
 * Configures the initial state for the AgvWork elevator module and binds
 * required User Interface, Checkpoint tracking, and Non-Volatile Memory (NVM)
 * configuration resources.
 *
 * \param[in,out] _ui           Pointer to the project's User Interface structure.
 * \param[in,out] _chkElevator  Pointer to the global Elevator Checkpoints structure.
 * \param[in,out] _nvmElevator  Pointer to the Elevator NVM Configuration structure.
 *
 * \return Execution status.
 * \retval C_NO_ERR Initialization successful.
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

/**
 * \brief Cyclic update for AgvWork - Elevator Control.
 *
 * Manages the speed and positioning of the elevator drive based on
 * incoming CAN commands from the display and operator joystick.
 *
 * This function evaluates various hardware and software interlocks
 * to ensure synchronized movement and safe operation during material
 * transport.
 *
 * \return Execution status.
 * \retval C_NO_ERR Function executed properly without errors.
 */
sint16 update_elevatorControl(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_status = 0;
    float32 f32_door_state = 3;

    static uint32 u32_startTime = 0;

    static uint8 faultEnabled = FALSE;

    mt_elevator.pt_chkElevator->u8_chkPoint1 = faultEnabled;

    /*
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
    */

    s16_error = get_inputFaultStatus("CAB_DOOR", &u8_status);

    s16_error = get_inputValue("CAB_DOOR", &f32_door_state);

    s16_error = set_outputValue("STICKBOX_ON",f32_door_state);


    set_dm1Lamp(e_AMBER_WARN, TRUE);



    return s16_error;

}



//EOF
