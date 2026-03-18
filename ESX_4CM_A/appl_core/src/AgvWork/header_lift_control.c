//-----------------------------------------------------------------------------
/*! \file       header_lift_control.c
    \brief      <description>

    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 24, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "header_lift_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_HeaderControl mt_hdr_control;
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Header Lift Control
 *
 *  This function initializes the AgvWork - Header Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkElevator Pointer to the global Elevator Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_headerControl(T_UserInterface *_ui, T_Config_HeaderControl *_nvmHeaderControl)
{
    sint16 s16_error = C_NO_ERR;

    if((_ui == NULL) || ( _nvmHeaderControl == NULL))
    {
        return C_WARN;
    }

    //populate local copy of TX ui elements
    mt_hdr_control.pu8_relief_switch = &_ui->t_display.u8_relief_switch_status;

    //populate local copy of RX ui elements
    mt_hdr_control.pu8_joy_lwr_header = &_ui->t_joystick.u8_b5_state;
    mt_hdr_control.pu8_joy_lift_header = &_ui->t_joystick.u8_b6_state;

    //populate local copy of NVM elements
    mt_hdr_control.pt_nvm_hdr_control = _nvmHeaderControl;

    //Initialize command variables
    mt_hdr_control.u8_lift_command = FALSE;
    mt_hdr_control.u8_lower_command = FALSE;

    return s16_error;
}

/** \brief Update AgvWork - Header Lift Control
 *
 *  This function contains the cyclical logic for AgvWork - Header Lift Control.
 *
 *  Primary logic for this function is to lift of lower the header based on operator commands
 *  This logic tackles functionality described in FR1.X of the Functional Requirements
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_headerControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_right_pedal = 0.0F;
    float32 f32_left_pedal  = 0.0F;
    float32 f32_limit_switch = 0.0F;
    float32 f32_relief_switch = 0.0F;

    uint8 u8_right_pedal_fault = FALSE;
    uint8 u8_left_pedal_fault = FALSE;
    uint8 u8_head_lift_fault = FALSE;
    uint8 u8_head_lower_fault = FALSE;

    uint8 u8_lift_output = FALSE;
    uint8 u8_lower_output = FALSE;

    if((mt_hdr_control.pu8_relief_switch == NULL) || ( mt_hdr_control.pt_nvm_hdr_control == NULL))
    {
        return C_WARN;
    }

    //FR-1.6 Get the relief switch status and pass value onto CAN
    get_inputValue("RELIEF_PRESS", &f32_relief_switch);
    *(mt_hdr_control.pu8_relief_switch) = (uint8)f32_relief_switch;

    //FR-1.1 - FR-1.2 Check if joystick HLL is disabled or not
    if(mt_hdr_control.pt_nvm_hdr_control->u8_joystick_hll_enable == TRUE)
    {
        get_inputFaultStatus("RIGHT_SWITCH", &u8_right_pedal_fault);
        get_inputFaultStatus("LEFT_SWITCH", &u8_left_pedal_fault);

        if((u8_right_pedal_fault == FALSE) && (u8_left_pedal_fault == FALSE))
        {
            get_inputValue("RIGHT_SWITCH", &f32_right_pedal);
            get_inputValue("LEFT_SWITCH", &f32_left_pedal);

            mt_hdr_control.u8_lift_command = (uint8)f32_right_pedal;
            mt_hdr_control.u8_lower_command = (uint8)f32_left_pedal;
        }
        else
        {
            u8_lower_output = FALSE;
            u8_lift_output = FALSE;
            s16_error += C_WARN;
        }
    }
    else if(mt_hdr_control.pt_nvm_hdr_control->u8_joystick_hll_enable == FALSE)
    {
        if((mt_hdr_control.pu8_joy_lwr_header != NULL) && (mt_hdr_control.pu8_joy_lift_header != NULL))
        {
            mt_hdr_control.u8_lift_command = (*(mt_hdr_control.pu8_joy_lift_header) != FALSE) ? TRUE : FALSE;
            mt_hdr_control.u8_lower_command = (*(mt_hdr_control.pu8_joy_lwr_header) != FALSE) ? TRUE : FALSE;
        }
        else
        {
            u8_lower_output = FALSE;
            u8_lift_output = FALSE;
            s16_error += C_WARN;
        }
    }

    //FR-1.3 Set the hardware output
    get_outputFaultStatus("HEAD_LOWER_COIL", &u8_head_lower_fault);
    get_outputFaultStatus("HEAD_LIFT_COIL", &u8_head_lift_fault);

    if((u8_head_lift_fault == TRUE) || (u8_head_lower_fault == TRUE))
    {
        //Default to safe state NO MOVEMENT
        mt_hdr_control.u8_lift_command= FALSE;
        mt_hdr_control.u8_lower_command = FALSE;
        s16_error += C_WARN;
    }

    //FR-1.3 Header lift takes priority- perform logic
    if(mt_hdr_control.u8_lift_command)
    {
        //FR-1.5 Check the limit switch
        get_inputValue("HEAD_LIMIT", &f32_limit_switch);
        u8_lower_output = FALSE;

        if(f32_limit_switch != 0.0F)
        {
            u8_lift_output = FALSE;
        }
        else
        {
            u8_lift_output = TRUE;
        }
    }

    else if (mt_hdr_control.u8_lower_command)
    {
        u8_lower_output = TRUE;
        u8_lift_output = FALSE;
    }
    //Default to safe state NO MOVEMENT
    else
    {
        u8_lower_output = FALSE;
        u8_lift_output = FALSE;
    }

    set_outputValue("HEAD_LOWER_COIL", (float32)u8_lower_output);
    set_outputValue("HEAD_LIFT_COIL", (float32)u8_lift_output);

    return s16_error;
}

//EOF
