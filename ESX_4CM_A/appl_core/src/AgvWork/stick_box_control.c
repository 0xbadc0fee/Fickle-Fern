//-----------------------------------------------------------------------------
/*! \file       stick_box_control.h
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
#include "system.h"
//PROJECT
#include "stick_box_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS      (3000u) /* 3 seconds */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_StickBControl mt_stick_box;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Stick Box Control
 *
 *  This function initializes the AgvWork - Stick Box Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkElevator Pointer to the global Stick Box Control Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_stickBControl(T_UserInterface *_ui, T_Config_StickBoxControl *_nvmStickBControl)
{
    sint16 s16_error = C_NO_ERR;

    if(_ui == NULL)
    {
        return C_WARN;
    }

    //Populate local RX/TX pointers
    mt_stick_box.pu8_close_button = &_ui->t_buttonPanel.u8_b7_state;
    mt_stick_box.pu8_open_button = &_ui->t_buttonPanel.u8_b3_state;
    mt_stick_box.pu8_close_led_status = &_ui->t_buttonPanel.u8_b7_lights;
    mt_stick_box.pu8_open_led_status = &_ui->t_buttonPanel.u8_b3_lights;

    //Populate local copy of NVM elements
    mt_stick_box.pt_nvm_stick_control = _nvmStickBControl;

    //Initialize command variables
    mt_stick_box.u8_closed_cmd = STICK_BOX_CMD_OFF;
    mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
    mt_stick_box.u8_safe_state = STICK_BOX_CMD_OFF;
    mt_stick_box.u32_ign_start_time_ms = 0u;
    mt_stick_box.u8_prev_ign_on = FALSE;

    //Initialize toggle button helper
    mt_stick_box.t_btn_close_aux.pu_btn_state = &mt_stick_box.u8_closed_cmd;
    mt_stick_box.t_btn_close_aux.u32_hold_ms  = 0u;
    mt_stick_box.t_btn_close_aux.u8_btn_set   = TRUE;

    return s16_error;
}

