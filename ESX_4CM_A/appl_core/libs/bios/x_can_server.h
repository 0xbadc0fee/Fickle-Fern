//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief   Message queue based CAN communication (Server side)

   This module implements the Server side methods to transfer CAN traffic from a physical CAN interface that is
   attached to one core to a other core by using message queues (icc_mq).

   For a detailed description see x_can_client().

   \copyright   Copyright Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_CAN_SERVER_H
#define  X_CAN_SERVER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_can_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_CAN_SERVER_MODE_POLLING         1u //!< Do message handling on Server in task function (icc_can_task_server)
#define X_CAN_SERVER_MODE_EVENT           2u //!< Do message handling on Server event driven by CAN/MQ callbacks
#define X_CAN_SERVER_MODE_LOCAL_ECHO      4u /*!< Received messages can be read on client side AND server side
                                                  (for RX objects).
                                                  Messages sent by client can be read on server side (for TX objects).
                                                  Option only allowed in combination with X_CAN_SERVER_MODE_EVENT and
                                                  for u8_ObjType==X_CAN_OBJ_TYPE_RX or u8_ObjType==X_CAN_OBJ_TYPE_TX */

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! CAN server object handle structure (Opaque definition)
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: No definition for structure*/
/*Reason: Dir 4.8 The pointer to the structure is never dereferenced, thus implementation of the object is hidden*/
/*Potential risks: none*/
/*Prevention of risks:  opaque type, nothing to do*/
/*PRQA S 3313 1*/
typedef struct T_x_can_server_obj_struct T_x_can_server_obj;

//! CAN server object configuration structure
typedef struct
{
   uint16 u16_ObjId;         //!< Unique object id (0...0xFFFF)
   T_x_can_id t_FilterId;    //!< CAN filter identifier
   T_x_can_id t_FilterMask;  //!< CAN filter mask
   uint8 u8_ObjType;         /*!< X_CAN_OBJ_TYPE_RX: Receive from CAN bus, forward to ICC CAN client
                                  X_CAN_OBJ_TYPE_TX: Receive from ICC CAN client, forward to CAN bus
                                  X_CAN_OBJ_TYPE_TX_RTR: Receive from ICC CAN client, forward to CAN bus
                                                         (send on RTR reception)
                                  X_CAN_OBJ_TYPE_RTR_SENDER: Receive RTR frame from ICC CAN client, forward to CAN bus
                                  X_CAN_OBJ_TYPE_RTR_RECEIVER: Receive RTR frame from CAN bus, forward to
                                                               ICC CAN client */
   uint16 u16_Buffer;        //!< Size of RX/TX buffer (number of CAN messages)
   uint16 u16_CanBusChannel; //!< CAN bus channel number
   uint8 u8_Mode;            //!< CAN message forwarding mode (X_CAN_SERVER_MODE_POLLING, X_CAN_SERVER_MODE_EVENT)
} T_x_can_server_obj_config;

//!< CAN server object status structure
typedef struct
{
   uint8 u8_MsgLostFlag; /*!< RX Message lost flag (at least one message is lost, either on client side, server
                              side or both sides; flag is reset on read)
                              0: no message lost, 1: message lost */
} T_x_can_server_obj_status;

//!< CAN server status structure
typedef struct
{
   uint8 u8_GlobalMsgLostFlag; /*!< Global message lost flag (sum over all objects, reset when read)
                                    0: no message lost, 1: message lost) */
} T_x_can_server_status;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_can_server_obj_init(const T_x_can_server_obj_config * const opt_Config,
                             T_x_can_server_obj ** const oppt_Handle);

void x_can_server_task(void);

sint16 x_can_server_get_status(T_x_can_server_status * const opt_Status);

sint16 x_can_server_obj_get_status(T_x_can_server_obj * const opt_Handle, T_x_can_server_obj_status * const opt_Status);

sint16 x_can_server_obj_get_buf_count(const T_x_can_server_obj * const opt_Handle, uint16 * const opu16_Count);

sint16 x_can_server_obj_get_msg(T_x_can_server_obj * const opt_Handle, T_x_can_msg * const opt_Msg);

sint16 x_can_server_obj_clear_buf(T_x_can_server_obj * const opt_Handle);

#ifdef __cplusplus
}
#endif

#endif /* X_CAN_SERVER_H */
