//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Intermediate development stage to keep x_diag.h/x_diag.c clean

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_DIAG_DEVELOP_H___
#define X_DIAG_DEVELOP_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "stwerrors.h"
#include "x_errlog_types.h"
#include "x_errlog_cputype.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_errlog_log_error(const uint16 ou16_Flags, const uint8 ou8_ErrorId, const uint16 ou16_Data0,
                          const uint32 ou32_Data1, const uint32 ou32_Data2, const uint32 ou32_Data3);

sint16 x_errlog_get_error(const uint16 ou16_Age, T_x_errlog_error * const opt_Error, T_x_errlog_info * const opt_Info);

uint16 x_errlog_get_size(void);
uint16 x_errlog_get_capacity(void);
sint16 x_errlog_clear(void);

uint16 x_errlog_get_cycle_counter(void);
uint32 x_errlog_get_sequence_counter(void);

sint16 x_errlog_enumerate_all(const uint32 ou32_CoreMask, const PR_x_errlog_enumeration_callback opr_Callback,
                              void * const opv_Argument);
sint16 x_errlog_enumerate_current(const uint32 ou32_CoreMask, const PR_x_errlog_enumeration_callback opr_Callback,
                                  void * const opv_Argument);
sint16 x_errlog_enumerate_startup(const uint32 ou32_CoreMask, const PR_x_errlog_enumeration_callback opr_Callback,
                                  void * const opv_Argument);
sint16 x_errlog_enumerate_last(const uint32 ou32_CoreMask, const PR_x_errlog_enumeration_callback opr_Callback,
                               void * const opv_Argument);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_ERRLOG_H___
