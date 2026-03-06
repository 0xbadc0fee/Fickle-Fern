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

#define PROGRAM_START_DEB_MS (3000u) //3 seconds

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

    if((_ui == NULL) || (_chkCleaningShaft == NULL))
    {
        return C_WARN;
    }

    //populate local RX/TX pointers
    mt_cchains.pu8_shaft_drive_command   = &_ui->t_joystick.u8_b2_state;
    mt_cchains.pu8_shaft_drive_value = &_ui->t_display.u8_shaft_drive_status;

    //populate local copy of checkpoints
    mt_cchains.pt_cp_cchains = _chkCleaningShaft;

    //Initialize local variables
    mt_cchains.u8_door_fault_status = FALSE;
    mt_cchains.u8_door_value = DOOR_CLOSED;
    mt_cchains.u8_shaft_fault_status= FALSE;
    mt_cchains.u8_ign_value = IGN_OFF;
    mt_cchains.u8_ign_fault_status = FALSE;
    mt_cchains.u32_ign_on_ms = 0u;
    mt_cchains.u32_dt_ms = 0u;
    mt_cchains.u32_deb_ms = 0u;
    mt_cchains.u8_safe_state = FALSE; //FR-8.2 & IR-8.2 Disabled safe state
    mt_cchains.u8_shaft_drive_latched = FALSE;

    //Initialize toggle button helper
    mt_cchains.t_btn_shaft.pu_btn_state = &mt_cchains.u8_shaft_drive_latched;
    mt_cchains.t_btn_shaft.u32_hold_ms = 0u;
    mt_cchains.t_btn_shaft.u8_btn_set = TRUE;

    //Initialize outputs to disabled state
    mt_cchains.u8_shaft_drive_latched = SHAFT_DRIVE_OFF;

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

    //Ready required interlock inputs
    get_inputFaultStatus("CAB_DOOR", &mt_cchains.u8_door_fault_status);
    get_inputValue("CAB_DOOR", &mt_cchains.u8_door_value);

    get_inputFaultStatus("IGN_SWITCH", &mt_cchains.u8_ign_fault_status);
    get_inputValue("IGN_SWITCH", &mt_cchains.u8_ign_value);

    //FR-8.2 Program Start debounce timing
    if((mt_cchains.u8_ign_fault_status == TRUE) || (mt_cchains.u8_ign_value == IGN_OFF))
    {
        mt_cchains.u32_ign_on_ms = 0u; //Reset timer
    }
    else
    {
        if(mt_cchains.u32_ign_on_ms  < (UINT32_MAX - mt_cchains.u32_dt_ms))
        {
            mt_cchains.u32_ign_on_ms += mt_cchains.u32_dt_ms;
        }
        else
        {
            mt_cchains.u32_ign_on_ms = UINT32_MAX;
        }
    }

    //FR-8.2 / IR-8.2 Disable Shaft Drive and reset when conditions not satisfied
    if((mt_cchains.u8_door_fault_status == TRUE) ||
    (mt_cchains.u8_door_value != DOOR_CLOSED) ||
    (mt_cchains.u8_ign_fault_status == TRUE) ||
    (mt_cchains.u8_ign_value == IGN_OFF) ||
    (mt_cchains.u32_ign_on_ms < PROGRAM_START_DEB_MS))
    {
        u8_faulted = TRUE;
    }

    //FR-8.1-2 IR-8.2 Apply latching and reset logic to Shaft Drive Enable. Force to safe state if fault.
    s16_error = toggleButton(&mt_cchains.t_btn_shaft, *mt_cchains.pu8_shaft_drive_command, mt_cchains.u32_dt_ms, mt_cchains.u32_deb_ms, u8_faulted, mt_cchains.u8_safe_state);

    //FR-8.3 Transmit Shaft Drive Enable to the display
    *mt_cchains.pu8_shaft_drive_value = mt_cchains.u8_shaft_drive_latched;

    //FR-8.4 Output Shaft Drive Enable status
    set_outputValue("SHAFT_DRIVE_GR_PUMP", (float32)(*mt_cchains.pu8_shaft_drive_value));

    //Publish checkpoints
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
