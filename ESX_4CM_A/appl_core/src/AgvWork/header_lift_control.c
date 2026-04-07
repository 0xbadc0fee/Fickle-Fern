//-----------------------------------------------------------------------------
/* Project:   Flory_8772-4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Feb 24, 2026 kyle.boch
 */
//-----------------------------------------------------------------------------
/**
 * \file       header_lift_control.c
 * \brief      AgvWork - Header Lift Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup HeaderLiftControl Header Lift Control
 *
 * The Header Lift Control Module shall control the lifting and lowering of
 * the machine "header" through the use of hydraulic control valves and
 * hardware or CAN switch inputs.
 *
 * @{
 *
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
T_HeaderControl mt_hdr_control;/**<Global persistent state for Header Lift Control. */
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/**
 * \brief Initialize AgvWork - Header Lift Control
 *
 * This function initializes the Header Lift Control Logic, linking the
 * required UI interface and persistent checkpoint/configuration memory.
 *
 * \param[in,out] _ui                 Pointer to the project's UI Structure
 * \param[in,out] _chkPoints          Pointer to the global Header Checkpoints Structure
 * \param[in]     _nvmHeaderControl   Pointer to the NVM Configuration Structure
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_headerControl(T_UserInterface *_ui, T_ChkPoints_Header *_chkPoints, T_Config_HeaderControl *_nvmHeaderControl)
{
    sint16 s16_error = C_NO_ERR;

    //populate local copy of TX ui elements
    mt_hdr_control.pu8_relief_swich = &_ui->t_display.u8_relief_switch_status;

    //populate local copy of RX ui elements
    mt_hdr_control.pu8_joy_lift_header = &_ui->t_joystick.u8_b5_state;
    mt_hdr_control.pu8_joy_lower_header = &_ui->t_joystick.u8_b6_state;

    //populate local copy of NVM elements
    mt_hdr_control.pt_nvm_hdr_control = _nvmHeaderControl;

    //populate local copy of checkpoints
    mt_hdr_control.pt_chkPoints = _chkPoints;

    //iniitalize command variables
    mt_hdr_control.u8_lift_command = FALSE;
    mt_hdr_control.u8_lower_command = FALSE;

    return s16_error;
}

/**
 * \brief Update AgvWork - Header Lift Control
 *
 * This function executes the cyclical logic for the Header Lift system.
 * It manages the lifting and lowering of the header based on operator
 * commands and safety interlocks.
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
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

    uint8 u8_hw_in_lift_fault = FALSE;
    uint8 u8_hw_in_lower_fault = FALSE;
    uint8 u8_hw_out_lift_fault = FALSE;
    uint8 u8_hw_out_lower_fault = FALSE;

    //FR-1.5 get the relief switch status and pass value onto CAN
    get_inputValue("RELIEF_PRESS", &f32_relief_switch);
    //FR-1.6
    *(mt_hdr_control.pu8_relief_swich) = (uint8)f32_relief_switch;

    mt_hdr_control.pt_chkPoints->f32_chk3 = f32_relief_switch;

    //check if joystick HLL is disabled or not
    if(!mt_hdr_control.pt_nvm_hdr_control->u8_joystick_hll_enable)
    {
        get_inputFaultStatus("RIGHT_SWITCH", &u8_hw_in_lift_fault);
        get_inputFaultStatus("LEFT_SWITCH", &u8_hw_in_lower_fault);


        if(u8_hw_in_lift_fault || u8_hw_in_lower_fault)
        {
            mt_hdr_control.u8_lift_command = FALSE;
            mt_hdr_control.u8_lower_command = FALSE;
        }
        else
        {
            get_inputValue("RIGHT_SWITCH", &f32_right_pedal);
            get_inputValue("LEFT_SWITCH", &f32_left_pedal);

            mt_hdr_control.u8_lift_command = (uint8)f32_right_pedal;
            mt_hdr_control.u8_lower_command = (uint8)f32_left_pedal;
        }


    }
    else //if joystick hll enabled - header lift lower through joystick
    {
        mt_hdr_control.u8_lift_command = *(mt_hdr_control.pu8_joy_lift_header);
        mt_hdr_control.u8_lower_command = *(mt_hdr_control.pu8_joy_lower_header);

        //IR-1.1
        if(mt_hdr_control.u8_lift_command == JS_BUTTON_FAULT || mt_hdr_control.u8_lower_command == JS_BUTTON_FAULT)
        {
            mt_hdr_control.u8_lift_command = FALSE;
            mt_hdr_control.u8_lower_command = FALSE;
        }


    }

    //FR-1.3 Header lift takes prio - perform logic
    if(mt_hdr_control.u8_lift_command)
    {
        //FR-1.4 check the limit switch
        get_inputValue("HEAD_LIMIT", &f32_limit_switch);
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

    //IR-1.2
    get_outputFaultStatus("HEAD_LIFT_COIL", &u8_hw_out_lift_fault);
    get_outputFaultStatus("HEAD_LOWER_COIL", &u8_hw_out_lower_fault);

    if(u8_hw_out_lift_fault || u8_hw_out_lower_fault)
    {
        u8_lower_output = FALSE;
        u8_lift_output = FALSE;
    }




    //set the hardware output
    set_outputValue("HEAD_LOWER_COIL", (float32)u8_lower_output);
    set_outputValue("HEAD_LIFT_COIL", (float32)u8_lift_output);


    return s16_error;

}


//EOF
