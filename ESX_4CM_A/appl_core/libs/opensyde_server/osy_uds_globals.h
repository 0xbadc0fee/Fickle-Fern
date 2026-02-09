//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       UDS: global types

   Provides UDS definitions that can be used in the UDS and TP layers.
   If a TP is to be used without the UDS layer this header has to be provided for the TP to compile.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_UDS_GLOBALSH
#define OSY_UDS_GLOBALSH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

#define OSY_UDS_LOCAL_BROADCAST_ADDRESS (0x7FU) ///< special target address to flag "broadcast to all local nodes"

#define OSY_UDS_NUM_BUSES          (16U) ///< number of possible buses in the system
#define OSY_UDS_NUM_NODES_PER_BUS (127U) ///< number of possible nodes in one bus (127 is reserved for broadcasts)

/* -- Types --------------------------------------------------------------------------------------------------------- */
///Describe a "global" node identifier consisting of bus id and node id
typedef struct
{
   uint8 u8_BusId;
   uint8 u8_NodeId;
} T_osy_uds_node_identifier;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
