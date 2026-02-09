//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       LIN API functions

   \copyright   Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_LIN_H
#define X_LIN_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"
#include "x_lin_types.h"
#include "x_lin_defs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
//! \brief Initialize the #T_x_lin_config structure with default values
#define X_LIN_CONFIG_DEFAULT_INITIALIZER(Bitrate, RequestBufSize, ResponseBufSize, ErrorBufSize)  { \
      .u16_Bitrate = (Bitrate), \
      .u16_RequestBufSize = (RequestBufSize), \
      .u16_ResponseBufSize = (ResponseBufSize), \
      .u16_ErrorBufSize = (ErrorBufSize), \
      .u32_RequestCallbackParameter = 0, \
      .pr_RequestCallback = NULL, \
      .u32_ResponseCallbackParameter = 0, \
      .pr_ResponseCallback = NULL, \
      .u32_ErrorCallbackParameter = 0, \
      .pr_ErrorCallback = NULL, \
      .u32_WakeupCallbackParameter = 0, \
      .pr_WakeupCallback = NULL \
}

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_lin_init(const uint16 ou16_Channel, const T_x_lin_config * const opt_Config);
sint16 x_lin_send_request(const uint16 ou16_Channel, T_x_lin_service * const opt_Request);
sint16 x_lin_get_response(const uint16 ou16_Channel, T_x_lin_service * const opt_Response);
sint16 x_lin_get_request_buf_count(const uint16 ou16_Channel, uint16 * const opu16_Count);
sint16 x_lin_get_response_buf_count(const uint16 ou16_Channel, uint16 * const opu16_Count);
sint16 x_lin_clear_request_buf(const uint16 ou16_Channel);
sint16 x_lin_clear_response_buf(const uint16 ou16_Channel);
sint16 x_lin_get_overflow_count(const uint16 ou16_Channel, uint8 * const opu8_Count);
sint16 x_lin_get_wakeup_state(const uint16 ou16_Channel, uint8 * const opu8_State);
sint16 x_lin_get_bus_state(const uint16 ou16_Channel, uint8 * const opu8_State);
sint16 x_lin_set_active_mode(const uint16 ou16_Channel);
sint16 x_lin_set_power_off_mode(const uint16 ou16_Channel);
sint16 x_lin_get_settings(const uint16 ou16_Channel, T_x_lin_config * const opt_Settings);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* X_LIN_H */
