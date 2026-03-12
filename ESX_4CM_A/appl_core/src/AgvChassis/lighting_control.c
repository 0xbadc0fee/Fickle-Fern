//-----------------------------------------------------------------------------
/*! \file       lights_control.c
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
#include "lighting_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_LightControl mt_lighting;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvChassis - Lighting Control
 *
 *  This function initializes the AgvChassis - Lighting Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkElevator Pointer to the global Lights Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_lightControl(T_UserInterface *_ui)
{
    sint16 s16_error = C_NO_ERR;

    if(_ui == NULL)
    {
        return C_WARN;
    }

    //populate local copy of TX ui elements
    mt_lighting.pu8_lgt_select_mode = &_ui->t_buttonPanel.u8_b4_lights;
    mt_lighting.pu8_head_status = &_ui->t_display.u8_headlights_status;
    mt_lighting.pu8_work_status = &_ui->t_display.u8_worklights_status;

    //FR 16.1 Populate local RX pointers from Display
    mt_lighting.pu8_light_value = &_ui->t_buttonPanel.u8_b4_state;

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
    u8_led_status = (mt_lighting.u8_work_status == WORK_ON) ? 0x30u :
                    (mt_lighting.u8_head_status == HEAD_ON) ? 0x10u :
                                                              0x01u;

    //FR-16.7 - FR-16.8 Transmit to the display and button panel
    *mt_lighting.pu8_lgt_select_mode = u8_led_status;
    *mt_lighting.pu8_head_status = mt_lighting.u8_head_status;
    *mt_lighting.pu8_work_status = mt_lighting.u8_work_status;

    //FR-16.4 - FR-16.6 Output Light status IR 16.2 Fault in Output results in NO CHANGE
    get_outputFaultStatus("HEAD_LIGHTS", &u8_head_flt_status);
    if(u8_head_flt_status == FALSE)
    {
        set_outputValue("HEAD_LIGHTS", (float32)(mt_lighting.u8_head_status));
    }
    get_outputFaultStatus("WORK_LIGHTS", &u8_work_flt_status);
    if(u8_work_flt_status == FALSE)
    {
        set_outputValue("WORK_LIGHTS", (float32)(mt_lighting.u8_work_status));
    }
    get_outputFaultStatus("TAIL_LIGHTS", &u8_tail_flt_status);
    if(u8_tail_flt_status == FALSE)
    {
        set_outputValue("TAIL_LIGHTS", (float32)(mt_lighting.u8_tail_status));
    }

    return s16_error;
}

//EOF
