//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to openSYDE J1939 driver for openSYDE Server COMM stack
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_J1939_DRIVERH
#define OSY_COM_J1939_DRIVERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_com_engine.h"
#include "osy_com_j1939_handler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

#define OSY_COM_J1939_DRIVER (&gt_OsyComDriverJ1939)

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern const T_osy_com_driver gt_OsyComDriverJ1939;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
