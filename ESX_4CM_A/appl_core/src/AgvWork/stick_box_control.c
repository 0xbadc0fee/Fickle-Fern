//-----------------------------------------------------------------------------
/**
 * \file       stick_box_control.c
 * \brief      AgvWork - Stick Box Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup StickBoxControl Stick Box Control
 *
 * The Stick Box Control Module manages the activation of Stick Box Relays
 * located on an attached tow-behind cart. It ensures proper signal
 * synchronization between the tractor and the cart hardware.
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
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "stick_box_control.h"
#include "engine_starter_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS      (3000u) //!< Program start debounce time [ms]
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_StickBControl mt_stick_box; //!< Module-local instance of the stick box control state structure.

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Stick Box Control
 *
 *  This function initializes the AgvWork - Stick Box Control Logic.
 *
 *  \param _can_dev Pointer to the project's UI Structure
 *  \param _nvmStickBControl Pointer to the global Stick Box Control Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_stickBControl(T_CANDevices                *_can_dev,
                          T_Config_StickBoxControl    *_nvmStickBControl)
{
    sint16 s16_error = C_NO_ERR;

    if(_can_dev == NULL || _nvmStickBControl == NULL)
    {
        return C_WARN;
    }

    //Populate local RX/TX pointers
    mt_stick_box.pu8_close_button = &_can_dev->t_buttonPanel.u8_b7_state;
    mt_stick_box.pu8_open_button = &_can_dev->t_buttonPanel.u8_b3_state;
    mt_stick_box.pu8_close_led_status = &_can_dev->t_buttonPanel.u8_b7_lights;
    mt_stick_box.pu8_open_led_status = &_can_dev->t_buttonPanel.u8_b3_lights;

    //Populate local copy of NVM elements
    mt_stick_box.pt_nvm_stick_control = _nvmStickBControl;

    //Initialize command variables
    mt_stick_box.u8_closed_cmd = STICK_BOX_CMD_OFF;
    mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
    mt_stick_box.u32_ign_start_time_ms = 0u;
    mt_stick_box.u8_prev_ign_on = FALSE;

    // Initialize toggle button helper
    s16_error += toggleButton_init(
    &mt_stick_box.t_btn_close_aux,
    &mt_stick_box.u8_closed_cmd,
    250u,
    STICK_BOX_CMD_OFF
    );

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
    uint8 u8_open_output_fault = FALSE;
    uint8 u8_on_output_fault = FALSE;

    float32 f32_door_value = DOOR_CLOSED;

    uint32 u32_engine_runtime = 0;

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

    //Read required interlock inputs
    get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    get_inputValue("CAB_DOOR", &f32_door_value);

    get_outputFaultStatus("STICKBOX_ON", &u8_on_output_fault);
    get_outputFaultStatus("STICKBOX_OPEN", &u8_open_output_fault);

    // Determine ignition ON status
    get_engineRuntime(&u32_engine_runtime);

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
        (u8_on_output_fault == TRUE) ||
        (f32_door_value != DOOR_CLOSED) ||
        (u32_engine_runtime < PROGRAM_START_DEB_MS))
        {
            u8_aux_close_reset = TRUE;
        }

        s16_error +=  toggleButton(&mt_stick_box.t_btn_close_aux, u8_close_btn, u8_aux_close_reset);

        // FR-10.5 Prevent any open activation in Auxiliary Mode
        mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
    }


    // FR-10.7 Mutual exclusivity, Close has priority if both requested
    if((mt_stick_box.u8_closed_cmd == STICK_BOX_CMD_ON) &&
    (mt_stick_box.u8_open_cmd == STICK_BOX_CMD_ON))
    {
        mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
    }

    // FR-10.8 / FR-10.9 Output hardware commands
    if(mt_stick_box.u8_closed_cmd)
    {
        if(!u8_on_output_fault)
        {
            set_outputValue("STICKBOX_ON", (float32)mt_stick_box.u8_closed_cmd);
            set_outputValue("STICKBOX_OPEN", STICK_BOX_CMD_OFF);

            //FR-10.10 LED indicators to button panel
            *(mt_stick_box.pu8_close_led_status) = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
        }
        else
        {
            mt_stick_box.u8_closed_cmd = STICK_BOX_CMD_OFF;
            s16_error += C_WARN;
        }
    }
    else if (mt_stick_box.u8_open_cmd)
    {
        if(!u8_open_output_fault && !u8_on_output_fault)
        {
            set_outputValue("STICKBOX_OPEN", (float32)mt_stick_box.u8_open_cmd);
            set_outputValue("STICKBOX_ON", (float32)mt_stick_box.u8_open_cmd);

            //FR-10.10 LED indicators to button panel
            *(mt_stick_box.pu8_open_led_status)  = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
        }
        else
        {
            mt_stick_box.u8_open_cmd = STICK_BOX_CMD_OFF;
            s16_error += C_WARN;
        }
    }
    else
    {
        if(!u8_open_output_fault && !u8_on_output_fault)
        {
            set_outputValue("STICKBOX_OPEN", STICK_BOX_CMD_OFF);
            set_outputValue("STICKBOX_ON", STICK_BOX_CMD_OFF);

            //FR-10.10 LED indicators to button panel
            *(mt_stick_box.pu8_close_led_status) = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_ON;
            *(mt_stick_box.pu8_open_led_status)  = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_ON;
        }
    }

    *(mt_stick_box.pu8_open_led_status) =
    (mt_stick_box.u8_closed_cmd == STICK_BOX_CMD_ON) ? 0x01u : //RED SOILD
    (mt_stick_box.u8_open_cmd == STICK_BOX_CMD_ON) ? 0x10u :  //GREEN SOLID
    0x00u;

    return s16_error;

}

//EOF
