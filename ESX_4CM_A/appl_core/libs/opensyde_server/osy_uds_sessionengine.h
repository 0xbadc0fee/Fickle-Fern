//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       UDS session handler
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_UDS_SESSIONENGINEH
#define OSY_UDS_SESSIONENGINEH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_uds_connectionhandler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef sint16 (* PR_uds_se_stop_transmissions)(const void * const opv_Connection);

//callback to check whether service is available in session
typedef uint8 (* PR_uds_se_check_permission)(const uint8 ou8_Session, const uint8 ou8_SaLevel,
                                             const uint8 * const opu8_Service, const uint16 ou16_ServiceLength);
//callback to check whether security level is applicable in session
typedef uint8 (* PR_uds_se_is_salevel_configured)(const uint8 ou8_Session, const uint8 ou8_SaLevel);

//----------------------------------------------------------------------------------------------------------------------

///Configuration structure for session engine
typedef struct
{
   const uint8 u8_NumberSessions;
   const uint8 * const pu8_SessionIds;                      ///< Array with u8_NumberSessions entries
   const PR_uds_se_stop_transmissions pr_StopTransmissions; ///< Function pointer to diagnostic server,
   // unused in flashloader
   const PR_uds_se_check_permission pr_CheckPermission;
   const PR_uds_se_is_salevel_configured pr_IsLevelConfigured;
} T_osy_uds_se_session_engine_configuration;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

// SessionEngine interface realization
extern sint16 osy_uds_se_init(const T_osy_uds_se_session_engine_configuration * const opt_Configuration);

extern void osy_uds_se_close(void);

extern sint16 osy_uds_se_enter_session(T_osy_uds_ch_connection_data * const opt_Connection, const uint8 ou8_SessionID);

extern sint16 osy_uds_se_leave_session(T_osy_uds_ch_connection_data * const opt_Connection);

extern sint16 osy_uds_se_get_current_session(const T_osy_uds_node_identifier * const opt_ClientIdentifier,
                                             uint8 * const opu8_SessionID);

extern sint16 osy_uds_se_get_current_sa_level(const T_osy_uds_node_identifier * const opt_ClientIdentifier,
                                              uint8 * const opu8_SaLevel);

// SecurityEngine interface realization
extern uint8 osy_uds_se_is_salevel_configured(const uint8 ou8_Session, const uint8 ou8_Level);

// PermissionEngine interface realization
extern sint16 osy_uds_se_check_permission(const T_osy_uds_ch_connection_data * const opt_Connection,
                                          const uint8 * const opu8_Service, const uint16 ou16_ServiceLength,
                                          uint8 * const opu8_Clearance);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
