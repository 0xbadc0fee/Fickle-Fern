//-----------------------------------------------------------------------------
/**
 * \file       hitch_position_control.c
 * \brief      AgvWork - Hitch Position Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup HitchPositionControl Hitch Position Control
 *
 * The Hitch Position Control Module manages the machine's "hitch" movement.
 * It processes operator Hitch "IN" and Hitch "OUT" commands to regulate
 * position and hydraulic engagement.
 *
 * @{
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Jan 6, 2026 STW Technic
 *
 * @{
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
#include "hitch_position_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_HitchPosControl mt_hp_control;/**< Global persistent state for Hitch Position Control. */
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/**
 * \brief Initialize AgvWork - Hitch Position Control
 *
 * This function initializes the Hitch Position Control logic, linking the
 * operator interface and the non-volatile configuration parameters.
 *
 * \param[in,out] _can_devs                Pointer to the project's UI Structure
 * \param[in]     _nvmHitchPosControl Pointer to the NVM Configuration Structure
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_hitchPosControl(T_CANDevices *_can_devs, T_Config_HeaderControl *_nvmHitchPosControl)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_devs == NULL) || (_nvmHitchPosControl == NULL))
    {
        return C_WARN;
    }

    //populate local copy of RX ui elements
    mt_hp_control.pu8_joy_hitch_in = &_can_devs->t_joystick.u8_b5_state;
    mt_hp_control.pu8_joy_hitch_out = &_can_devs->t_joystick.u8_b6_state;

    //populate local copy of NVM elements
    mt_hp_control.pt_nvm_hp_control = _nvmHitchPosControl;

    //Initialize command variables
    mt_hp_control.u8_in_command = FALSE;
    mt_hp_control.u8_out_command = FALSE;

    return s16_error;
}

/** \brief Update AgvWork - Hitch Position Control
 *
 *  This function contains the cyclical logic for AgvWork - Hitch Position Control.
 *
 *  Primary logic for this function is to move hitch in and out based on operator commands
 *  This logic tackles functionality described in FR2.X of the Functional Requirements
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_hitchPosControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_right_pedal = 0.0F;
    float32 f32_left_pedal  = 0.0F;

    uint8 u8_in_output = 0u;
    uint8 u8_out_output = 0u;

    uint8 u8_right_pedal_fault = FALSE;
    uint8 u8_left_pedal_fault = FALSE;
    uint8 u8_hitch_in_fault = FALSE;
    uint8 u8_hitch_out_fault = FALSE;

    //Default to safe state NO MOVEMENT
    mt_hp_control.u8_in_command = FALSE;
    mt_hp_control.u8_out_command = FALSE;

    if(mt_hp_control.pt_nvm_hp_control == NULL)
    {
        return C_WARN;
    }

    //FR-2.1 - FR-2.2 Check if joystick HLL is disabled or not
    //if joystick HLL is enabled - then use foot pedals for hitch
    if(mt_hp_control.pt_nvm_hp_control->u8_joystick_hll_enable)
    {
        get_inputFaultStatus("RIGHT_SWITCH", &u8_right_pedal_fault);
        get_inputFaultStatus("LEFT_SWITCH", &u8_left_pedal_fault);

        if((u8_right_pedal_fault == FALSE) && (u8_left_pedal_fault == FALSE))
        {
            get_inputValue("RIGHT_SWITCH", &f32_right_pedal);
            mt_hp_control.u8_out_command = (uint8)f32_right_pedal;

            get_inputValue("LEFT_SWITCH", &f32_left_pedal);
            mt_hp_control.u8_in_command = (uint8)f32_left_pedal;
        }
        else
        {
            u8_in_output = FALSE;
            u8_out_output = FALSE;
            s16_error += C_WARN;
        }
    }
    else //otherwise use joystick commands
    {
        if((mt_hp_control.pu8_joy_hitch_out != NULL) && (mt_hp_control.pu8_joy_hitch_in != NULL))
        {
            mt_hp_control.u8_out_command = *(mt_hp_control.pu8_joy_hitch_out);
            mt_hp_control.u8_in_command = *(mt_hp_control.pu8_joy_hitch_in);

            if(mt_hp_control.u8_in_command == JS_BUTTON_FAULT || mt_hp_control.u8_out_command == JS_BUTTON_FAULT)
            {
                mt_hp_control.u8_out_command = FALSE;
                mt_hp_control.u8_in_command = FALSE;
            }
        }
        else
        {
            u8_in_output = FALSE;
            u8_out_output = FALSE;
            s16_error += C_WARN;
        }
    }

    //FR-2.3 Set the hardware output
    get_outputFaultStatus("HITCH_RETRACT", &u8_hitch_in_fault);
    get_outputFaultStatus("HITCH_EXTEND", &u8_hitch_out_fault);
    if((u8_hitch_in_fault == TRUE) || (u8_hitch_out_fault == TRUE))
    {
        //Default to safe state NO MOVEMENT
        mt_hp_control.u8_in_command = FALSE;
        mt_hp_control.u8_out_command = FALSE;
        s16_error += C_WARN;
    }

    //FR-2.3 Prevent  simultaneous activation of  Hitch “IN” and “OUT” operations.
    if (mt_hp_control.u8_in_command == TRUE)
    {
        u8_in_output = TRUE;
        u8_out_output = FALSE;
    }
    else if (mt_hp_control.u8_out_command == TRUE)
    {
        u8_in_output = FALSE;
        u8_out_output = TRUE;
    }
    //Default to safe state NO MOVEMENT
    else
    {
        u8_in_output = FALSE;
        u8_out_output = FALSE;
    }

    set_outputValue("HITCH_RETRACT", (float32)u8_in_output);
    set_outputValue("HITCH_EXTEND", (float32)u8_out_output);

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
    if(pu8_hitchON == NULL)
    {
        return;
    }

    if ((mt_hp_control.u8_in_command == TRUE) || (mt_hp_control.u8_out_command == TRUE))
    {
        *pu8_hitchON = TRUE;
    }
    else
    {
        *pu8_hitchON = FALSE;
    }
}

//EOF
