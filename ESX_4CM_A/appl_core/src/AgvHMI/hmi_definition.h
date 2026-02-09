/*! \file      hmi_definition.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Jan 7, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_AGVHMI_HMI_DEF_H_
#define APPL_CORE_SRC_AGVHMI_HMI_DEF_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "hmi_joystick.h"
#include "hmi_8button_panel.h"
#include "hmi_8772_display.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Structure to contain all CAN UI Elements for the 8772 **/
typedef struct
{
        T_JoystickJS6000 t_joystick;    //!<JS6000 Joystick
        T_8ButtonPanel   t_buttonPanel; //!<8 Button UI Panel
        T_8772_Display   t_display;     //!<8772 Display

}T_UserInterface;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_HMI_DEF_H_ */

