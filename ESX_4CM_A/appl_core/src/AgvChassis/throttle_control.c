//-----------------------------------------------------------------------------
/**
 * \file       throttle_control.c
 * \brief      AgvChassis - Throttle Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup ThrottleControl Throttle Control
 *
 * The Throttle Control Module shall govern the vehicle's engine speed,
 * measured as crankshaft rpm, by calculating new "Engine Requested Speed" values
 * based on hardware input received from the operator and output via CAN based J1939
 * speed control messages.
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
 * Jan 6, 2026 Tiffany.Gohnert
 *
 * @{
 */
//-----------------------------------------------------------------------------
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
static T_ThrottleControl mt_throttle; //!< Module-local instance of the throttle control state structure.
// -- Implementation  ----------------------------------------------------------------------------------------------

/** \brief Initialize Throttle Control
 *
 * This function initializes the Throttle Control Logic.
 *
 * \param _can_devs Pointer to the CAN devices structure
 * \param _chk_throttle Pointer to the global Throttle Checkpoints structure
 *
 * \return s16_error Error code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_throttleControl(T_CANDevices *_can_devs ,T_ChkPoints_Throttle *_chk_throttle)
{
    sint16 s16_error = C_NO_ERR;

    if(_can_devs == NULL)
    {
        return C_WARN;
    }

    // Map TSC1 outputs
    mt_throttle.pu8_eng_ovrrd_ctrl_mode = &_can_devs->t_engine.u8_eng_ovrrd_ctrl_mode;//695
    mt_throttle.pu8_engine_speed_ctrl_req = &_can_devs->t_engine.u8_engine_speed_ctrl_req;; //SPN 696
    mt_throttle.pu8_engine_override_ctrl_pri = &_can_devs->t_engine.u8_engine_override_ctrl_pri;//SPN 897
    mt_throttle.pu16_engine_req_speed = &_can_devs->t_engine.u16_engine_req_speed;//SPN 898
    mt_throttle.pu8_eng_req_torq_limit =&_can_devs->t_engine.u8_eng_req_torq_limit;//SPN 518
    mt_throttle.pu8_ctrl_purpose = &_can_devs->t_engine.u8_ctrl_purpose;    //SPN 3350
    mt_throttle.pu8_eng_req_torq_hires = &_can_devs->t_engine.u8_eng_req_torq_hires; //SPN 4191

    // Initial state
    mt_throttle.f32_target_req_rpm = THROTTLE_DEFAULT_START_RPM;
    mt_throttle.u8_engine_status = ENGINE_OFF;
    mt_throttle.u8_prev_engine_status = ENGINE_OFF;

    mt_throttle.u32_last_update_time_ms = 0u;
    mt_throttle.u32_last_step_time_ms = 0u;
    mt_throttle.u32_engine_state_change_time_ms = 0u;

    mt_throttle.pt_cp_throttle = _chk_throttle;

    return s16_error;
}

/** \brief Update Throttle Control
 *
 * This function updates the Throttle Control Logic during the main execution loop.
 *
 * \return s16_error Error code
 * \retval C_NO_ERR Function Executed Properly
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

    float32 f32_final_req_rpm = THROTTLE_REQ_RPM_ZERO;

    uint32 u32_now_ms = get_system_time_ms();
    uint32 u32_engine_runtime = 0;

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

    // FR-14.1 Inputs
    get_inputFaultStatus("THROTTLE_UP", &u8_up_fault);
    get_inputFaultStatus("THROTTLE_DOWN", &u8_down_fault);

    if(u8_up_fault == FALSE)
    {
        get_inputValue("THROTTLE_UP", &f32_up_cmd);
        u8_up_cmd = ((uint8)f32_up_cmd >0)? FALSE:TRUE;
    }

    if(u8_down_fault == FALSE)
    {
        get_inputValue("THROTTLE_DOWN", &f32_down_cmd);
        u8_down_cmd = ((uint8)f32_down_cmd >0)? FALSE:TRUE;
    }

    //register throttle command
    if(u8_down_cmd)
    {
        mt_throttle.u8_throttle_cmd = THROTTLE_DECREASE;
        mt_throttle.pt_cp_throttle->u8_eng_spd_down_osc = TRUE;
        mt_throttle.pt_cp_throttle->u8_eng_spd_up_osc = FALSE;
    }
    else if(u8_up_cmd)
    {
        mt_throttle.u8_throttle_cmd = THROTTLE_INCREASE;
        mt_throttle.pt_cp_throttle->u8_eng_spd_down_osc = FALSE;
        mt_throttle.pt_cp_throttle->u8_eng_spd_up_osc = TRUE;
    }

    else
    {
        mt_throttle.u8_throttle_cmd = THROTTLE_MAINTAIN;
        mt_throttle.pt_cp_throttle->u8_eng_spd_down_osc = FALSE;
        mt_throttle.pt_cp_throttle->u8_eng_spd_up_osc = FALSE;
    }

    //register change time
    if(mt_throttle.u8_prev_throttle_cmd != mt_throttle.u8_throttle_cmd)
    {
        if(mt_throttle.u8_throttle_cmd == THROTTLE_INCREASE)
        {
            mt_throttle.u32_up_cmd_time = u32_now_ms;
            mt_throttle.u8_up_bump_ind = TRUE;
        }

        else if (mt_throttle.u8_throttle_cmd == THROTTLE_DECREASE)
        {
            mt_throttle.u32_dwn_cmd_time = u32_now_ms;
            mt_throttle.u8_down_bump_ind = TRUE;
        }
        else
        {
            mt_throttle.u8_up_bump_ind = FALSE;
            mt_throttle.u8_down_bump_ind = FALSE;
        }
    }
    else
    {
        mt_throttle.u8_up_bump_ind = FALSE;
        mt_throttle.u8_down_bump_ind = FALSE;
    }

    // get Engine status
    get_engineStatus(&mt_throttle.u8_engine_status);
    get_engineRuntime(&u32_engine_runtime);

    // engine status Transition detection
    if(mt_throttle.u8_engine_status != mt_throttle.u8_prev_engine_status)
    {
        mt_throttle.u32_engine_state_change_time_ms = u32_now_ms;
    }

    //FR-14.1/2 - check if throttle command is being held by timer (engine on for 6 seconds, held at idle)
    if((mt_throttle.u8_engine_status == ENGINE_RUNNING) && u32_engine_runtime < THROTTLE_6S_DELAY_MS)
    {
        mt_throttle.f32_target_req_rpm = THROTTLE_DEFAULT_START_RPM;
    }

    //FR-14.1/3 - check for DECREASE command
    else if (mt_throttle.u8_throttle_cmd == THROTTLE_DECREASE)
    {

        //14.1 - Check for "Bump"
        if(mt_throttle.u8_down_bump_ind == TRUE)
        {
            mt_throttle.f32_target_req_rpm -= THROTTLE_ADJUST_RPM;
            mt_throttle.u8_down_bump_ind = FALSE;
        }
        //14.3 - Check for "HOLD"
        else if ((u32_now_ms - mt_throttle.u32_dwn_cmd_time) > THROTTLE_HOLD_TIME_MS)
        {
            //adjust at rate of 50rpm / 100 ms
            if(((u32_now_ms - mt_throttle.u32_dwn_cmd_time)%50) == 0)
                mt_throttle.f32_target_req_rpm -= THROTTLE_ADJUST_RPM;
        }
    }

    //FR-14.1/3 - check for INCREASE command
    else if (mt_throttle.u8_throttle_cmd == THROTTLE_INCREASE)
    {

        //14.1 - Check for "Bump"
        if( mt_throttle.u8_up_bump_ind == TRUE)
        {
            mt_throttle.f32_target_req_rpm += THROTTLE_ADJUST_RPM;
            mt_throttle.u8_up_bump_ind = FALSE;
        }
        //14.3 - Check for "HOLD"
        else if ((u32_now_ms - mt_throttle.u32_up_cmd_time) > THROTTLE_HOLD_TIME_MS)
        {
            //adjust at rate of 50rpm / 100 ms
            if(((u32_now_ms - mt_throttle.u32_up_cmd_time)%50) == 0)
                mt_throttle.f32_target_req_rpm += THROTTLE_ADJUST_RPM;
        }
    }

    //FR-14.4 RAMP
    mt_throttle.f32_target_req_rpm = CLAMP_F32(mt_throttle.f32_target_req_rpm,
                                               THROTTLE_ENGINE_SPEED_MIN_RPM,
                                               THROTTLE_ENGINE_SPEED_MAX_RPM);

    //Default TSC1 fields
    *(mt_throttle.pu8_eng_req_torq_hires) = 0xF;
    *(mt_throttle.pu8_ctrl_purpose) = 0u;
    *(mt_throttle.pu8_eng_req_torq_limit) = 255u;
    *(mt_throttle.pu8_engine_override_ctrl_pri) = 0u;
    *(mt_throttle.pu8_engine_speed_ctrl_req) = 2u;
    *(mt_throttle.pu8_eng_ovrrd_ctrl_mode) = TRUE;

    // Output
    *(mt_throttle.pu16_engine_req_speed) = (uint16)(mt_throttle.f32_target_req_rpm / 0.125f);
    mt_throttle.pt_cp_throttle->u16_eng_spd = (uint16)(f32_final_req_rpm/0.125f);

    mt_throttle.u8_prev_engine_status = mt_throttle.u8_engine_status;
    mt_throttle.u8_prev_throttle_cmd = mt_throttle.u8_throttle_cmd;

    return s16_error;
}
//EOF
