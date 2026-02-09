//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       External sensor supply functions
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_UEXT_H
#define X_UEXT_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_uext_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
//! \brief external sensor supply faults
#define X_UEXT_FAULT_VOLTAGE_TOO_LOW       (1uL << 0uL) //!< fault sensor supply voltage too low
#define X_UEXT_FAULT_VOLTAGE_TOO_HIGH      (1uL << 1uL) //!< fault sensor supply voltage too high
#define X_UEXT_FAULT_VOLTAGE_DEVIATION     (1uL << 2uL) /*!< fault deviation of redundant sensor supply voltage
                                                             measurement too high*/

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 x_uext_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);
sint16 x_uext_set_voltage_setpoint(const uint16 ou16_Channel, const sint32 os32_Voltage);
sint16 x_uext_get_voltage_setpoint(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_uext_get_voltage_feedback(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_uext_get_sticky_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_uext_clear_sticky_faults(const uint16 ou16_Channel, const uint32 ou32_Faults);
sint16 x_uext_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_uext_get_fault_health(const uint16 ou16_Channel, const uint32 ou32_Fault, uint32 * const opu32_Health);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
