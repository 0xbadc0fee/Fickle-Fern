/**
 * \file       auger_cart_control.c
 * \brief      AgvWork - Auger Cart Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup AugerCartControl Auger Cart Control
 *
 * The Auger Cart Control Module shall universally control all unloading
 * operations of a variety of possible attached cart configurations and do so in an operator safe manner.
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
//PROJECT
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "auger_cart_control.h"
#include "engine_starter_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS (3000u)/**< Program start sequence debounce time [ms] */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_AugerControl mt_augerc; /**< Global persistent state for Auger Cart Control. */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Auger Cart Control
 *
 *  This function initializes the AgvWork - Auger Cart Control Logic.
 *
 *  \param _can_dev Pointer to the project's UI Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_augerControl(T_CANDevices *_can_dev)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_dev == NULL))
    {
        return C_WARN;
    }

    //populate local RX/TX pointers
    mt_augerc.pu8_auto_command       = &_can_dev->t_buttonPanel.u8_b2_state;
    mt_augerc.pu8_manual_command      = &_can_dev->t_buttonPanel.u8_b6_state;

    mt_augerc.pu8_auto_enable_status  = &_can_dev->t_display.u8_auger_status;
    mt_augerc.pu8_auto_status_indic   = &_can_dev->t_buttonPanel.u8_b2_lights;
    mt_augerc.pu8_manual_status_indic = &_can_dev->t_buttonPanel.u8_b6_lights;

    //Initialize local variables
    mt_augerc.u8_safe_state = AUGER_DISABLED; //IR-9.2 Disabled safe state
    mt_augerc.u32_ign_start_time_ms = 0u;
    mt_augerc.u8_prev_ign_on = FALSE;

    //Initialize outputs to disabled state
    mt_augerc.u8_auto_latched = AUGER_DISABLED;
    mt_augerc.u8_manual_latched = AUGER_DISABLED;

    //Initialize toggle button helper - Auger
    s16_error += toggleButton_init(
    &mt_augerc.t_btn_auto,
    &mt_augerc.u8_auto_latched,
    500u,
    AUGER_DISABLED
    );

    // Initialize toggle button helper - Manual
    s16_error += toggleButton_init(
    &mt_augerc.t_btn_manual,
    &mt_augerc.u8_manual_latched,
    500u,
    AUGER_DISABLED
    );

    return s16_error;
}

