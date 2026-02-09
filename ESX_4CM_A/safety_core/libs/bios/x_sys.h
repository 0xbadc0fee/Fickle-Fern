//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Header file for system domain
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SYS_H
#define X_SYS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_sys_types.h"
#include "x_sys_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! \brief shutdown callback function type
typedef sint16 (* PR_x_sys_close_callback)(const uint8 ou8_Requester, void * const opv_Parameter);

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Global Function Prototypes ------------------------------------------------------------------------------------ */

sint16 x_sys_close(void);
void x_sys_force_close(void);
sint16 x_sys_close_config(void * const opv_CallbackParameter, const PR_x_sys_close_callback opr_Callback,
                          const uint32 ou32_TimeoutUs);
uint8 x_sys_shutdown_ongoing(void);

sint16 x_sys_get_ignition(uint8 * const opu8_OnOff);
sint16 x_sys_get_info(const uint16 ou16_InfoId, uint32 * const opu32_Value);
sint16 x_sys_get_cpu_clock(const uint8 ou8_CoreId, uint32 * const opu32_Frequency);
void x_sys_get_init_error(T_x_sys_init_err * const opt_InitError);
void x_sys_get_lib_version(uint32 * const opu32_Value);
sint16 x_sys_get_temperature(const uint16 ou16_Channel, sint16 * const ops16_Value);
sint16 x_sys_get_wakeup_status(T_x_sys_wakeup_status * const opt_WakeupStatus);
sint16 x_sys_stay_alive(const uint8 ou8_OnOff);
uint32 x_sys_get_time_ms(void);
uint64 x_sys_get_time_us(void);
uint64 x_sys_get_timer(void);
sint16 x_sys_get_cpu_load(uint16 * const opu16_Performance);
sint16 x_sys_get_active_faults(uint32 * const opu32_Faults);
sint16 x_sys_get_sticky_faults(uint32 * const opu32_Faults);
sint16 x_sys_clear_sticky_faults(const uint32 ou32_Faults);

sint16 x_sys_get_system_voltage(const uint32 ou32_Voltage, sint32 * const ops32_Value);

sint16 x_sys_get_monitored_voltage_monitor_data(const uint32 ou32_Voltage,
                                                T_x_sys_monitored_voltage_measurements * const opt_MonitorData);
sint16 x_sys_get_monitored_voltage_monitor_setup(const uint32 ou32_Voltage,
                                                 T_x_sys_monitored_voltage_constants * const opt_MonitorSetup);
sint16 x_sys_monitored_voltage_get_active_faults(uint32 * const opu32_Faults);
sint16 x_sys_monitored_voltage_get_sticky_faults(uint32 * const opu32_Faults);
sint16 x_sys_monitored_voltage_clear_sticky_faults(const uint32 ou32_Faults);
void x_sys_get_hw_config(T_x_sys_hw_cfg * const opt_HwConfig);

sint16 x_sys_get_local_trap_info(T_x_sys_local_trap_info * const opt_Info);
void x_sys_clear_local_trap_info(void);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /* _X_SYS_H */
