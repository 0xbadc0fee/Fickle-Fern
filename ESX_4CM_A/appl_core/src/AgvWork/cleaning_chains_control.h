/*! \file       cleaning_chains_control.h
    \brief      <description>


    \implementation
    project     Flory_8772_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 7, 2026 Tiffany.Gohnert
    \endimplementation
 */
#ifndef APPL_CORE_SRC_AGVWORK_CLEANING_CHAINS_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_CLEANING_CHAINS_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include <stdbool.h>

#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define INPUT_FAULTED (1u)
#define INPUT_OK (0u)

#define DOOR_OPEN (1u)
#define DOOR_CLOSED (0u)

#define IGN_ON (1u)
#define IGN_OFF (0u)

#define SHAFT_DRIVE_ON (1u)
#define SHAFT_DRIVE_OFF (0u)
/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Cleaning Chains Control
 *
 * This structure represents all checkpoints that are relevant
 * to engine start control
 */
typedef struct
{
        uint8 u8_chkPoint1; //!<Shaft Drive Btn Checkpoint

}T_ChkPoints_CChains;


/** \brief Control Structure - Cleaning Chains Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for elevator control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_door_fault_status; //!<Local Door Closed Status Variable
        uint8 u8_door_value;    //!< Cab Door Switch OPEN/CLOSE Status
        uint8 u8_ign_fault_status;  //!<Local Ignition Start Switch Status Variable
        uint8 u8_ign_value; //!<Ignition Switch ON/OFF Status
        uint8 u8_shaft_fault_status;    //!<Ignition Switch Fault Status
        uint32 u32_dt_ms;   //!<Toggle Button DT MS
        uint32 u32_deb_ms ; //!<Toggle Button Time Filter
        uint8 u8_safe_state;    //!<Toggle Button Safe State
        uint8 u8_shaft_drive_latched;   //!< Shaft Drive Button Latched Status
        uint8 u32_ign_on_ms;    //!<OS Start MS timer
        uint8 u32_dt_timer_ms;  //!<OS Start DT timer

        //TX CAN Variables
        uint8 *pu8_shaft_drive_value;   //!<On/Off Status of Shaft Drive (To Display)

        //RX CAN Variables
        uint8 *pu8_shaft_drive_command; //!<Shaft Drive On/Off Command (From Joystick)

        //Control Checkpoints
        T_ChkPoints_CChains *pt_cp_cchains; //!<Cleaning Chains Control Checkpoints Structure

        T_ToggleBtn t_btn_shaft;   //!<Toggle Button Control


}T_CChainsControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_cChainsControl(T_UserInterface *_ui, T_ChkPoints_CChains * _chkCleaningShaft);
sint16 update_cChainsControl(void);
void getShaftDriveStatus(uint8 *pu8_shaft_drive_status);

#endif /* APPL_CORE_SRC_AGVWORK_CLEANING_CHAINS_CONTROL_H_ */
