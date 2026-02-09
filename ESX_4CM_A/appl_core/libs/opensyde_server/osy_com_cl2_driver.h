//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to process data exchange stack CAN layer 2 driver functions
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_CL2_DRIVERH
#define OSY_COM_CL2_DRIVERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_com_engine.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

#define OSY_COM_PROTOCOL_TYPE_CL2  (0U)

#define OSY_COM_CL2_DRIVER (&gt_OsyComDriverCl2)

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern const T_osy_com_driver gt_OsyComDriverCl2;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
