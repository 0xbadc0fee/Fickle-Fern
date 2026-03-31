//-----------------------------------------------------------------------------
/*! \file       throttle_control.c
    \brief      The Throttle Control Module shall govern the vehicle's engine speed,
    measured as crankshaft rpm, by calculating new “Engine Requested Speed” values
    based on hardware input received from the operator and output via CAN based J1939
    speed control messages.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */

//-- Includes ------------------------------------------------------------------------------------------------------
//STD
#include <stdint.h>
#include "x_stdtypes.h"
#include "math.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "throttle_control.h"
#include "engine_starter_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

// -- Defines ------------------------------------------------------------------------------------------------------

// -- Types --------------------------------------------------------------------------------------------------------
// -- Module Global Function Prototypes ----------------------------------------------------------------------------
// -- Module Global Variables --------------------------------------------------------------------------------------
static T_ThrottleControl mt_throttle;
// -- Implementation  ----------------------------------------------------------------------------------------------

/** \brief Initialize Throttle Control
 *
 *  \param _ui Pointer to UI structure
 *
 *  \return s16_error Error code
 */
sint16 init_throttleControl(T_UserInterface *_ui ,T_ChkPoints_Throttle *_chk_throttle)
{
    sint16 s16_error = C_NO_ERR;

    if(_ui == NULL)
    {
        return C_WARN;
    }

    // Map TSC1 outputs
    mt_throttle.pu8_eng_ovrrd_ctrl_mode =
    &_ui->t_engine.u8_eng_ovrrd_ctrl_mode;//695

    mt_throttle.pu8_engine_speed_ctrl_req =
    &_ui->t_engine.u8_engine_speed_ctrl_req;; //SPN 696
    mt_throttle.pu8_engine_override_ctrl_pri =
    &_ui->t_engine.u8_engine_override_ctrl_pri;//SPN 897
    mt_throttle.pu16_engine_req_speed =
    &_ui->t_engine.u16_engine_req_speed;//SPN 898
    mt_throttle.pu8_eng_req_torq_limit =
    &_ui->t_engine.u8_eng_req_torq_limit;//SPN 518

    mt_throttle.pu8_ctrl_purpose =
    &_ui->t_engine.u8_ctrl_purpose;    //SPN 3350

    mt_throttle.pu8_eng_req_torq_hires =
    &_ui->t_engine.u8_eng_req_torq_hires; //SPN 4191

    // Initial state
    mt_throttle.f32_target_req_rpm = THROTTLE_DEFAULT_START_RPM;
    mt_throttle.u8_prev_engine_off = TRUE;

    mt_throttle.u32_last_update_time_ms = 0u;
    mt_throttle.u32_last_step_time_ms = 0u;
    mt_throttle.u32_engine_state_change_time_ms = 0u;

    mt_throttle.pt_cp_throttle = _chk_throttle;

    // Ramp init
    s16_error += rampInit(&mt_throttle.t_throttle_ramp,
    THROTTLE_RAMP_RATE,
    THROTTLE_ENGINE_SPEED_MIN_RPM,
    THROTTLE_ENGINE_SPEED_MAX_RPM,
    THROTTLE_DEFAULT_START_RPM);

    return s16_error;
}

/** \brief Update Throttle Control
 *
 *  \return s16_error Error code
 */
