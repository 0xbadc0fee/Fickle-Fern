//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      CAN initialization
   \copyright  Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef CAN_INIT_H
#define CAN_INIT_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include <stdbool.h>

#include "hmi_definition.h"
#include "j1939_data_pool.h"

//Include SPNS (current location for DP Assignment MACRO)
#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_UserInterface gt_ui;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 init_canInterfaces(void);
sint16 update_canInputs(void);
sint16 update_canOutputs(void);

bool can_get_availability_state(const uint16 ou16_Channel);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // CAN_INIT_H
