//-----------------------------------------------------------------------------
/**
 * \file       can_device_definition.h
 * \brief      System - CAN Device Network Definitions
 *
 * \addtogroup System
 * @{
 * \defgroup CanDeviceDefinition CAN Device Definition
 * \brief This header defines the network configuration, source addresses,
 * and hardware abstraction layers for all CAN-based peripherals in the system.
 * @{
 * \copyright   STW Technic (c) 2026
 * use only under terms of contract / confidential
 *
 * \author      Jan 7, 2026 kyle.boch
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_CAN_CANDEV_DEF_H_
#define APPL_CORE_SRC_CAN_CANDEV_DEF_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "hmi_joystick.h"
#include "hmi_8button_panel.h"
#include "hmi_8772_display.h"
#include "can_engine.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Structure to contain all CAN UI Elements for the 8772 **/
typedef struct
{
        //UI Devices
        T_JoystickJS6000 t_joystick;    //!<JS6000 Joystick
        T_8ButtonPanel   t_buttonPanel; //!<8 Button UI Panel
        T_8772_Display   t_display;     //!<8772 Display

        //Engine
        T_Engine         t_engine;      //!<Engine

}T_CANDevices;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_CAN_CANDEV_DEF_H_ */

