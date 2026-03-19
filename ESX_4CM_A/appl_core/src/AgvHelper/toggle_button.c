//-----------------------------------------------------------------------------
/*! \file       toggle_button.c
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
//PROJECT
#include "toggle_button.h"
#include "system.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Initialize AgvHelper - Toggle Button Init
 *
 *  This function initializes the toggle button.
 *
 *  \param pt_btn Pointer to the toggle button structure
 *  \param pu8_btn_state Pointer to variable to hold the output button state
 *  \param _u32_deb_ms Minimum press duration required to toggle
 *  \param u8_safe_state Forced output state during fault or after reset
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 toggleButton_init(T_ToggleBtn *pt_btn, uint8 *pu8_btn_state_set, uint32 u32_deb_ms_set, uint8 u8_safe_state_set)
{
    sint16 s16_error = C_NO_ERR;

    if((pt_btn == NULL) || (pu8_btn_state_set == NULL))
    {
        return C_WARN;
    }

    pt_btn->pu_btn_state   = pu8_btn_state_set;

    pt_btn->u32_deb_ms   = u32_deb_ms_set;
    pt_btn->u8_safe_state = u8_safe_state_set;

    *(pt_btn->pu_btn_state) = FALSE;

    pt_btn->u32_hold_ms = 0;
    pt_btn->u8_btn_set = TRUE;

    return s16_error;
}

/** \brief Initialize AgvHelper - Toggle Button
 *
 *  This function maintains a toggle button with a set debounce.
 *
 *  \param pt_btn Pointer to the toggle button structure
 *  \param u8_raw_btn Current raw button input value
 *  \param u8_reset Indicates reset condition
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 toggleButton(T_ToggleBtn *pt_btn, uint8 u8_raw_btn, uint8 u8_reset)
{
    sint16 s16_error = C_NO_ERR;
    uint32 u32_now_ms = get_system_time_ms();

    if((pt_btn == NULL) || (pt_btn->pu_btn_state == NULL))
    {
        return C_WARN;
    }

    u8_raw_btn = (u8_raw_btn != FALSE) ? TRUE : FALSE;

    //IR-21.1 Fault/Interlock forces Safe State
    if(u8_reset == TRUE)
    {
        *(pt_btn->pu_btn_state) = (pt_btn->u8_safe_state != FALSE) ? TRUE : FALSE;
        pt_btn->u32_hold_ms = 0u;
        //IR-21.2 Requires a new press after fault clears
        pt_btn->u8_btn_set = FALSE;
        return C_WARN;
    }

    //FR-21.2 Required release before accepting new press
    if(u8_raw_btn == FALSE)
    {
        pt_btn->u32_hold_ms = 0u;
        pt_btn->u8_btn_set = TRUE;
    }
    else
    {

        //FR-21.1 Measure continuous press time
        if(pt_btn->u32_hold_ms == 0u)
        {
            pt_btn->u32_hold_ms = u32_now_ms;
        }

        //FR-21.1 & FR-21.2 Toggle once when press duration >= debounce, only once per press
        if( (pt_btn->u8_btn_set == TRUE) && ((u32_now_ms - pt_btn->u32_hold_ms) >= pt_btn->u32_deb_ms))
        {
            *(pt_btn->pu_btn_state) = (*(pt_btn->pu_btn_state) == FALSE) ? TRUE : FALSE;
            pt_btn->u8_btn_set = FALSE;
        }
    }

    return s16_error;
}

//EOF
