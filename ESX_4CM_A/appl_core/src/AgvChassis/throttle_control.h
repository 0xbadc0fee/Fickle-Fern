//-----------------------------------------------------------------------------
/**
 * \file       throttle_control.h
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

#ifndef APPL_CORE_SRC_AGVCHASSIS_THROTTLE_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_THROTTLE_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "can_device_definition.h"
#include "ramp_calc.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define THROTTLE_REQ_RPM_ZERO           (0.0F)      //!< Zero RPM request value
#define THROTTLE_DEFAULT_START_RPM      (100.0F)    //!< Default starting RPM value
#define THROTTLE_ENGINE_SPEED_MIN_RPM   (100.0F)    //!< Minimum allowable engine speed in RPM
#define THROTTLE_ENGINE_SPEED_MAX_RPM   (2350.0F)   //!< Maximum allowable engine speed in RPM

#define THROTTLE_RAMP_RATE              (500.0F)    //!< Ramping rate for engine speed changes
#define THROTTLE_ADJUST_RPM             (50.0F)     //!< RPM step size for incremental throttle adjustments

#define THROTTLE_HOLD_TIME_MS           (1000u)     //!< Button hold time threshold in milliseconds
#define THROTTLE_6S_DELAY_MS            (6000u)     //!< 6-second timeout/delay threshold in milliseconds

#define THROTTLE_DECREASE               (2u)        //!< Command indicator to decrease engine speed
#define THROTTLE_INCREASE               (5u)        //!< Command indicator to increase engine speed
#define THROTTLE_MAINTAIN               (0u)        //!< Command indicator to maintain current engine speed

/* -- Types -------------------------------------------------------------------------------------------------------- */

/**
 * \struct ChkPoints_Throttle
 * \brief Checkpoints Structure - Throttle Control
 *
 * This structure represents all checkpoints that are relevant
 * to throttle control.
 */
typedef struct
{
        uint16 u16_eng_spd;         //!<Checkpoint #1 Engine Speed RPM
        uint8  u8_eng_spd_up_osc;   //!<Checkpoint #2 Throttle UP
        uint8  u8_eng_spd_down_osc; //!<Checkpoint #3 Throttle Down

}T_ChkPoints_Throttle;

/**
 * \struct ThrottleControl
 * \brief Control Structure - Throttle Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for throttle control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        // TX CAN Variables
        uint8  *pu8_eng_ovrrd_ctrl_mode;          //!< Pointer to Engine Override Control Mode
        uint8  *pu8_engine_speed_ctrl_req;        //!< Pointer to Engine Speed Control Request
        uint8  *pu8_engine_override_ctrl_pri;     //!< Pointer to Engine Override Control Priority
        uint16 *pu16_engine_req_speed;            //!< Pointer to Engine Requested Speed
        uint8  *pu8_eng_req_torq_limit;           //!< Pointer to Engine Requested Torque Limit
        uint8  *pu8_ctrl_purpose;                 //!< Pointer to Control Purpose
        uint8  *pu8_eng_req_torq_hires;           //!< Pointer to High-Resolution Engine Requested Torque

        // Local Control Variables
        uint8   u8_throttle_cmd;                  //!< Current throttle command
        uint8   u8_prev_throttle_cmd;             //!< Previous throttle command

        uint32  u32_dwn_cmd_time;                 //!< Timestamp of throttle down command
        uint32  u32_up_cmd_time;                  //!< Timestamp of throttle up command

        uint8   u8_up_bump_ind;                   //!< Indicator for throttle up bump/step
        uint8   u8_down_bump_ind;                 //!< Indicator for throttle down bump/step

        float32 f32_target_req_rpm;               //!< Target requested engine speed in RPM

        uint32  u32_last_update_time_ms;          //!< Timestamp of the last update in milliseconds
        uint32  u32_last_step_time_ms;            //!< Timestamp of the last adjustment step in milliseconds

        uint8   u8_engine_status;                 //!< Current engine status
        uint8   u8_prev_engine_status;            //!< Previous engine status
        uint32  u32_engine_state_change_time_ms;  //!< Timestamp of the last engine state change in milliseconds

        T_ChkPoints_Throttle *pt_cp_throttle;     //!< Pointer to throttle checkpoints structure
} T_ThrottleControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_throttleControl(T_CANDevices *_can_devs, T_ChkPoints_Throttle *_chk_throttle);
sint16 update_throttleControl(void);

#endif /* APPL_CORE_SRC_AGVCHASSIS_THROTTLE_CONTROL_H_ */
