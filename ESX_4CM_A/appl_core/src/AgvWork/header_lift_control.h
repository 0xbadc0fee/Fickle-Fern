/*! \file       header_lift_control.h
    \brief      <description>


   	\implementation
   	project     Flory_8772-4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 24, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_AGVWORK_HEADER_LIFT_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_HEADER_LIFT_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "hmi_definition.h"
#include "stwtypes.h"

#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
/** \brief Checkpoints Structure - Header Control
 *
 * This structure represents all checkpoints that are relevant
 * to header control
 */
typedef struct
{
    uint8   u8_chk1;   //!<Checkpoint #1
    sint16  s16_chk2;  //!<Checkpoint #2
    float32 f32_chk3;  //!<Checkpoint #3

}T_ChkPoints_Header;

/** \brief Configuration Structure - Header Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to header control
 */
typedef struct
{
    uint8 u8_joystick_hll_enable; //!<Configuration parameter for if the Joystick or Hardware switches are used for HLL commands

}T_Config_HeaderControl;

/** \brief Control Structure - Header Lift  Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for header lift lower control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
    //Local Control Variables
    uint8 u8_lift_command;                  //!<Lift Command
    uint8 u8_lower_command;                 //!<Lower Command

    //TX CAN Variables
    uint8 *pu8_relief_swich;                //!<Pointer to the Releif Switch Status to Display

    //RX CAN Variables
    uint8 *pu8_joy_lift_header;              //!<Pointer to Header Lift Button from Joystick
    uint8 *pu8_joy_lower_header;             //!<Pointer to Header Lower Button from Joystick

    //NVM Configuration Parameters
    T_Config_HeaderControl *pt_nvm_hdr_control;      //!<Header Control Configuration Structure

    //Control Checkpoints
    T_ChkPoints_Header *pt_chkPoints;   //!<Header Control Checkpoints Structure


}T_HeaderControl;
/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_headerControl(T_UserInterface *_ui, T_ChkPoints_Header *_chkPoints, T_Config_HeaderControl *_nvmHeaderControl);
sint16 update_headerControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_HEADER_LIFT_CONTROL_H_ */

