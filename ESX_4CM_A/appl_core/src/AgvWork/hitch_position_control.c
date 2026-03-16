//-----------------------------------------------------------------------------
/*! \file       hitch_position_control.c
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
#include "hitch_position_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_HitchPosControl mt_hp_control;
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Hitch Position Control
 *
 *  This function initializes the AgvWork - Hitch Position Control.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _nvmHitchPosControl Pointer to the global Elevator Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_hitchPosControl(T_UserInterface *_ui, T_Config_HeaderControl *_nvmHitchPosControl)
{
    sint16 s16_error = C_NO_ERR;

    //populate local copy of RX ui elements
    mt_hp_control.pu8_joy_hitch_in = &_ui->t_joystick.u8_b5_state;
    mt_hp_control.pu8_joy_hitch_out = &_ui->t_joystick.u8_b6_state;

    //populate local copy of NVM elements
    mt_hp_control.pt_nvm_hp_control = _nvmHitchPosControl;

    //iniitalize command variables
    mt_hp_control.u8_in_command = FALSE;
    mt_hp_control.u8_out_command = FALSE;

    if(_ui == NULL)
    {
        return C_WARN;
    }

    return s16_error;
}

/** \brief Update AgvWork - Hitch Position Control
 *
 *  This function contains the cyclical logic for AgvWork - Hitch Position Control.
 *
 *  Primary logic for this function is to move hitch in and out based on operator commands
 *  This logic tackles functionality descibed in FR2.X of the Functional Requirements
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_hitchPosControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_right_pedal = 0u;
    float32 f32_left_pedal  = 0u;

    uint8 u8_in_output = 0u;
    uint8 u8_out_output = 0u;

    uint8 u8_right_pedal_fault = FALSE;
    uint8 u8_left_pedal_fault = FALSE;
    uint8 u8_hitch_in_fault = FALSE;
    uint8 u8_hitch_out_fault = FALSE;

    //Default to safe state NO MOVEMENT
    mt_hp_control.u8_in_command = FALSE;
    mt_hp_control.u8_out_command = FALSE;

    //FR-2.1 - FR-2.2 Check if joystick HLL is disabled or not
    if(mt_hp_control.pt_nvm_hp_control->u8_joystick_hll_enable == FALSE)
    {
        get_inputFaultStatus("RIGHT_PEDAL", &u8_right_pedal_fault);
        get_inputFaultStatus("LEFT_PEDAL", &u8_left_pedal_fault);

        if((u8_right_pedal_fault == FALSE) && (u8_left_pedal_fault == FALSE))
        {
            get_inputValue("RIGHT_PEDAL", &f32_right_pedal);
            mt_hp_control.u8_out_command = (uint8)f32_right_pedal;

            get_inputValue("LEFT_PEDAL", &f32_left_pedal);
            mt_hp_control.u8_in_command = (uint8)f32_left_pedal;
        }
        else
        {
            s16_error = C_WARN;
        }
    }
    else if(mt_hp_control.pt_nvm_hp_control->u8_joystick_hll_enable == TRUE)
    {
        if((mt_hp_control.pu8_joy_hitch_out != NULL) && (mt_hp_control.pu8_joy_hitch_in != NULL))
        {
            mt_hp_control.u8_out_command = (*(mt_hp_control.pu8_joy_hitch_out) == 1u);
            mt_hp_control.u8_in_command = (*(mt_hp_control.pu8_joy_hitch_in) == 1u);
        }
        else
        {
            s16_error = C_WARN;
        }
    }

    //FR-2.3 Prevent  simultaneous activation of  Hitch “IN” and “OUT” operations.
    if (mt_hp_control.u8_in_command == TRUE)
    {
        u8_in_output = TRUE;
        u8_out_output = FALSE;
    }
    else if (mt_hp_control.u8_in_command == TRUE)
    {
        u8_in_output = FALSE;
        u8_out_output = TRUE;
    }
    else
    {
        u8_in_output = FALSE;
        u8_out_output = FALSE;
    }

    //FR-2.3 Set the hardware output
    get_outputFaultStatus("HITCH_IN", &u8_hitch_in_fault);
    get_outputFaultStatus("HITCH_OUT", &u8_hitch_out_fault);
    if((u8_right_pedal_fault == TRUE) && (u8_left_pedal_fault == TRUE))
    {
        //Default to safe state NO MOVEMENT
        mt_hp_control.u8_in_command = FALSE;
        mt_hp_control.u8_out_command = FALSE;
        s16_error = C_WARN;
    }

    set_outputValue("HITCH_IN", (float32)u8_in_output);
    set_outputValue("HITCH_OUT", (float32)u8_out_output);

    return s16_error;
}

/** \brief Update AgvWork - Get Hitch Position Control Status
 *
 *  This function contains the cyclical logic for AgvWork - Hitch Position Control.
 *
 *  Primary logic for this function is to get the status of Hitch Position when in use FR-9.4.
 *
 * \param pu8_hitchON
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
void get_hitchPosStatus(uint8 * pu8_hitchON)
{
    if ((mt_hp_control.u8_in_command == TRUE) || (mt_hp_control.u8_in_command == TRUE))
    {
        *pu8_hitchON = TRUE;
    }
    else
    {
        *pu8_hitchON = FALSE;
    }
}

//EOF
