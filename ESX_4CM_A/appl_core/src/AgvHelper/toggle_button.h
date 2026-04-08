//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   March 6, 2026 STW Technic
 *
 * \file       toggle_button.h
 * \brief      Interface for Toggle Button Module.
 *
 * \addtogroup AgvHelper
 * @{
 * \addtogroup ToggleButton Toggle Button
 * @{
 */

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

/**
 * \struct T_ToggleBtn
 * \brief  Structure containing all relevant Toggle Button Parameters.
 */
typedef struct
{
        //Output Variable
        uint8 *pu_btn_state;    //!<Button ON/OFF state

        //Working Variables
        uint32 u32_hold_ms;     //!<Hold Button MS
        uint8 u8_btn_set;       //!<Button Armed State

        //Params
        uint8 u8_safe_state; //!<Safe State
        uint32 u32_deb_ms; //!<Debounce in milliseconds

}T_ToggleBtn;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 toggleButton_init(T_ToggleBtn *pt_btn, uint8 *pu8_btn_state_set, uint32 u32_deb_ms_set, uint8 u8_safe_state_set);
sint16 toggleButton(T_ToggleBtn *pt_btn, uint8 u8_raw_btn, uint8 u8_faulted);

#endif /* APPL_CORE_SRC_AGVHELPER_TOGGLE_BUTTON_H_ */
