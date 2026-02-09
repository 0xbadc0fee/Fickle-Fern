//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief   Sytem resource control

   \copyright   Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_MEM_RESOURCES_H___
#define X_MEM_RESOURCES_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "gcc_attributes.h"
#include "x_memtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Statically allocate specified number of system resources

   \param[in]   NumHandles   Number of system handles available at runtime
   \param[in]   NumProxies   Number of message proxies available at runtime
*/
//----------------------------------------------------------------------------------------------------------------------
#define X_MEM_RESOURCES_MAKE(NumHandles, NumProxies) \
   X_MEM_SYSTEM_HANDLES ALIGNED(32) uint8 mau8_SystemHandles___[8u *  (NumHandles)]; \
   X_MEM_ICC_MSG_PROXIES ALIGNED(32) uint8 mau8_IccMsgProxies___[32u * (NumProxies)];

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_MEM_RESOURCES_H___
