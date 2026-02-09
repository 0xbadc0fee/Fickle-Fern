//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Interface to process data exchange stack ESX CAN Open Safety Protocol driver functions
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_ECOS_DRIVERH
#define OSY_COM_ECOS_DRIVERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_com_engine.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

#define OSY_COM_PROTOCOL_TYPE_ECOS (2U)

#define OSY_COM_ECOS_DRIVER (&gt_OsyComDriverEcos)

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern const T_osy_com_driver gt_OsyComDriverEcos;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
