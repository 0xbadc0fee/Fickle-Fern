//-----------------------------------------------------------------------------
/*! \file       moving_avg_filter.h
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVHELPER_MOVING_ADV_FILTER_H_
#define APPL_CORE_SRC_AGVHELPER_MOVING_ADV_FILTER_H_

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

/** \brief Structure containing all relevant Moving Average Parameters*/
typedef struct
{
        float32 * pf32_buf; //!<Caller buffer
        uint16 u16_buf_len;  //!<Buffer capacity
        uint16 u16_head; //!<Next write index
        uint16 u16_count; //!<Samples currently stored
        uint32 u32_accum_ms; //!<Accumulates dt until >= sample time ms
        float32 f32_sum; //!<Running sum of samples in window
        float32 f32_out; //!<Current filtered output
        uint8 u8_faulted; //!<1 = Faulted and forced safe

        //Config values
        uint16 u16_sample_time_ms;
        uint16 u16_sample_no;
        float32 f32_safe_state; //!<Safe State

}T_MoveAvgFilter;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 movingFltInit(T_MoveAvgFilter * const pt_mv_adv_flt, float32 * const pf32_buffer, uint16 u16_buf_len, float32 f32_safe_output, uint16 u16_sample_no_set, uint16 u16_sample_time_ms_set);
sint16 movingAdvFlt(T_MoveAvgFilter * const pt_mv_adv_flt, float32 f32_new_value);

#endif /* APPL_CORE_SRC_AGVHELPER_MOVING_ADV_FILTER_H_ */
