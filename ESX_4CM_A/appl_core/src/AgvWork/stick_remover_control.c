//-----------------------------------------------------------------------------
/**
 * \file       stick_remover_control.c
 * \brief      AgvWork - Stick Remover Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup StickRemoverControl Stick Remover Control
 *
 * The Stick Remover Control Module shall control ON/OFF operation
 * of the optional, cart installed Stick Remover conveyor chain.
 *
 * @par Project
 * Flory_8772-4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Mar 12, 2026 t.gohn
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
#include "stick_remover_control.h"
#include "engine_starter_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS      (3000u) //!< Startup debounce delay in milliseconds (3 seconds)
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_StickRemoverControl mt_stick_remover; //!<  Module-local instance of the stick remover control state structure.

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Stick Remover Control
 *
 *  This function initializes the AgvWork - Stick Remover Control Logic.
 *
 *  \param _can_dev Pointer to the project's UI Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_stickRemoverControl(T_CANDevices *_can_dev)
{
    sint16 s16_error = C_NO_ERR;

    if(_can_dev == NULL)
    {
        return C_WARN;
    }

    // Populate local RX/TX pointers
    mt_stick_remover.pu8_stick_remover_command    = &_can_dev->t_buttonPanel.u8_b8_state;
    mt_stick_remover.pu8_stick_remover_status     = &_can_dev->t_display.u8_stick_remover_status;
    mt_stick_remover.pu8_stick_remover_led_status = &_can_dev->t_buttonPanel.u8_b8_lights;

    // Initialize local variables
    mt_stick_remover.u8_stick_remover_latched = STICK_REMOVER_DISABLED;
    mt_stick_remover.u32_ign_start_time_ms    = 0u;
    mt_stick_remover.u8_prev_ign_on           = FALSE;

    // Initialize toggle button helper
    s16_error += toggleButton_init(
    &mt_stick_remover.t_btn_stick_remover,
    &mt_stick_remover.u8_stick_remover_latched,
    250u,
    STICK_REMOVER_DISABLED
    );

    return C_NO_ERR;
}

/** \brief Update AgvWork - Stick Remover Control
 *
 *  This function contains the cyclical logic for AgvWork - Stick Remover Control.
 *
 *  The Stick Remover Control Module shall control ON/OFF operation of the
 *  optional, cart installed Stick Remover conveyor chain.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_stickRemoverControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_reset = FALSE;
    uint8 u8_stick_cmd = STICK_REMOVER_DISABLED;

    uint8 u8_door_fault_status = FALSE;
    uint8 u8_output_fault_status = FALSE;
    float32 f32_door_value = DOOR_CLOSED;

    uint32 u32_engine_runtime = 0;

    if(mt_stick_remover.pu8_stick_remover_command != NULL)
    {
        u8_stick_cmd = *(mt_stick_remover.pu8_stick_remover_command);
        u8_stick_cmd = (u8_stick_cmd != FALSE) ? TRUE : FALSE;
    }

    //Read required interlock inputs
    get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    get_inputValue("CAB_DOOR", &f32_door_value);

    get_engineRuntime(&u32_engine_runtime);


    // FR-11.2 Force disabled state and reset when conditions not satisfied
    if((u8_door_fault_status == TRUE) ||
    (f32_door_value != DOOR_CLOSED) ||
    (u32_engine_runtime < PROGRAM_START_DEB_MS))
    {
        u8_reset = TRUE;
    }

    // FR-11.1 / FR-11.2 / IR-11.1 Apply latching and reset logic
    toggleButton(&mt_stick_remover.t_btn_stick_remover, u8_stick_cmd, u8_reset);

    // FR-11.3 / IR-11.2 Output status, no movement on output fault
    get_outputFaultStatus("STICK_REMOVER", &u8_output_fault_status);

    if(u8_output_fault_status == FALSE)
    {
        set_outputValue("STICK_REMOVER", (float32)mt_stick_remover.u8_stick_remover_latched);

        if(mt_stick_remover.u8_stick_remover_latched)
            *(mt_stick_remover.pu8_stick_remover_led_status) = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
        else
            *(mt_stick_remover.pu8_stick_remover_led_status) = BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_ON;


    }
    else
    {
        mt_stick_remover.u8_stick_remover_latched = STICK_REMOVER_DISABLED;
        s16_error = C_WARN;
    }

    //* FR-11.4 Output status to button panel and display
    if(mt_stick_remover.pu8_stick_remover_status != NULL)
    {
        *(mt_stick_remover.pu8_stick_remover_status) = mt_stick_remover.u8_stick_remover_latched;
    }


    return s16_error;
}

//EOF
