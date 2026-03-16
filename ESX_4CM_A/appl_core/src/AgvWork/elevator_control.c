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
#include "elevator_control.h"

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

    // Default enable state (FR-6.3)
    mt_elevator.u8_elevator_enabled = ELEVATOR_ON;
    //Initialize toggle button
    mt_elevator.t_btn_enable.pu_btn_state = &mt_elevator.u8_elevator_enabled;
    mt_elevator.t_btn_enable.u32_hold_ms  = 0u;
    mt_elevator.t_btn_enable.u8_btn_set   = TRUE;

    // Ramp initialization
    mt_elevator.t_ramp_state.f32_output       = 0.0F;
    mt_elevator.t_ramp_state.u8_faulted       = FALSE;
    mt_elevator.t_ramp_params.f32_ramp_rate = (ELEVATOR_MAX_CURRENT_A - ELEVATOR_MIN_CURRENT_A);
    mt_elevator.t_ramp_params.f32_min_limit = ELEVATOR_MIN_CURRENT_A;
    mt_elevator.t_ramp_params.f32_max_limit = ELEVATOR_MAX_CURRENT_A;
    mt_elevator.t_ramp_params.f32_safe_state =  0.0F;

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

    uint8 u8_enable_cmd = FALSE;
    uint8 u8_enable_fault = FALSE;
    uint8 u8_valve_fault = FALSE;
    uint8 u8_reset = FALSE;

    float32 f32_speed_req_pct = 0.0F;
    float32 f32_target_current = 0.0F;
    float32 f32_output_current = 0.0F;

    if((mt_elevator.pu8_onOffCommand == NULL) ||
    (mt_elevator.pu8_requestedSpeed == NULL) ||
    (mt_elevator.pu8_elevatorStatus == NULL) ||
    (mt_elevator.pt_chkElevator == NULL) ||
    (mt_elevator.pt_nvmElevator == NULL))
    {
        return C_WARN;
    }

    // FR-6.1 Read Elevator Enable button and Elevator Speed Command
    u8_enable_cmd = (*(mt_elevator.pu8_onOffCommand) != FALSE) ? TRUE : FALSE;
    f32_speed_req_pct = (float32)(*(mt_elevator.pu8_requestedSpeed));

    // Clamp requested speed to 0..100%
    f32_speed_req_pct = CLAMP_F32(f32_speed_req_pct, 0.0F, 100.0F);

    // IR-6.1 Invalid/faulted Elevator Enable signal => zero speed
    u8_enable_fault = FALSE;

    // IR-6.2 Faulted Elevator Control Valve => zero speed
    get_outputFaultStatus("ELEVATOR_CONTROL_VALVE", &u8_valve_fault);

    if((u8_enable_fault == TRUE) || (u8_valve_fault == TRUE))
    {
        u8_reset = TRUE;
    }
    // FR-6.1 Read Elevator Enable as latched signal
    s16_error += toggleButton(&mt_elevator.t_btn_enable,
    u8_enable_cmd,
    0u,
    0u,
    u8_reset,
    ELEVATOR_OFF);

    // FR-6.4 Set elevator speed command to zero when disabled
    if((mt_elevator.u8_elevator_enabled == ELEVATOR_OFF) ||
    (u8_enable_fault == TRUE) ||
    (u8_valve_fault == TRUE))
    {
        f32_target_current = 0.0F;
    }
    else
    {
        // FR-6.7 Scale command to configurable maximum current
        f32_target_current = f32_speed_req_pct * (ELEVATOR_MAX_CURRENT_A * 0.01F);
    }

    // FR-6.7 Bind command within configurable maximum current
    f32_target_current = CLAMP_F32(f32_target_current, ELEVATOR_MAX_CURRENT_A, ELEVATOR_MAX_CURRENT_A);

    // FR-6.5 Apply ramping using helper
    mt_elevator.t_ramp_state.u8_faulted = (uint8)((u8_enable_fault == TRUE) || (u8_valve_fault == TRUE));

    s16_error += rampCalc(f32_target_current,
    &mt_elevator.t_ramp_params,
    &mt_elevator.t_ramp_state);
    //FR-6.7 Bind command within configurable maximum current
    f32_output_current = CLAMP_F32(mt_elevator.t_ramp_state.f32_output, ELEVATOR_MIN_CURRENT_A, ELEVATOR_MAX_CURRENT_A);

    //Apply threshold
    if((f32_output_current > 0.0F) && (f32_output_current < mt_elevator.pt_nvmElevator->f32_max_current))
    {
        f32_output_current = mt_elevator.pt_nvmElevator->f32_max_current;
    }


    // IR-6.2 Faulted valve => zero command
    if(u8_valve_fault == TRUE)
    {
        f32_output_current = 0.0F;
        mt_elevator.t_ramp_state.f32_output = 0.0F;
    }

    // FR-6.7 Output to speed flow control valve
    if(u8_valve_fault == FALSE)
    {
        set_outputValue("ELEVATOR_CONTROL_VALVE", f32_output_current);
    }

    // FR-6.8 Output Elevator On/Off Status to display
    *(mt_elevator.pu8_elevatorStatus) = (mt_elevator.u8_elevator_enabled != FALSE) ? TRUE : FALSE;

    // Checkpoints
    mt_elevator.pt_chkElevator->u8_chkPoint1 = mt_elevator.u8_elevator_enabled;
    mt_elevator.pt_chkElevator->u8_chkPoint2 = (uint8)f32_speed_req_pct;
    mt_elevator.pt_chkElevator->f32_chkPoint1 = f32_output_current;

    return s16_error;

}



//EOF
