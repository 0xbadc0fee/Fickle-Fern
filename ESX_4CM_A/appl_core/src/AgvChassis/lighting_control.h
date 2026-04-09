//-----------------------------------------------------------------------------
/**
 * \file       lights_control.h
 * \brief      AgvChassis - Lights Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup LightsControl Lights Control
 *
 * The Lights Control Module manages the activation and state of the machine's
 * lighting systems, including work lights, road lights, and auxiliary
 * illumination based on operator inputs and system conditions.
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
#ifndef APPL_CORE_SRC_AGVCHASSIS_LIGHTING_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_LIGHTING_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "output_handler_lib.h"
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define WORK_ON               (1u)  /**< Work lights activation state ON */
#define WORK_OFF              (0u)  /**< Work lights activation state OFF */
#define HEAD_ON               (1u)  /**< Headlights activation state ON */
#define HEAD_OFF              (0u)  /**< Headlights activation state OFF */
#define TAIL_ON               (1u)  /**< Tail lights activation state ON */
#define TAIL_OFF              (0u)  /**< Tail lights activation state OFF */

#define LIGHT_MODE_OFF        (0u)  /**< Lighting mode: All lights disabled */
#define LIGHT_MODE_HEAD_TAIL  (1u)  /**< Lighting mode: Headlights and Tail lights enabled */
#define LIGHT_MODE_WORK       (2u)  /**< Lighting mode: Work lights enabled */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_LightControl
 * \brief Control Structure - Lighting Control
 *
 * Encapsulates the persistent state variables and interface pointers
 * required for the management of external headlights and worklights.
 * These members persist across cyclic calls to maintain logic states
 * and safety interlocks.
 *
 * \note This structure excludes transient or temporary variables.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_work_status;    //!<Lighting Work Status
        uint8 u8_head_status;    //!<Lighting Head Status
        uint8 u8_tail_status;    //!<Lighting Tail Status
        uint8 u8_light_mode;     //!<Light Mode Setting Current
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
