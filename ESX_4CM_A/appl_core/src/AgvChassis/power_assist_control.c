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
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "power_assist_control.h"
#include "propulsion_control.h"

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
sint16 init_powerAssistControl(T_CANDevices *_can_dev, T_Config_PowerAssistControl *_nvmPAControl)
{
    sint16 s16_error = C_NO_ERR;

       if((_can_dev == NULL) || (_nvmPAControl == NULL))
       {
           return C_WARN;
       }

       //Populate local RX/TX pointers
       mt_power_assist.pu8_power_assist_button = &_can_dev->t_buttonPanel.u8_b5_state;
       mt_power_assist.pu8_power_assist_led_status = &_can_dev->t_buttonPanel.u8_b5_lights;

       //Populate local copy of NVM elements
       mt_power_assist.pt_nvm_pa_control = _nvmPAControl;

       //Initialize local variables
       mt_power_assist.u8_safe_state = POWER_ASSIST_DISABLED; //FR-15.4 Disabled safe state
       mt_power_assist.u8_power_assist_latched = POWER_ASSIST_DISABLED;
       mt_power_assist.u8_power_assist_status = POWER_ASSIST_DISABLED;
       mt_power_assist.u8_traction_valve_cmd = TRACTION_VALVE_OFF;
       mt_power_assist.u8_fault_active = FALSE;
       mt_power_assist.u32_ign_start_time_ms = 0u;
       mt_power_assist.u8_prev_ign_on = FALSE;

       //Initialize toggle button helper
       s16_error += toggleButton_init(
       &mt_power_assist.t_btn_power_assist,
       &mt_power_assist.u8_power_assist_latched,
       250u,
       POWER_ASSIST_DISABLED
       );

       return s16_error;
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

       uint8 u8_reset = FALSE;
       uint8 u8_power_assist_cmd = POWER_ASSIST_DISABLED;
       uint8 u8_high_gear_enabled = HIGH_GEAR_DISABLED;
       uint8 u8_ign_on = FALSE;
       uint8 u8_high_gear_status = FALSE; //TODO_STW Add getter HIGH GEAR ENABLE

       uint8 u8_power_assist_output_fault = FALSE;
       uint8 u8_traction_output_fault = FALSE;

       uint8 u8_trac_switch_fault = FALSE;
       uint8 u8_ign_fault_status = FALSE;

       float32 f32_trac_switch_cmd = 0.0F;
       float32 f32_ign_value = 0.0F;

       uint32 u32_now_ms = get_system_time_ms();

       if(mt_power_assist.pt_nvm_pa_control == NULL)
       {
           return C_WARN;
       }

       mt_power_assist.u8_fault_active = FALSE;

       //FR-15.1 Read Power Assist Enabled input from button panel as latched input
       if(mt_power_assist.pu8_power_assist_button != NULL)
       {
           u8_power_assist_cmd = (*(mt_power_assist.pu8_power_assist_button) != FALSE) ? TRUE : FALSE;
       }
       else
       {
           u8_reset = TRUE;
           mt_power_assist.u8_fault_active = TRUE;
       }

       //FR-15.2 Interpret High Gear Enable status from Propulsion Control
       get_gearSelection(&u8_high_gear_status);
       u8_high_gear_enabled = (u8_high_gear_status != FALSE) ? HIGH_GEAR_ENABLED : HIGH_GEAR_DISABLED;

       //FR-15.5 Read hardware input Traction Valve Switch command
       get_inputFaultStatus("TRACTION_VALVE", &u8_trac_switch_fault);
       get_inputValue("TRACTION_VALVE", &f32_trac_switch_cmd);

       //Read ignition / program-start timing inputs
       get_inputFaultStatus("IGNITION_SWITCH", &u8_ign_fault_status);
       get_inputValue("IGNITION_SWITCH", &f32_ign_value);

       //FR-15.3 Program start debounce timing
       u8_ign_on = ((u8_ign_fault_status == FALSE) && (f32_ign_value != 0.0F)) ? TRUE : FALSE;

       if((u8_ign_on == TRUE) && (mt_power_assist.u8_prev_ign_on == FALSE))
       {
           mt_power_assist.u32_ign_start_time_ms = u32_now_ms;
       }

       if(u8_ign_on == FALSE)
       {
           mt_power_assist.u32_ign_start_time_ms = 0u;
       }

       get_outputFaultStatus("TRACTION_VALVE", &u8_traction_output_fault);
       get_outputFaultStatus("POWER_ASSIST", &u8_power_assist_output_fault);

       //FR-15.3 Force disabled state and reset when conditions are not satisfied
       if((u8_high_gear_enabled == HIGH_GEAR_ENABLED) ||
          (u8_ign_fault_status == TRUE) ||
          ((u32_now_ms - mt_power_assist.u32_ign_start_time_ms) < PROGRAM_START_DEB_MS))
       {
           u8_reset = TRUE;
       }

       //IR-15.2 Faulted inputs result in NO TRACTION ASSIST operation
       if(u8_trac_switch_fault == TRUE)
       {
           u8_reset = TRUE;
           mt_power_assist.u8_fault_active = TRUE;
       }

       //IR-15.X Faulted outputs result in reset / safe state
       if((u8_traction_output_fault == TRUE) || (u8_power_assist_output_fault == TRUE))
       {
           u8_reset = TRUE;
           mt_power_assist.u8_fault_active = TRUE;
       }

       //FR-15.1 / FR-15.3 Apply latching and reset logic
       s16_error += toggleButton(&mt_power_assist.t_btn_power_assist,
                                 u8_power_assist_cmd,
                                 u8_reset);

       //FR-15.8 Set Power Assist status from latched state
       mt_power_assist.u8_power_assist_status = mt_power_assist.u8_power_assist_latched;

       //FR-15.6 Set Traction Valve command to zero if config disabled or button disabled
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

       //IR-15.1 / IR-15.3 Any invalid output/input combination = NO TRACTION ASSIST operation
       if((mt_power_assist.u8_power_assist_status == POWER_ASSIST_DISABLED) &&
          (mt_power_assist.u8_traction_valve_cmd == TRACTION_VALVE_ON))
       {
           mt_power_assist.u8_traction_valve_cmd = TRACTION_VALVE_OFF;
           mt_power_assist.u8_fault_active = TRUE;
           s16_error = C_WARN;
       }

       //FR-15.7 Output Traction Valve Switch command to hardware Traction Valve Coil
           set_outputValue("TRACTION_VALVE", (float32)mt_power_assist.u8_traction_valve_cmd);

       //FR-15.8 Output Power Assist button state to hardware Power Assist Valve Coil
           set_outputValue("POWER_ASSIST", (float32)mt_power_assist.u8_power_assist_status);

       //FR-15.9 Transmit Power Assist Enable Status to operator button panel via CAN
       if(mt_power_assist.pu8_power_assist_led_status != NULL)
       {
           if((u8_power_assist_output_fault == TRUE) || (u8_traction_output_fault == TRUE) || (mt_power_assist.u8_fault_active == TRUE))
               *(mt_power_assist.pu8_power_assist_led_status) =  BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_OFF;
           else if (mt_power_assist.u8_power_assist_status == POWER_ASSIST_ENABLED)
               *(mt_power_assist.pu8_power_assist_led_status) =  BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
           else if (mt_power_assist.u8_power_assist_status == POWER_ASSIST_DISABLED)
               *(mt_power_assist.pu8_power_assist_led_status) =  BLUE_OFF | GREEN_OFF | AMBER_OFF | RED_ON;
       }
       else
       {
           s16_error = C_WARN;
       }

       mt_power_assist.u8_prev_ign_on = u8_ign_on;

       return s16_error;
   }

   //EOF
