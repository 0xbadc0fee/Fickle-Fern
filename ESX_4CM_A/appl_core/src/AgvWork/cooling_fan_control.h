//-----------------------------------------------------------------------------
/*! \file       cooling_fan_control.h
    \brief      The Cooling Fan Control Module controls two output valves (Speed Control Valve
    and Fan Direction Valve) to control the fan speed and direction.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVWORK_COOLING_FAN_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_COOLING_FAN_CONTROL_H_


/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "hmi_definition.h"
#include "ramp_calc.h"
#include "moving_avg_filter.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define COOLING_FAN_FORWARD                    (0u)
#define COOLING_FAN_REVERSE                    (1u)

#define COOLING_FAN_PWM_LOW_LIMIT              (4000.0F)
#define COOLING_FAN_PWM_HIGH_LIMIT             (10000.0F)

#define COOLING_FAN_PWM                        (10000.0F)
#define COOLING_FAN_PWM_OFF                    (0.0F)
#define COOLING_FAN_FAULT_SAFE_PWM             (8700.0F)

#define COOLING_FAN_DIR_FORWARD_CMD_PCT        (0.0F)
#define COOLING_FAN_DIR_REVERSE_CMD_PCT        (100.0F)

#define COOLING_FAN_MACHINE_IN_MOTION_RPM      (1.0F)
#define COOLING_FAN_PURGE_ACTIVE_MS            (10000u)
#define COOLING_FAN_AUTO_CLEANOUT_DELAY_MS     (600000u)

#define COOLING_FAN_SPEED_RAMP_RATE            (1000.0F)
#define COOLING_FAN_DIR_RAMP_RATE              (100.0F)

#define COOLING_FAN_MAX_TEMP_DEGC              (200.0F)

#define COOLING_FAN_T1_MIN_C                   (92.0F)
#define COOLING_FAN_T1_MAX_C                   (98.0F)
#define COOLING_FAN_T1_OVERTEMP_C              (190.0F)

#define COOLING_FAN_T2_MIN_C                   (38.0F)
#define COOLING_FAN_T2_MAX_C                   (65.0F)
#define COOLING_FAN_T2_OVERTEMP_C              (123.0F)

#define COOLING_FAN_T3_MIN_C                   (45.0F)
#define COOLING_FAN_T3_MAX_C                   (85.0F)
#define COOLING_FAN_T3_OVERTEMP_C              (96.0F) //88?

#define COOLING_FAN_HYD_BUF_LEN     (8u)
#define COOLING_FAN_HYD_FILTER_SAFE_OUTPUT (0.0F)
#define COOLING_FAN_HYD_FILTER_SAMPLE_NO   (5u)
#define COOLING_FAN_HYD_FILTER_SAMPLE_MS   (100u)

#define COOLING_FAN_RAMP_UP_MS                 (2500u)
#define COOLING_FAN_RAMP_DOWN_MS               (2500u)
#define COOLING_FAN_REVERSE_RUN_MS             (10000u)
#define COOLING_FAN_DIR_STOP_DELAY_MS          (1500u)

#define IGN_ON (1u)
#define IGN_OFF (0u)
/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Cooling Fan Control
 *
 * This structure represents Reverse Fan States
 */
typedef enum
{
    CF_REV_IDLE_FORWARD = 0u,//!<Fan moving forward at X speed
    CF_REV_RAMP_DOWN_TO_REV,//!<Fan slows down to 0 over 2.5s
    CF_REV_STOP_BEFORE_REV,//!<Fan stays stopped for 1.5s
    CF_REV_RAMP_UP_REV,//!<Fan ramps to target speed Y over 2.5s in reverse
    CF_REV_RUN_REV,//!< Fan stays at target speed Y for 10 seconds
    CF_REV_RAMP_DOWN_TO_FWD,//!<Fan slows down to 0 over 2.5s
    CF_REV_STOP_BEFORE_FWD,//!<Fan stays stopped for 1.5s
    CF_REV_RAMP_UP_FWD//!<Fan ramps to target speed X over 2.5s in forward
} E_CoolingFanRevState;

/** \brief Checkpoints Structure - Cooling Fan Control
 *
 * This structure represents all checkpoints that are relevant
 * to Cooling Fan Control
 */
typedef struct
{
        uint8 u8_leadsensornumber; //!<Lead Sensor Number Checkpoint
        float32 f32_cooling_demand_pct;//!<Cooling Demand Percentage Checkpoint

}T_ChkPoints_CoolingFan;


/** \brief Control Structure - Cooling Fan Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for cooling fan control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        uint32 u32_ign_start_time_ms;    //!<OS Start MS timer
        uint8 u8_prev_ign_on; //!<Previous IGN ON state
        uint8   u8_cleanout_active;//!<Cleanout Active
        uint8   u8_fan_direction;//!<Fan Direction
        uint8   u8_manual_purge_latched;//!<Manual Purge Latched
        uint8   u8_cooling_fault;//!<Cooling Demand Fault

        //TX CAN Variables
        uint16   *pu16_disp_hyd_oil_temp_degC;//!<Hydraulic Oil Temperature Degree C
        uint8   *pu8_disp_fan_reverse_ind;//!<Fan Reverse
        uint8   *pu8_disp_cooling_system_fault;//!<Cooling System Fault
        uint8   *pu8_disp_hyd_oil_overtemp;//!<Hydraulic Oil Temperature Over Temperature
        uint8   *pu8_disp_intake_overtemp;//!<Intake Over Temperature
        uint8   *pu8_disp_coolant_overtemp;//!<Engine Coolant Over Temperature

        //RX CAN Variables
        float32 *pf32_engine_coolant_temp_degC;//!<Engine Coolant Temperature RX
        float32 *pf32_intake_manifold_temp_degC;//!<Intake Manifold Temperature RX
        uint8 *pu8_manual_purge_req;//!<Manual Purge Request RX

        //Local Control Variables
        uint32  u32_forward_run_start_ms;//!<Fan Forward Timer
        uint32  u32_ign_on_start_ms;//!<Program Start Timer
        uint32  u32_rev_state_start_ms;//!<Fan Reverse Timer
        float32 f32_dir_cmd_target_pct;//!<Fan Reverse Speed Target

        E_CoolingFanRevState e_rev_state;//!<Fan Reversal States

        //Control Checkpoints
        T_ChkPoints_CoolingFan *pt_cp_cooling; //!<Cooling Fan Control Checkpoints Structure

        //Ramp variables
        T_RampState          t_speed_ramp;//!<Fan Speed Ramp Config
        T_RampState          t_dir_ramp;//!<Fan Reversal Direction Config
        T_MoveAvgFilter  t_hyd_oil_temp_filt;//!<Hydraulic Oil Temperature Filter Config
        float32 f32_hyd_buff[COOLING_FAN_HYD_BUF_LEN];//!<Moving Average Buffer

}T_CoolingFanControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_coolingFanControl(T_UserInterface *_ui, T_ChkPoints_CoolingFan *_chkCoolingFan);
sint16 update_CoolingFanControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_COOLING_FAN_CONTROL_H_ */
