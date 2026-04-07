//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/**
 * \file       lowpass_filter.h
 * \brief      Interface for Low Pass Filter Module.
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup LowPassFilter Low Pass Filter
 * @{
 */
//-----------------------------------------------------------------------------

#ifndef APPL_CORE_SRC_AGVHELPER_LOWPASS_FILTER_H_
#define APPL_CORE_SRC_AGVHELPER_LOWPASS_FILTER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define CLAMP_F32(x, lo, hi)(((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi): (x))) //!<Restricts a 32-bit floating point value to a specified range [lo, hi].

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * @struct T_LowPassFilter
 * \brief Structure containing all relevant Low Pass Filter output information
 *
 * This structure maintains the state and parameters for a single-pole
 * low pass filter instance, allowing for persistent filtering across
 * execution cycles.
 */
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
