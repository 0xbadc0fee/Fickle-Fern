//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to process data exchange stack engine functions
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_ENGINEH
#define OSY_COM_ENGINEH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_com_configuration.h"
#include "osy_com_trg_definitions.h" // target specific definitions

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

///message type
#define OSY_COM_MESSAGE_TYPE_RX (0U)
#define OSY_COM_MESSAGE_TYPE_TX (1U)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_com_engine_init(const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration,
                                  const T_osy_com_buffer_config * const opt_Buffers,
                                  const T_osy_com_trg_can_init_params * const opt_CanParameters);
extern sint16 osy_com_engine_start(const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration);
extern sint16 osy_com_engine_handle_tx(const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration);
extern sint16 osy_com_engine_handle_rx(const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration);
extern sint16 osy_com_engine_force_tx_message(const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration,
                                              const uint16 ou16_MessageIndex);
extern sint16 osy_com_engine_get_message_status(
   const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration, const uint16 ou16_MessageIndex,
   const uint8 ou8_MessageType, uint8 * const opu8_MessageStatus, uint8 * const opu8_ProtocolError);
extern sint16 osy_com_engine_get_mux_message_status(
   const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration, const uint16 ou16_MessageIndex,
   const uint16 ou16_MuxIndex, const uint8 ou8_MessageType, uint8 * const opu8_MessageStatus,
   uint8 * const opu8_ProtocolError);
extern sint16 osy_com_engine_reset_message_status(
   const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration, const uint16 ou16_MessageIndex,
   const uint8 ou8_MessageType);
extern sint16 osy_com_engine_reset_mux_message_status(
   const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration, const uint16 ou16_MessageIndex,
   const uint16 ou16_MuxIndex, const uint8 ou8_MessageType);
extern sint16 osy_com_engine_set_message_active(
   const T_osy_com_protocol_configuration * const opt_ProtocolConfiguration, const uint16 ou16_MessageIndex,
   const uint8 ou8_MessageType, const uint8 ou8_Status);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
