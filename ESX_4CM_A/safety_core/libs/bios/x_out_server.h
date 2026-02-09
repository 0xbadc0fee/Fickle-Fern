//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Output remote core server
   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OUT_SERVER_H
#define X_OUT_SERVER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_msw_defs.h"
#include "x_out_defs.h"
#include "x_uext_defs.h"
#include "x_led_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 x_out_server_lock_allocations(void);
#if ((defined (X_OUT_COUNT)) && (X_OUT_COUNT > 0))
sint16 x_out_server_allocate_channel(const uint16 ou16_Channel);
sint16 x_out_server_allocate_channel_group(const uint16 ou16_Channel, const uint16 ou16_Size);
sint16 x_out_server_allocate_channel_redundant(const uint16 ou16_Channel);
sint16 x_out_server_allocate_channel_full_bridge(const uint16 ou16_Channel);
#endif
#if ((defined (X_MSW_COUNT)) && (X_MSW_COUNT > 0))
sint16 x_out_server_allocate_msw(const uint16 ou16_Channel);
#endif
#if ((defined (X_UEXT_COUNT)) && (X_UEXT_COUNT > 0))
sint16 x_out_server_allocate_uext(const uint16 ou16_Channel);
#endif
#if ((defined (X_LED_COUNT)) && (X_LED_COUNT > 0))
sint16 x_out_server_allocate_led(const uint16 ou16_Channel);
#endif

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_OUT_SERVER_H
