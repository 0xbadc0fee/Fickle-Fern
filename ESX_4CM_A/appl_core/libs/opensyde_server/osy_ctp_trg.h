//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       CAN-TP: target driver definitions

   CAN-TP target definitions for Aurix Plattform

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_TRG_CANH
#define OSY_TRG_CANH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_can.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define OSY_CTP_TRG_MAX_CAN_INTERFACES    (X_CAN_COUNT) //< number of CAN interfaces to support

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
   uint16 u16_RxBufferSize;      // Number of CAN frames (T_x_can_msg) for Rx buffer
   uint16 u16_TxBufferSize;      // Number of CAN frames (T_x_can_msg) for Tx buffer
} T_osy_ctp_trg_can_init_params; //!< target specific initialization parameters

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif
