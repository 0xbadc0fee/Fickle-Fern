//-----------------------------------------------------------------------------
/*! \file       rotary_trap_control.h
    \brief      The Rotary Trap Control Module shall read the operator request
    commands and convert the Rotary Trap Speed to a proportional PWM output to
    control the speed of the windrow feed.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
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
#define ROTARY_TRAP_DISABLED         (0.0F)
#define ROTARY_TRAP_PWM_SAFE_STATE   (0.0F)

#define ROTARY_TRAP_RANGE_LOW        (0u)
#define ROTARY_TRAP_RANGE_MED        (1u)
#define ROTARY_TRAP_RANGE_MAX        (2u)

#define ROTARY_TRAP_LOW_MIN          (900.0F)
#define ROTARY_TRAP_LOW_MAX          (7200.0F)

#define ROTARY_TRAP_MED_MIN          (3160.0F)
#define ROTARY_TRAP_MED_MAX          (7200.0F)

#define ROTARY_TRAP_MAX_MIN          (8485.0F)
#define ROTARY_TRAP_MAX_MAX          (10000.0F)

#define ROTARY_TRAP_PWM_MIN          (0.0F)
#define ROTARY_TRAP_PWM_MAX          (10000.0F)

#define ROTARY_TRAP_PWM_THRESHOLD_CURRENT (4000.0F)
#define ROTARY_TRAP_PWM_END_CURRENT (18000.0F)

#define ROTARY_TRAP_RAMP_RATE (10.0F)

// -- Types --------------------------------------------------------------------------------------------------------

/** \brief Checkpoints Structure - Rotary Trap Control
 *
 * This structure represents all checkpoints that are relevant
 * to shaft drive control
 */
typedef struct
{
        uint8 u8_checkpoint1; //!< CP TR IN

}T_ChkPoints_RTrap;

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
sint16 init_rotaryTrapControl(T_UserInterface *_ui, T_ChkPoints_RTrap *_chkRotaryTrap);
sint16 update_rotaryTrapControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_ROTARY_TRAP_CONTROL_H_ */
