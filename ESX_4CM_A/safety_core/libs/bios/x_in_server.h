//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Input remote core server

   This file contains types and function declarations for the input server component running on a master safety core
   and serving remote safe and non-safe cores. The server uses RPC mechanisms for serialized calls like initialization
   and select setters, as well as an Exposed-If portion for non-destructive read operations.

   \note This file is not intended to be distributed with the deployment

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_IN_SERVER_H___
#define X_IN_SERVER_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "stwerrors.h"
#include "x_in.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_in_server_allocate_channel(const uint16 ou16_Channel);
sint16 x_in_server_allocate_channel_redundant(const uint16 ou16_Channel);
sint16 x_in_server_allocate_channel_incremental(const uint16 ou16_Channel);
sint16 x_in_server_allocate_channel_incremental_redundant(const uint16 ou16_Channel);
sint16 x_in_server_lock_allocations(void);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_IN_SERVER_H___
