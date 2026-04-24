//-----------------------------------------------------------------------------
/**
 * \file       can_handler_lib.h
 * \brief      System - CAN Device Interface
 *
 * \addtogroup System
 * @{
 * \defgroup CanDeviceInterface CAN Device Interface
 * \brief Interface layer for CAN device communication and management.
 * @{
 *
 * \copyright   STW Technic (c) 2026
 *              use only under terms of contract / confidential
 *
 * \author     Jan 7, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
#ifndef CAN_INIT_H
#define CAN_INIT_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include <stdbool.h>

#include "can_device_definition.h"
#include "j1939_data_pool.h"

//Include SPNS (current location for DP Assignment MACRO)
#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_CANDevices gt_can_devs; //!< External reference to the global structure managing all CAN devices

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_canInterfaces(void);
sint16 update_canInputs(void);
sint16 update_canOutputs(void);
void force_canMessage(uint8 u8_can_bus, uint32 u32_id);
void set_canMessageActive(uint8 u8_can_bus, uint32 u32_can_id, uint8 u8_status);
bool can_get_availability_state(const uint16 ou16_Channel);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // CAN_INIT_H
