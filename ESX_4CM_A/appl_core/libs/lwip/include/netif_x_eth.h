//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Network interface driver for STW SEPI x_eth API
   \details     This header file provides functions of the driver that are required to be accessible from
                application level.
   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef NETIF_X_ETH_H
#define NETIF_X_ETH_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "x_eth.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define NETIF_HOSTNAME_LEN                32u
#define NETIF_THREAD_STACKSIZE            4096u

// Return values for RX callback:
#define NETIF_DUMP_FRAME                  0 // Dump the frame and free the pbuf
#define NETIF_FORWARD_FRAME               1 // Forward the frame to lwip stack
#define NETIF_PRESERVE_FRAME              2 // Don't forward the frame to lwip stack but don't free the pbuf
                                            // (This means we handle the frame in a custom protocol stack later)

// For T_netif_x_eth_link_config.u16_LinkMode
// Usually defined in x_eth; if not use this ones
#ifndef X_ETH_LINK_MODE_100MB_FULL_DUPLEX
#define X_ETH_LINK_MODE_10MB_HALF_DUPLEX     (0x1u)
#define X_ETH_LINK_MODE_10MB_FULL_DUPLEX     (0x2u)
#define X_ETH_LINK_MODE_100MB_HALF_DUPLEX    (0x4u)
#define X_ETH_LINK_MODE_100MB_FULL_DUPLEX    (0x8u)
#define X_ETH_LINK_MODE_1000MB_HALF_DUPLEX   (0x10u)
#define X_ETH_LINK_MODE_1000MB_FULL_DUPLEX   (0x20u)
#define X_ETH_LINK_MODE_ADVERTISE_ALL        (0x3Fu)
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef sint16 (* PR_netif_x_eth_RxCallback)(void * const opv_CallbackParameter, struct netif * const opt_NetIf,
                                             struct pbuf * const opt_PBuf, T_x_eth_ptp_time * const opt_TimeStamp);
typedef void (* PR_netif_x_eth_TxTimeStampCallback)(void * const opv_CallbackParameter, struct netif * const opt_NetIf,
                                                    const T_x_eth_ptp_time * const opt_TimeStamp,
                                                    const uint32 ou32_FrameIndex);
typedef struct
{
   charn acn_Hostname[NETIF_HOSTNAME_LEN + 1u];
   uint16 u16_Channel;
   uint8 u8_PollIf;
   uint8 au8_MacAddress[6];
} T_netif_x_eth_config;
typedef struct
{
   uint8 u8_AutoNeg;    // X_ON, X_OFF
   uint16 u16_LinkMode; // X_ETH_LINK_MODE_...
   uint32 u32_Feature;
} T_netif_x_eth_link_config;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
err_t netif_x_eth_init(struct netif * const opt_NetIf);
err_t netif_x_eth_poll(struct netif * const opt_NetIf);
void netif_x_eth_task(void * opv_Arg);
sint16 netif_x_eth_get_channel(const struct netif * const opt_NetIf, uint16 * const opu16_Channel);
sint16 netif_x_eth_get_handle(const struct netif * const opt_NetIf, T_x_eth_handle ** const oppt_EthHandle);
sint16 netif_x_eth_get_interface_from_handle(const T_x_eth_handle * const opt_EthHandle,
                                             struct netif ** const oppt_NetIf);
sint16 netif_x_eth_get_interface_from_channel(const uint16 ou16_Channel, struct netif ** const oppt_NetIf);
// netif_x_eth_get_interface() is deprecated, use netif_x_eth_get_interface_from_channel() instead:
sint16 netif_x_eth_get_interface(const uint16 ou16_Channel, struct netif ** const oppt_NetIf) DEPRECATED;
sint16 netif_x_eth_install_rx_callback(struct netif * const opt_NetIf, PR_netif_x_eth_RxCallback const opr_Callback,
                                       void * const opv_CallbackParameter);
sint16 netif_x_eth_install_tx_time_stamp_callback(struct netif * const opt_NetIf,
                                                  PR_netif_x_eth_TxTimeStampCallback const opr_Callback,
                                                  void * const opv_CallbackParameter);
sint16 netif_x_eth_send_frame_pbuf(struct netif * const opt_NetIf, struct pbuf * const opt_Pbuf,
                                   const uint8 ou8_TimeStamp, uint32 * const opu32_FrameIndex);
sint16 netif_x_eth_send_frame(struct netif * const opt_NetIf, const uint16 ou16_Length, const uint8 * const opu8_Data,
                              const uint8 ou8_TimeStamp, uint32 * const opu32_FrameIndex);
sint16 netif_x_eth_set_link_config(const uint16 ou16_Channel, const T_netif_x_eth_link_config * const opt_LinkConfig);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // NETIF_X_ETH_H
