//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      RTOS API - Mailbox functions
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_MBOX_H
#define X_OS_MBOX_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_os_mbox_init(void * const opv_Message, const uint16 ou16_SafetyLevel, T_x_os_mbox ** const oppt_Mailbox);
sint16 x_os_mbox_get_message(T_x_os_mbox * const opt_Mailbox, void ** const oppv_Message, const uint32 ou32_Timeout);
sint16 x_os_mbox_send_message(T_x_os_mbox * const opt_Mailbox, void * const opv_Message, const uint16 ou16_Options);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
