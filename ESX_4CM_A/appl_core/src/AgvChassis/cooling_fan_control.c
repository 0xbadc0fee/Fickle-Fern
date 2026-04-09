//-----------------------------------------------------------------------------
/*! \file       cooling_fan_control.c
    \brief      The Cooling Fan Control Module controls two output valves (Speed Control Valve
    and Fan Direction Valve) to control the fan speed and direction.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
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
#include "cooling_fan_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "propulsion_control.h"
#include "engine_starter_control.h"


/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
void update_coolingFanReversal(void);
void calc_coolingDemand(void);
void calc_maxDemand(float32 f32_cmd1, float32 f32_cmd2, float32 f32_cmd3, float32 *_maxval);
void set_fanSafeState(void);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_CoolingFanControl mt_cf;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */


/** \brief Initialize AgvWork - Cooling Fan Control
 *
 *  This function initializes the AgvWork - Cooling Fan Control Logic.
 *
 *  \param _ui Pointer to the project's UI Structure
 *  \param _chkCooling Fan Pointer to the global Cooling Fan Checkpoints Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_coolingFanControl(T_CANDevices *_can_devs, T_ChkPoints_CoolingFan *_chkCoolingFan)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_devs == NULL) || (_chkCoolingFan == NULL))
    {
        return C_WARN;
    }

    //populate local RX/TX pointers
    //RX
    mt_cf.pu8_engine_coolant_temp_degC  = &_can_devs->t_engine.u8_coolant_temp;
    mt_cf.pu8_intake_manifold_temp_degC = &_can_devs->t_engine.u8_intake_temp;
    mt_cf.pu8_manual_purge_req          = &_can_devs->t_display.u8_manual_purge_req;

    //TX
    mt_cf.pu16_disp_hyd_oil_temp_degC   = &_can_devs->t_display.u16_hyd_oil_temp_degC;
    mt_cf.pu8_disp_fan_reverse_ind      = &_can_devs->t_display.u8_cooling_fan_reverse_ind;
    mt_cf.pu8_disp_cooling_system_fault = &_can_devs->t_display.u8_cooling_system_fault;
    mt_cf.pu8_disp_hyd_oil_overtemp     = &_can_devs->t_display.u8_hyd_oil_overtemp;
    mt_cf.pu8_disp_intake_overtemp      = &_can_devs->t_display.u8_intake_manifold_overtemp;
    mt_cf.pu8_disp_coolant_overtemp     = &_can_devs->t_display.u8_engine_coolant_overtemp;

    //Populate local copy of checkpoints
    mt_cf.pt_cp_cooling = _chkCoolingFan;

    //Initialize local variables

    mt_cf.u8_cleanout_active = FALSE;
    mt_cf.u8_sequence_fault = FALSE;

    mt_cf.f32_dir_cmd = CF_DIR_FORWARD;
    mt_cf.f32_speed_cmd = CF_PWM_HIGH_LIMIT;

    mt_cf.e_fanstate = CF_STATE_RUN_FWD;
    mt_cf.e_prev_fanstate = CF_STATE_STOPPED;

    s16_error += rampInit(&mt_cf.t_speed_ramp,
                           CF_FWD_RUN_RAMP,
                           CF_PWM_LOW_LIMIT,
                           CF_PWM_HIGH_LIMIT,
                           CF_SAFE_PWM);


    s16_error += movingFltInit(&mt_cf.t_hyd_oil_temp_filt,
                                mt_cf.f32_hyd_buff,
                                CF_HYD_BUF_LEN,
                                0.0F,
                                CF_HYD_FILTER_SAMPLE_NO,
                                CF_HYD_FILTER_SAMPLE_MS);

    return s16_error;
}

/** \brief Update AgvWork - Cooling Fan Control
 *
 *  This function contains the cyclically logic for AgvWork - Cooling Fan Control.
 *
 *  Primary logic for this function
 *
 *  Additional interlocks are utilized throughout the logic.
 *
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_coolingFanControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_engine_status = 0;
    uint32 u32_engine_runtime = 0;

    uint8 u8_speed_output_fault = FALSE;
    uint8 u8_dir_output_fault = FALSE;

    // IR-7.3 output faults
    get_outputFaultStatus("COOL_FAN_SPEED", &u8_speed_output_fault);
    get_outputFaultStatus("COOL_FAN_DIRECTION", &u8_dir_output_fault);

    get_engineStatus(&u8_engine_status);
    get_engineRuntime(&u32_engine_runtime);

    //if engine has only been running for <3.5 seconds or output fault - default fan to safe state
    if((u8_engine_status == ENGINE_OFF     ||
        u32_engine_runtime <= 3500)        ||
        u8_dir_output_fault                ||
        u8_speed_output_fault)
    {
        set_fanSafeState();
    }
    else
    {
        //Calculate Cooling Demand
        calc_coolingDemand();

        //Update Reversal State Machine
        update_coolingFanReversal();

    }

    //ramp speed output
    rampCalc(mt_cf.f32_speed_cmd, &mt_cf.t_speed_ramp);

    // Hardware outputs
    set_outputValue("COOL_FAN_SPEED",      mt_cf.t_speed_ramp.f32_output);
    set_outputValue("COOL_FAN_DIRECTION",  mt_cf.f32_dir_cmd);

    // FR-7.15 CAN/display outputs
    *(mt_cf.pu16_disp_hyd_oil_temp_degC)   = mt_cf.f32_hydoil_temp;
    *(mt_cf.pu8_disp_fan_reverse_ind)      = (mt_cf.f32_dir_cmd == CF_DIR_REVERSE) ? TRUE : FALSE;
    *(mt_cf.pu8_disp_cooling_system_fault) = mt_cf.u8_sequence_fault;
    *(mt_cf.pu8_disp_hyd_oil_overtemp) = (mt_cf.f32_coolant_temp >= CF_COOLANT_OVERTEMP_C) ? TRUE : FALSE;
    *(mt_cf.pu8_disp_intake_overtemp)  = (mt_cf.f32_intake_temp  >= CF_INTAKE_OVERTEMP_C)  ? TRUE : FALSE;
    *(mt_cf.pu8_disp_coolant_overtemp) = (mt_cf.f32_hydoil_temp  >= CF_HYDOIL_OVERTEMP_C)  ? TRUE : FALSE;

    return s16_error;
}

void calc_coolingDemand(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_hydoil_fault = 0;

    //Get all temperatures
    mt_cf.f32_coolant_temp = (float32)*(mt_cf.pu8_engine_coolant_temp_degC) - 40.0F;
    mt_cf.f32_intake_temp = (float32)*(mt_cf.pu8_intake_manifold_temp_degC) - 40.0F;

    // FR-7.3 hydraulic oil plausibility / filtering
    get_inputFaultStatus("HYD_OIL_TEMP", &u8_hydoil_fault);
    if(u8_hydoil_fault == FALSE)
    {
        get_inputValue("HYD_OIL_TEMP", &mt_cf.f32_hydoil_temp);
    }


    s16_error += movingAdvFlt(&mt_cf.t_hyd_oil_temp_filt, mt_cf.f32_hydoil_temp);
    mt_cf.f32_hydoil_temp = mt_cf.t_hyd_oil_temp_filt.f32_out;

    if((mt_cf.f32_hydoil_temp  > CF_FAULT_TEMP)||
       (mt_cf.f32_coolant_temp > CF_FAULT_TEMP)||
       (mt_cf.f32_intake_temp  > CF_FAULT_TEMP))
    {
        set_fanSafeState();
    }
    else
    {
        // FR-7.4 temperature to cooling demand
        mt_cf.f32_coolant_cmd = ((mt_cf.f32_coolant_temp - CF_COOLANT_MIN_C) * (CF_MAX_COOL_DEMAND)) /
                                 (CF_COOLANT_MAX_C - CF_COOLANT_MIN_C);

        mt_cf.f32_intake_cmd = ((mt_cf.f32_intake_temp - CF_INTAKE_MIN_C) * (CF_MAX_COOL_DEMAND)) /
                                (CF_INTAKE_MAX_C - CF_INTAKE_MIN_C);

        mt_cf.f32_hydoil_cmd = ((mt_cf.f32_hydoil_temp - CF_HYDOIL_MIN_C) * (CF_MAX_COOL_DEMAND)) /
                                (CF_HYDOIL_MAX_C - CF_HYDOIL_MIN_C);

        //calculate the highest cooling demand from the 3 termperatures
        calc_maxDemand (mt_cf.f32_hydoil_cmd, mt_cf.f32_intake_cmd, mt_cf.f32_coolant_cmd, &mt_cf.f32_cooling_demand);
        mt_cf.f32_cooling_demand = CLAMP_F32(mt_cf.f32_cooling_demand, CF_MIN_COOL_DEMAND, CF_MAX_COOL_DEMAND);
        mt_cf.pt_cp_cooling->f32_cooling_demand_pct = mt_cf.f32_cooling_demand;

    }
}


void set_fanSafeState(void)
{
    mt_cf.u8_cleanout_active = FALSE;
    mt_cf.e_fanstate = CF_STATE_RUN_FWD;

    mt_cf.f32_dir_cmd = CF_DIR_FORWARD;
    mt_cf.f32_speed_cmd = CF_SAFE_PWM;
}


void calc_maxDemand(float32 f32_cmd1, float32 f32_cmd2, float32 f32_cmd3, float32 *_maxval)
{
    *_maxval = f32_cmd1;
    mt_cf.pt_cp_cooling->u8_leadsensornumber = 1;

    if(f32_cmd2 > *_maxval)
    {
        *_maxval = f32_cmd2;
        mt_cf.pt_cp_cooling->u8_leadsensornumber = 2;
    }

    if(f32_cmd3 > *_maxval)
    {
        *_maxval = f32_cmd3;
        mt_cf.pt_cp_cooling->u8_leadsensornumber = 3;
    }
}

/** \brief Initialize AgvWork - update_coolingFanReversal Cooling Fan Control
 *
 *  This function is for the cooling fan REVERSE to PWM AgvWork - Cooling Fan Control Logic.
 *
 *  \param f32_forward_speed_target_pwm
 *
 */