/** \brief Update AgvWork - Auger Cart Control
 *
 *  This function contains the cyclical logic for AgvWork - Auger Cart Control.
 *
 *   The Auger Cart Control Module shall universally control all unloading operations of a variety of
 *   possible attached cart configurations and do so in an operator safe manner.
 *
 *  Additional interlocks are utilized throughout the logic.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_augerControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_common_reset = FALSE;
    uint8 u8_aug_btn_reset = FALSE;
    uint8 u8_man_btn_reset = FALSE;

    uint8 u8_aug_cmd = mt_augerc.u8_safe_state;
    uint8 u8_man_cmd = mt_augerc.u8_safe_state;
    uint8 u8_hitch_on = FALSE;

    uint8 u8_door_fault_status = FALSE;
    uint8 u8_aug_output_fault = FALSE;
    uint8 u8_man_output_fault = FALSE;

    uint32 u32_engine_runtime = 0;

    float32 f32_door_value = DOOR_CLOSED;

    // Read commands safely
    if(mt_augerc.pu8_auto_command != NULL && *(mt_augerc.pu8_auto_command) != BTN_FAULT)
    {
        u8_aug_cmd = (*(mt_augerc.pu8_auto_command) != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_aug_btn_reset = TRUE;
    }

    if(mt_augerc.pu8_manual_command != NULL && *(mt_augerc.pu8_manual_command) != BTN_FAULT)
    {
        u8_man_cmd = (*(mt_augerc.pu8_manual_command) != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_man_btn_reset = TRUE;
    }

    // Read required interlock inputs
    get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    get_inputValue("CAB_DOOR", &f32_door_value);

    //Read ignition / program-start timing inputs
    get_engineRuntime(&u32_engine_runtime);


    //FR-9.4 The control module shall prohibit enabling the Auger Unload Enable and Manual Unload Enable commands if either the Hitch “IN” command or Hitch “OUT” commands are active.
    get_hitchPosStatus(&u8_hitch_on);

    //FR-9.2 Disable Auger Cart and reset when conditions not satisfied
    if((u8_door_fault_status == TRUE) ||
    (f32_door_value != DOOR_CLOSED) ||
    (u32_engine_runtime <= PROGRAM_START_DEB_MS) ||
    (u8_hitch_on == TRUE))
    {
        u8_common_reset = TRUE;
    }

    get_outputFaultStatus("AUTO_UNLOAD", &u8_aug_output_fault);
    get_outputFaultStatus("MANUAL_UNLOAD", &u8_man_output_fault);

    //Combine common reset with output-specific reset
    u8_aug_btn_reset = (uint8)(u8_aug_btn_reset || u8_common_reset || u8_aug_output_fault);
    u8_man_btn_reset = (uint8)(u8_man_btn_reset || u8_common_reset || u8_man_output_fault);


    //FR-9.1-2 Apply latching and reset logic to Auger and Manual Unload. Force to safe state if fault.
    s16_error = toggleButton(&mt_augerc.t_btn_auto, u8_aug_cmd, u8_aug_btn_reset);
    s16_error += toggleButton(&mt_augerc.t_btn_manual, u8_man_cmd, u8_man_btn_reset);

    //FR-9.3 The control module shall enforce mutual exclusivity to the Auger Unload Enable and Manual Unload commands giving preference to Manual Unload Enable in case of a conflict.
    if((mt_augerc.u8_manual_latched == AUGER_ENABLED) && (mt_augerc.u8_auto_latched == AUGER_ENABLED))
    {
        mt_augerc.u8_auto_latched = AUGER_DISABLED;
    }

    //FR-9.5 Output status
    if(u8_aug_output_fault == FALSE)
    {
        set_outputValue("AUTO_UNLOAD", (float32)mt_augerc.u8_auto_latched);
    }
    else
    {
        s16_error += C_WARN;
    }

    if(u8_man_output_fault == FALSE)
    {
        set_outputValue("MANUAL_UNLOAD", (float32)mt_augerc.u8_manual_latched);
    }
    else
    {
        s16_error += C_WARN;
    }

    //FR-9.6 Transmit button panel and display
    //Keypad Auto Button Indicator
    if(mt_augerc.pu8_auto_status_indic != NULL)
    {
        if(u8_aug_output_fault)
            *(mt_augerc.pu8_auto_status_indic) = BLUE_OFF | GREEN_OFF | AMBER_FLASH | RED_OFF;
        else if(mt_augerc.u8_auto_latched == AUGER_ENABLED)
            *(mt_augerc.pu8_auto_status_indic) = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
        else if (mt_augerc.u8_auto_latched == AUGER_DISABLED)
            *(mt_augerc.pu8_auto_status_indic) = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_ON;
        else
            *(mt_augerc.pu8_auto_status_indic) = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_OFF;
    }

    //Keypad Manual Button Indicator
    if(mt_augerc.pu8_manual_status_indic != NULL)
    {
        if(u8_man_output_fault)
            *(mt_augerc.pu8_manual_status_indic) = BLUE_OFF | GREEN_OFF | AMBER_FLASH | RED_OFF;
        else if(mt_augerc.u8_manual_latched == AUGER_ENABLED)
            *(mt_augerc.pu8_manual_status_indic) = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
        else if (mt_augerc.u8_manual_latched == AUGER_DISABLED)
            *(mt_augerc.pu8_manual_status_indic) = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_ON;
        else
            *(mt_augerc.pu8_manual_status_indic) = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_OFF;
    }

    //Display
    if(mt_augerc.pu8_auto_enable_status != NULL)
    {
        *(mt_augerc.pu8_auto_enable_status) = mt_augerc.u8_auto_latched;
    }

    return s16_error;
}

//EOF
