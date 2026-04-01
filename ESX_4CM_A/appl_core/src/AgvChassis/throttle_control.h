//-----------------------------------------------------------------------------
/*! \file       throttle_control.h
    \brief      The Throttle Control Module shall govern the vehicle's engine speed,
    measured as crankshaft rpm, by calculating new “Engine Requested Speed” values
    based on hardware input received from the operator and output via CAN based J1939
    speed control messages.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */

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
#define THROTTLE_REQ_RPM_ZERO               (0.0F)
#define THROTTLE_DEFAULT_START_RPM          (100.0F)
#define THROTTLE_RAMP_RATE                  (500.0F)
#define THROTTLE_ENGINE_SPEED_MIN_RPM       (100.0F)
#define THROTTLE_ENGINE_SPEED_MAX_RPM       (2350.0F)

#define THROTTLE_ADJUST_RPM                 (50.0F)

#define THROTTLE_HOLD_TIME_MS               (1000u)
#define THROTTLE_6S_DELAY_MS                (6000u)

#define THROTTLE_DECREASE   (2u)
#define THROTTLE_INCREASE   (5u)
#define THROTTLE_MAINTAIN   (0u)

/* -- Types -------------------------------------------------------------------------------------------------------- */
/** \brief Checkpoints Structure - Throttle Control
 *
 * This structure represents all checkpoints that are relevant
 * to throttle control
 */
typedef struct
{
        uint16   u16_chk1_eng_spd;   //!<Checkpoint #1 Engine Speed RPM
        uint8  u8_chk2_eng_spd_up_osc;  //!<Checkpoint #2 Throttle UP
        uint8 u8_chk3_eng_spd_down_osc;  //!<Checkpoint #3 Throttle Down

}T_ChkPoints_Throttle;

typedef struct
{
        //TX CAN Variables
        uint8 *pu8_eng_ovrrd_ctrl_mode;
        uint8 * pu8_engine_speed_ctrl_req;
        uint8 * pu8_engine_override_ctrl_pri;
        uint16 *pu16_engine_req_speed;
        uint8 *pu8_eng_req_torq_limit;
        uint8 *pu8_ctrl_purpose;
        uint8 *pu8_eng_req_torq_hires;

        //Local Control Variables
        uint8 u8_throttle_cmd;
        uint8 u8_prev_throttle_cmd;

        uint32 u32_dwn_cmd_time;
        uint32 u32_up_cmd_time;

        uint8 u8_up_bump_ind;
        uint8 u8_down_bump_ind;

        float32 f32_target_req_rpm;

        uint32 u32_last_update_time_ms;
        uint32 u32_last_step_time_ms;

        uint8 u8_engine_status;
        uint8 u8_prev_engine_status;
        uint32 u32_engine_state_change_time_ms;

        T_ChkPoints_Throttle* pt_cp_throttle;

} T_ThrottleControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_throttleControl(T_CANDevices *_can_devs, T_ChkPoints_Throttle *_chk_throttle);
sint16 update_throttleControl(void);

#endif /* APPL_CORE_SRC_AGVCHASSIS_THROTTLE_CONTROL_H_ */
