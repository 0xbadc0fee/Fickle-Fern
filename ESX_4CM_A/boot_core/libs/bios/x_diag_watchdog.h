//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       diag watchdog user layer API
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_DIAG_WATCHDOG_H___
#define X_DIAG_WATCHDOG_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_DIAG_WDT_REACTION_RESET         0u //!< Perform an ECU reset on watchdog fault
#define X_DIAG_WDT_REACTION_NONE          2u //!< Perform no action on watchdog fault

//! Watchdog Status
#define X_DIAG_WDT_INACTIVE               0u //!< Watchdog is inactive
#define X_DIAG_WDT_WAITING                1u //!< Watchdog waiting for first service
#define X_DIAG_WDT_GOOD                   2u //!< Watchdog is served correctly
#define X_DIAG_WDT_UNDERFLOW              3u //!< Watchdog timer underflow (window watchdog)
#define X_DIAG_WDT_OVERFLOW               4u //!< Watchdog timer overflow
#define X_DIAG_WDT_WRONG_SIGNATURE        5u //!< Watchdog got wrong signature

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_diag_watchdog_init(const uint16 ou16_ServiceMin, const uint16 ou16_ServiceMax, const uint16 ou16_Reaction);
sint16 x_diag_watchdog_service(void);

sint16 x_diag_watchdog_get_status(uint8 * const opu8_Status);

sint16 x_diag_watchdog_flow_init(const uint16 ou16_ServiceMin, const uint16 ou16_ServiceMax, const uint16 ou16_Reaction,
                                 const uint32 * const opu32_Checksums, uint8 * const opu8_Seed);
sint16 x_diag_watchdog_flow_service(const uint32 ou32_Signature, uint8 * const opu8_Seed);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_DIAG_WATCHDOG_H___
