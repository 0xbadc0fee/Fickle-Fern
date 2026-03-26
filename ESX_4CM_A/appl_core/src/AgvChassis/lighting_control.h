/*! \file       lighting_control.h
    \brief      The Lighting Control Module shall control all required combinations of external headlights and worklights.

    \implementation
    project     Flory_8772_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 7, 2026 Tiffany.Gohnert
    \endimplementation
 */
#ifndef APPL_CORE_SRC_AGVCHASSIS_LIGHTING_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_LIGHTING_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "output_handler_lib.h"
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define WORK_ON (1u)
#define WORK_OFF (0u)
#define HEAD_ON (1u)
#define HEAD_OFF (0u)
#define TAIL_ON (1u)
#define TAIL_OFF (0u)

#define LIGHT_MODE_OFF (0u)
#define LIGHT_MODE_HEAD_TAIL (1u)
#define LIGHT_MODE_WORK (2u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Control Structure - Lighting Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for light controls that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_work_status;    //!<Lighting Work Status
        uint8 u8_head_status;    //!<Lighting Head Status
        uint8 u8_tail_status;    //!<Lighting Tail Status
        uint8 u8_light_mode; //!<Light Mode Setting Current
        uint8 u8_prev_light_btn; //<!Previous Light button State

        //TX CAN Variables
        uint8 *pu8_lgt_select_mode;   //!<Light Selector Mode Indicator TX
        uint8 *pu8_head_status;   //!<On/Off Status of Headlights TX
        uint8 *pu8_work_status;   //!<On/Off Status of Worklights TX

        //RX CAN Variables
        uint8 *pu8_light_value;   //!<On/Off Status of LIGHTS RX

}T_LightControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_lightControl(T_CANDevices *_can_dev);
sint16 update_lightControl(void);

#endif /* APPL_CORE_SRC_AGVCHASSIS_LIGHTING_CONTROL_H_ */
