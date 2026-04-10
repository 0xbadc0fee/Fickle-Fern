//-----------------------------------------------------------------------------
/**
 * \file       system.h
 * \brief      AgvCore - System Module
 *
 * \addtogroup HAL
 * @{
 * \addtogroup STW_4CM_HAL STW_4CM_HAL
 *
 * The System module serves as the central coordination hub for the
 * controller. It handles core initialization sequences, manages the
 * high-level state machine, and oversees routine top-level operations
 * required for the machine's primary functionality.
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
 * Feb 6, 2026 kyle.boch
 *
 * @{
 */
//-----------------------------------------------------------------------------
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

