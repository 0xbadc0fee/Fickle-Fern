//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Target-specific definitions for Inter-Core-Communication
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_ICC_DEFS_H
#define  X_ICC_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ICC_SIG_CORE0             0x01u //!< Boot core (may be ORed with other cores for broadcast)
#define X_ICC_SIG_CORE1             0x02u //!< Safety core (may be ORed with other cores for broadcast)
#define X_ICC_SIG_CORE2             0x04u //!< Application core (may be ORed with other cores for broadcast)

#define X_ICC_SIG_CORES_ALL         (X_ICC_SIG_CORE0 | X_ICC_SIG_CORE1 | X_ICC_SIG_CORE2) //!< All available cores
#define X_ICC_SIG_CORE_COUNT        3u                                                    //!< Number of available cores

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* X_ICC_DEFS_H */
