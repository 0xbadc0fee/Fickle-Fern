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
#include "system.h"
//PROJECT
#include "cleaning_chains_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

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
    mt_cchains.u32_ign_start_time_ms = 0u;
    mt_cchains.u8_prev_ign_on = FALSE;
    mt_cchains.u8_safe_state = SHAFT_DRIVE_OFF; //FR-8.2 & IR-8.2 Disabled safe state
    mt_cchains.u8_shaft_drive_latched = SHAFT_DRIVE_OFF;
    //Initialize toggle button helper
    mt_cchains.t_btn_shaft.pu_btn_state = &mt_cchains.u8_shaft_drive_latched;
    mt_cchains.t_btn_shaft.u32_hold_ms = 0u;
    mt_cchains.t_btn_shaft.u8_btn_set = TRUE;

    //Initialize toggle button helper
    s16_error += toggleButton_init(
    &mt_cchains.t_btn_shaft,
    &mt_cchains.u8_shaft_drive_latched,
    0u,
    SHAFT_DRIVE_OFF
    );

    return s16_error;
}

/** \brief Update AgvWork - Clean Chains Control
 *
 *  This function contains the cyclically logic for AgvWork - Cleaning Chains Control.
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

    uint8 u8_reset = FALSE;
    uint8 u8_btn_reset = FALSE;
    uint8 u8_door_fault_status = FALSE;
    uint8 u8_ign_fault_status = FALSE;
    uint8 u8_shaft_output_fault = FALSE;

    float32 f32_door_value = DOOR_CLOSED;
    float32 f32_ign_value = IGN_OFF;
    uint8 u8_shaft_cmd = FALSE;

    uint8 u8_ign_on = FALSE;
    uint32 u32_now_ms = get_system_time_ms();

    if(mt_cchains.pu8_shaft_drive_command != NULL)
    {
        u8_shaft_cmd = ((*mt_cchains.pu8_shaft_drive_command) == 1u);
    }
    else
    {
        u8_btn_reset = TRUE;
    }

    //Ready required interlock inputs
    get_inputFaultStatus("CAB_DOOR", &u8_door_fault_status);
    get_inputValue("CAB_DOOR", &f32_door_value);

    get_inputFaultStatus("IGNITION_SWITCH", &u8_ign_fault_status);
    get_inputValue("IGNITION_SWITCH", &f32_ign_value);

    //FR-8.2 Program Start debounce timing
    u8_ign_on = ((u8_ign_fault_status == FALSE) && (f32_ign_value != IGN_OFF)) ? TRUE : FALSE;
    if((u8_ign_on == TRUE) && (mt_cchains.u8_prev_ign_on == FALSE))
    {
        mt_cchains.u32_ign_start_time_ms = u32_now_ms; //Start timer
    }

    if(u8_ign_on == FALSE)
    {
        mt_cchains.u32_ign_start_time_ms = 0u;
    }

    get_outputFaultStatus("SHAFT_PUM", &u8_shaft_output_fault);

    //FR-8.2 / IR-8.2 Disable Shaft Drive and reset when conditions not satisfied
    if((u8_door_fault_status == TRUE) ||
    (f32_door_value != DOOR_CLOSED) ||
    (u8_ign_fault_status == TRUE) ||
    (f32_ign_value == IGN_OFF) ||
    (u8_shaft_output_fault == TRUE) ||
    (u8_btn_reset = TRUE) ||
    ((u32_now_ms - mt_cchains.u32_ign_start_time_ms) < PROGRAM_START_DEB_MS))
    {
        u8_reset = TRUE;
    }

    //FR-8.1-2 IR-8.2 Apply latching and reset logic to Shaft Drive Enable. Force to safe state if fault.
    s16_error = toggleButton(&mt_cchains.t_btn_shaft, u8_shaft_cmd , u8_reset);
    //FR-8.3 Transmit Shaft Drive Enable to the display
    *mt_cchains.pu8_shaft_drive_value = mt_cchains.u8_shaft_drive_latched;

    //FR-8.4 Output Shaft Drive Enable status
    set_outputValue("SHAFT_PUMP", (float32)(*mt_cchains.pu8_shaft_drive_value));

    //Publish checkpoints
    mt_cchains.pt_cp_cchains->u8_chkPoint1 = *mt_cchains.pu8_shaft_drive_value;

    mt_cchains.u8_prev_ign_on = u8_ign_on;

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
    if(mt_cchains.pu8_shaft_drive_value != NULL && pu8_shaft_drive_status != NULL)
    {
        *pu8_shaft_drive_status = *mt_cchains.pu8_shaft_drive_value;
    }
}

//EOF
