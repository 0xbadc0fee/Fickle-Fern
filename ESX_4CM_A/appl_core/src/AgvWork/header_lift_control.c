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
//STW
//PROJECT
#include "header_lift_control.h"
#include "stwerrors.h"
#include "stwtypes.h"
#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_HeaderControl mt_hdr_control;
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

    //populate local copy of TX ui elements
    mt_hdr_control.pu8_relief_swich = &_ui->t_display.u8_relief_switch_status;

    //populate local copy of RX ui elements
    //TODO_STW - map these pointers to the correct buttons
    mt_hdr_control.pu8_joy_lift_header = &_ui->t_joystick.u8_b1_state;
    mt_hdr_control.pu8_joy_lift_header = &_ui->t_joystick.u8_b2_state;

    //populate local copy of NVM elements
    mt_hdr_control.pt_nvm_hdr_control = _nvmHeaderControl;

    //iniitalize command variables
    mt_hdr_control.u8_lift_command = FALSE;
    mt_hdr_control.u8_lower_command = FALSE;

    return s16_error;
}

/** \brief Update AgvWork - Header Lift Control
 *
 *  This function contains the cyclical logic for AgvWork - Header Lift Control.
 *
 *  Primary logic for this function is to lift of lower the header based on operator commands
 *  This logic tackles functionality descibed in FR1.X of the Functional Requirements
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_headerControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_right_pedal = 0;
    float32 f32_left_pedal  = 0;
    float32 f32_limit_switch = 0;
    float32 f32_relief_switch = 0;

    uint8 u8_lift_output = FALSE;
    uint8 u8_lower_output = FALSE;

    //get the relief switch status and pass value onto CAN
    get_inputValue("RIGHT_PEDAL", &f32_relief_switch);
    *(mt_hdr_control.pu8_relief_swich) = (uint8)f32_relief_switch;

    //check if joystick HLL is disabled or not
    if(!mt_hdr_control.pt_nvm_hdr_control->u8_joystick_hll_enable)
    {
        get_inputValue("RIGHT_PEDAL", &f32_right_pedal);
        get_inputValue("LEFT_PEDAL", &f32_left_pedal);

        mt_hdr_control.u8_lift_command = (uint8)f32_right_pedal;
        mt_hdr_control.u8_lower_command = (uint8)f32_left_pedal;

    }
    else //if joystick hll enabled - header lift lower through joystick
    {
        mt_hdr_control.u8_lift_command = *(mt_hdr_control.pu8_joy_lift_header);
        mt_hdr_control.u8_lower_command = *(mt_hdr_control.pu8_joy_lift_header);
    }

    //Header lift takes prio - perform logic
    if(mt_hdr_control.u8_lift_command)
    {
        //check the limit switch
        get_inputValue("HDR_LIMIT_SWITCH", &f32_limit_switch);
        u8_lower_output = FALSE;

        if(f32_limit_switch)
            u8_lift_output = FALSE;
        else
            u8_lift_output = TRUE;
    }

    else if (mt_hdr_control.u8_lower_command)
    {
        u8_lower_output = TRUE;
        u8_lift_output = FALSE;
    }

    else
    {
        u8_lower_output = FALSE;
        u8_lift_output = FALSE;
    }

    //set the hardware output
    set_outputValue("HDR_LOWER", (float32)u8_lower_output);
    set_outputValue("HDR_LIFT", (float32)u8_lift_output);


    return s16_error;

}


//EOF
