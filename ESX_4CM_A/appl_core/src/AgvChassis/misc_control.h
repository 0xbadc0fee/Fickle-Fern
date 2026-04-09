//-----------------------------------------------------------------------------
/**
 * \file       misc_control.h
 * \brief      AgvWork - Miscellaneous Control Implementation
 *
 * \addtogroup AgvWork
 * @{
 * \addtogroup MiscControl
 * @{
 */
/**
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
 * March 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------

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
#include "can_device_definition.h"
#include "moving_avg_filter.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define FILTER_MINDER_RAW_MIN               (0.0F)     //!< Minimum raw value for filter minder sensor
#define FILTER_MINDER_RAW_MAX               (5000.0F)  //!< Maximum raw value for filter minder sensor
#define FILTER_MINDER_FAULT_THRESHOLD       (4.0F)     //!< Threshold value to trigger a filter minder fault
#define FILTER_MINDER_SAFE_STATE            (0.0F)     //!< Safe fallback state for the filter minder
#define FILTER_MINDER_BUF_LEN               (8u)       //!< Buffer length for the filter moving average
#define FILTER_MINDER_FILTER_SAFE_OUTPUT    (0.0F)     //!< Safe output value for the filter function
#define FILTER_MINDER_FILTER_SAMPLE_NO      (5u)       //!< Number of samples required for the filter
#define FILTER_MINDER_FILTER_SAMPLE_MS      (250u)     //!< Sampling interval in milliseconds

#define FUEL_RAW_MIN                        (0.0F)     //!< Minimum raw value for fuel level sensor
#define FUEL_RAW_MAX                        (5000.0F)  //!< Maximum raw value for fuel level sensor
#define FUEL_FAULT_THRESHOLD                (6.25F)    //!< Threshold value to trigger a fuel sensor fault
#define FUEL_LOW_SETPOINT                   (7.0F)     //!< Value at which low fuel warning is triggered
#define FUEL_LOW_DELAY_MS                   (5000u)    //!< Delay in ms before triggering low fuel warning
#define FUEL_SAFE_STATE                     (0.0F)     //!< Safe fallback state for fuel level
#define FUEL_BUF_LEN                        (8u)       //!< Buffer length for the fuel filter moving average
#define FUEL_FILTER_SAFE_OUTPUT             (0.0F)     //!< Safe output value for the fuel filter function
#define FUEL_FILTER_SAMPLE_NO               (7u)       //!< Number of samples required for the fuel filter
#define FUEL_FILTER_SAMPLE_MS               (100u)     //!< Sampling interval in milliseconds

#define PERCENT_SCALE                       (100.0F)   //!< Standard multiplier for percentage scaling
#define PERCENT_SCALE_01PCT                 (10000.0F) //!< Multiplier for percentage scaling with 0.01% resolution

#define MISC_SW_MAJOR_REV                   (1u)       //!< Major software revision number
#define MISC_SW_MINOR_REV                   (0u)       //!< Minor software revision number
/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct ChkPoints_Mis
 * \brief Checkpoints Structure - Miscellaneous Control
 *
 * This structure represents all checkpoints that are relevant
 * to Miscellaneous Control
 */
typedef struct
{
        float32 f32_fuel_level_sensor;//!< Checkpoint Fuel Level Sensor
        float32 f32_fuel_level_gauge_pct; //!<Checkpoint Fuel Level Gauge
        float32 f32_filter_rest_pct;//!<Checkpoint Filter Rest Percent
        float32 f32_minder_gauge_pct;//!<Checkpoint Minder Gauge
        uint8 u8_service_filter_status;//!<Checkpoint Service Filter Status
        uint8 u8_door_open_status;//!<Checkpoint Door Open Status
        uint8 u8_low_hyd_fluid_indicator;//!<Checkpoint Hydraulic Fluid Low Indicator
        uint8 u8_brakes_engaged;//!<Checkpoint Brakes Engaged
        uint8 u8_sw_major_revision;//!<Checkpoint SW Major Revision
        uint8 u8_sw_minor_revision;//!<Checkpoint SW Minor Revision
}T_ChkPoints_Mis;

/**
 * \struct Config_MiscrControl
 * \brief Configuration Structure - Miscellaneous Control
 *
 * This structure represents all NVM configuration variables
 * that are relevant to Miscellaneous Control
 */
typedef struct
{
        uint16 u16_fuel_high_deadband; //!<Configuration parameter for Fuel Level deadband
        uint8  u8_filter_rstn_max; //!<Configuration parameter for Filter Minder

}T_Config_MiscrControl;

/**
 * \struct MiscControl
 * \brief Control Structure - Miscellaneous Control
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
        uint8 *pu8_filter_minder_gauge;  //!<Filter Minder Gauge 255
        float32 *pf32_filter_restriction_pct;//!<Filter Restriction Percentage
        uint8   *pu8_service_filter_status;//!<Service Filter Status

        uint8 *pu8_fuel_level_sensor;//!<Fuel Level Sensor 255
        float32 *pf32_fuel_level_gauge_pct;//!<TX Fuel Level Gauge Percent
        uint8   *pu8_low_fuel_status;//!<TX Low Fuel Status

        uint8   *pu8_door_open_status;//!<TX Door Open Status
        uint8   *pu8_low_hydraulic_fluid_indicator;//!<TX Low Hydrualic Fluid Indicator
        uint8   *pu8_brakes_engaged;//!<TX Brakes Engaged

        uint8   *pu8_sw_major_revision;//!<TX Software Major Revision
        uint8   *pu8_sw_minor_revision;//!<TX Software Minor Revision

        //RX CAN Variables
        uint8   *pu8_clear_faults_cmd;//!<RX Clear Machine Faults

        //Local Control Variables
        float32 f32_last_fuel_gauge;//!<Last Fuel Gauge

        //Local Filter Variables
        T_MoveAvgFilter t_fuel_level_flt;//!<Moving Average Filter Fuel Level
        float32 f32_fuel_level_buf[FUEL_BUF_LEN];//!<Moving Average Buffer
        uint32 u32_low_fuel_timer_start_ms;//!< Low Fuel timer
        uint8 u8_low_fuel_timer_active;//!< Low fuel timer active

        T_MoveAvgFilter t_minder_flt; //!<Moving Average Filter Filter Minder
        float32 f32_minder_buf[FILTER_MINDER_BUF_LEN];//!<Moving Average Buffer
        uint8 u8_minder_timer_active;//!<Filter Minder Timer Active
        uint32 u32_minder_timer_start_ms;//!<Filter Minder timer

        //NVM Configuration Parameters
        T_Config_MiscrControl *pt_nvm_misc_control;      //!<Header Control Configuration Structure

        //Control Checkpoints
        T_ChkPoints_Mis *pt_cp_misc; //!<Miscellaneous Control Checkpoints Structure

}T_MiscControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_miscControl(T_CANDevices *_can_devs, T_ChkPoints_Mis *_chk_misc, T_Config_MiscrControl *_nvm_misc_control);
sint16 update_miscControl(void);

#endif /* APPL_CORE_SRC_AGVCHASSIS_MISC_CONTROL_H_ */
