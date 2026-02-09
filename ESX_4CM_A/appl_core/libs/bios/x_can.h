//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       MultiCAN API functions
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_CAN_H
#define X_CAN_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_can_types.h"
#include "x_can_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_can_bus_init(const uint16 ou16_Channel, const T_x_can_bus_config * const opt_Config);

sint16 x_can_bus_perform_busoff_recovery(const uint16 ou16_Channel);

sint16 x_can_bus_change_busoff_time(const uint16 ou16_Channel, const uint16 ou16_Time1, const uint16 ou16_Time2,
                                    const uint16 ou16_Time3, const uint16 ou16_Time4, const uint16 ou16_Time5,
                                    const uint16 ou16_WaitTime);

sint16 x_can_bus_get_status(const uint16 ou16_Channel, T_x_can_bus_status * const opt_Status);
sint16 x_can_bus_get_settings(const uint16 ou16_Channel, T_x_can_bus_settings * const opt_Settings);
sint16 x_can_bus_get_errors(const uint16 ou16_Channel, T_x_can_bus_errors * const opt_Errors);

sint16 x_can_obj_init(const uint16 ou16_Channel, const T_x_can_obj_config * const opt_Config,
                      T_x_can_obj ** const oppt_Handle);

sint16 x_can_obj_get_msg(T_x_can_obj * const opt_Handle, T_x_can_msg * const opt_Msg);

sint16 x_can_obj_send_msg(T_x_can_obj * const opt_Handle, const T_x_can_msg * const opt_Msg);

sint16 x_can_obj_get_buf_count(const T_x_can_obj * const opt_Handle, uint16 * const opu16_Count);

sint16 x_can_obj_clear_buf(T_x_can_obj * const opt_Handle);

sint16 x_can_obj_set_filter(T_x_can_obj * const opt_Handle, const T_x_can_id * const opt_CompareId,
                            const T_x_can_id * const opt_Mask);

sint16 x_can_obj_lock(T_x_can_obj * const opt_Handle, const uint8 ou8_OnOff);

sint16 x_can_obj_get_status(T_x_can_obj * const opt_Handle, T_x_can_obj_status * const opt_Status);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // _X_CAN_H