void update_coolingFanReversal()
{
    uint8 u8_manual_purge_cmd = FALSE;

    uint8 u8_machine_in_motion = FALSE;
    float32 f32_wheel_speed = 0.0F;

    float32 f32_ramp_rate = CF_FWD_RUN_RAMP;

    uint32 u32_now_ms = get_system_time_ms();



    //manual purge command
    u8_manual_purge_cmd = (uint8)(*mt_cf.pu8_manual_purge_req);

    get_wheelSpeed(&f32_wheel_speed);
    u8_machine_in_motion = (f32_wheel_speed > 0.0F) ? TRUE : FALSE;


    // FR-7.5/7 manual purge request initiates cleanout cycle
    //If manual purge command is received and fan not in active cleanout
    if((u8_manual_purge_cmd == TRUE)            &&
       !mt_cf.u8_cleanout_active)
    {
        mt_cf.e_fanstate = CF_STATE_RAMP_DOWN;
        mt_cf.u8_cleanout_active = TRUE;
    }

    //TODO_STW: Check if CF_AUTO_CLEANOUT_DELAY_MS needs to be replaced with NVM Parameter
    // FR-7.6/7 automatic cleanout after preset forward time while machine is in motion
    else if(((u32_now_ms - mt_cf.u32_fwd_run_starttime) >= CF_AUTO_CLEANOUT_DELAY_MS) &&
              (u8_machine_in_motion)                                                           &&
              (!mt_cf.u8_cleanout_active))
    {
        mt_cf.e_fanstate = CF_STATE_RAMP_DOWN;
        mt_cf.u8_cleanout_active = TRUE;
    }

    //u32_elapsed_ms = u32_now_ms - mt_cooling_fan.u32_rev_state_start_ms;

    switch(mt_cf.e_fanstate)
    {
        case CF_STATE_RUN_FWD:
            //check if this is the first time in normal run state.
            if(mt_cf.e_prev_fanstate != mt_cf.e_fanstate)
            {
                mt_cf.u32_fwd_run_starttime = u32_now_ms;

                //set speed ramp value
                set_rampRate(&mt_cf.t_speed_ramp, CF_FWD_RUN_RAMP);

                mt_cf.e_prev_fanstate = mt_cf.e_fanstate;
            }

            //set speed command based off cooling demand
            mt_cf.f32_speed_cmd = (mt_cf.f32_cooling_demand * (CF_PWM_LOW_LIMIT - CF_PWM_HIGH_LIMIT) /
                                   CF_MAX_COOL_DEMAND) + CF_PWM_HIGH_LIMIT;

            mt_cf.u8_cleanout_active = FALSE;
            break;

        case CF_STATE_RAMP_DOWN:

            if(mt_cf.e_prev_fanstate == CF_STATE_RUN_FWD ||
               mt_cf.e_prev_fanstate == CF_STATE_RUN_REV )
            {
                mt_cf.u32_ramp_down_starttime = u32_now_ms;

                //Calculate the required ramp rate given the target speed
                f32_ramp_rate = (CF_PWM_HIGH_LIMIT - mt_cf.f32_speed_cmd) / (CF_RAMP_DOWN_TIME/1000.0F);
                set_rampRate(&mt_cf.t_speed_ramp, f32_ramp_rate);

                //set the target speed command to 0
                mt_cf.f32_speed_cmd = CF_PWM_HIGH_LIMIT;

                mt_cf.e_prev_fanstate = mt_cf.e_fanstate;
            }

            // if fan slowing down for 2.5S - go to next state
            if((u32_now_ms - mt_cf.u32_ramp_down_starttime) > CF_RAMP_DOWN_TIME)
            {
                mt_cf.e_prev_fanstate = mt_cf.e_fanstate;
                mt_cf.e_fanstate = CF_STATE_STOPPED;
                mt_cf.u32_stop_starttime = u32_now_ms;
            }
            break;

        case CF_STATE_STOPPED:

            if(mt_cf.e_prev_fanstate == CF_STATE_RAMP_DOWN)
            {
                mt_cf.u32_stop_starttime = u32_now_ms;

                //Switch the direction command of the fan when stopped.
                mt_cf.f32_dir_cmd = (mt_cf.f32_dir_cmd == CF_DIR_FORWARD)? CF_DIR_REVERSE: CF_DIR_FORWARD;
                mt_cf.e_prev_fanstate = mt_cf.e_fanstate;
            }

            // Fan stays stopped for 1.5s
            if((u32_now_ms - mt_cf.u32_stop_starttime) >= CF_STOPPED_TIME)
                mt_cf.e_fanstate = CF_STATE_RAMP_UP;
            break;

        case CF_STATE_RAMP_UP:

            if(mt_cf.e_prev_fanstate == CF_STATE_STOPPED)
            {
                //start the ramp up timer
                mt_cf.u32_ramp_up_starttime = u32_now_ms;

                //calculate speed command based on cooling demand
                mt_cf.f32_speed_cmd = (mt_cf.f32_cooling_demand * (CF_PWM_LOW_LIMIT - CF_PWM_HIGH_LIMIT) /
                                       CF_MAX_COOL_DEMAND) + CF_PWM_HIGH_LIMIT;

                //Calculate the required ramp rate given the target speed
                f32_ramp_rate = (CF_PWM_HIGH_LIMIT - mt_cf.f32_speed_cmd) / (CF_RAMP_UP_TIME/1000.0F);
                set_rampRate(&mt_cf.t_speed_ramp, f32_ramp_rate);

                mt_cf.e_prev_fanstate = mt_cf.e_fanstate;
            }

            // fan speeding up for 2.5S - go to next state
            if((u32_now_ms - mt_cf.u32_ramp_up_starttime) > CF_RAMP_DOWN_TIME)
            {
                //Move to Forward run state if driving the fan forward
                if(mt_cf.f32_dir_cmd == CF_DIR_FORWARD)
                    mt_cf.e_fanstate = CF_STATE_RUN_FWD;

                //Move to Reverse run state if driving the fan in reverse
                else if (mt_cf.f32_dir_cmd == CF_DIR_REVERSE)
                    mt_cf.e_fanstate = CF_STATE_RUN_REV;

            }
            break;

        case CF_STATE_RUN_REV:

            if(mt_cf.e_prev_fanstate == CF_STATE_RAMP_UP)
            {
                //start the ramp up timer
                mt_cf.u32_rev_run_starttime = u32_now_ms;
                mt_cf.e_prev_fanstate = mt_cf.e_fanstate;
            }

            //TODO_STW: Check if CF_REV_RUN_TIME needs to be replaced with NVM Parameter
            //reverse for 10 seconds then ramp back down
            if((u32_now_ms - mt_cf.u32_rev_run_starttime) > CF_REV_RUN_TIME)
                mt_cf.e_fanstate = CF_STATE_RAMP_DOWN;

            break;



        default:
            // IR-7.2 invalid sequence -> safe forward state
            set_fanSafeState();
            mt_cf.u8_sequence_fault = TRUE;

            break;
    }
}


//EOF
