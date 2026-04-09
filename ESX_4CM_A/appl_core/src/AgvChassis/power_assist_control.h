//-----------------------------------------------------------------------------
/**
 * \file       power_assist_control.h
 * \brief      AgvChassis - Power Assist Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup PowerAssistControl Power Assist Control
 *
 * The Power Assist Control Module manages auxiliary power systems, such as
 * steering or traction assist, optimizing power distribution based on operator
 * inputs and machine load conditions.
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
 * Feb 24, 2026 t.gohn
 *
 * @{
 */
#ifndef APPL_CORE_SRC_AGVCHASSIS_POWER_ASSIST_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_POWER_ASSIST_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "can_device_definition.h"
#include "toggle_button.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define POWER_ASSIST_DISABLED      (0u) //!< Indicates power assist is disabled
#define POWER_ASSIST_ENABLED       (1u) //!< Indicates power assist is enabled

#define HIGH_GEAR_DISABLED         (0u) //!< Indicates high gear is disabled
#define HIGH_GEAR_ENABLED          (1u) //!< Indicates high gear is enabled

#define TRACTION_VALVE_OFF         (0u) //!< Indicates the traction valve is deactivated (off)
#define TRACTION_VALVE_ON          (1u) //!< Indicates the traction valve is activated (on)

/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/**
 * \struct Config_PowerAssistControl
 * \brief Configuration Structure - Power Assist Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to Power Assist Control.
 */
typedef struct
{
    uint8 u8_power_assist_installed; //!<Configuration parameter for Power Assist Enable

}T_Config_PowerAssistControl;

/**
 * \struct PowerAssistControl
 * \brief Control Structure - Power Assist Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for power assist control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{

        /* Local Control Variables */
        uint8 u8_safe_state;                  //!< Toggle Button Safe State
        uint8 u8_power_assist_latched;        //!< Latched Power Assist Command
        uint8 u8_power_assist_status;         //!< Final Power Assist Output State
        uint8 u8_traction_valve_cmd;          //!< Final Traction Valve Command
        uint8 u8_fault_active;                //!< Any Power Assist fault/reset active
        uint32 u32_ign_start_time_ms;         //!< System time captured on IGN OFF->ON transition
        uint8 u8_prev_ign_on;                 //!< Previous IGN ON state

        /* TX CAN Variables */
        uint8 *pu8_power_assist_led_status;   //!< Power Assist Enable Status to Button Panel

        /* RX CAN Variables */
        uint8 *pu8_power_assist_button;       //!< Power Assist Button from Button Panel

        /* Helper Control */
        T_ToggleBtn t_btn_power_assist;       //!< Toggle Button Control

        //NVM Configuration Parameters
        T_Config_PowerAssistControl *pt_nvm_pa_control;      //!<Header Control Configuration Structure

} T_PowerAssistControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_powerAssistControl(T_CANDevices *_can_devs, T_Config_PowerAssistControl *_nvmPAControl);
sint16 update_powerAssistControl(void);

#endif /* APPL_CORE_SRC_AGVCHASSIS_POWER_ASSIST_CONTROL_H_ */

