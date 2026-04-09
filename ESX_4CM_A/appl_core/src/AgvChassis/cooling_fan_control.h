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
#include "ramp_calc.h"
#include "moving_avg_filter.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CF_DIR_FORWARD            (0.0F)
#define CF_DIR_REVERSE            (1.0F)

#define CF_PWM_LOW_LIMIT          (4000.0F)
#define CF_PWM_HIGH_LIMIT         (10000.0F)
#define CF_SAFE_PWM               (8700.0F)
#define CF_MAX_COOL_DEMAND        (100.0F)

#define CF_AUTO_CLEANOUT_DELAY_MS (600000u)
#define CF_FAULT_TEMP             (200.0F)

#define CF_COOLANT_MIN_C          (92.0F)
#define CF_COOLANT_MAX_C          (98.0F)
#define CF_COOLANT_OVERTEMP_C     (190.0F)

#define CF_INTAKE_MIN_C           (38.0F)
#define CF_INTAKE_MAX_C           (65.0F)
#define CF_INTAKE_OVERTEMP_C      (123.0F)

#define CF_HYDOIL_MIN_C           (45.0F)
#define CF_HYDOIL_MAX_C           (85.0F)
#define CF_HYDOIL_OVERTEMP_C      (96.0F)

#define CF_HYD_BUF_LEN            (8u)
#define CF_HYD_FILTER_SAMPLE_NO   (5u)
#define CF_HYD_FILTER_SAMPLE_MS   (100u)

#define CF_FWD_RUN_RAMP           (40.0F)

#define CF_RAMP_DOWN_TIME         (2500.0F)
#define CF_RAMP_UP_TIME           (2500.0F)
#define CF_STOPPED_TIME           (1500.0F)
#define CF_REV_RUN_TIME           (10000.0F)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Cooling Fan Control
 *
 * This structure represents Reverse Fan States
 */
typedef enum
{
    CF_STATE_RUN_FWD = 0u,        //!<Fan moving forward at X speed
    CF_STATE_RAMP_DOWN,   //!<Fan slows down to 0 over 2.5s
    CF_STATE_STOPPED,             //!<Fan stays stopped for 1.5s (both directions
    CF_STATE_RUN_REV,             //!< Fan stays at target speed Y for 10 seconds
    CF_STATE_RAMP_UP          //!<Fan ramps to target speed X over 2.5s in forward
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

/** \brief NVM Structure - Cooling Fan Control
 *
 */
typedef struct
{
    uint8  u8_purge_active_time;
    uint16 u16_auto_cycle_time;

}T_Config_CF;


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

        //TX CAN Variables
        uint16  *pu16_disp_hyd_oil_temp_degC;   //!<Hydraulic Oil Temperature Degree C
        uint8   *pu8_disp_fan_reverse_ind;      //!<Fan Reverse
        uint8   *pu8_disp_cooling_system_fault; //!<Cooling System Fault
        uint8   *pu8_disp_hyd_oil_overtemp;     //!<Hydraulic Oil Temperature Over Temperature
        uint8   *pu8_disp_intake_overtemp;      //!<Intake Over Temperature
        uint8   *pu8_disp_coolant_overtemp;     //!<Engine Coolant Over Temperature

        //RX CAN Variables
        uint8 *pu8_engine_coolant_temp_degC; //!<Engine Coolant Temperature RX
        uint8 *pu8_intake_manifold_temp_degC;//!<Intake Manifold Temperature RX
        uint8 *pu8_manual_purge_req;            //!<Manual Purge Request RX

        //Local Control Variables
        float32 f32_dir_cmd;         //!<Fan Direction Valve Output Command
        float32 f32_speed_cmd;       //!< Fan Speed Control Valve Output Command

        //Cooling Demand Variables
        float32 f32_hydoil_temp;        //!<Hydraulic Oil Temperature (deg C)
        float32 f32_intake_temp;        //!<Engine Intake Temperature (deg C)
        float32 f32_coolant_temp;       //!<Engine Coolant Temperature (deg C)

        float32 f32_hydoil_cmd;         //!< Hydraulic Oil Cooling Demand %
        float32 f32_intake_cmd;         //!< Engine Intake Cooling Demand %
        float32 f32_coolant_cmd;        //!< Engine Coolant Cooling Demand %

        float32 f32_cooling_demand;     //!< Maximum Cooling Demand %

        //Fan Reverse / State Variables
        E_CoolingFanRevState e_fanstate;       //!<Fan State
        E_CoolingFanRevState e_prev_fanstate;  //!<Previous Fan State

        uint8   u8_cleanout_active;            //!<Cleanout Active
        uint8   u8_sequence_fault;             //!<Fan Control Sequence Error variable

        uint32 u32_ramp_down_starttime;        //!<Start time of Ramp Down State
        uint32 u32_ramp_up_starttime;          //!<Start time of Ramp Up State
        uint32 u32_fwd_run_starttime;          //!<Start time of Run Forward State
        uint32 u32_rev_run_starttime;          //!<Start time of Run Reverse State
        uint32 u32_stop_starttime;             //!<Start time of Stopped State


        //Control Checkpoints
        T_ChkPoints_CoolingFan *pt_cp_cooling;  //!<Cooling Fan Control Checkpoints Structure

        //Ramp variables
        T_RampState          t_speed_ramp;      //!<Fan Speed Ramp Config
        T_MoveAvgFilter  t_hyd_oil_temp_filt;   //!<Hydraulic Oil Temperature Filter Config
        float32 f32_hyd_buff[CF_HYD_BUF_LEN];   //!<Moving Average Buffer

}T_CoolingFanControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_coolingFanControl(T_CANDevices *_can_devs, T_ChkPoints_CoolingFan *_chkCoolingFan);
sint16 update_CoolingFanControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_COOLING_FAN_CONTROL_H_ */
