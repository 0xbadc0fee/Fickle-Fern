//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Types and constants for generic Ethernet API
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_TYPES_H
#define X_ETH_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "x_eth_phy_types.h"
#include "x_eth_ptp_types.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ETH_FRAME_INCOMPLETE               (0u) //!< Ethernet Frame not complete
#define X_ETH_FRAME_COMPLETE                 (1u) //!< Ethernet Frame complete
#define X_ETH_TIMESTAMP                      (2u) //!< Ethernet Time stamp

#define X_ETH_MAC_FILTER_DEST_ADDR           (0u) //!< apply MAC address filter to a packet's destination address
#define X_ETH_MAC_FILTER_SOURCE_ADDR         (1u) //!< apply MAC address filter to a packet's source address

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! \brief Ethernet Rx Callback Function
typedef void (* PR_x_eth_rx_callback)(void * const opv_CallbackParameter, T_x_eth_handle * const opt_Handle);

//! \brief Ethernet Tx Callback Function
typedef void (* PR_x_eth_tx_callback)(void * const opv_CallbackParameter, T_x_eth_handle * const opt_Handle,
                                      const uint32 * const opu32_FrameIndex);
//! \brief Ethernet Link Callback Function
typedef void (* PR_x_eth_link_callback)(void * const opv_CallbackParameter, T_x_eth_handle * const opt_Handle,
                                        const uint8 ou8_LinkStatus);
//! \brief Ethernet Tx Time stamp Callback
typedef void (* PR_x_eth_tx_time_stamp_callback)(void * const opv_CallbackParameter, T_x_eth_handle * const opt_Handle,
                                                 const T_x_eth_ptp_time * const opt_TimeStamp,
                                                 const uint32 * const opu32_FrameIndex);

/*! \brief Ethernet interface configuration data */
typedef struct
{
   uint8 u8_AutoNeg;                                       //!< Auto-negotiation ON/OFF (X_ON, X_OFF)
   uint16 u16_LinkMode;                                    /*!< Requested link speed and duplex mode
                                                                (X_ETH_LINK_MODE_...) */
   uint32 u32_Feature;                                     //!< Additional configuration option
   uint8 au8_MacAddr[6];                                   //!< Interface MAC address, (0: Use built-in MAC)
   uint16 u16_MaxFrameLen;                                 /*!< Max. transmission unit (incl. header and FCS), (0: use
                                                                default=1518) */
   PR_x_eth_rx_callback pr_RxCallback;                     //!< Data RX callback
   void * pv_RxCallbackParameter;                          //!< Callback parameter for RX callback
   PR_x_eth_tx_callback pr_TxCallback;                     //!< Data TX callback
   void * pv_TxCallbackParameter;                          //!< Callback parameter for TX callback
   PR_x_eth_link_callback pr_LinkCallback;                 //!< Link change notification callback
   void * pv_LinkCallbackParameter;                        //!< Callback parameter for link change notification callback
   PR_x_eth_tx_time_stamp_callback pr_TxTimeStampCallback; //!< TX time stamp callback
   void * pv_TxTimeStampCallbackParameter;                 //!< Callback parameter for TX time stamp callback
} T_x_eth_config;

/*! \brief Ethernet interface status information */
typedef struct
{
   uint8 u8_LinkStatus;   //!< Link status (X_ETH_LINK_UP, X_ETH_LINK_DOWN)
   uint16 u16_LinkMode;   //!< Requested link speed and duplex mode (X_ETH_LINK_MODE_...)
   uint16 u16_RxPending;  //!< Number of received Ethernet frames ready for reading
   uint16 u16_RxOverflow; //!< Number of dumped RX Ethernet frames (due to buffer overflow)
   uint16 u16_RxError;    //!< Number of corrupted RX Ethernet frames
   uint16 u16_TxPending;  //!< Number of Ethernet frames ready for sending
} T_x_eth_status;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_TYPES_H
