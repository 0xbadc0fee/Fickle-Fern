//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Feb 6, 2026 kyle.boch
 *
 * \file       system.h
 * \brief      Interface for System Module.
 *
 * \addtogroup HAL
 * @{
 * \addtogroup STW_4CM_HAL STW_4CM_HAL
 * @{
 */
#ifndef APPL_CORE_SRC_HAL_STW_4CM_HAL_SYSTEM_H_
#define APPL_CORE_SRC_HAL_STW_4CM_HAL_SYSTEM_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

//Ignition / Shutdown Information
sint16 get_ignition_status(uint8 * const opu8_Status);
sint16 system_keep_alive (const uint8 u8_OnOff);
uint8 get_system_reset_status(void);

//CPU Information
sint16 get_cpu_temperature(sint16 * const ops16_Temp);
sint16 get_cpu_load(uint16 * const opu16_Load);

//System Time
uint64 get_system_time_us(void);
uint32 get_system_time_ms(void);

//Controller Information
sint16 get_system_voltage(sint32 * const ops32_Voltage);

#endif /* APPL_CORE_SRC_HAL_STW_4CM_HAL_SYSTEM_H_ */

