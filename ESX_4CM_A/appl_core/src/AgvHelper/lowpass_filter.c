//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/**
 * \file       lowpass_filter.c
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
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
#include <stdint.h>
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "../HAL/STW_4CM_HAL/system.h"
//PROJECT
#include "lowpass_filter.h"
#include "math.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/**
 * \brief Initialize Low Pass Filter
 *
 * This function initializes the low pass filter instance by setting
 * the filtering coefficient, the safe state value, and resetting
 * the persistent output to the specified safe state.
 *
 * \param[out] pt_filter           Pointer to the filter structure to initialize
 * \param[in]  f32_alpha_set       Filter coefficient [0.0 to 1.0]
 * \param[in]  f32_safe_state_set  Default value for initialization and safe state
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 lowpassFilter_init(T_LowPassFilter *pt_filter, float32 f32_alpha_set, float32 f32_safe_state_set)
{
    sint16 s16_error = C_NO_ERR;

    if(pt_filter == NULL)
    {
        return C_WARN;
    }

    pt_filter->f32_output = f32_safe_state_set;
    pt_filter->f32_alpha = f32_alpha_set;
    pt_filter->f32_safe_state = f32_safe_state_set;

    return s16_error;
}

/**
 * \brief Execute Low Pass Filter logic
 *
 * This function calculates the filtered output by combining the new input
 * with the previous state using the filter coefficient. If the faulted flag
 * is active, the filter output is forced to the pre-defined safe state.
 *
 * \param[in,out] pt_filter   Pointer to the filter state structure
 * \param[in]     f32_input   The new raw input value to be filtered
 * \param[in]     u8_faulted  Flag indicating if the input source is in a fault state
 *
 * \return sint16 Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 lowpassFilter(T_LowPassFilter *pt_filter, float32 f32_input, uint8 u8_faulted)
{
    sint16 s16_error = C_NO_ERR;

    // IR-20.1 Invalid Coefficient  OR IR-20.2 Invalid Input
   if(pt_filter == NULL)
   {
       return C_WARN;
   }
    if((pt_filter->f32_alpha < 0.0f) || (pt_filter->f32_alpha > 1.0f) || isnan(pt_filter->f32_alpha) || (u8_faulted == TRUE) || isnan(f32_input))
    {
        pt_filter->f32_output =  pt_filter->f32_safe_state;
        return C_WARN;
    }

    if (f32_input != pt_filter->f32_output)
    {
        // FR-20.1 - FR 20.2
        pt_filter->f32_output = pt_filter->f32_output + pt_filter->f32_alpha * (f32_input - pt_filter->f32_output);
    }

    return s16_error;
}

//EOF
