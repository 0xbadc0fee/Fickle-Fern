//-----------------------------------------------------------------------------
/**
 * \file        hmi_8772_display.h
 * \brief       System - HMI 8772 Display Module
 *
 * \addtogroup System
 * @{
 * \addtogroup HmiDisplay HMI 8772 Display
 *
 * The HMI 8772 Display module manages the communication and data formatting
 * for the 8772 Human-Machine Interface panel.
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
 * Feb 4, 2026 kyle.boch
 *
 * @{
 */

#ifndef APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_
#define APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/** \brief HMI Device Structure - 8772 Display
 *
 * This structure represents all variables that are transmitted and
 * received from an 8772 Display.
 */
typedef struct
{
        //RX Variables
        uint8 u8_elevatorSpeedRequest;       //!<Requested Speed of Elevator
        uint8 u8_drum_speed_request;       //!<Requested Speed of Front Sweeps
        uint8 u8_drum_speed_enable;       //!<Requested Front Sweeps enabled
        uint8 u8_trap_speed_range; //!<Requested Rotary Trap Speed Range
        uint16 u16_sf_speed_req; //!<Requested Suction Fan Speed Request

        uint8 u8_speed_limit_enable;  //!< Flag to enable or disable the speed limit function
        uint8 u8_max_speed_set;       //!< Configured maximum speed setpoint or status
        uint8 u8_gear_select;         //!< Currently selected transmission gear
        uint8 u8_manual_purge_req;    //!< Request flag to trigger a manual system purge (e.g., fan reversal)

        //TX Variables
        uint8 u8_controllerVersionMinor;     //!<Controller Software Version - Minor
        uint8 u8_controllerVersionMajor;     //!<Controller Software Version - Major

        uint8 u8_elevatorStatus;             //!<Elevator Control On/Off Status

        uint8 u8_relief_switch_status;       //!<Header Lift/Lower Relief Switch Status
        uint8 u8_auger_status;

        uint8 u8_headlights_status; //!<Head Light Status
        uint8 u8_worklights_status; //!<Work Light Status
        uint8 u8_shaft_drive_status;         //!<Cleaning Chains Shaft Drive Status

        uint8 u8_stick_remover_status; //!<Stick Remover Status
        uint8 u8_speed_limit_set;                 //!< Speed limit set indicator flag
        uint8 u8_neutral_state;                   //!< Drive/transmission neutral state flag
        uint8 u8_wheel_speed_10;                  //!< Wheel speed measurement (scaled x10)

        uint8 u8_filter_minder_gauge;             //!< Filter minder gauge status or raw reading
        float32 f32_filter_restriction_pct;       //!< Filter restriction measurement in percentage
        uint8 u8_service_filter_status;           //!< Service filter maintenance warning flag
        uint8 u8_fuel_level_sensor;               //!< Fuel level sensor status or raw reading
        float32 f32_fuel_level_gauge_pct;         //!< Calculated fuel level percentage for gauge display
        uint8 u8_low_fuel_status;                 //!< Low fuel level warning indicator flag
        uint8 u8_door_open_status;                //!< Cabin/enclosure door open status flag
        uint8 u8_low_hydraulic_fluid_indicator;   //!< Low hydraulic fluid level warning flag
        uint8 u8_brakes_engaged_status;           //!< Brakes engaged or parking brake status flag
        uint8 u8_software_major_revision;         //!< Controller software major revision number
        uint8 u8_software_minor_revision;         //!< Controller software minor revision number
        uint8 u8_clear_faults_cmd;                //!< Command flag to clear active system faults

        uint8 u8_suction_fan_enable_status;//!<Suction Fan Enable Status
        uint16 u16_suction_fan_speed_status_rpm; //!<Suction Fan Speed Status

        uint8 u8_neutral_safe_status; //!<Joystick Neutral Safe Status

        uint8 u8_cooling_fan_reverse_ind;     //!< Cooling fan reverse operation indicator flag
        uint8 u8_cooling_system_fault;        //!< Cooling system general fault/error flag
        uint8 u8_hyd_oil_overtemp;            //!< Hydraulic oil over-temperature warning/fault flag
        uint8 u8_intake_manifold_overtemp;    //!< Intake manifold over-temperature warning/fault flag
        uint8 u8_engine_coolant_overtemp;     //!< Engine coolant over-temperature warning/fault flag
        uint16 u16_hyd_oil_temp_degC;         //!< Hydraulic oil temperature in degrees Celsius

}T_8772_Display;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_ */

