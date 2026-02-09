//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Target specific defines for external sensor supply

   In case of no external ensor supply on the target, leave this file empty.

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_UEXT_DEFS_H
#define X_UEXT_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_UEXT_5V_1                 0u //!< 5V fixed voltage supply
#define X_UEXT_5V_2                 1u //!< 5V fixed voltage supply
#define X_UEXT_ADJ_5V_12V_1         2u //!< adjustable 5..12V voltage supply
#define X_UEXT_ADJ_5V_12V_2         3u //!< adjustable 5..12V voltage supply
#define X_UEXT_COUNT                4u //!< number of available external sensor supply outputs

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* X_UEXT_DEFS_H */
