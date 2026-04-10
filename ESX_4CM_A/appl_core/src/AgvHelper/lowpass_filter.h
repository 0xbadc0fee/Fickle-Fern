//-----------------------------------------------------------------------------
/**
 * \file       lowpass_filter.h
 * \brief      AgvHelper - Low Pass Filter
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup LowPassFilter Low Pass Filter
 *
 * The Low Pass Filter Module produces a smoothed output by combining the
 * new input value with the previous filtered value using a single
 * filtering coefficient.
 *
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Jan 6, 2026 STW Technic
 *
 * @{
 */

#ifndef APPL_CORE_SRC_AGVHELPER_LOWPASS_FILTER_H_
#define APPL_CORE_SRC_AGVHELPER_LOWPASS_FILTER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x))) //!<Clamp F32 Macro

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_LowPassFilter
 * \brief Structure containing all relevant Low Pass Filter output information*/
typedef struct
{
        float32 f32_output; //!<Previous filtered output
        float32 f32_alpha; //!<Filter Coefficient
        float32 f32_safe_state; //!<Safe State
} T_LowPassFilter;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 lowpassFilter_init(T_LowPassFilter *pt_filter, float32 f32_alpha_set, float32 f32_safe_state_set);
sint16 lowpassFilter(T_LowPassFilter *pt_filter, float32 f32_input, uint8 u8_faulted);

#endif /* APPL_CORE_SRC_AGVHELPER_LOWPASS_FILTER_H_ */
