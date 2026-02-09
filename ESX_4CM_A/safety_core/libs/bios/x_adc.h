//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      ADC API implementations
   \copyright  Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ADC_H
#define X_ADC_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//! \brief ADC faults
#define  X_ADC_FAULT_FIXED_VOLTAGE     (1uL << 0uL)   //!< Fault fixed voltage of an ADC channel was out of range
#define  X_ADC_FAULT_ALIVE             (1uL << 1uL)   //!< Fault ADC did not start a conversion within expected time
#define  X_ADC_FAULT_DIVIDER           (1uL << 2uL)   //!< VADC clock divider changed (DIVA) while system is running
#define  X_ADC_FAULT_ALL               (0xFFFFFFFFLu) //!< All fault flags set

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_adc_get_active_faults(uint32 * const opu32_Faults);
sint16 x_adc_get_sticky_faults(uint32 * const opu32_Faults);
sint16 x_adc_clear_sticky_faults(const uint32 ou32_Faults);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /* XB_ADC_H */
