//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Main switch functions
   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_MSW_H
#define X_MSW_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_msw_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
//! \brief main switch faults
#define X_MSW_FAULT_OVERVOLTAGE        (1uL << 0uL) //!< fault main switch limit for voltage exceeded
#define X_MSW_FAULT_OVERLOAD           (1uL << 1uL) //!< fault main switch limit for current exceeded
#define X_MSW_FAULT_DROP_TOO_LOW       (1uL << 2uL) //!< fault main switch voltage drop too low
#define X_MSW_FAULT_DROP_TOO_HIGH      (1uL << 3uL) //!< fault main switch voltage drop too high

//! \brief main switch safe state test results
#define X_MSW_SAFE_STATE_TEST_NOT_STARTED          (0u) //!< safe state test not performed yet
#define X_MSW_SAFE_STATE_TEST_NO_SUPPLY_VOLTAGE    (1u) //!< safe state test did not detect a valid supply voltage
#define X_MSW_SAFE_STATE_TEST_FAILED               (2u) //!< safe state test failed
#define X_MSW_SAFE_STATE_TEST_PASSED               (3u) //!< safe state test was successful

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 x_msw_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);
sint16 x_msw_set_state(const uint16 ou16_Channel, const uint8 ou8_State);
sint16 x_msw_get_state(const uint16 ou16_Channel, uint8 * const opu8_State);
sint16 x_msw_get_fsp_enable(const uint16 ou16_Channel, uint8 * const opu8_Value);
sint16 x_msw_get_current(const uint16 ou16_Channel, sint32 * const ops32_Current);
sint16 x_msw_get_voltage_before(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_msw_get_voltage_switched(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_msw_get_sticky_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_msw_clear_sticky_faults(const uint16 ou16_Channel, const uint32 ou32_Faults);
sint16 x_msw_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_msw_get_fault_health(const uint16 ou16_Channel, const uint32 ou32_Fault, uint32 * const opu32_Health);
sint16 x_msw_test_safe_state(void);
sint16 x_msw_get_safe_state_test_result(const uint16 ou16_Channel, uint8 * const opu8_Result);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
