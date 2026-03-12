/*! \file       stick_remover_control.h
    \brief      The Stick Remover Control Module shall control ON/OFF operation
    of the optional, cart installed Stick Remover conveyor chain.

    \implementation
    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Mar 12, 2026 t.gohn
    \endimplementation
 */
#ifndef APPL_CORE_SRC_AGVWORK_STICK_REMOVER_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_STICK_REMOVER_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "hmi_definition.h"
#include "input_handler_lib.h"
#include "output_handler_lib.h"
#include "helper_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DOOR_OPEN                 (1u)
#define DOOR_CLOSED               (0u)

#define IGN_ON                    (1u)
#define IGN_OFF                   (0u)

#define STICK_REMOVER_ENABLED     (1u)
#define STICK_REMOVER_DISABLED    (0u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Control Structure - Stick Remover Control
 *
 *  This structure represents all variables and pointers that
 *  are utilized and tracked for stick remover control that need to
 *  persist through cyclic calls (static).
 *
 *  This structure does not include any variables that are considered
 *  temporary.
 */
typedef struct
{
    /* Local Control Variables */
    uint8 u8_safe_state;                 //!< Toggle Button Safe State
    uint8 u8_stick_remover_latched;      //!< Stick Remover Latched Status
    uint32 u32_ign_start_time_ms;        //!< System time captured on IGN OFF->ON transition
    uint8 u8_prev_ign_on;                //!< Previous IGN ON state

    /* TX CAN Variables */
    uint8 *pu8_stick_remover_status;     //!< Stick Remover Enable Status (to Display)
    uint8 *pu8_stick_remover_led_status; //!< Stick Remover LED Status (to Button Panel)

    /* RX CAN Variables */
    uint8 *pu8_stick_remover_command;    //!< Stick Remover Command (from Button Panel)

    /* Helper Control */
    T_ToggleBtn t_btn_stick_remover;     //!< Toggle Button Control

} T_StickRemoverControl;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_stickRemoverControl(T_UserInterface *_ui);
sint16 update_stickRemoverControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_STICK_REMOVER_CONTROL_H_ */
