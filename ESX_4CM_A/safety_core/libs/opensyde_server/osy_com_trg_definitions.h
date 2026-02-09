//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       openSYDE server target layer: definitions for process data exchange stacks

   Type definitions for compilation of the library.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_TRG_DEFINITIONSH
#define OSY_COM_TRG_DEFINITIONSH

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define OSY_COM_TRG_MAX_CAN_INTERFACES    (6U) //< Number of CAN interfaces to support

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
   uint8 u8_TxBufferEnable;           //!< Enable or disable CAN Tx buffer
   uint16 u16_CanClientServerStartId; //!< Start ID for CAN Client/Server Tx and Rx objects
} T_osy_com_trg_can_init_params;      //!< Target specific initialization parameters

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif
