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
#include "moving_avg_filter.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define FILTER_MINDER_RAW_MIN                 (0.0F)
#define FILTER_MINDER_RAW_MAX                 (5000.0F)
#define FILTER_MINDER_FAULT_THRESHOLD         (4.0F)
#define FILTER_MINDER_SAFE_STATE                    (0.0F)
#define FILTER_MINDER_BUF_LEN                       (8u)
#define FILTER_MINDER_FILTER_SAFE_OUTPUT            (0.0F)
#define FILTER_MINDER_FILTER_SAMPLE_NO              (5u)
#define FILTER_MINDER_FILTER_SAMPLE_MS              (250u)
// Fuel
#define FUEL_RAW_MIN                       (0.0F)
#define FUEL_RAW_MAX                       (5000.0F)

#define FUEL_FAULT_THRESHOLD               (6.25F)
#define FUEL_LOW_SETPOINT                  (7.0F)
#define FUEL_LOW_DELAY_MS                  (5000u)
#define FUEL_SAFE_STATE                    (0.0F)
#define FUEL_BUF_LEN                       (8u)
#define FUEL_FILTER_SAFE_OUTPUT            (0.0F)
#define FUEL_FILTER_SAMPLE_NO              (7u)
#define FUEL_FILTER_SAMPLE_MS              (100u)

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
        float32 f32_chk_fuel_level_sensor;//!< Checkpoint
        float32 f32_chk_fuel_level_gauge_pct; //!<Checkpoint
        float32 f32_chk_filter_rest_pct;//!<Checkpoint
        float32 f32_chk_minder_gauge_pct;//!<Checkpoint
        uint8 u8_chk_service_filter_status;//!<Checkpoint
        uint8 u8_chk_door_open_status;//!<Checkpoint
        uint8 u8_chk_low_hyd_fluid_indicator;//!<Checkpoint
        uint8 u8_chk_brakes_engaged;//!<Checkpoint
        uint8 u8_chk_sw_major_revision;//!<Checkpoint
        uint8 u8_chk_sw_minor_revision;//!<Checkpoint
}T_ChkPoints_Mis;

/** \brief Configuration Structure - Miscellaneous Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to Miscellaneous Control
 */
typedef struct
{
        uint16 u16_fuel_high_deadband; //!<Configuration parameter for Fuel Level deadband
        uint16 u16_filter_rstn_max; //!<Configuration parameter for Filter Minder

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
        float32 *pf32_filter_minder_gauge_pct;
        float32 *pf32_filter_restriction_pct;
        uint8   *pu8_service_filter_status;

        float32 *pf32_fuel_level_sensor;
        float32 *pf32_fuel_level_gauge_pct;
        uint8   *pu8_low_fuel_status;

        uint8   *pu8_door_open_status;
        uint8   *pu8_low_hydraulic_fluid_indicator;
        uint8   *pu8_brakes_engaged;

        uint8   *pu8_sw_major_revision;
        uint8   *pu8_sw_minor_revision;

        //RX CAN Variables
        uint8   *pu8_clear_machine_faults_cmd;

        //Local Control Variables
        float32 f32_last_fuel_gauge;

        //Local Filter Variables
        T_MoveAvgFilter t_fuel_level_flt;
        float32 f32_fuel_level_buf[FUEL_BUF_LEN];//!<Moving Average Buffer
        uint32 u32_low_fuel_timer_start_ms;
        uint8 u8_low_fuel_timer_active;

        T_MoveAvgFilter t_minder_flt;
        float32 f32_minder_buf[FILTER_MINDER_BUF_LEN];//!<Moving Average Buffer
        uint8 u8_filter_max_reset_timer_active;
        uint32 u32_filter_max_reset_timer_start_ms;

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
