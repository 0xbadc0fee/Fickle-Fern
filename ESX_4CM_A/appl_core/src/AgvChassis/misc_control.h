//-----------------------------------------------------------------------------
/*! \file       misc_control.h
    \brief      The Miscellaneous Control Module shall provide supporting logic for
    auxiliary features within the 8772 Harvester application.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 Tiffany.Gohnert
 */

#ifndef APPL_CORE_SRC_AGVCHASSIS_MISC_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_MISC_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define MISC_SAFE_PERCENT                     (0.0F)

#define FILTER_MINDER_RAW_MIN                 (0.0F)
#define FILTER_MINDER_RAW_MAX                 (100.0F)
#define FILTER_MINDER_RESTRICTION_MAX_PCT     (80.0F)

// Fuel
#define FUEL_RAW_MIN                       (1000.0F)
#define FUEL_RAW_MAX                       (9000.0F)
#define FUEL_FAULT_THRESHOLD               (625.0F)
#define FUEL_LOW_SETPOINT                  (700.0F)   // 7.00% in 0.01% units
#define FUEL_LOW_DELAY_MS                  (5000u)
#define FUEL_SAFE_STATE                     (0.0F)

// Scaling
#define PERCENT_SCALE                      (100.0F)
#define PERCENT_SCALE_01PCT                (10000.0F)

// SW Version
#define MISC_SW_MAJOR_REV                  (1u)
#define MISC_SW_MINOR_REV                  (0u)
/* -- Types --------------------------------------------------------------------------------------------------------- */


/** \brief Checkpoints Structure - Miscellaneous Control
 *
 * This structure represents all checkpoints that are relevant
 * to Miscellaneous Control
 */
typedef struct
{
        uint8 u8_chk1; //!<Checkpoint
        float32 f32_chk2;//!< Checkpoint

}T_ChkPoints_Mis;

/** \brief Configuration Structure - Miscellaneous Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to Miscellaneous Control
 */
typedef struct
{
    uint8 u16_fuel_high_deadband; //!<Configuration parameter for
    uint8 u16_filter_rstn_max; //!<Configuration parameter for

}T_Config_MiscrControl;

/** \brief Control Structure - Miscellaneous Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for Miscellaneous Control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //TX CAN Variables
        // CAN outputs
        float32 *pf32_filter_minder_gauge_pct;
        float32 *pf32_filter_restriction_pct;
        uint8   *pu8_service_filter_status;

        float32 *pf32_fuel_level_sensor_pct;
        float32 *pf32_fuel_level_gauge_pct;
        uint8   *pu8_low_fuel_status;

        uint8   *pu8_door_open_status;
        uint8   *pu8_low_hydraulic_fluid_indicator;
        uint8   *pu8_display_brakes_engaged;

        uint8   *pu8_sw_major_revision;
        uint8   *pu8_sw_minor_revision;

        uint8   *pu8_clear_machine_faults_cmd;
        //RX CAN Variables
          uint8   *pu8_clear_machine_faults_cmd;
        //Local Control Variables
          float32  f32_last_fuel_gauge_pct;
            float32 f32_last_fuel_gauge;
            T_LowPassFilter t_fuel_level_lpf;
            uint32 u32_low_fuel_timer_start_ms;
            uint8 u8_low_fuel_timer_active;

          //NVM Configuration Parameters
          T_Config_MiscrControl *pt_nvm_misc_control;      //!<Header Control Configuration Structure

        //Control Checkpoints
        T_ChkPoints_Mis *pt_cp_misc; //!<Miscellaneous Control Checkpoints Structure

}T_MiscControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_miscControl(T_UserInterface *_ui,T_ChkPoints_Mis *_chk_misc,T_Config_MiscrControl *_nvm_misc_control);
sint16 update_miscControl(void);

#endif /* APPL_CORE_SRC_AGVCHASSIS_MISC_CONTROL_H_ */
