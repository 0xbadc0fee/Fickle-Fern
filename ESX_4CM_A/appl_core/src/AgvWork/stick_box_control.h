//-----------------------------------------------------------------------------
/**
 * \file       stick_box_control.h
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
#ifndef APPL_CORE_SRC_AGVWORK_STICK_BOX_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_STICK_BOX_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "can_device_definition.h"
#include "stwtypes.h"
#include "toggle_button.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DOOR_OPEN                       (1u)        //!< Indicator for door in OPEN state
#define DOOR_CLOSED                     (0u)        //!< Indicator for door in CLOSED state

#define IGN_ON                          (1u)        //!< Ignition is in the ON state
#define IGN_OFF                         (0u)        //!< Ignition is in the OFF state

#define STICK_BOX_MODE_DISABLED         (0u)        //!< Stick Box functionality is disabled
#define STICK_BOX_MODE_ENABLED          (1u)        //!< Stick Box functionality is enabled

#define STICK_BOX_CMD_ON                (1u)        //!< Logic command to activate stick box relays
#define STICK_BOX_CMD_OFF               (0u)        //!< Logic command to deactivate stick box relays

/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/**
 * \struct Config_StickBoxControl
 * \brief Configuration Structure - Stick Box Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to Stick Box control.
 */
typedef struct
{
        uint8 u8_stick_box_installed; //!<Configuration parameter

}T_Config_StickBoxControl;

/**
 * \struct StickBControl
 * \brief Control Structure - Stick Box Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for Stick Box Control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_stick_box_mode; //<!Stick Box Installed = Stick Box Mode enabled
        uint8 u8_closed_cmd; //!<Final Close Command
        uint8 u8_open_cmd; //!<Final Open Command
        uint8 u8_safe_state; //!<Toggle Button Safe State for Auxiliary Close latch
        uint32 u32_ign_start_time_ms;        //!< System time captured on IGN OFF->ON transition
        uint8 u8_prev_ign_on;                //!< Previous IGN ON state

        //TX CAN Variables
        uint8 *pu8_close_led_status;                //!<Pointer to the Stick Box Close LED Button Panel
        uint8 *pu8_open_led_status;                //!<Pointer to the Stick Box Open LED Button Panel

        //RX CAN Variables
        uint8 *pu8_close_button;                //!<Pointer to the Stick Box Close from Button Panel
        uint8 *pu8_open_button;                //!<Pointer to the Stick Box Open from Button Panel

        //Helper Control
        T_ToggleBtn t_btn_close_aux;   //!<Auxiliary Mode latched closed button control

        //NVM Configuration Parameters
        T_Config_StickBoxControl *pt_nvm_stick_control;      //!<StickB Control Configuration Structure

}T_StickBControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_stickBControl(T_CANDevices *_can_dev, T_Config_StickBoxControl *_nvmStickBControl);
sint16 update_stickBControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_STICK_BOX_CONTROL_H_ */

