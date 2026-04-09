//-----------------------------------------------------------------------------
/* * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Mar 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------
/**
 * \file       front_sweeps_control.h
 * \brief      Interface for Front Sweeps Control Module.
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup FrontSweepsControl Front Sweeps Control
 * @{
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

#define FRONT_SWEEPS_DISABLED              (0.0F)     /**< Command value for disabled state */
#define FRONT_SWEEPS_PWM_SAFE_STATE        (0.0F)     /**< Default PWM safety output (0%) */

#define FRONT_SWEEPS_MIN                   (0.0F)     /**< Minimum operational speed command */
#define FRONT_SWEEPS_MAX                   (10000.0F) /**< Maximum operational speed command */

#define FRONT_SWEEPS_PWM_THRESHOLD_CURRENT (4000.0F)  /**< Starting current threshold [mA] */
#define FRONT_SWEEPS_PWM_END_CURRENT       (18000.0F) /**< Maximum allowable current [mA] */

#define FRONT_SWEEPS_RAMP_RATE             (1000.0F)  /**< Speed adjustment ramp rate [units/sec] */


/* -- Types -------------------------------------------------------------------------------------------------------- */

/**
 * @struct T_ChkPoints_FSweeps
 * \brief Checkpoints Structure - Front Sweeps Control
 *
 * Encapsulates the state-tracking checkpoints required for the Front Sweeps
 * control logic. These members are utilized to validate the sequencing of
 * Drum V-Sweep commands and safety interlocks.
 */
typedef struct
{
        uint8 u8_speed_cmd; //!< CP DV IN

}T_ChkPoints_FSweeps;

/**
 * @struct T_FrontSweepsControl
 * \brief Configuration Structure - Front Sweeps Control
 *
 * Encapsulates the non-volatile memory (NVM) configuration variables for
 * the Front Sweeps module. Defines PWM thresholds, operational limits,
 * and trajectory parameters for the Drum V-Sweep system.
 */
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
sint16 init_frontSweepsControl(T_CANDevices *_can_devs, T_ChkPoints_FSweeps *_chkFrontSweeps);
sint16 update_frontSweepsControl(void);

#endif /* APPL_CORE_SRC_AGVWORK_FRONT_SWEEPS_CONTROL_H_ */
