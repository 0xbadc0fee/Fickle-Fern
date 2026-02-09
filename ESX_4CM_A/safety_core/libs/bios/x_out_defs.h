//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Target specific output defines

   In case of no outputs on the target, leave this file empty.

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OUT_DEFS_H
#define X_OUT_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//MSW 01 connected
#define X_OUT_OPHSP2A_1    0u
#define X_OUT_OPHSP2A_2    1u
#define X_OUT_OPHSP2A_3    2u
#define X_OUT_OPHSP2A_4    3u
//MSW 02 connected
#define X_OUT_OPHSP2A_5    4u
#define X_OUT_OPHSP2A_6    5u
#define X_OUT_OPHSP2A_7    6u
#define X_OUT_OPHSP2A_8    7u
//MSW 03 connected
#define X_OUT_OPHSP2A_9    8u
#define X_OUT_OPHSP2A_10   9u
#define X_OUT_OPHSP2A_11   10u
#define X_OUT_OPHSP2A_12   11u
#define X_OUT_OPHSP2A_13   12u
#define X_OUT_OPHSP2A_14   13u
#define X_OUT_OPHSP2A_15   14u
#define X_OUT_OPHSP2A_16   15u

//MSW 01 connected
#define X_OUT_OPHSP4A_1    16u
#define X_OUT_OPHSP4A_2    17u
//MSW 02 connected
#define X_OUT_OPHSP4A_3    18u
#define X_OUT_OPHSP4A_4    19u

// Lowside Outputs
#define X_OUT_OPL2A_1      20u
#define X_OUT_OPL2A_2      21u
#define X_OUT_OPL2A_3      22u
#define X_OUT_OPL2A_4      23u

#define X_OUT_OPL4A_1      24u
#define X_OUT_OPL4A_2      25u
#define X_OUT_OPL4A_3      26u
#define X_OUT_OPL4A_4      27u

#define X_OUT_OPL0A5_1     28u //alt X_OUT_ODL0A5_1

#define X_OUT_COUNT        29u

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* X_OUT_DEFS_H */
