//-----------------------------------------------------------------------------
/**
 * \file       hmi_8772_display.h
 * \brief      AgvHMI - HMI 8772 Display Module
 *
 * \addtogroup System
 * @{
 * \addtogroup Hmi8772Display HMI 8772 Display
 *
 * The HMI 8772 Display module handles communication and data synchronization
 * between the main controller and the 8772 Human Machine Interface. It processes
 * incoming user inputs, button commands, and settings, while transmitting
 * machine status, telemetry, and fault data to the screen for the operator.
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
 * Feb 4, 2026 kyle.boch
 *
 * @{
 */
#ifndef APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_
#define APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_8772_Display
 * \brief HMI Device Structure - 8772 Display
 *
 * This structure represents all variables that are transmitted and
 * received from an 8772 Display.
 */
typedef struct
{
   //RX Variables
   uint8 u8_elevatorSpeedRequest;       //!<Requested Speed of Elevator
   uint8 u8_drum_speed_request;       //!<Requested Speed of Front Sweeps
   uint8 u8_drum_speed_enable;       //!<Requested Front Sweeps enabled
   uint8 u8_trap_speed_range; //!<Requested Rotary Trap Speed Range

   //TX Variables
   uint8 u8_controllerVersionMinor;     //!<Controller Software Version - Minor
   uint8 u8_controllerVersionMajor;     //!<Controller Software Version - Major

   uint8 u8_elevatorStatus;             //!<Elevator Control On/Off Status

   uint8 u8_relief_switch_status;       //!<Header Lift/Lower Relief Switch Status
   uint8 u8_auger_status;

   uint8 u8_headlights_status; //!<Head Light Status
   uint8 u8_worklights_status; //!<Work Light Status
   uint8 u8_shaft_drive_status;         //!<Cleaning Chains Shaft Drive Status

}T_8772_Display;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_ */

