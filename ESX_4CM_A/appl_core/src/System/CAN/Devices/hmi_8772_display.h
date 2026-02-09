/*! \file       hmi_8772_display.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 4, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_
#define APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/** \brief HMI Device Structure - 8772 Display
 *
 * This structure represents all variables that are transmitted and
 * received from an 8772 Display.
 */
typedef struct
{
   //RX Variables
   uint8 u8_elevatorSpeedRequest;       //!<Requested Speed of Elevator

   //TX Variables
   uint8 u8_controllerVersionMinor;     //!<Controller Software Version - Minor
   uint8 u8_controllerVersionMajor;     //!<Controller Software Version - Major

   uint8 u8_elevatorStatus;             //!<Elevator Control On/Off Status

}T_8772_Display;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_ */

