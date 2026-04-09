//-----------------------------------------------------------------------------
/**
 * \file       system.c
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
/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_sys.h"
#include "x_sys_rst.h"
#include "stwerrors.h"
/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

//Ignition / Shutdown Information
/*! \brief Get the Ingition Pin (KL15) Status
 *
 *  Get the status of the KL15 pin and return corresponding
 *  success/error code.
 *
 *  \param opu8_Status Pointer to Response Variable
 *
 *  \retval C_NO_ERR
 *  \retval
 */
sint16 get_ignition_status(uint8 * const opu8_Status)
{
    return x_sys_get_ignition(opu8_Status);
}
sint16 system_keep_alive (const uint8 u8_OnOff)
{
    return x_sys_stay_alive(u8_OnOff);
}

uint8 get_system_reset_status(void)
{
    return x_sys_rst_ongoing();
}

//CPU Information
sint16 get_cpu_temperature(sint16 * const ops16_Temp)
{
    return x_sys_get_temperature(X_SYS_TEMP_CPU, ops16_Temp);
}
sint16 get_cpu_load(uint16 * const opu16_Load)
{
    return x_sys_get_cpu_load(opu16_Load);
}

//System Time
uint64 get_system_time_us(void)
{
    return x_sys_get_time_us();
}
uint32 get_system_time_ms(void)
{
    return x_sys_get_time_ms();
}

//Controller Information
sint16 get_system_voltage(sint32 * const ops32_Voltage)
{
    return x_sys_get_system_voltage(X_SYS_UF_VFB, ops32_Voltage);
}




//EOF
