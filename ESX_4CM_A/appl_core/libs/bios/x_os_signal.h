//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      RTOS API - Signal object functions
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_SIGNAL_H
#define X_OS_SIGNAL_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_os_signal_init(const uint32 ou32_Signals, const uint16 ou16_SafetyLevel, T_x_os_signal ** const oppt_Signal);
sint16 x_os_signal_wait_for(T_x_os_signal * const opt_Signal, const uint32 ou32_SignalMask, const uint16 ou16_Mode,
                            uint32 * const opu32_Signals, const uint32 ou32_TimeOut);
sint16 x_os_signal_send(T_x_os_signal * const opt_Signal, const uint32 ou32_Signals, const uint32 ou32_SignalMask);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
