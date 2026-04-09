//-----------------------------------------------------------------------------
/* * Project:   Flory_8772-4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Feb 24, 2026 kyle.boch
 */
//-----------------------------------------------------------------------------
/**
 * \file       header_lift_control.h
 * \brief      Interface for Header Lift Control Module.
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup HeaderLiftControl Header Lift Control
 * @{
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVWORK_HEADER_LIFT_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_HEADER_LIFT_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "can_device_definition.h"
#include "stwtypes.h"

#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
/**
 * @struct T_ChkPoints_Header
 * \brief Checkpoints Structure - Header Control
 *
 * Encapsulates the state-tracking checkpoints relevant to the Header Lift
 * control logic. Used to monitor transition states and safety milestones
 * across execution cycles.
 */
typedef struct
{
    uint8   u8_chk1;   //!<Checkpoint #1
    sint16  s16_chk2;  //!<Checkpoint #2
    float32 f32_chk3;  //!<Checkpoint #3

}T_ChkPoints_Header;

/**
 * @struct T_Config_HeaderControl
 * \brief Configuration Structure - Header Control
 *
 * Encapsulates all non-volatile memory (NVM) configuration variables
 * utilized by the Header Lift system to define machine-specific behavior.
 */
typedef struct
{
    uint8 u8_joystick_hll_enable; //!<Configuration parameter for if the Joystick or Hardware switches are used for HLL commands

}T_Config_HeaderControl;

/**
 * @struct T_HeaderControl
 * \brief Control Structure - Header Lift Control
 *
 * Encapsulates all persistent variables and pointers required for Header
 * Lift/Lower operations. These members persist across cyclic calls to
 * maintain state and timing.
 *
 * \note This structure excludes transient or temporary variables.
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
sint16 init_headerControl(T_CANDevices *_can_dev, T_ChkPoints_Header *_chkPoints, T_Config_HeaderControl *_nvmHeaderControl);
sint16 update_headerControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_HEADER_LIFT_CONTROL_H_ */

