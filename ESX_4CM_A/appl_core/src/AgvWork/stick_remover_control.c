//-----------------------------------------------------------------------------
/*! \file       stick_remover_control.c
    \brief      The Stick Remover Control Module shall control ON/OFF operation of the optional, cart installed Stick Remover conveyor chain.

    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Mar 12, 2026 t.gohn
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "stick_remover_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS      (3000u) /* 3 seconds */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_StickRemoverControl mt_stick_remover;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Stick Remover Control
 *
 *  This function initializes the AgvWork - Stick Remover Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_stickRemoverControl(T_UserInterface *_ui)
{
    if(_ui == NULL)
    {
        return C_WARN;
    }

    /* Populate local RX/TX pointers */
    mt_stick_remover.pu8_stick_remover_command    = &_ui->t_buttonPanel.u8_b5_state;
    mt_stick_remover.pu8_stick_remover_status     = &_ui->t_display.u8_stick_remover_status;
    mt_stick_remover.pu8_stick_remover_led_status = &_ui->t_buttonPanel.u8_b5_lights;

    /* Initialize local variables */
    mt_stick_remover.u8_safe_state            = STICK_REMOVER_DISABLED;
    mt_stick_remover.u8_stick_remover_latched = STICK_REMOVER_DISABLED;
    mt_stick_remover.u32_ign_start_time_ms    = 0u;
    mt_stick_remover.u8_prev_ign_on           = FALSE;

    /* Initialize toggle button helper */
    mt_stick_remover.t_btn_stick_remover.pu_btn_state = &mt_stick_remover.u8_stick_remover_latched;
    mt_stick_remover.t_btn_stick_remover.u32_hold_ms  = 0u;
    mt_stick_remover.t_btn_stick_remover.u8_btn_set   = TRUE;

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
    uint8 u8_stick_cmd = mt_stick_remover.u8_safe_state;

    uint8 u8_door_fault_status = FALSE;
    uint8 u8_ign_fault_status = FALSE;
    uint8 u8_output_fault_status = FALSE;
    uint8 u8_ign_on = FALSE;

    float32 f32_door_value = DOOR_CLOSED;
    float32 f32_ign_value = IGN_OFF;

    uint32 u32_now_ms = get_system_time_ms();

    /* Read command safely */
    if(mt_stick_remover.pu8_stick_remover_command != NULL)
    {
        u8_stick_cmd = *(mt_stick_remover.pu8_stick_remover_command);
    }
    else
    {
        /* IR-11.1 Invalid button reading results in OFF */
        u8_reset = TRUE;
    }

    /* Read required interlock inputs */
    get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    get_inputValue("CAB_DOOR", &f32_door_value);

    get_inputFaultStatus("IGN_SWITCH", &u8_ign_fault_status);
    get_inputValue("IGN_SWITCH", &f32_ign_value);

    /* Program start debounce timing */
    u8_ign_on = ((u8_ign_fault_status == FALSE) && (f32_ign_value != IGN_OFF)) ? TRUE : FALSE;

    if((u8_ign_on == TRUE) && (mt_stick_remover.u8_prev_ign_on == FALSE))
    {
        mt_stick_remover.u32_ign_start_time_ms = u32_now_ms;
    }

    if(u8_ign_on == FALSE)
    {
        mt_stick_remover.u32_ign_start_time_ms = 0u;
    }

    /* FR-11.2 Force disabled state and reset when conditions not satisfied */
    if((u8_door_fault_status == TRUE) ||
       (f32_door_value != DOOR_CLOSED) ||
       (u8_ign_fault_status == TRUE) ||
       (f32_ign_value == IGN_OFF) ||
       ((u32_now_ms - mt_stick_remover.u32_ign_start_time_ms) < PROGRAM_START_DEB_MS))
    {
        u8_reset = TRUE;
    }

    /* FR-11.1 / FR-11.2 / IR-11.1 Apply latching and reset logic */
    s16_error = toggleButton(&mt_stick_remover.t_btn_stick_remover,
                             u8_stick_cmd,
                             0u,
                             0u,
                             u8_reset,
                             mt_stick_remover.u8_safe_state);

    /* FR-11.3 / IR-11.2 Output status, no movement on output fault */
    get_outputFaultStatus("STICK_REMOVER_RELAY", &u8_output_fault_status);
    if(u8_output_fault_status == FALSE)
    {
        set_outputValue("STICK_REMOVER_RELAY", (float32)mt_stick_remover.u8_stick_remover_latched);
    }
    else
    {
        mt_stick_remover.u8_stick_remover_latched = STICK_REMOVER_DISABLED;
        s16_error = C_WARN;
    }

    /* FR-11.4 Output status to button panel and display */
    if(mt_stick_remover.pu8_stick_remover_status != NULL)
    {
        *(mt_stick_remover.pu8_stick_remover_status) = mt_stick_remover.u8_stick_remover_latched;
    }
    else
    {
        s16_error = C_WARN;
    }

    if(mt_stick_remover.pu8_stick_remover_led_status != NULL)
    {
        *(mt_stick_remover.pu8_stick_remover_led_status) =
            (u8_output_fault_status == TRUE) ? 0x10u :
            (mt_stick_remover.u8_stick_remover_latched == STICK_REMOVER_ENABLED) ? 0x08u :
            0x01u;
    }
    else
    {
        s16_error = C_WARN;
    }

    mt_stick_remover.u8_prev_ign_on = u8_ign_on;

    return s16_error;
}

//EOF
