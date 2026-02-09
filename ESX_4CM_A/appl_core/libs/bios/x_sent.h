//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Single Edge Nibble Transmission (SENT) API
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SENT_H
#define X_SENT_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_sent_types.h"
#include "x_sent_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_sent_init(const uint16 ou16_Channel, const T_x_sent_config * const opt_Config);
sint16 x_sent_get_slow_channel_buf_count(const uint16 ou16_Channel, uint16 * const opu16_Count);
sint16 x_sent_get_status(const uint16 ou16_Channel, T_x_sent_status * const opt_Status);
sint16 x_sent_read_fast_channel(const uint16 ou16_Channel, T_x_sent_fast_channel_data * const opt_Data);
sint16 x_sent_read_slow_channel(const uint16 ou16_Channel, T_x_sent_slow_channel_data * const opt_Data);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /* X_SENT_H */
