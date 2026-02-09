//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       System Reset Functions
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SYS_RST_H
#define X_SYS_RST_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "x_stdtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/*! \brief  Reason for the last reset

   This structure is used for identification of the last reset which occurred.
*/
typedef struct
{
   uint8 u8_Core;          //!< Originating Core
   uint32 u32_Reason;      //!< Originating Software Module
   uint32 u32_SwReason;    //!< Information about a occurred software reset
   uint32 u32_Information; //!< Additional Information bitfield for use by the User
} T_x_sys_rst_reason;

//----------------------------------------------------------------------------------------------------------------------
/*! \brief Handler function to accept or reject system reset requests.

   Only on safety master core. When safety master core receives a reset request (from any core, including form itself)
   this function can decide whether or not the reset request shall be processed. This is controlled by the reset value
   of the function.

   When function accepts the reset request, it has to return C_NO_ERR. In this case a reset timer is started that will
   lead to a system reset within TBD milli seconds. Subsequently function "x_sys_rst_ongoing" will return 1 to indicate
   (to the application) that a reset is in progress.
   If the reset request shall be rejected a value different to C_NO_ERR (e.g. C_RD_WR) shall be returned.

   \note
   If no handler function is set (using "x_sys_rst_config") all reset request will be accepted per default.

   \param[in]     opv_CallbackParameter      User defined pointer, as given at "x_sys_rst_config"
   \param[in]     ou16_Requester             CORE ID of the CPU that requested the reset
   \param[in]     ou32_SwResetReason         Information about a occurred software reset

   \retval C_NO_ERR  reset request is accepted
   \retval otherwise reset is rejected
*/
//----------------------------------------------------------------------------------------------------------------------
typedef sint16 (* PR_x_sys_rst_OnRequest)(void * const opv_CallbackParameter, const uint16 ou16_Requester,
                                          const uint32 ou32_SwResetReason);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief Handler function to accept or reject system reset requests.

   Only on safety master core. When safety master core receives a reset request (from any core, including form itself)
   this function can decide whether or not the reset request shall be processed. This is controlled by the reset value
   of the function.

   When function accepts the reset request, it has to return C_NO_ERR. In this case a reset timer is started that will
   lead to a system reset within TBD milli seconds. Subsequently function "x_sys_rst_ongoing" will return 1 to indicate
   (to the application) that a reset is in progress.
   If the reset request shall be rejected a value different to C_NO_ERR (e.g. C_RD_WR) shall be returned.

   \note
   If no handler function is set (using "x_sys_rst_config") all reset request will be accepted per default.

   \param[in]  opv_CallbackParameter      User defined pointer, as given at "x_sys_rst_config"
   \param[in]  opt_ResetReason            Structure which shall contain:
                                          - u32_SwReason; Type of Software Reset which shall be executed,
                                                         range X_SYS_RST_SW_REASON_APPL0 - (X_SYS_RST_SW_REASON_COUNT-1)
                                          - u8_Core; Core which requested Reset,
                                                     range X_SYS_ID_CORE0 - (X_SYS_NUMBER_OF_CORES-1)
                                          - u32_Information; user definable information

   \retval C_NO_ERR  reset request is accepted
   \retval otherwise reset is rejected
*/
//----------------------------------------------------------------------------------------------------------------------
typedef sint16 (* PR_x_sys_rst_OnRequest_v2)(void * const opv_CallbackParameter,
                                             T_x_sys_rst_reason * const opt_ResetReason);

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_sys_rst_config(void * const opv_CallbackParameter, const PR_x_sys_rst_OnRequest opr_OnRequest); //master only
sint16 x_sys_rst_config_v2(void * const opv_CallbackParameter, const PR_x_sys_rst_OnRequest_v2 opr_OnRequest);
sint16 x_sys_rst_request(const uint32 ou32_SwResetReason);
sint16 x_sys_rst_request_v2(const uint32 ou32_SwResetReason, const uint32 ou32_Information);
void x_sys_rst_get_last_reason(T_x_sys_rst_reason * const opt_ResetReason);
uint8 x_sys_rst_ongoing(void);
sint16 x_sys_rst_force(const uint32 ou32_SwResetReason);                     //master only
sint16 x_sys_rst_force_v2(const T_x_sys_rst_reason * const opt_ResetReason); //master only

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
