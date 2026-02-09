//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief     Signals Safety-API for Inter-Core-Communication

   This module implements the application interface for the Signals of the Inter-Core-Communication
   for Safety Cores.

   \copyright   Copyright Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_ICC_SIG_H
#define  X_ICC_SIG_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_icc_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ICC_SIG_MAX_SIGNAL_NUM             0x00FFFFFFu //!< Signal number range 0...X_ICC_SIG_MAX_SIGNAL_NUM

#define X_ICC_SIG_OS_NOWAIT                  0xFFFFFFFFu //!< Timeout value for non-blocking operation
#define X_ICC_SIG_OS_INFINITE                         0u //!< Timeout value for infinite timeout

/* -- Types --------------------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Signature of signal RX Callback function.

   The signal RX callback is called on reception of a signal.

   \param[in]     ou32_Signal             Signal number
   \param[in]     opv_CallbackParameter   Callback parameter (specified at xs_icc_sig_register())
*/
//----------------------------------------------------------------------------------------------------------------------
// Match following prototype with PR_xb_icc_sig_signal_handler
typedef void (* PR_x_icc_sig_signal_handler)(const uint32 ou32_Signal, void * const opv_CallbackParameter);

//! Signal module status information.
typedef struct
{
   uint32 u32_RxQueueLen;      //!< Length of signal RX queue for this core
   uint32 u32_MaxNumRxSignals; //!< Maximum number of RX signals that can be installed on this core
   uint32 u32_NumRxSignals;    //!< Number of actually installed RX signals on this core
   uint8 u8_TxErrorCnt;        /*!< TX error indicator; incremented whenever a signal could not be delivered
                                 to any core, reset when reading status info */
} T_x_icc_sig_status;

//! Signal handle structure (Opaque definition).
typedef struct T_x_icc_sig_struct T_x_icc_sig;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_icc_sig_register(const uint32 ou32_Signal, const uint32 ou32_Mask,
                          const PR_x_icc_sig_signal_handler opr_Callback, void * const opv_CallbackParameter);

sint16 x_icc_sig_register_os(const uint32 ou32_Signal, const uint32 ou32_Mask, T_x_icc_sig ** const oppt_Handle);

sint16 x_icc_sig_wait_for_os(T_x_icc_sig * const opt_Handle, uint32 * const opu32_Signal, const uint32 ou32_TimeOut);

sint16 x_icc_sig_send(const uint32 ou32_Signal, const uint32 ou32_Cores, uint32 * const opu32_CoresAck);

sint16 x_icc_sig_get_status(T_x_icc_sig_status * const opt_Status);

sint16 x_icc_sig_poll_one(void);

#ifdef __cplusplus
}
#endif

#endif
