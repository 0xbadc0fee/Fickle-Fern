//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       openSYDE server target layer: definitions

   General definitions for compilation of the library.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPA_TRG_DEFINITIONSH
#define OSY_DPA_TRG_DEFINITIONSH

// Check GCC version at preprocessing
#if ((__GNUC__ != 4) || (__GNUC_MINOR__ != 9) || (__GNUC_PATCHLEVEL__ != 4))
#error This Library shall only be used with HighTec TriCore Development Platform V4.9.2.0 (based on GCC V4.9.4)
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_memtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

///Modifiers to place variables in specific RAM areas.
///Which of the modifiers are required depends on the target architecture.
///The postfixes "_ZERO" rsp. "_NONZERO" refer to the need for initialization at startup.
///In the simplest of cases (no safety, single-process) all modifiers can be kept empty.

///Marker for potentially safety relevant data (if supported by the owning process).
///Only the owning process has read-write permission.
#define OSY_DPA_SAFE_RAM_DATA_PRIVATE_ZERO

///Marker for potentially safety relevant data (if supported by the owning process).
///The owning process has read-write permission and all other processes only have read permission.
///This data shall be placed in a memory area where other processes have read access.
#define OSY_DPA_SAFE_RAM_DATA_READALL_ZERO      X_MEM_SHARED_BSS_SAFE
#define OSY_DPA_SAFE_RAM_DATA_READALL_NONZERO   X_MEM_SHARED_DATA_SAFE

///Marker for non-safe data.
///Only the owning process has read-write permission.
#define OSY_DPA_NONSAFE_RAM_DATA_PRIVATE_ZERO // Is also safe on this core

///Marker for non-safe data.
///All processes have read-write permission.
///This data shall be placed in a memory area where other processes have read-write access.
#define OSY_DPA_NONSAFE_RAM_DATA_WRITEALL_ZERO  X_MEM_SHARED_BSS

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif
