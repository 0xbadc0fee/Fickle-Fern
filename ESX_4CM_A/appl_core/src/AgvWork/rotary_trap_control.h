//-----------------------------------------------------------------------------
/**
 * \file       rotary_trap_control.h
 * \brief      AgvWork - Rotary Trap Control
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup RotaryTrapControl Rotary Trap Control
 *
 * The Rotary Trap Control Module processes operator requests to regulate the
 * speed of the windrow feed. It converts commanded Rotary Trap speed into
 * proportional PWM outputs for precise hydraulic control.
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

#ifndef APPL_CORE_SRC_AGVWORK_ROTARY_TRAP_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_ROTARY_TRAP_CONTROL_H_

// -- Includes ------------------------------------------------------------------------------------------------------
//STD
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "ramp_calc.h"

// -- Defines ------------------------------------------------------------------------------------------------------

#define ROTARY_TRAP_DISABLED            (0.0F)      //!< Rotary trap disabled state value
#define ROTARY_TRAP_PWM_SAFE_STATE      (0.0F)      //!< Default safe PWM output value

#define ROTARY_TRAP_RANGE_LOW           (0u)        //!< Index for Low speed range
#define ROTARY_TRAP_RANGE_MED           (1u)        //!< Index for Medium speed range
#define ROTARY_TRAP_RANGE_MAX           (2u)        //!< Index for Maximum speed range

#define ROTARY_TRAP_LOW_MIN             (900.0F)    //!< Minimum current for Low range
#define ROTARY_TRAP_LOW_MAX             (7200.0F)   //!< Maximum current for Low range
#define ROTARY_TRAP_MED_MIN             (3160.0F)   //!< Minimum current for Medium range
#define ROTARY_TRAP_MED_MAX             (7200.0F)   //!< Maximum current for Medium range
#define ROTARY_TRAP_MAX_MIN             (8485.0F)   //!< Minimum current for Max range
#define ROTARY_TRAP_MAX_MAX             (10000.0F)  //!< Maximum current for Max range

#define ROTARY_TRAP_PWM_MIN             (0.0F)      //!< Absolute minimum PWM duty cycle
#define ROTARY_TRAP_PWM_MAX             (10000.0F)  //!< Absolute maximum PWM duty cycle
#define ROTARY_TRAP_PWM_THRESHOLD_CURRENT (4000.0F) //!< Threshold current for PWM activation
#define ROTARY_TRAP_PWM_END_CURRENT     (18000.0F)  //!< End current limit for PWM control

#define ROTARY_TRAP_RAMP_RATE           (10.0F)     //!< Current ramp rate (mA per second)
#define MAX_FIELD_GS                    (8.0F)      //!< Maximum Field Strength (Gauss)
#define MIN_FIELD_GS                    (0.0F)      //!< Minimum Field Strength (Gauss)

// -- Types --------------------------------------------------------------------------------------------------------

/**
 * \struct ChkPoints_RTrap
 * \brief Checkpoints Structure - Rotary Trap Control
 *
 * This structure represents all checkpoints that are relevant
 * to the operational monitoring of the rotary trap shaft drive system.
 */
typedef struct
{
        float32 f32_trap_target_cmd; //!< Checkpoint: Current target trap command value

}T_ChkPoints_RTrap;

/**
 * \struct RotaryTrapControl
 * \brief Control Structure - Rotary Trap Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for Rotary Trap Control that need to
 * persist through cyclic calls (static).
 */
typedef struct
{
        //Local Control Variables

        //RX CAN Variables
        uint8 *pu8_trap_speed_increase;//!<Trap Speed Increase
        uint8 *pu8_trap_speed_range;//!<Trap Speed Range

        //Control Checkpoints
        T_ChkPoints_RTrap *pt_cp_rotarytrap; //!<Checkpoints Structure

        //Ramp variables
        T_RampState t_trap_ramp;//!<Trap Speed Ramp Params

} T_RotaryTrapControl;

// -- Module Global Function Prototypes ----------------------------------------------------------------------------
sint16 init_rotaryTrapControl(T_CANDevices *_can_dev, T_ChkPoints_RTrap *_chkRotaryTrap);
sint16 update_rotaryTrapControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_ROTARY_TRAP_CONTROL_H_ */
