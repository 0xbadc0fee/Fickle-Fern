/*! \file       auger_cart_control.h
    \brief     The Auger Cart Control Module shall universally control all unloading operations of a variety of
   possible attached cart configurations and do so in an operator safe manner.

    \implementation
    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 24, 2026 tiffany.gohnert
    \endimplementation
 */
#ifndef APPL_CORE_SRC_AGVWORK_AUGER_CART_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_AUGER_CART_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "can_device_definition.h"
#include "system.h"
#include "stwtypes.h"

#include "toggle_button.h"
#include "hitch_position_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DOOR_OPEN (1u)
#define DOOR_CLOSED (0u)

#define IGN_ON (1u)
#define IGN_OFF (0u)

#define AUGER_ENABLED (1u)
#define AUGER_DISABLED (0u)
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */


/** \brief Control Structure - Auger Cart Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for auger cart control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_safe_state; //!<Auger Safe State
        uint8 u8_auto_latched; //!<Auger Latched state
        uint8 u8_manual_latched; //!<Manual Latched state

        uint32 u32_ign_start_time_ms;    //!<OS Start MS timer
        uint8 u8_prev_ign_on; //!<Previous IGN ON state

        //TX CAN Variables
        uint8 *pu8_auto_enable_status;   //!<Auger Unload Enable Status (To Display)
        uint8 *pu8_auto_status_indic;   //!<Auger Status Indicator (To Button Panel)
        uint8 *pu8_manual_status_indic;   //!<Manual Unload Enable Status (To Button Panel)

        //RX CAN Variables
        uint8 *pu8_auto_command; //!<Auger On/Off Command (From Joystick)
        uint8 *pu8_manual_command; //!<Manual On/Off Command (From Joystick)

        //Button Variables
        T_ToggleBtn t_btn_auto;   //!<Toggle Button Control
        T_ToggleBtn t_btn_manual;   //!<Toggle Button Control


}T_AugerControl;
/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_augerControl(T_CANDevices *_can_dev);
sint16 update_augerControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_AUGER_CART_CONTROL_H_ */
