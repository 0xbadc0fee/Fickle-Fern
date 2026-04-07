//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Jan 7, 2026 kyle.boch
 */
//-----------------------------------------------------------------------------
/**
 * \file       hmi_definition.h
 * \brief      AgvHMI - HMI Definition
 *
 * \addtogroup AgvHMI
 * @{
 * \addtogroup HmiDefinition HMI Definition
 *
 * The HMI Definition Module provides the global configuration, object
 * identifiers, and layout definitions for the Human-Machine Interface
 * system, ensuring consistent data mapping between the control logic
 * and the operator display.
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
#ifndef APPL_CORE_SRC_AGVHMI_HMI_DEF_H_
#define APPL_CORE_SRC_AGVHMI_HMI_DEF_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "hmi_joystick.h"
#include "hmi_8button_panel.h"
#include "hmi_8772_display.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * @struct T_UserInterface
 * \brief Structure to contain all CAN UI Elements for the 8772
 *
 * This structure aggregates the state and communication objects for the
 * operator interface hardware, including the joystick, button panel,
 * and primary display module.
 */
typedef struct
{
        T_JoystickJS6000 t_joystick;    //!<JS6000 Joystick
        T_8ButtonPanel   t_buttonPanel; //!<8 Button UI Panel
        T_8772_Display   t_display;     //!<8772 Display

}T_UserInterface;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_HMI_DEF_H_ */

