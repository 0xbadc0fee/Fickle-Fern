//-----------------------------------------------------------------------------
/*! \file       power_assist_control.c
    \brief      <description>

    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 24, 2026 t.gohn
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
#include "power_assist_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS      (3000u) /* 3 seconds */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_PowerAssistControl mt_power_assist;
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Power Assist Control
 *
 *  This function initializes the AgvChassis - Power Assist Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_powerAssistControl(T_UserInterface *_ui, T_Config_PowerAssistControl *_nvmPAControl)
{
    if(_ui == NULL)
    {
        return C_WARN;
    }

    /* Populate local RX/TX pointers */
    mt_power_assist.pu8_power_assist_button     = &_ui->t_buttonPanel.u8_b5_state;
    mt_power_assist.pu8_power_assist_led_status = &_ui->t_buttonPanel.u8_b5_lights;

    /* FR-15.4 Default all functionality disabled on startup */
    mt_power_assist.u8_safe_state           = POWER_ASSIST_DISABLED;
    mt_power_assist.u8_power_assist_latched = POWER_ASSIST_DISABLED;
    mt_power_assist.u8_power_assist_status  = POWER_ASSIST_DISABLED;
    mt_power_assist.u8_traction_valve_cmd   = TRACTION_VALVE_OFF;
    mt_power_assist.u8_fault_active         = FALSE;
    mt_power_assist.u32_ign_start_time_ms   = 0u;
    mt_power_assist.u8_prev_ign_on          = FALSE;

    /* Initialize toggle helper */
    mt_power_assist.t_btn_power_assist.pu_btn_state = &mt_power_assist.u8_power_assist_latched;
    mt_power_assist.t_btn_power_assist.u32_hold_ms  = 0u;
    mt_power_assist.t_btn_power_assist.u8_btn_set   = TRUE;

    //NVM Params
    mt_power_assist.pt_nvm_pa_control = _nvmPAControl;

    return C_NO_ERR;
}

