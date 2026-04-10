//-----------------------------------------------------------------------------
/**
 * \file       suction_fan_control.h
 * \brief      AgvWork - Suction Fan Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup SuctionFanControl Suction Fan Control
 *
 * The Suction Fan Control Module shall regulate the PWM-controlled fan
 * output using operator speed requests and current suction fan RPM to ensure
 * consistent airflow and debris removal.
 *
 * @par Project
 * Flory_8772-4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Feb 24, 2026 Tiffany Gohnert
 *
 * @{
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVWORK_SUCTION_FAN_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_SUCTION_FAN_CONTROL_H_
/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "toggle_button.h"
#include "moving_avg_filter.h"
#include "pid_output.h"
#include "ramp_calc.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define DOOR_OPEN                       (1u)        //!< Indicator for door in OPEN state
#define DOOR_CLOSED                     (0u)        //!< Indicator for door in CLOSED state
#define ENG_ON                          (1u)        //!< Indicator for Engine ON state
#define ENG_OFF                         (0u)        //!< Indicator for Engine OFF state
#define SUCTION_FAN_ENABLED             (1u)        //!< Suction fan logic enabled
#define SUCTION_FAN_DISABLED            (0u)        //!< Suction fan logic disabled

#define SUCTION_FAN_SAFE_OUTPUT         (0.0F)      //!< Default safe PWM output
#define SUCTION_FAN_SAFE_SPEED_RPM      (0.0F)      //!< Default safe speed in RPM
#define SUCTION_FAN_CMD_MIN             (0.0F)      //!< Minimum operator command value
#define SUCTION_FAN_CMD_MAX             (1000.0F)   //!< Maximum operator command value
#define SUCTION_FAN_PWM_MIN             (0.0F)      //!< Minimum allowable PWM duty cycle
#define SUCTION_FAN_PWM_MAX             (10000.0F)  //!< Maximum allowable PWM duty cycle

#define SUCTION_FAN_RAMP_RATE           (50.0F)     //!< Rate of change for fan speed ramping
#define SUCTION_FAN_FILTER_BUF_LEN      (10u)       //!< Buffer length for fan speed filtering
#define SF_PPR                          (46.0F)     //!< Pulses Per Revolution for speed sensor scaling

/* -- Types -------------------------------------------------------------------------------------------------------- */
/**
 * \struct ChkPoints_SFan
 * \brief Checkpoints Structure - Suction Fan Control
 *
 * This structure represents all checkpoints that are relevant
 * to the operational monitoring of the suction fan system.
 */
typedef struct
{
    uint8   u8_suctionFanOn;   //!<Checkpoint #1
    sint16  u16_pwmStatus;  //!<Checkpoint #2
}T_ChkPoints_SFan;

/**
 * \struct Config_SFan
 * \brief Configuration Structure - Suction Fan Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to suction fan control and mechanical scaling.
 */
typedef struct
{
    uint8   u8_fan_dec_time;//!<Configuration parameter for Suction Fan Decrease Time Ramp
    uint8   u8_fan_inc_time;//!<Configuration parameter for Suction Fan Increase Time Ramp
    float32 f32_drive_ratio;//!<Configuration parameter for Suction Fan Drive Ratio
} T_Config_SFan;

/**
 * \struct SuctionFanControl
 * \brief Control Structure - Suction Fan Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for Suction Fan Control that need to
 * persist through cyclic calls (static).
 */
typedef struct
{
        //RX CAN Variables
    uint8   *pu8_enable_cmd;//!<RX Suction Fan On/Off command
    uint16 *pu16_speed_req_rpm;//!<RX Speed required RPM

    //TX CAN Variables
    uint8   *pu8_enable_status;//!<TX Suction Fan enable status
    uint16 *pu16_speed_status_rpm;//!<TX Suction Fan RPM status

    //Local Control Variables
    uint8   u8_enable_latched;//!<Button Latched
    uint8   u8_prev_engine_on;//!<Previous Engine On/Off status
    uint32  u32_eng_start_time_ms;//!<Engine Start time ms
    float32 af32_speed_buf[SUCTION_FAN_FILTER_BUF_LEN];//!<Speed average filter buffer

    float32 f32_fan_frequency;
    float32 f32_shaft_rpm;
    float32 f32_prev_req_rpm;

    // Control Blocks
    T_ToggleBtn        t_btn_enable;//!<Button enabled struct
    T_MoveAvgFilter    t_speed_filter;//!<Speed average filter struct
    T_RampState        t_speed_ramp;//!<Ramp struct

    //Control Checkpoints
    T_ChkPoints_SFan *pt_cp_sfan; //!<Suction Fan Control Checkpoints Structure
    //NVM Configuration Parameters
    T_Config_SFan *pt_nvm;//!<NVM Structure

    //PID Parameters
    T_PID_state        t_pid_state;//!<PID Structure
    T_PID_coeff t_pid_coeff;//!<PID Coefficient Structure

} T_SuctionFanControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_suctionFanControl(T_CANDevices *_can_devs, T_Config_SFan *_nvmSuctionFan, T_ChkPoints_SFan *_chkSuctionFan);
sint16 update_suctionFanControl(void);
void get_suctionFanStatus(uint8 *pu8_sfan_status);

#endif /* APPL_CORE_SRC_AGVWORK_SUCTION_FAN_CONTROL_H_ */
