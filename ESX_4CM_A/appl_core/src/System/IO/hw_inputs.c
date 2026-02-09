//-----------------------------------------------------------------------------
/*! \file       hw_inputs.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2025
    license     use only under terms of contract / confidential

    created     Dec 9, 2025 kyle.boch
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
#include "stwtypes.h"

//PROJECT
#include "input_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
//TODO_STW: Abstract X_IN_.... to "CX_PINXX"

// Define vehicle specific inputs
T_VehicleInput throttlePosition =
        {
            .Name_Description       = "THROTPOS",
            .u16_hardwareID         = X_IN_IDA35V_7,
            .e_inputType            = IT_DIGITAL,

            .f32_inputValue         = FALSE,
            .f32_prevInputValue     = FALSE,
            .mq_inputChanged        = TRUE,
            .u8_diagEnabled         = TRUE,
            .u32_SPN                = 520192,
            .u16_dti                = 1000,
            .s32_diagMin            = 500,
            .s32_diagMax            = 4500,
        };


/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize Hardware Inputs
 *
 *  This funciton takes all the inputs described as modules global variables and adds them to the update list using
 *  add_hwInput().  After all inputs are added to the update list, the handler (and all inputs in the update list) are initialized
 *  according to their set configuration.
 *
 *  \return s16_error Error Code
 */
sint16 init_hwInputs(void)
{
    sint16 s16_return = C_NO_ERR;

    s16_return |= add_hwInput(throttlePosition);
    s16_return |= init_inputHandler();

    return s16_return;
}

/** \brief Update Hardware Inputs
 *
 *  This funciton makes a single call to update_inputHandler (see corresponding documentation)takes all the inputs described as modules global variables and adds them to the update list using
 *  add_hwInput().  After all inputs are added to the update list, the handler (and all inputs in the update list) are initialized
 *  according to their set configuration.
 *
 *  \return update_inputHandler() - Pass through return code for indication of update_inputHandler execution.
 */
sint16 update_hwInputs(void)
{
    return update_inputHandler();
}


//EOF
