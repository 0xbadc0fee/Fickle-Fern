//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to openSYDE J1939 DM1 module
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_J1939_DM1H
#define OSY_COM_J1939_DM1H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "osy_com_j1939_dm_generic.h"
#include "osy_com_j1939_trg.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

//Error types RX
#define J1939_DM1_ERR_RX_DUPLICATE_SQ_NR      (0U) // Packet was already handled
#define J1939_DM1_ERR_RX_WRONG_SEQ_NR         (1U) // Packet has incorrect/unexpected sequence number
#define J1939_DM1_ERR_RX_TIMEOUT_DT           (2U) // Timeout of 750ms between two DT messages exceeded

//Error types TX
#define J1939_DM1_ERR_TX_SEND_CM            (3U) // Something went wrong when sending Communication Management message
#define J1939_DM1_ERR_TX_SEND_SINGLE_FRAME  (4U) // Something went wrong when sending single message
#define J1939_DM1_ERR_TX_SEND_DT            (5U) // Something went wrong when sending DT message

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

// TX callback types
typedef void (* PR_osy_com_j1939_dm1_tx_prepare_send)(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress);

///Callback type initialization structure
typedef struct
{
   PR_osy_com_j1939_dm_rx_dtc_received pr_Dm1RxDtcReceived;  // Callback when a DTC is received
   PR_osy_com_j1939_dm_error pr_Dm1RxError;                  // Error callback for the BAM receiving process
   PR_osy_com_j1939_dm1_tx_prepare_send pr_Dm1TxPrepareSend; // Callback before a complete DM1 transfer begins
   PR_osy_com_j1939_dm_error pr_Dm1TxError;                  // Error callback for the BAM transmitting process
} T_osy_com_j1939_dm1_callbacks;

///configuration for one logical J1939 node with entire TP buffer
typedef struct
{
   T_osy_com_j1939_dm_logic_node_tx_config t_DmConfig;
   uint8 * const pu8_TpBuffer;                                    // Buffer for an entire DM1 transport
   uint8 au8_StateBuffer[OSY_COM_J1939_TRG_TX_STATE_BUFFER_SIZE]; // Internal buffer for the TX state for this
                                                                  // configuration
   const uint16 u16_TpBufferSize;                                 // Size of the TP buffer
   uint8 u8_Enabled;                                              // 1: DM1 transmission is enabled, 0: not enabled
} T_osy_com_j1939_dm1_appl_tx_config;

//----------------------------------------------------------------------------------------------------------------------
///this is the parameter passed to the osy_com_j1939_dm1_init_tx function
typedef struct
{
   T_osy_com_j1939_dm1_appl_tx_config * const pt_DmTxConfigs;
   const uint16 u16_NumConfigs; // Number of configurations equals number of logical J1939 nodes
} T_osy_com_j1939_dm1_appl_tx_configs;

//----------------------------------------------------------------------------------------------------------------------
///configuration for each RX source address combination. Source address is in case of RX the address of the other node
typedef struct
{
   const uint8 u8_SourceAddress;                                  // Source address of the other sender node
   uint8 au8_TpBuffer[6];                                         // TP Buffer size for holding one DTC (4 byte) and the
                                                                  // lamp information (2 byte)
   uint8 au8_StateBuffer[OSY_COM_J1939_TRG_RX_STATE_BUFFER_SIZE]; // Internal buffer for the RX state for this
                                                                  // configuration
} T_osy_com_j1939_dm1_appl_rx_config;

//----------------------------------------------------------------------------------------------------------------------
///this is the parameter passed to the osy_com_j1939_dm1_init_rx function
typedef struct
{
   T_osy_com_j1939_dm1_appl_rx_config * const pt_DmRxConfigs;
   const uint16 u16_NumConfigs;
} T_osy_com_j1939_dm1_appl_rx_configs;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

extern void osy_com_j1939_dm1_init_callbacks(const T_osy_com_j1939_dm1_callbacks * const opt_Callbacks);
extern sint16 osy_com_j1939_dm1_init_tx(const uint8 ou8_CanChannel,
                                        const T_osy_com_j1939_dm1_appl_tx_configs * const opt_DmTxConfigs);
extern sint16 osy_com_j1939_dm1_init_rx(const uint8 ou8_CanChannel,
                                        const T_osy_com_j1939_dm1_appl_rx_configs * const opt_DmRxConfigs);

extern sint16 osy_com_j1939_dm1_enable_tx_cyclic_sending(const uint8 ou8_CanChannel, const uint8 ou8_Dm1TxConfigIndex,
                                                         const uint8 ou8_Enabled);
extern sint16 osy_com_j1939_dm1_set_tx_cycle_time(const uint8 ou8_CanChannel, const uint8 ou8_Dm1TxConfigIndex,
                                                  const uint16 ou16_CycleTimeMs);

extern sint16 osy_com_j1939_dm1_lock_tx(const uint8 ou8_CanChannel, const uint16 ou16_Dm1TxConfigIndex);
extern sint16 osy_com_j1939_dm1_unlock_tx(const uint8 ou8_CanChannel, const uint16 ou16_Dm1TxConfigIndex);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
