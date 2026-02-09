//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Constants for physical layer
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_PHY_TYPES_H
#define X_ETH_PHY_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_ETH_LINK_DOWN                      (0u)
#define X_ETH_LINK_UP                        (1u)

#define X_ETH_LINK_MODE_10MB_HALF_DUPLEX     (0x1u)
#define X_ETH_LINK_MODE_10MB_FULL_DUPLEX     (0x2u)
#define X_ETH_LINK_MODE_100MB_HALF_DUPLEX    (0x4u)
#define X_ETH_LINK_MODE_100MB_FULL_DUPLEX    (0x8u)
#define X_ETH_LINK_MODE_1000MB_HALF_DUPLEX   (0x10u)
#define X_ETH_LINK_MODE_1000MB_FULL_DUPLEX   (0x20u)
#define X_ETH_LINK_MODE_ADVERTISE_ALL        (0x3Fu)
#define X_ETH_LINK_MODE_NONE                 (0x40u)
#define X_ETH_LINK_MODE_PENDING              (0x80u) // PHY is not sure yet if it has a stable link, ask again later...

#define X_ETH_FEAT_BRR_SLAVE                 (0x0uL)
#define X_ETH_FEAT_BRR_MASTER                (0x1uL)
#define X_ETH_FEAT_GIGABIT_SLAVE             (0x2uL)
#define X_ETH_FEAT_GIGABIT_MASTER            (0x4uL)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_PHY_TYPES_H
