//-----------------------------------------------------------------------------
/* * Project:   Flory_8772-4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Feb 24, 2026 kyle.boch
 */
//-----------------------------------------------------------------------------
/**
 * \file       stick_box_control.h
 * \brief      Interface for Stick Box Control Module.
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup StickBoxControl Stick Box Control
 * @{
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVWORK_STICK_BOX_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_STICK_BOX_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "hmi_definition.h"
#include "stwtypes.h"
#include "toggle_button.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DOOR_OPEN                  (1u)  /**< Door sensor in open position */
#define DOOR_CLOSED                (0u)  /**< Door sensor in closed position */

#define IGN_ON                     (1u)  /**< Ignition signal active */
#define IGN_OFF                    (0u)  /**< Ignition signal inactive */

#define STICK_BOX_MODE_DISABLED    (0u)  /**< Stick Box operational mode disabled */
#define STICK_BOX_MODE_ENABLED     (1u)  /**< Stick Box operational mode enabled */

#define STICK_BOX_CMD_ON           (1u)  /**< Relay activation command ON */
#define STICK_BOX_CMD_OFF          (0u)  /**< Relay activation command OFF */

/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/**
 * @struct T_Config_StickBoxControl
 * \brief Configuration Structure - Stick Box Control
 *
 * Encapsulates the non-volatile memory (NVM) configuration parameters
 * that determine if the Stick Box hardware is present on the system.
 */
typedef struct
{
        uint8 u8_stick_box_installed; //!<Configuration parameter

}T_Config_StickBoxControl;

/**
 * @struct T_StickBoxControl
 * \brief Control Structure - Stick Box Control
 *
 * Encapsulates the persistent state variables and interface pointers
 * required for Stick Box relay management. These members persist across
 * cyclic calls to maintain timing and logic states.
 *
 * \note This structure excludes transient or temporary variables.
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
sint16 init_stickBControl(T_UserInterface *_ui, T_Config_StickBoxControl *_nvmStickBControl);
sint16 update_stickBControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_STICK_BOX_CONTROL_H_ */

