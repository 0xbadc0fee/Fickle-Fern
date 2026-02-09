/*! \file       hmi_8button_panel.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Jan 7, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_AGVHMI_HMI_8BUTTON_PANEL_H_
#define APPL_CORE_SRC_AGVHMI_HMI_8BUTTON_PANEL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/** \brief HMI Device Structure - 8 Button Panel
 *
 * This structure represents all variables that are transmitted to and
 * received from an 8 button Keypad UI Element
 */
typedef struct{

    //variables for keypad button states
    uint8 u8_b1_state;  //!<Button 1 State (0 = not pressed, 1 = pressed)
    uint8 u8_b2_state;  //!<Button 2 State (0 = not pressed, 1 = pressed)
    uint8 u8_b3_state;  //!<Button 3 State (0 = not pressed, 1 = pressed)
    uint8 u8_b4_state;  //!<Button 4 State (0 = not pressed, 1 = pressed)
    uint8 u8_b5_state;  //!<Button 5 State (0 = not pressed, 1 = pressed)
    uint8 u8_b6_state;  //!<Button 6 State (0 = not pressed, 1 = pressed)
    uint8 u8_b7_state;  //!<Button 7 State (0 = not pressed, 1 = pressed)
    uint8 u8_b8_state;  //!<Button 8 State (0 = not pressed, 1 = pressed)

    //placeholders for keypad indicator lights
    uint8 u8_b1_lights; //!<Button 1 LED Configuration - See documentation for mapping
    uint8 u8_b2_lights; //!<Button 2 LED Configuration - See documentation for mapping
    uint8 u8_b3_lights; //!<Button 3 LED Configuration - See documentation for mapping
    uint8 u8_b4_lights; //!<Button 4 LED Configuration - See documentation for mapping
    uint8 u8_b5_lights; //!<Button 5 LED Configuration - See documentation for mapping
    uint8 u8_b6_lights; //!<Button 6 LED Configuration - See documentation for mapping
    uint8 u8_b7_lights; //!<Button 7 LED Configuration - See documentation for mapping
    uint8 u8_b8_lights; //!<Button 8 LED Configuration - See documentation for mapping

}T_8ButtonPanel;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_HMI_8BUTTON_PANEL_H_ */

