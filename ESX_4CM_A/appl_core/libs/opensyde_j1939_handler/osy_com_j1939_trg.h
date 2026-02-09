//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       J1939 Handler: target driver

   Target specific settings for the openSYDE J1939 Handler

   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_J1939_TRG_H
#define OSY_COM_J1939_TRG_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// These are fix values which depend on the compiler (see COMPILE_TIME_ASSERT).
// To find out the correct values at compile time, we can use for example the following construct:
//    uint8 checker(int);
//    uint8 checkSizeOfInt[sizeof(T_osy_com_j1939_bam_tx_state)]={checker(&checkSizeOfInt)};
#define OSY_COM_J1939_TRG_TX_STATE_BUFFER_SIZE   (20u)
#define OSY_COM_J1939_TRG_RX_STATE_BUFFER_SIZE   (12u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
