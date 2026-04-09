//-----------------------------------------------------------------------------
/**
 * \file       joystick_handler.h
 * \brief      System - Joystick Handler Interface
 *
 * \addtogroup System
 * @{
 * \defgroup JoystickHandler Joystick Handler
 * \brief This module provides the interface for processing CAN-based joystick
 * inputs, including button mapping and axis normalization for machine control.
 * @{
 *
 * \implementation
 * project     FloryTemplate_4CM
 * copyright   STW Technic (c) 2026
 * license     use only under terms of contract / confidential
 *
 * created     Jan 7, 2026 kyle.boch
 * \endimplementation
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVHMI_JOYSTICK_HANDLER_H_
#define APPL_CORE_SRC_AGVHMI_JOYSTICK_HANDLER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define JS_BUTTON_PRESSED 0b01
#define JS_BUTTON_FAULT   0b11

#define JS_MAX_Y_POS     10000
#define JS_MIN_Y_POS     -10000
#define JS_NEUTRAL_POS       0
/* -- Types --------------------------------------------------------------------------------------------------------- */
/** \brief HMI Device Structure - JS6000 Joystick
 *
 * This structure represents all used variables that are transmitted to and
 * received from a JS6000 Joystick
 */
typedef struct{

    uint8 u8_joystickActive;

    uint16 u16_yPos;        //!<Y Position of Joystick (0-100%)
    uint8  u8_fwd_status;    //!<Forward Status(Y Pos) of Joystick
    uint8  u8_rev_status;   //!<Reverse Status (YPos) of Joystick
    uint8  u8_b1_state;     //!<Button 1 State (0 = not pressed, 1 = pressed)
    uint8  u8_b2_state;     //!<Button 2 State (0 = not pressed, 1 = pressed)
    uint8  u8_b3_state;     //!<Button 3 State (0 = not pressed, 1 = pressed)
    uint8  u8_b4_state;     //!<Button 4 State (0 = not pressed, 1 = pressed)
    uint8  u8_b5_state;     //!<Button 5 State (0 = not pressed, 1 = pressed)
    uint8  u8_b6_state;     //!<Button 6 State (0 = not pressed, 1 = pressed)

}T_JoystickJS6000;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_JOYSTICK_HANDLER_H_ */

