//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Generic Serial Interface functions

   Header defines generic Serial Interface functions.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SER_H
#define X_SER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "stwtypes.h"
#include "x_ser_types.h"
#include "x_ser_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//! \brief Initialize the #T_x_ser_config structure with default values
#define X_SER_CONFIG_DEFAULT_INITIALIZER(Bitrate, RxBufSize, TxBufSize)  { \
      .u32_Bitrate = (Bitrate), \
      .u8_DataLen = X_SER_FRAME_LEN_8_BIT, \
      .u8_Parity = X_SER_PARITY_NONE, \
      .u8_Stopbits = 1, \
      .u16_RxBufsize = (RxBufSize), \
      .u16_TxBufsize = (TxBufSize), \
      .pv_RxCallbackParameter = 0, \
      .pr_RxEventCallback = NULL, \
      .pv_TxCallbackParameter = 0, \
      .pr_TxEventCallback = NULL, \
      .pv_ErrorCallbackParameter = 0, \
      .pr_ErrorCallback = NULL \
}

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_ser_init(const uint16 ou16_Channel, const T_x_ser_config * const opt_Config);
sint16 x_ser_set_com_cfg(const uint16 ou16_Channel, const T_x_ser_com_config * const opt_Config);
sint16 x_ser_clear_rx_buf(const uint16 ou16_Channel);
sint16 x_ser_clear_tx_buf(const uint16 ou16_Channel);
sint16 x_ser_get_rx_buf_count(const uint16 ou16_Channel, uint16 * const opu16_Count);
sint16 x_ser_get_tx_buf_count(const uint16 ou16_Channel, uint16 * const opu16_Count);
sint16 x_ser_read(const uint16 ou16_Channel, const uint16 ou16_Count, uint8 * const opu8_Data);
sint16 x_ser_write(const uint16 ou16_Channel, const uint16 ou16_Count, const uint8 * const opu8_Data);
sint16 x_ser_write_string(const uint16 ou16_Channel, const charn * const opcn_String);
sint16 x_ser_get_status(const uint16 ou16_Channel, T_x_ser_status * const opt_Status);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif //_X_SER_H
