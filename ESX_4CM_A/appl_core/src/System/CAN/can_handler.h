//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Jan 7, 2026 STW Technic
 *
 * \file       can_device_interface.h
 * \brief      Interface for CAN Device Interface Module.
 *
 * \addtogroup System
 * @{
 * \addtogroup CanDeviceInterface CAN Device Interface
 * @{
 */

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
extern T_UserInterface gt_ui; //!< Global instance of the User Interface structure.

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 init_canInterfaces(void);
sint16 update_canInputs(void);
sint16 update_canOutputs(void);

bool can_get_availability_state(const uint16 ou16_Channel);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // CAN_INIT_H
