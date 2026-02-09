//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Target specific LED API defines

   In case of no LED on the target, leave this file empty.

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_LED_DEFS_H
#define X_LED_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_LED_01                       0u //!< LED 01 output

#define X_LED_COUNT                    1u //!< number of available LEDs

//! \brief LED blending configurations
#define X_LED_COLOR_RED            (10000u) //!< LED color = pure red
#define X_LED_COLOR_ORANGE         (2500u)  //!< LED color = orange
#define X_LED_COLOR_YELLOW         (1000u)  //!< LED color = yellow
#define X_LED_COLOR_GREEN          (1u)     //!< LED color = pure green

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* X_LED_DEFS_H */