/** \brief Update AgvWork - Stick Box Control
 *
 *  This function contains the cyclical logic for AgvWork - Stick Box Control.
 *
 *  The Stick Box Control Module shall control activation of the Stick Box Relays on an attached tow-behind cart.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_stickBControl(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_close_btn = FALSE;
    uint8 u8_open_btn = FALSE;
    uint8 u8_aux_close_reset = FALSE;
    uint8 u8_door_fault_status = FALSE;
    uint8 u8_ign_fault_status = FALSE;
    uint8 u8_close_output_fault = FALSE;
    uint8 u8_open_output_fault = FALSE;
    uint8 u8_ign_on = FALSE;

    float32 f32_door_value = DOOR_CLOSED;
    float32 f32_ign_value = IGN_OFF;

    uint32 u32_now_ms = get_system_time_ms();

    // IR-10.1 Read button values safely
    if(mt_stick_box.pu8_close_button != NULL)
    {
        u8_close_btn = (*(mt_stick_box.pu8_close_button) != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_aux_close_reset = TRUE;
        u8_close_btn = FALSE;
    }

    if(mt_stick_box.pu8_open_button != NULL)
    {
        u8_open_btn = (*(mt_stick_box.pu8_open_button) != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_open_btn = FALSE;
    }

    // Read required interlock inputs
    get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    get_inputValue("CAB_DOOR", &f32_door_value);

    get_inputFaultStatus("IGN_SWITCH", &u8_ign_fault_status);
    get_inputValue("IGN_SWITCH", &f32_ign_value);

    // Program start debounce timing
    u8_ign_on = ((u8_ign_fault_status == FALSE) && (f32_ign_value != IGN_OFF)) ? TRUE : FALSE;

    if((u8_ign_on == TRUE) && (mt_stick_box.u8_prev_ign_on == FALSE))
    {
        mt_stick_box.u32_ign_start_time_ms = u32_now_ms;
    }

    if(u8_ign_on == FALSE)
    {
        mt_stick_box.u32_ign_start_time_ms = 0u;
    }

    // FR-10.2 Default output commands disabled on boot/update before logic
    mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;

    // FR-10.1 Stick Box Mode / Auxiliary Mode behavior */
    if(mt_stick_box.pt_nvm_stick_control->u8_stick_box_installed == STICK_BOX_MODE_ENABLED)
    {
        // FR-10.3 Close interpreted as momentary in Stick Box Mode
        mt_stick_box.u8_closed_cmd = u8_close_btn;

        // FR-10.4 Open interpreted as momentary in Stick Box Mode
        mt_stick_box.u8_open_cmd = u8_open_btn;
    }
    else
    {
        // FR-10.3 Close interpreted as latched in Auxiliary Mode
        if((u8_door_fault_status == TRUE) ||
        (f32_door_value != DOOR_CLOSED) ||
        ((u32_now_ms - mt_stick_box.u32_ign_start_time_ms) < PROGRAM_START_DEB_MS))
        {
            u8_aux_close_reset = TRUE;
        }

        s16_error +=  toggleButton(&mt_stick_box.t_btn_close_aux,
        u8_close_btn,
        0u,
        0u,
        u8_aux_close_reset,
        mt_stick_box.u8_safe_state);

        // FR-10.5 Prevent any open activation in Auxiliary Mode
        mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
    }
    // FR-10.6 Auxiliary Mode close overwritten FALSE when door open or startup < 3 sec
    if((mt_stick_box.u8_stick_box_mode == STICK_BOX_MODE_DISABLED) &&
    ((u8_door_fault_status == TRUE) ||
    (f32_door_value != DOOR_CLOSED) ||
    ((u32_now_ms - mt_stick_box.u32_ign_start_time_ms) < PROGRAM_START_DEB_MS)))
    {
        mt_stick_box.u8_closed_cmd = STICK_BOX_CMD_OFF;
    }

    // FR-10.7 Mutual exclusivity, Close has priority if both requested
    if((mt_stick_box.u8_closed_cmd == STICK_BOX_CMD_ON) &&
    (mt_stick_box.u8_open_cmd == STICK_BOX_CMD_ON))
    {
        mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
    }

    // FR-10.8 / FR-10.9 Output hardware commands
    get_outputFaultStatus("STICK_BOX_CLOSE", &u8_close_output_fault);
    if(u8_close_output_fault == FALSE)
    {
        set_outputValue("STICK_BOX_CLOSE", (float32)mt_stick_box.u8_closed_cmd);
    }
    else
    {
        mt_stick_box.u8_closed_cmd = STICK_BOX_CMD_OFF;
        s16_error += C_WARN;
    }

    get_outputFaultStatus("STICK_BOX_OPEN", &u8_open_output_fault);
    if(u8_open_output_fault == FALSE)
    {
        set_outputValue("STICK_BOX_OPEN", (float32)mt_stick_box.u8_open_cmd);
    }
    else
    {
        mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
        s16_error += C_WARN;
    }

    //FR-10.10 LED indicators to button panel
    *(mt_stick_box.pu8_close_led_status) =
    (mt_stick_box.u8_closed_cmd == STICK_BOX_CMD_ON) ? 0x01u : //RED SOILD
    (mt_stick_box.u8_open_cmd == STICK_BOX_CMD_ON) ? 0x10u :  //GREEN SOLID
    0x00u;

    *(mt_stick_box.pu8_open_led_status) =
    (mt_stick_box.u8_closed_cmd == STICK_BOX_CMD_ON) ? 0x01u : //RED SOILD
    (mt_stick_box.u8_open_cmd == STICK_BOX_CMD_ON) ? 0x10u :  //GREEN SOLID
    0x00u;


    mt_stick_box.u8_prev_ign_on = u8_ign_on;

    return s16_error;

}

//EOF
