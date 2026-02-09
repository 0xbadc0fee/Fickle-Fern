//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Generic Ethernet functions (Layer 2 API)
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_H
#define X_ETH_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"
#include "x_eth_types.h"
#include "x_eth_defs.h"
#include "x_eth_ptp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_eth_init(const uint16 ou16_Channel, const T_x_eth_config * const opt_Config,
                  T_x_eth_handle ** const oppt_Handle);

sint16 x_eth_send_frame(T_x_eth_handle * const opt_Handle, const uint16 ou16_Length, const uint8 * const opu8_Data,
                        const uint8 ou8_Flag, uint32 * const opu32_FrameIndex);

sint16 x_eth_get_frame(T_x_eth_handle * const opt_Handle, const uint16 ou16_BufSize, uint8 * const opu8_Data,
                       uint16 * const opu16_Length, uint8 * const opu8_FrameComplete,
                       T_x_eth_ptp_time * const opt_TimeStamp);

sint16 x_eth_get_status(T_x_eth_handle * const opt_Handle, T_x_eth_status * const opt_Status);

sint16 x_eth_get_rx_buf_count(T_x_eth_handle * const opt_Handle, uint16 * const opu16_Count);

sint16 x_eth_get_tx_buf_count(T_x_eth_handle * const opt_Handle, uint16 * const opu16_Count);

sint16 x_eth_get_tx_buf_space(T_x_eth_handle * const opt_Handle, uint16 * const opu16_Count);

sint16 x_eth_get_rx_frame_length(T_x_eth_handle * const opt_Handle, uint16 * const opu16_Length);

sint16 x_eth_get_config(T_x_eth_handle * const opt_Handle, T_x_eth_config * const opt_Config);

sint16 x_eth_get_handle(const uint16 ou16_Channel, T_x_eth_handle ** const oppt_Handle);

sint16 x_eth_add_mac_address_filter(T_x_eth_handle * const opt_Handle, const uint8 opu8_MacAddr[6],
                                    const uint8 ou8_Mask, const uint8 ou8_Type);

sint16 x_eth_del_mac_address_filter(T_x_eth_handle * const opt_Handle, const uint8 opu8_MacAddr[6],
                                    const uint8 ou8_Mask, const uint8 ou8_Type);

sint16 x_eth_add_hash_filter(T_x_eth_handle * const opt_Handle, const uint8 opu8_MacAddr[6]);

sint16 x_eth_del_hash_filter(T_x_eth_handle * const opt_Handle, const uint8 opu8_MacAddr[6]);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_ETH_H
