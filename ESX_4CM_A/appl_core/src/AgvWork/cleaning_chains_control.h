//-----------------------------------------------------------------------------
/**
 * \file     cleaning_chains_control.h
 * \brief    AgvWork - Cleaning Chains Control
 *
 * This module manages the operational status of the three cleaning chains.
 * It processes the Operator Shaft Drive Enable command to establish state
 * synchronization across the broader control system.
 *
 * \project   FloryTemplate_4CM
 * \copyright STW Technic (c) 2026
 * \license   use only under terms of contract / confidential
 *
 * \created   Jan 6, 2026 STW Technic
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup CleaningChainsControl Cleaning Chains Control
 * @{
 *
 * @{
 */
#ifndef APPL_CORE_SRC_AGVWORK_CLEANING_CHAINS_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_CLEANING_CHAINS_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "can_device_definition.h"
#include "toggle_button.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DOOR_OPEN                      (1u)       /**< Door is in the OPEN position */
#define DOOR_CLOSED                    (0u)       /**< Door is in the CLOSED position */

#define IGN_ON                         (1u)       /**< Vehicle Ignition is ACTIVE */
#define IGN_OFF                        (0u)       /**< Vehicle Ignition is INACTIVE */

#define SHAFT_DRIVE_ON                 (1u)       /**< Cleaning Shaft drive is ENABLED */
#define SHAFT_DRIVE_OFF                (0u)       /**< Cleaning Shaft drive is DISABLED */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * @struct T_ChkPoints_CChains
 * @brief Checkpoints Structure - Cleaning Chains Control
 *
 * Encapsulates the state-tracking checkpoints for the cleaning chain
 * drive logic. These members are utilized to validate the sequencing
 * of the Shaft Drive Enable command and associated interlocks.
 */
typedef struct
{
        uint8 u8_status; //!<Shaft Drive Btn Checkpoint

}T_ChkPoints_CChains;


/** * @struct T_CChainsControl
 * \brief Control Structure - Cleaning Chains Control
 *
 * Encapsulates all persistent state variables and pointers required for
 * the Cleaning Chains control logic. This context is maintained across
 * cyclic execution to facilitate state transitions and drive monitoring.
 *
 * \note This structure is reserved for persistent data only;
 * transient/temporary variables are excluded.
 */
typedef struct
{
        //Local Control Variables
        uint8 u8_safe_state;    //!<Toggle Button Safe State
        uint8 u8_shaft_drive_latched;   //!< Shaft Drive Button Latched Status
        uint32 u32_ign_start_time_ms;    //!<OS Start MS timer
        uint8 u8_prev_ign_on; //!<Previous IGN ON state

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
sint16 init_cChainsControl(T_CANDevices *_can_dev, T_ChkPoints_CChains *_chkCleaningShaft);
sint16 update_cChainsControl(void);
void get_shaftDriveStatus(uint8 *pu8_shaft_drive_status);

#endif /* APPL_CORE_SRC_AGVWORK_CLEANING_CHAINS_CONTROL_H_ */
