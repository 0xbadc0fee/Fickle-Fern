//-----------------------------------------------------------------------------
/*! \file       lowpass_filter.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
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

/** \brief Initialize Low Pass Filter
 *
 *  Initializes the previous filtered output value.
 *
 *  \param pt_filter Pointer to filter structure
 *  \param f32_alpha_set Filter Coefficient
 *  \param f32_safe_state_set Safe State
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
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

/** \brief Low Pass Filter AgvHelper - Helper Control
 *
 *  The Low Pass Filter Module produces a smoothed output by combining the new input value with the previous filtered value using a single filtering coefficient.
 *
 *  \param pt_filter Struct previous filtered output
 *  \param f32_input New raw input value
 *  \param u8_faulted Fault flag
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
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
