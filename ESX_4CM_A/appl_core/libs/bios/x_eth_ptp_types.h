//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Generic Ethernet functions (Layer 2 API)
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_PTP_TYPES_H
#define X_ETH_PTP_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ETH_PTP_ADD_CLK_OFFSET             (0u) //!< Add Offset to PTP Clock Time
#define X_ETH_PTP_SUB_CLK_OFFSET             (1u) //!< Subtract Offset of PTP Clock Time

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! \brief Forward declaration of Ethernet interface handle to avoid circular include
/*STW Deviation Coding Rules v4*/
/*Violated Rule: No MISRA C:2012 or STW Rules applicable to message (additional QA C message)*/
/*Circumstances: SW Architecture*/
/*Reason: Couldn't find definition of T_x_eth_handle_struct*/
/*Potential risks: */
/*Prevention of risks: */
/*PRQA S 3313 1*/
typedef struct T_x_eth_handle_struct T_x_eth_handle;

//! \brief Ethernet frame time stamp information
typedef struct
{
   uint32 u32_Sec;
   uint32 u32_NanoSec;
} T_x_eth_ptp_time;

//! \brief Ethernet PTP alarm Callback
typedef void (* PR_x_eth_ptp_alarm_callback)(void * const opv_CallbackParameter, T_x_eth_handle * const opt_Handle);

//! \brief Deprecated type for T_x_eth_ptp_time
typedef T_x_eth_ptp_time T_x_eth_time_stamp DEPRECATED;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_PTP_TYPES_H
