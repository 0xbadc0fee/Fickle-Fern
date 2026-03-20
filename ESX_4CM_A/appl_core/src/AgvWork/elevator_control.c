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
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "elevator_control.h"

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
 *  \param _nvmElevator Pointer to the global Elevator NVM Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_elevatorControl(T_UserInterface *_ui, T_ChkPoints_Elevator *_chkElevator, T_Config_Elevator *_nvmElevator)
{
    sint16 s16_error = C_NO_ERR;

    if((_ui == NULL) || (_chkElevator == NULL) || (_nvmElevator == NULL))
    {
        return C_WARN;
    }

    //populate local copy of RX ui elements
    mt_elevator.pu8_onOffCommand   = &_ui->t_joystick.u8_b1_state;
    mt_elevator.pu8_requestedSpeed = &_ui->t_display.u8_elevatorSpeedRequest;

    //populate local copy of TX ui elements
    mt_elevator.pu8_elevatorStatus = &_ui->t_display.u8_elevatorStatus;

    //populate local copy of checkpoints
    mt_elevator.pt_chkElevator = _chkElevator;

    //populate local copy of NVM elements
    mt_elevator.pt_nvmElevator = _nvmElevator;

    // Default enable state (FR-6.3)
    mt_elevator.u8_elevator_enabled = ELEVATOR_ON;


    // Initialize toggle button helper
    s16_error += toggleButton_init(
    &mt_elevator.t_btn_enable,
    &mt_elevator.u8_elevator_enabled,
    250,
    ELEVATOR_OFF
    );

    // Ramp initialization
    s16_error += rampInit(&mt_elevator.t_ramp_state, ELEVATOR_RAMP_RATE, ELEVATOR_MIN_CURRENT_MA, ELEVATOR_MAX_CURRENT_MA, ELEVATOR_SAFE_STATE);

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

    uint8 u8_enable_cmd = ELEVATOR_OFF;
    uint8 u8_enable_fault = FALSE;
    uint8 u8_valve_fault = FALSE;
    uint8 u8_reset = FALSE;

    float32 f32_speed_req_pct = ELEVATOR_SAFE_STATE;
    float32 f32_output_current = ELEVATOR_SAFE_STATE;
    float32 f32_maxCurrent = ELEVATOR_MAX_CURRENT_MA;

    if((mt_elevator.pu8_onOffCommand == NULL) ||
    (mt_elevator.pu8_requestedSpeed == NULL) ||
    (mt_elevator.pu8_elevatorStatus == NULL) ||
    (mt_elevator.pt_chkElevator == NULL) ||
    (mt_elevator.pt_nvmElevator == NULL))
    {
        // IR-6.1 Invalid/faulted Elevator Enable signal => zero speed
        u8_enable_fault = TRUE;
        f32_speed_req_pct = ELEVATOR_SAFE_STATE;
        u8_enable_cmd = ELEVATOR_OFF;
    }
    else
    {
        // FR-6.1 Read Elevator Enable button and Elevator Speed Command
        u8_enable_cmd = *(mt_elevator.pu8_onOffCommand);
        f32_speed_req_pct = (float32)(*(mt_elevator.pu8_requestedSpeed));

        //Publish Checkpoint
        mt_elevator.pt_chkElevator->f32_checkpoint2 = f32_speed_req_pct;
    }

    if(mt_elevator.pt_nvmElevator != NULL)
    {
        f32_maxCurrent = mt_elevator.pt_nvmElevator->f32_max_current;
        mt_elevator.t_ramp_state.f32_max_limit= f32_maxCurrent;
    }

    // IR-6.2 Faulted Elevator Control Valve => zero speed
    s16_error += get_outputFaultStatus("FLOW_CONTROL", &u8_valve_fault);

    if((u8_enable_fault == TRUE) || (u8_valve_fault == TRUE) || u8_enable_cmd == JS_BUTTON_FAULT)
    {
        u8_reset = TRUE;
    }
    // FR-6.1 Read Elevator Enable as latched signal
    s16_error += toggleButton(&mt_elevator.t_btn_enable, u8_enable_cmd, u8_reset);

    //Publish Checkpoint
    mt_elevator.pt_chkElevator->u8_checkpoint1 = mt_elevator.u8_elevator_enabled;

    // FR-6.4 Set elevator speed command to zero when disabled
    if((mt_elevator.u8_elevator_enabled == ELEVATOR_OFF) ||
    (u8_enable_fault == TRUE) ||
    (u8_valve_fault == TRUE))// IR-6.2 Faulted valve => zero command
    {
        f32_speed_req_pct = ELEVATOR_SAFE_STATE;
    }

    f32_output_current = (f32_speed_req_pct * ((ELEVATOR_MAX_CURRENT_MA - ELEVATOR_MIN_CURRENT_MA)/100.0F)) + ELEVATOR_MIN_CURRENT_MA;
    // FR-6.5/7 Apply ramping using helper and clamp output to min and max
    s16_error += rampCalc(f32_output_current, &mt_elevator.t_ramp_state);

    // FR-6.7 Output to speed flow control valve
    if(u8_valve_fault == FALSE)
    {
        s16_error += set_outputValue("FLOW_CONTROL", mt_elevator.t_ramp_state.f32_output);
    }

    //Publish Checkpoint
    mt_elevator.pt_chkElevator->f32_checkpoint3 = mt_elevator.t_ramp_state.f32_output;

    // FR-6.8 Output Elevator On/Off Status to display
    *(mt_elevator.pu8_elevatorStatus) = (mt_elevator.u8_elevator_enabled != FALSE) ? TRUE : FALSE;

    return s16_error;
}

//EOF
