//-----------------------------------------------------------------------------
/**
 * \file       hmi_8button_panel.h
 * \brief      AgvHMI - HMI 8-Button Panel Module
 *
 * \addtogroup System
 * @{
 * \addtogroup Hmi8ButtonPanel HMI 8-Button Panel
 *
 * The HMI 8-Button Panel module processes incoming CAN messages and
 * physical inputs from the 8-button operator panel. It translates raw
 * button presses, releases, and holds into actionable system commands
 * and handles the illumination status (LED feedback) for the panel.
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
 * Jan 7, 2026 kyle.boch
 *
 * @{
 */
#ifndef APPL_CORE_SRC_AGVHMI_HMI_8BUTTON_PANEL_H_
#define APPL_CORE_SRC_AGVHMI_HMI_8BUTTON_PANEL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define BTN_FAULT 0b11 //!< Button fault state indicator

//LED Indicator Options
#define RED_ON       (0b01)      //!< Red LED Solid On
#define RED_FLASH    (0b10)      //!< Red LED Flashing
#define RED_OFF      (0b00)      //!< Red LED Off

#define AMBER_ON     (0b01)<<2   //!< Amber LED Solid On
#define AMBER_FLASH  (0b10)<<2   //!< Amber LED Flashing
#define AMBER_OFF    (0b00)<<2   //!< Amber LED Off

#define GREEN_ON     (0b01)<<4   //!< Green LED Solid On
#define GREEN_FLASH  (0b10)<<4   //!< Green LED Flashing
#define GREEN_OFF    (0b00)<<4   //!< Green LED Off

#define BLUE_ON      (0b01)<<6   //!< Blue LED Solid On
#define BLUE_FLASH   (0b10)<<6   //!< Blue LED Flashing
#define BLUE_OFF     (0b00)<<6   //!< Blue LED Off
/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_8ButtonPanel
 * \brief HMI Device Structure - 8 Button Panel
 *
 * This structure represents all variables that are transmitted to and
 * received from an 8 button Keypad UI Element
 */
typedef struct{

    //variables for keypad button states
    uint8 u8_b1_state;  //!<Button 1 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b2_state;  //!<Button 2 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b3_state;  //!<Button 3 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b4_state;  //!<Button 4 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b5_state;  //!<Button 5 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b6_state;  //!<Button 6 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b7_state;  //!<Button 7 State (0 = not pressed, 1 = pressed, 3 = fault)
    uint8 u8_b8_state;  //!<Button 8 State (0 = not pressed, 1 = pressed, 3 = fault)

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

