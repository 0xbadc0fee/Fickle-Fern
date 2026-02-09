//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief   Message queue based CAN communication (Client side)

   This module implements methods to transfer CAN traffic from a physical CAN interface that is attached to one core
   to another core by using message queues (icc_mq).

   <b> Principal operation: </b> \n
   The core that is owning the physical CAN interface is called "Server".
   A core (other than the Server) that wants do CAN communication through this interface is called the "Client".

   CAN frames for the Client are received from the physical CAN interface by the Server.
   The Server passes the frames on to the Client via a message queue (icc_mq).
   The Client sends its CAN frames to the Server via a message queue. The Server passes
   the frames on to the physical CAN interface.

   CAN message object initialization is done in two steps:
   On the Server, all CAN specific settings are done (e.g. CAN filter settings) and a message queue for each CAN
   object is created.
   On the Client, the CAN object is initialized by attaching to the according message queue (assignment of objects
   between Server and Client is done via matching of the message queue identifier).

   Initialization and handling of the physical CAN interface (e.g. set bitrate, handle busoff...) shall only be done
   by the Server.

   This module contains functions that shall be used on the Client side.
   For functions for the Server side see icc_can_server in standard core.

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_CAN_CLIENT_H
#define  X_CAN_CLIENT_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_can_types_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct T_x_can_client_obj_struct T_x_can_client_obj;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_can_client_obj_init(const uint16 ou16_ObjId, const uint8 ou8_ObjType, T_x_can_client_obj ** const oppt_Handle);

sint16 x_can_client_obj_get_buf_count(T_x_can_client_obj * const opt_Handle, uint16 * const opu16_Count);

sint16 x_can_client_obj_send_msg(T_x_can_client_obj * const opt_Handle, const T_x_can_msg * const opt_Msg);

sint16 x_can_client_obj_get_msg(T_x_can_client_obj * const opt_Handle, T_x_can_msg * const opt_Msg);

sint16 x_can_client_obj_clear_buf(T_x_can_client_obj * const opt_Handle);

#ifdef __cplusplus
}
#endif

#endif /* X_CAN_CLIENT_H */
