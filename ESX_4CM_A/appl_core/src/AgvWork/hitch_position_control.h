//-----------------------------------------------------------------------------
/**
 * \file       hitch_position_control.h
 * \brief      AgvWork - Hitch Position Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup HitchPositionControl Hitch Position Control
 *
 * The Hitch Position Control Module manages the machine's "hitch" movement.
 * It processes operator Hitch "IN" and Hitch "OUT" commands to regulate
 * position and hydraulic engagement.
 *
 * @{
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

#ifndef APPL_CORE_SRC_AGVWORK_HITCH_POSITION_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_HITCH_POSITION_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "can_device_definition.h"
#include "header_lift_control.h"
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/** \brief Control Structure - Hitch Position Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for hitch position control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
    //Local Control Variables
    uint8 u8_in_command;                  //!<In Command
    uint8 u8_out_command;                 //!<Out Command

    //TX CAN Variables

    //RX CAN Variables
    uint8 *pu8_joy_hitch_in;              //!<Pointer to Hitch In Button from Joystick
    uint8 *pu8_joy_hitch_out;               //!<Pointer to Hitch Out Button from Joystick

    //NVM Configuration Parameters
    T_Config_HeaderControl *pt_nvm_hp_control;      //!<Hitch Position Configuration Structure

}T_HitchPosControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_hitchPosControl(T_CANDevices *_can_dev, T_Config_HeaderControl *_nvmHeaderControl);
sint16 update_hitchPosControl(void);
void get_hitchPosStatus(uint8 * pu8_hitchON);

#endif /* APPL_CORE_SRC_AGVWORK_HITCH_POSITION_CONTROL_H_ */
