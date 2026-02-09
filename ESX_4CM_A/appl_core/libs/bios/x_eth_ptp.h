//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Generic Ethernet functions (Layer 2 API)
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_PTP_H
#define X_ETH_PTP_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"
#include "x_eth_ptp_types.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_eth_ptp_set_time(T_x_eth_handle * const opt_Handle, const T_x_eth_ptp_time * const opt_TimeStamp);

sint16 x_eth_ptp_set_clock_offset(T_x_eth_handle * const opt_Handle, const T_x_eth_ptp_time * const opt_TimeOffset,
                                  const uint8 ou8_AddSub);

sint16 x_eth_ptp_get_time(T_x_eth_handle * const opt_Handle, T_x_eth_ptp_time * const opt_TimeStamp);

sint16 x_eth_ptp_adjust_clock(T_x_eth_handle * const opt_Handle, const sint32 os32_Ppb);

sint16 x_eth_ptp_install_alarm_callback(T_x_eth_handle * const opt_Handle,
                                        const PR_x_eth_ptp_alarm_callback opr_Callback,
                                        void * const opv_CallbackParameter);

sint16 x_eth_ptp_set_alarm_time(T_x_eth_handle * const opt_Handle, const T_x_eth_ptp_time * const opt_TimeStamp);

sint16 x_eth_ptp_get_alarm_time(T_x_eth_handle * const opt_Handle, T_x_eth_ptp_time * const opt_TimeStamp);

sint16 x_eth_ptp_increment_alarm_time(T_x_eth_handle * const opt_Handle, const uint64 ou64_TimeIncrement);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_PTP_H
