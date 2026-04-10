//-----------------------------------------------------------------------------
/**
 * \file       lights_control.c
 * \brief      AgvChassis - Lights Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup LightsControl Lights Control
 *
 * The Lights Control Module manages the activation and state of the machine's
 * lighting systems, including work lights, road lights, and auxiliary
 * illumination based on operator inputs and system conditions.
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
 * Jan 6, 2026 STW Technic
 *
 * @{
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "lighting_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_LightControl mt_lighting;  //!< Global persistent state for Lights Control.

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/**
 * \brief Initialize AgvChassis - Lighting Control
 *
 * This function initializes the Lighting Control logic, establishing the
 * link to the operator interface for manual and automatic light management.
 *
 * \param[in,out] _can_devs  Pointer to the project's UI Structure
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_lightControl(T_CANDevices *_can_devs)
{
    sint16 s16_error = C_NO_ERR;

    if(_can_devs == NULL)
    {
        return C_WARN;
    }

    //populate local copy of TX ui elements
    mt_lighting.pu8_lgt_select_mode = &_can_devs->t_buttonPanel.u8_b4_lights;
    mt_lighting.pu8_head_status = &_can_devs->t_display.u8_headlights_status;
    mt_lighting.pu8_work_status = &_can_devs->t_display.u8_worklights_status;

    //FR 16.1 Populate local RX pointers from Display
    mt_lighting.pu8_light_value = &_can_devs->t_buttonPanel.u8_b4_state;

    //Initialize local variables FR-16.2 Default State OFF
    mt_lighting.u8_work_status = WORK_OFF;
    mt_lighting.u8_head_status = HEAD_OFF;
    mt_lighting.u8_tail_status = TAIL_OFF;
    mt_lighting.u8_prev_light_btn = 0u;
    mt_lighting.u8_light_mode = LIGHT_MODE_OFF;

    return s16_error;
}

/** \brief Update AgvChassis - Lighting Control
 *
 *  This function contains the cyclical logic for AgvChassis - Lighting Control.
 *
 * The Lighting Control Module shall control all required combinations of external headlights and worklights.
 *
 *  Additional interlocks are utilized throughout the logic.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_lightControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_light_cmd = 0u;
    uint8 u8_tail_flt_status = FALSE;
    uint8 u8_work_flt_status = FALSE;
    uint8 u8_head_flt_status = FALSE;
    uint8 u8_led_status = 0x01u;

    //IR-16.1 Input fault shall result in NO CHANGE.
    if(mt_lighting.pu8_light_value == NULL)
    {
        return C_WARN;
    }

    //FR-16.1 Read light selector command
    u8_light_cmd = (*(mt_lighting.pu8_light_value) != 0u) ? 1u : 0u;

    //FR-16.3 Count one toggle per new button press
    if((u8_light_cmd == 1u) && (mt_lighting.u8_prev_light_btn == 0u))
    {
        mt_lighting.u8_light_mode++;

        if(mt_lighting.u8_light_mode > LIGHT_MODE_WORK)
        {
            mt_lighting.u8_light_mode = LIGHT_MODE_OFF;
        }
    }

    //Save current button state for next cycle
    mt_lighting.u8_prev_light_btn = u8_light_cmd;

    //FR-16.3 Cycle the Lighting System
    if(mt_lighting.u8_light_mode == LIGHT_MODE_HEAD_TAIL) //Head Lights
    {
        mt_lighting.u8_work_status = WORK_OFF;
        mt_lighting.u8_head_status = HEAD_ON;
        mt_lighting.u8_tail_status = TAIL_ON;
    }
    else if(mt_lighting.u8_light_mode == LIGHT_MODE_WORK) //Work Lights
    {
        mt_lighting.u8_work_status = WORK_ON;
        mt_lighting.u8_head_status = HEAD_ON;
        mt_lighting.u8_tail_status = TAIL_OFF;
    }
    else
    {
        mt_lighting.u8_work_status = WORK_OFF;
        mt_lighting.u8_head_status = HEAD_OFF;
        mt_lighting.u8_tail_status = TAIL_OFF;
        mt_lighting.u8_light_mode = LIGHT_MODE_OFF;
    }

    //LED Indicator
    if(mt_lighting.u8_work_status == WORK_ON)
        u8_led_status = BLUE_ON | GREEN_OFF | AMBER_OFF | RED_OFF;
    else if(mt_lighting.u8_head_status == HEAD_ON)
        u8_led_status = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_OFF;
    else
        u8_led_status = BLUE_OFF | GREEN_ON | AMBER_OFF | RED_ON;

    //FR-16.7 - FR-16.8 Transmit to the display and button panel
    *mt_lighting.pu8_lgt_select_mode = u8_led_status;
    *mt_lighting.pu8_head_status = mt_lighting.u8_head_status;
    *mt_lighting.pu8_work_status = mt_lighting.u8_work_status;

    //FR-16.4 - FR-16.6 Output Light status IR 16.2 Fault in Output results in NO CHANGE
    get_outputFaultStatus("HEADLIGHTS", &u8_head_flt_status);
    if(u8_head_flt_status == FALSE)
    {
        set_outputValue("HEADLIGHTS", (float32)(mt_lighting.u8_head_status));
    }
    get_outputFaultStatus("WORKLIGHTS", &u8_work_flt_status);
    if(u8_work_flt_status == FALSE)
    {
        set_outputValue("WORKLIGHTS", (float32)(mt_lighting.u8_work_status));
    }
    get_outputFaultStatus("TAILLIGHTS", &u8_tail_flt_status);
    if(u8_tail_flt_status == FALSE)
    {
        set_outputValue("TAILLIGHTS", (float32)(mt_lighting.u8_tail_status));
    }

    return s16_error;
}

//EOF
