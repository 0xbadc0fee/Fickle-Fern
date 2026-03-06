//-----------------------------------------------------------------------------
/*! \file       cleaning_chains_control.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "cleaning_chains_control.h"
#include "helper_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

#define PROGRAM_START (3u)

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_CChainsControl mt_cchains;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvWork - Cleaning Chains Control
 *
 *  This function initializes the AgvWork - Cleaning Chains Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkElevator Pointer to the global Cleaning Chains Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_cChainsControl(T_UserInterface *_ui, T_ChkPoints_CChains *_chkCleaningShaft)
{
    sint16 s16_error = C_NO_ERR;

    //populate local RX/TX pointers
    mt_cchains.pu8_shaft_drive_command   = &_ui->t_joystick.u8_b2_state;
    mt_cchains.pu8_shaft_drive_value = &_ui->t_display.u8_shaft_drive_status;

    //populate local copy of checkpoints
    mt_cchains.pt_cp_cchains = _chkCleaningShaft;

    //init local variables
    mt_cchains.u8_door_fault_status = FALSE;
    mt_cchains.u8_door_value = DOOR_CLOSED;
    mt_cchains.u8_shaft_fault_status= FALSE;
    mt_cchains.u8_ign_value = IGN_OFF;
    mt_cchains.u8_ign_fault_status = FALSE;

    //init toggle btn
    mt_cchains.u8_shaft_drive_latched = FALSE;
    mt_cchains.t_btn_shaft.pu_btn_state = &mt_cchains.u8_shaft_drive_latched;
    mt_cchains.t_btn_shaft.u32_hold_ms = 0u;
    mt_cchains.t_btn_shaft.u8_btn_set = 1u;

    return s16_error;

}

/** \brief Update AgvWork - Clean Chains Control
 *
 *  This function contains the cyclical logic for AgvWork - Cleaning Chains Control.
 *
 *  Primary logic for this function is to set the speed of the cleaning chains drive (cleaning shafts)
 *  based on CAN commands from the joystick.
 *
 *  Additional interlocks are utilized throughout the logic.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_cChainsControl(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_faulted = FALSE;

    // TODO inputs
    get_inputFaultStatus("CAB_DOOR", &mt_cchains.u8_door_fault_status);
    get_inputFaultStatus("IGN_SWITCH", &mt_cchains.u8_ign_fault_status);
       get_inputValue("IGN_SWITCH", &mt_cchains.u8_ign_value);
       get_inputValue("CAB_DOOR", &mt_cchains.u8_door_value);


    /*IGN start delay */
    if((mt_cchains.u8_ign_fault_status == TRUE) || (mt_cchains.u8_ign_value == FALSE))
    {
        mt_cchains.u32_ign_on_ms = 0u; //Reset timer
    }
    else
    {
        if(mt_cchains.u32_ign_on_ms  <= (UINT32_MAX - PROGRAM_START))
        {
            mt_cchains.u32_ign_on_ms += PROGRAM_START;
        }
        else
        {
            mt_cchains.u32_ign_on_ms = UINT32_MAX;
        }
    }

    //Interlocks /faulted conditions
    if((mt_cchains.u8_door_fault_status == TRUE) ||
    (mt_cchains.u8_door_value == DOOR_OPEN) ||
    (mt_cchains.u8_ign_fault_status == TRUE) ||
    (mt_cchains.u8_ign_value == IGN_OFF) ||
    (mt_cchains.u32_ign_on_ms < 30u))
    {
        u8_faulted = TRUE;
        s16_error = C_WARN;
    }

    //Toggle Joystick btn and Drive Output
    s16_error = toggleButton(&mt_cchains.t_btn_shaft, *mt_cchains.pu8_shaft_drive_command, mt_cchains.u32_dt_ms, mt_cchains.u32_deb_ms, u8_faulted, mt_cchains.u8_safe_state);
    *mt_cchains.pu8_shaft_drive_value = mt_cchains.u8_shaft_drive_latched;
    set_outputValue("SHAFT_DRIVE_GR_PUMP", (float32)mt_cchains.pu8_shaft_drive_value);

    //Publich Checkpoints
    mt_cchains.pt_cp_cchains->u8_chkPoint1 = *mt_cchains.pu8_shaft_drive_value;

    return s16_error;

}
/** \brief Get AgvWork - Shaft Drive Status
 *
 *  This function
 *
 *  Primary logic for this function is
 *
 * *  \param pu8_shaft_drive_status Pointer to the Shaft Drive ON/OFF Status
 *
 *  \return boolean
 */
void getShaftDriveStatus(uint8 *pu8_shaft_drive_status)
{
    *pu8_shaft_drive_status = *mt_cchains.pu8_shaft_drive_value;
}


//EOF
