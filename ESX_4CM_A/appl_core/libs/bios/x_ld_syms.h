//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Default linker script symbols
   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_LDSYMS_H
#define X_LDSYMS_H

/* -- Includes ------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------- */

// These constants contain the last used flash address of the application
extern const uint32 gu32_Ld_EndOfUsedRom0;
extern const uint32 gu32_Ld_EndOfUsedRom1;
extern const uint32 gu32_Ld_EndOfUsedRom2;
extern const uint32 gu32_Ld_EndOfUsedRom3;
extern const uint32 gu32_Ld_EndOfUsedRom4;

/* -- Global Symbols ------------------------------------------------------ */

#endif /* X_LDSYMS_H */
