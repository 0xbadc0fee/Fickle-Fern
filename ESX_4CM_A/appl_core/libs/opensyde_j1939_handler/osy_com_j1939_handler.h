//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to openSYDE J1939 handler
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_J1939_HANDLERH
#define OSY_COM_J1939_HANDLERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

#define OSY_COM_PROTOCOL_TYPE_J1939  (3U)

#define OSY_COM_J1939_ACK_POSITIVE       (0U) // Positive Acknowledgment (ACK)
#define OSY_COM_J1939_ACK_NEGATIVE       (1U) // Negative Acknowledgment (NACK)
#define OSY_COM_J1939_ACK_ACCESS_DENIED  (2U) // Access denied Acknowledgment
#define OSY_COM_J1939_ACK_CANNOT_RESPOND (3U) // Cannot respond Acknowledgment

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

typedef void (* PR_osy_com_j1939_handler_request_received)(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                                           const uint8 ou8_DestinationAddress,
                                                           const uint32 ou32_RequestedPgn);
typedef void (* PR_osy_com_j1939_handler_ack_received)(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                                       const uint8 ou8_DestinationAddress, const uint8 ou8_AckType,
                                                       const uint32 ou32_RequestedPgn,
                                                       const uint8 ou8_GroupFunction);

///Callback type initialization structure
typedef struct
{
   PR_osy_com_j1939_handler_request_received pr_RequestPgReceivedEvent;
   PR_osy_com_j1939_handler_ack_received pr_AckReceivedEvent;
} T_osy_com_j1939_handler_callbacks;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

extern sint16 osy_com_j1939_handler_init(const T_osy_com_j1939_handler_callbacks * const opt_Callbacks);

extern sint16 osy_com_j1939_handler_send_ack(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                             const uint8 ou8_DestinationAddress, const uint8 ou8_AckType,
                                             const uint32 ou32_Pgn, const uint8 ou8_GroupFunction);

extern sint16 osy_com_j1939_handler_send_request_pgn(const uint8 ou8_CanChannel, const uint32 ou32_RequestedPgn,
                                                     const uint8 ou8_SourceAddress, const uint8 ou8_DestinationAddress);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
