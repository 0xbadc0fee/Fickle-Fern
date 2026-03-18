//-----------------------------------------------------------------------------
/*! \file       front_sweeps_control.h
    \brief      The Front Sweeps Control Module shall read the operator Drum V-Sweep
    commands and convert the requested Drum V-Sweep Speed to a PWM output to control
    how far the product is thrown into the throat of the pick-up belt.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Mar 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------

#ifndef APPL_CORE_SRC_AGVWORK_FRONT_SWEEPS_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_FRONT_SWEEPS_CONTROL_H_
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "ramp_calc.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define FRONT_SWEEPS_DISABLED           (0.0F)
#define FRONT_SWEEPS_PWM_SAFE_STATE     (0.0F)

#define FRONT_SWEEPS_MIN (0.0F)
#define FRONT_SWEEPS_MAX (10000.0F)

#define FRONT_SWEEPS_PWM_THRESHOLD_CURRENT (4000.0F)
#define FRONT_SWEEPS_PWM_END_CURRENT (18000.0F)

#define FRONT_SWEEPS_RAMP_RATE (10.0F)

/* -- Types -------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Front Sweeps Control
 *
 * This structure represents all checkpoints that are relevant
 * to shaft drive control
 */
typedef struct
{
        uint8 u8_checkpoint1; //!< CP DV IN

}T_ChkPoints_FSweeps;


typedef struct
{
        //Local Control Variables

        //RX CAN Variables
        uint8 *pu8_drum_speed_request;//!<Drum Speed Request
        uint8 *pu8_drum_speed_enable;//!<Drum Speed Enable

        //Control Checkpoints
        T_ChkPoints_FSweeps *pt_cp_frontsweeps; //!<Checkpoints Structure

        //Ramp variables
        T_RampState t_sweeps_ramp;//!<Front Sweeps Ramp Params

} T_FrontSweepsControl;

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
sint16 init_frontSweepsControl(T_UserInterface *_ui, T_ChkPoints_FSweeps *_chkFrontSweeps);
sint16 update_frontSweepsControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_FRONT_SWEEPS_CONTROL_H_ */
