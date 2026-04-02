/*! \file       hmi_8772_display.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 4, 2026 kyle.boch
   	\endimplementation
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

   uint8 u8_manual_purge_req;

   //TX Variables
   uint8 u8_controllerVersionMinor;     //!<Controller Software Version - Minor
   uint8 u8_controllerVersionMajor;     //!<Controller Software Version - Major

   uint8 u8_elevatorStatus;             //!<Elevator Control On/Off Status

   uint8 u8_relief_switch_status;       //!<Header Lift/Lower Relief Switch Status
   uint8 u8_auger_status;

   uint8 u8_headlights_status; //!<Head Light Status
   uint8 u8_worklights_status; //!<Work Light Status
   uint8 u8_shaft_drive_status;         //!<Cleaning Chains Shaft Drive Status

   uint8 u8_cooling_fan_reverse_ind;//!<
   uint8 u8_cooling_system_fault;//!<
   uint8 u8_hyd_oil_overtemp;//!<
   uint8 u8_intake_manifold_overtemp;//!<
   uint8 u8_engine_coolant_overtemp;//!<
   uint16 u16_hyd_oil_temp_degC;//!<

   uint8 u8_filter_minder_gauge;//!<
   float32 f32_filter_restriction_pct;//!<
   uint8 u8_service_filter_status;//!<
   uint8 u8_fuel_level_sensor;//!<
   float32 f32_fuel_level_gauge_pct;//!<
   uint8 u8_low_fuel_status;//!<
   uint8 u8_door_open_status;//!<
   uint8 u8_low_hydraulic_fluid_indicator;//!<
   uint8 u8_brakes_engaged_status;//!<
   uint8 u8_software_major_revision;//!<
   uint8 u8_software_minor_revision;//!<
   uint8 u8_clear_machine_faults_cmd;//!<


}T_8772_Display;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_AGVHMI_DEVICES_HMI_8772_DISPLAY_H_ */