/** \brief Update AgvWork - Power Assist Control
 *
 *  This function contains the cyclical logic for AgvChassis - Power Assist Control.
 *
 *  The Power Assist Control Module determines the ultimate power assist control state
 *  through CAN and hardware input switches.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_powerAssistControl(void)
{
    sint16 s16_error = C_NO_ERR;
    sint16 s16_tmp = C_NO_ERR;

    uint8 u8_reset = FALSE;
    uint8 u8_power_assist_cmd = POWER_ASSIST_DISABLED;
    uint8 u8_high_gear_enabled = HIGH_GEAR_DISABLED;
    uint8 u8_ign_on = FALSE;
    uint8 u8_high_gear_status = FALSE;

    uint8 u8_power_assist_output_fault = FALSE;
    uint8 u8_traction_output_fault = FALSE;

    uint8 u8_trac_switch_fault = FALSE;
    uint8 u8_ign_fault_status = FALSE;

    float32 f32_trac_switch_cmd = 0u;
    float32 f32_ign_value = 0u;

    uint32 u32_now_ms = get_system_time_ms();

    mt_power_assist.u8_fault_active = FALSE;

    /* FR-15.1 Read Power Assist Enabled input from button panel as latched input */
    if(mt_power_assist.pu8_power_assist_button != NULL)
    {
        u8_power_assist_cmd = (*(mt_power_assist.pu8_power_assist_button) != FALSE) ? TRUE : FALSE;
    }
    else
    {
        u8_reset = TRUE;
        mt_power_assist.u8_fault_active = TRUE;
    }

    /* FR-15.2 Interpret High Gear Enable status from Propulsion Control */
    //getHighGearEnable(&u8_high_gear_status); //TODO

    /* FR-15.5 Read hardware input Traction Valve Switch command */
    get_inputFaultStatus("TRACTION_VALVE_SWITCH", &u8_trac_switch_fault);
    get_inputValue("TRACTION_VALVE_SWITCH", &f32_trac_switch_cmd);

    /* Ignition / program-start timing */
    get_inputFaultStatus("IGN_SWITCH", &u8_ign_fault_status);
    get_inputValue("IGN_SWITCH", &f32_ign_value);

    u8_ign_on = ((u8_ign_fault_status == FALSE) && (f32_ign_value != 0.0F)) ? TRUE : FALSE;

    if((u8_ign_on == TRUE) && (mt_power_assist.u8_prev_ign_on == FALSE))
    {
        mt_power_assist.u32_ign_start_time_ms = u32_now_ms;
    }

    if(u8_ign_on == FALSE)
    {
        mt_power_assist.u32_ign_start_time_ms = 0u;
    }

    /* FR-15.3 Force disabled state and reset when High Speed or Program Start debounce not satisfied */
    if((u8_high_gear_enabled == HIGH_GEAR_ENABLED) ||
       (u8_ign_fault_status == TRUE) ||
       ((u32_now_ms - mt_power_assist.u32_ign_start_time_ms) < PROGRAM_START_DEB_MS))
    {
        u8_reset = TRUE;
    }

    /* IR-15.2 Faulted inputs result in NO TRACTION ASSIST operation */
    if(u8_trac_switch_fault == TRUE)
    {
        u8_reset = TRUE;
        mt_power_assist.u8_fault_active = TRUE;
    }

    /* FR-15.1 / FR-15.3 Apply latching logic */
    s16_tmp = toggleButton(&mt_power_assist.t_btn_power_assist,
                           u8_power_assist_cmd,
                           0u,
                           0u,
                           u8_reset,
                           mt_power_assist.u8_safe_state);

    if(s16_tmp != C_NO_ERR)
    {
        s16_error = C_WARN;
    }

    /* FR-15.8 Output Power Assist button state to hardware Power Assist Valve Coil */
    mt_power_assist.u8_power_assist_status = mt_power_assist.u8_power_assist_latched;

    /* FR-15.6 Set Traction Valve Switch command to zero if config disabled or button disabled */
    if((mt_power_assist.pt_nvm_pa_control->u8_power_assist_installed == FALSE) ||
       (mt_power_assist.u8_power_assist_latched == POWER_ASSIST_DISABLED))
    {
        mt_power_assist.u8_traction_valve_cmd = TRACTION_VALVE_OFF;
    }
    else
    {
        mt_power_assist.u8_traction_valve_cmd =
            (f32_trac_switch_cmd != 0.0F) ? TRACTION_VALVE_ON : TRACTION_VALVE_OFF;
    }

    /* IR-15.1 / IR-15.3 Any invalid output/input combination = NO TRACTION ASSIST operation */
    if((mt_power_assist.u8_power_assist_status == POWER_ASSIST_DISABLED) &&
       (mt_power_assist.u8_traction_valve_cmd == TRACTION_VALVE_ON))
    {
        mt_power_assist.u8_traction_valve_cmd = TRACTION_VALVE_OFF;
        mt_power_assist.u8_fault_active = TRUE;
        s16_error = C_WARN;
    }

    /* FR-15.7 Output Traction Valve Switch command to hardware Traction Valve Coil */
    get_outputFaultStatus("TRACTION_VALVE_COIL", &u8_traction_output_fault);
    if(u8_traction_output_fault == FALSE)
    {
        set_outputValue("TRACTION_VALVE_COIL", (float32)mt_power_assist.u8_traction_valve_cmd);
    }
    else
    {
        mt_power_assist.u8_traction_valve_cmd = TRACTION_VALVE_OFF;
        mt_power_assist.u8_power_assist_status = POWER_ASSIST_DISABLED;
        mt_power_assist.u8_fault_active = TRUE;
        s16_error = C_WARN;
    }

    get_outputFaultStatus("POWER_ASSIST_VALVE_COIL", &u8_power_assist_output_fault);
    if(u8_power_assist_output_fault == FALSE)
    {
        set_outputValue("POWER_ASSIST_VALVE_COIL", (float32)mt_power_assist.u8_power_assist_status);
    }
    else
    {
        mt_power_assist.u8_power_assist_status = POWER_ASSIST_DISABLED;
        mt_power_assist.u8_traction_valve_cmd = TRACTION_VALVE_OFF;
        mt_power_assist.u8_fault_active = TRUE;
        s16_error = C_WARN;
    }

    /* FR-15.9 Transmit Power Assist Enable Status to operator button panel via CAN */
    if(mt_power_assist.pu8_power_assist_led_status != NULL)
    {
        *(mt_power_assist.pu8_power_assist_led_status) =
            ((u8_power_assist_output_fault == TRUE) || (u8_traction_output_fault == TRUE) || (mt_power_assist.u8_fault_active == TRUE)) ? 0x08u :
            ((mt_power_assist.u8_power_assist_status == POWER_ASSIST_ENABLED) && (mt_power_assist.pt_nvm_pa_control->u8_power_assist_installed == TRUE)) ? 0x10u :
            ((mt_power_assist.u8_power_assist_status == POWER_ASSIST_DISABLED) && (mt_power_assist.pt_nvm_pa_control->u8_power_assist_installed == TRUE)) ? 0x01u :
            0x01u;
    }
    else
    {
        s16_error = C_WARN;
    }

    mt_power_assist.u8_prev_ign_on = u8_ign_on;

    return s16_error;
}

//EOF
