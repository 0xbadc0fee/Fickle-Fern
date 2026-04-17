//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/**
 * \file       moving_avg_filter.c
 * \brief      AgvHelper - Moving Average Filter
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup MovingAvgFilter Moving Average Filter
 *
 * The Moving Average Filter Module produces a smoothed output by averaging
 * values within a configurable sample window. It ensures system stability
 * by forcing a safe output when required parameters or input values are
 * detected as invalid.
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
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
#include <stdint.h>
//STW
#include "stwerrors.h"
#include "stwtypes.h"
//PROJECT
#include "moving_avg_filter.h"
#include "system.h"
#include "math.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Moving Average Filter Init AgvHelper - Helper Control
 *
 *  The Moving Average Filter Module Init initialized movingAdvFlt.
 *
 *  \param pt_mvAdvFlt Average Filter Pointer
 *  \param pf32_buffer Buffer
 *  \param u16_buf_len Buffer Length
 *  \param f32_safe_output Safe Output
 *  \param u16_sample_no_set Number Set
 *  \param u16_sample_time_ms_set Sample Time in Milliseconds
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 movingFltInit(T_MoveAvgFilter * const pt_mv_adv_flt, float32 * const pf32_buffer, uint16 u16_buf_len, float32 f32_safe_output, uint16 u16_sample_no_set, uint16 u16_sample_time_ms_set)
{
    sint16 s16_error = C_NO_ERR;

    uint16 u16_clear_buff =0u;

    if((pt_mv_adv_flt == NULL) || (pf32_buffer == NULL) || (u16_buf_len == 0u))
    {
        return C_WARN;
    }

    pt_mv_adv_flt->pf32_buf = pf32_buffer;
    pt_mv_adv_flt->u16_buf_len = u16_buf_len;
    pt_mv_adv_flt->u16_head = 0u;
    pt_mv_adv_flt->u16_count = 0u;
    pt_mv_adv_flt->u32_accum_ms = 0u;
    pt_mv_adv_flt->f32_sum = 0.0F;
    pt_mv_adv_flt->f32_out = f32_safe_output;

    pt_mv_adv_flt->u16_sample_no = 0u;
    pt_mv_adv_flt->f32_safe_state = f32_safe_output;
    pt_mv_adv_flt->u16_sample_time_ms = u16_sample_time_ms_set;
    pt_mv_adv_flt->u16_sample_no = u16_sample_no_set;

    for(u16_clear_buff = 0u; u16_clear_buff <u16_buf_len; u16_clear_buff++)
    {
        pt_mv_adv_flt->pf32_buf[u16_clear_buff] = 0.0F;
    }

    return s16_error;
}

/** \brief Moving Average Filter AgvHelper - Helper Control
 *
 *  The Moving Average Filter Module produces a smoothed output by averaging values within a configurable sample window and forces a safe output when required parameters or values are invalid.
 *
 *  \param pt_mv_adv_flt Average Filter Pointer
 *  \param f32_new_value New Value
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 movingAdvFlt(T_MoveAvgFilter * const pt_mv_adv_flt, float32 f32_new_value)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_old = 0.0f;
    uint32 u32_now_ms = get_system_time_ms();

    if(pt_mv_adv_flt == NULL)
    {
        return C_WARN;
    }

    //IR-19.1 Invalid Params
    if((pt_mv_adv_flt->pf32_buf == NULL) ||
    (pt_mv_adv_flt->u16_buf_len == 0u)||
    (pt_mv_adv_flt->u16_sample_time_ms == 0u) ||
    (pt_mv_adv_flt->u16_sample_no == 0u) ||
    (pt_mv_adv_flt->u16_sample_no > pt_mv_adv_flt->u16_buf_len)||
    isnan(f32_new_value))  //IR-19.2 Invalid sample value
    {
        pt_mv_adv_flt->f32_out =  pt_mv_adv_flt->f32_safe_state;
        pt_mv_adv_flt->u8_faulted = TRUE;
        return C_WARN;
    }

    //IR-19.3
    pt_mv_adv_flt->u8_faulted = FALSE;

    //FR-19.2/19.3 First valid sample
    if(pt_mv_adv_flt->u16_count == 0u)
    {
        pt_mv_adv_flt->pf32_buf[0] = f32_new_value;
        pt_mv_adv_flt->f32_sum = f32_new_value;
        pt_mv_adv_flt->f32_out = f32_new_value;
        pt_mv_adv_flt->u16_head = 1u;
        pt_mv_adv_flt->u16_count = 1u;
        pt_mv_adv_flt->u32_accum_ms = u32_now_ms;
        return s16_error;
    }

    //FR-19.2/19.3 Filter
    if((u32_now_ms - pt_mv_adv_flt->u32_accum_ms) >= ((uint32)pt_mv_adv_flt->u16_sample_time_ms))
    {
        pt_mv_adv_flt->u32_accum_ms = u32_now_ms;

        if(pt_mv_adv_flt->u16_count < pt_mv_adv_flt->u16_sample_no)
        {
            pt_mv_adv_flt->pf32_buf[pt_mv_adv_flt->u16_head] = f32_new_value;
            pt_mv_adv_flt->f32_sum += f32_new_value;
            pt_mv_adv_flt->u16_head++;

            if(pt_mv_adv_flt->u16_head >= pt_mv_adv_flt->u16_sample_no)
            {
                pt_mv_adv_flt->u16_head = 0u;
            }

            pt_mv_adv_flt->u16_count++; //FR-19.4
        }

        else
        {
            f32_old = pt_mv_adv_flt->pf32_buf[pt_mv_adv_flt->u16_head];
            pt_mv_adv_flt->pf32_buf[pt_mv_adv_flt->u16_head] = f32_new_value;
            pt_mv_adv_flt->f32_sum += (f32_new_value - f32_old);

            pt_mv_adv_flt->u16_head++;
            if(pt_mv_adv_flt->u16_head >= pt_mv_adv_flt->u16_sample_no)
            {
                pt_mv_adv_flt->u16_head = 0u;
            }
        }

        pt_mv_adv_flt->f32_out = pt_mv_adv_flt->f32_sum / (float32)pt_mv_adv_flt->u16_count; //FR-19.1/19.4
    }

    return s16_error;
}

//EOF
