/**
 * \file       auger_cart_control.h
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
#ifndef APPL_CORE_SRC_AGVWORK_AUGER_CART_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_AUGER_CART_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "can_device_definition.h"
#include "system.h"
#include "stwtypes.h"

#include "toggle_button.h"
#include "hitch_position_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DOOR_OPEN                      (1u)       /**< Door is in the OPEN position */
#define DOOR_CLOSED                    (0u)       /**< Door is in the CLOSED position */
#define IGN_ON                         (1u)       /**< Vehicle Ignition is ACTIVE */
#define IGN_OFF                        (0u)       /**< Vehicle Ignition is INACTIVE */
#define AUGER_ENABLED                  (1u)       /**< Auger drive is ENABLED for operation */
#define AUGER_DISABLED                 (0u)       /**< Auger drive is DISABLED/LOCKED */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/**
 * \struct T_AugerControl
 * \brief Persistent state and control data for the Auger Cart.
 *
 * Encapsulates all pointers and variables required to maintain state across
 * cyclic execution. This structure is reserved for persistent data;
 * transient/temporary variables should be managed locally.
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
