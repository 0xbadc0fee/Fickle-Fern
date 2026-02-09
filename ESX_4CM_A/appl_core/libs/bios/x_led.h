//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       LED API functions
   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_LED_H
#define X_LED_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_led_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
//! \brief type of LEDs
#define X_LED_TYPE_NONE             (0u) //!< LED type uninitialized
#define X_LED_TYPE_BLENDING         (1u) //!< dual LED with stage-less color configuration

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 x_led_blending_init(const uint16 ou16_Channel);
sint16 x_led_get_type(const uint16 ou16_Channel, uint16 * const opu16_Type);
sint16 x_led_set_blend(const uint16 ou16_Channel, const uint32 ou32_Intensity, const uint32 ou32_Blend);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_LED_H