sint16 update_throttleControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_up_fault = FALSE;
    uint8 u8_down_fault = FALSE;
    uint8 u8_up_cmd = FALSE;
    uint8 u8_down_cmd = FALSE;

    float32 f32_up_cmd = FALSE;
    float32 f32_down_cmd = FALSE;
    uint8 u8_engine_off = TRUE;

    float32 f32_final_req_rpm = THROTTLE_REQ_RPM_ZERO;

    uint32 u32_now_ms = get_system_time_ms();
    uint32 u32_elapsed_ms;
    uint32 u32_engine_state_elapsed_ms;

    if((mt_throttle.pu8_eng_ovrrd_ctrl_mode == NULL) ||
    (mt_throttle.pu8_engine_speed_ctrl_req == NULL) ||
    (mt_throttle.pu8_engine_override_ctrl_pri == NULL) ||
    (mt_throttle.pu16_engine_req_speed == NULL) ||
    (mt_throttle.pu8_eng_req_torq_limit == NULL) ||
    (mt_throttle.pu8_ctrl_purpose == NULL) ||
    (mt_throttle.pu8_eng_req_torq_hires == NULL))
    {
        return C_WARN;
    }

    // Init timing
    if(mt_throttle.u32_last_update_time_ms == 0u)
    {
        mt_throttle.u32_last_step_time_ms = u32_now_ms;
        mt_throttle.u32_engine_state_change_time_ms = u32_now_ms;
    }

    mt_throttle.u32_last_update_time_ms = u32_now_ms;

    // FR-14.1 Inputs
    get_inputFaultStatus("THROTTLE_UP", &u8_up_fault);
    get_inputFaultStatus("THROTTLE_DOWN", &u8_down_fault);

    if(u8_up_fault == FALSE)
    {
        get_inputValue("THROTTLE_UP", &f32_up_cmd);
        u8_up_cmd = (uint8)f32_up_cmd;
    }

    if(u8_down_fault == FALSE)
    {
        get_inputValue("THROTTLE_DOWN", &f32_down_cmd);
        u8_down_cmd = (uint8)f32_down_cmd;
    }

    // Engine state
    getEngineOffStatus(&u8_engine_off);

    // Transition detection
    if(u8_engine_off != mt_throttle.u8_prev_engine_off)
    {
        mt_throttle.u32_engine_state_change_time_ms = u32_now_ms;
        mt_throttle.u32_last_step_time_ms = u32_now_ms;

        if(u8_engine_off == FALSE)
        {
            mt_throttle.f32_target_req_rpm = THROTTLE_DEFAULT_START_RPM;
        }
    }

    u32_engine_state_elapsed_ms = (u32_now_ms - mt_throttle.u32_engine_state_change_time_ms);

    // IR-14.1
    if((u8_up_fault == TRUE) || (u8_down_fault == TRUE))
    {
        mt_throttle.f32_target_req_rpm = THROTTLE_REQ_RPM_ZERO;
    }
    else
    {
        // FR-14.5 timeout
        if((u8_engine_off == TRUE) &&
        (u32_engine_state_elapsed_ms >= THROTTLE_6S_DELAY_MS))
        {
            mt_throttle.f32_target_req_rpm = THROTTLE_REQ_RPM_ZERO;
        }
        else
        {
            // FR-14.2 hold
            if((u8_engine_off == FALSE) &&
            (u32_engine_state_elapsed_ms < THROTTLE_6S_DELAY_MS))
            {
                mt_throttle.f32_target_req_rpm = THROTTLE_DEFAULT_START_RPM;
            }
            else if(u8_engine_off == FALSE)
            {
                u32_elapsed_ms = u32_now_ms - mt_throttle.u32_last_step_time_ms;

                if(u32_elapsed_ms >= THROTTLE_STEP_PERIOD_MS)
                {
                    mt_throttle.u32_last_step_time_ms = u32_now_ms;

                    mt_throttle.pt_cp_throttle->u8_chk2_eng_spd_up_osc = u8_up_cmd;
                    mt_throttle.pt_cp_throttle->u8_chk3_eng_spd_down_osc = u8_down_cmd;

                    if(u8_up_cmd && !u8_down_cmd)
                    {
                        mt_throttle.f32_target_req_rpm += THROTTLE_UP_ENGINE_SPEED_RPM;
                    }
                    else if(u8_down_cmd && !u8_up_cmd)
                    {
                        mt_throttle.f32_target_req_rpm -= THROTTLE_DOWN_ENGINE_SPEED_RPM;
                    }
                }

                mt_throttle.f32_target_req_rpm =
                CLAMP_F32(mt_throttle.f32_target_req_rpm,
                THROTTLE_ENGINE_SPEED_MIN_RPM,
                THROTTLE_ENGINE_SPEED_MAX_RPM);
            }

            // FR-14.4 ramp
            s16_error += rampCalc(mt_throttle.f32_target_req_rpm,
            &mt_throttle.t_throttle_ramp);
        }
    }

    //Default TSC1 fields
    *(mt_throttle.pu8_eng_req_torq_hires) = 0;
    *(mt_throttle.pu8_ctrl_purpose) = 0u;
    *(mt_throttle.pu8_eng_req_torq_limit) = 125u;
    *(mt_throttle.pu8_engine_override_ctrl_pri) = 0u;
    *(mt_throttle.pu8_engine_speed_ctrl_req) = 2u;

    f32_final_req_rpm = mt_throttle.t_throttle_ramp.f32_output;

    // Timeout-specific control
    if((u8_engine_off == TRUE) &&
    (u32_engine_state_elapsed_ms >= THROTTLE_6S_DELAY_MS))
    {
        *(mt_throttle.pu8_eng_ovrrd_ctrl_mode) = TRUE;
    }
    else{
        *(mt_throttle.pu8_eng_ovrrd_ctrl_mode) = FALSE;
    }

    // Output
    *(mt_throttle.pu16_engine_req_speed) = f32_final_req_rpm;
    mt_throttle.pt_cp_throttle->u16_chk1_eng_spd = f32_final_req_rpm;
    mt_throttle.u8_prev_engine_off = u8_engine_off;

    return s16_error;
}
//EOF
