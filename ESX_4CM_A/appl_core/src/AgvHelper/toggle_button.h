//-----------------------------------------------------------------------------
/*! \file       toggle_button.h
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------

#ifndef APPL_CORE_SRC_AGVHELPER_TOGGLE_BUTTON_H_
#define APPL_CORE_SRC_AGVHELPER_TOGGLE_BUTTON_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwtypes.h"
//PROJECT
#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */


/** \brief Structure containing all relevant Toggle Button Parameters*/
typedef struct
{
        uint8 *pu_btn_state; //!<Button ON/OFF state
        uint32 u32_hold_ms; //!<Hold Button MS
        uint8 u8_btn_set; //!<Button Armed State

        //Params
        uint8 u8_safe_state; //!<Safe State
        uint32 u32_deb_ms; //!<Debounce in milliseconds

}T_ToggleBtn;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 toggleButton_init(T_ToggleBtn *pt_btn, uint8 *pu8_btn_state_set, uint32 u32_deb_ms_set, uint8 u32_hold_ms_set, uint8 u8_safe_state_set);
sint16 toggleButton(T_ToggleBtn *pt_btn, uint8 u8_raw_btn, uint8 u8_faulted);
#endif /* APPL_CORE_SRC_AGVHELPER_TOGGLE_BUTTON_H_ */
