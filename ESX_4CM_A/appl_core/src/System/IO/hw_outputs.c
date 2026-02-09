//-----------------------------------------------------------------------------
/*! \file       hw_outputs.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 4, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_out.h"
#include "output_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
// Define vehicle specific inputs
T_VehicleOutput elevatorControlValve =
        {
            .Name_Description       = "VLV_ELEV_SPEED",
            .u16_hardwareID         = X_OUT_OPHSP2A_1,
            .e_outputType            = OT_PWM,

            .f32_outputValue        = 0.0f,
            .f32_prevOutputValue    = 0.0f,
            .mq_outputChanged       = TRUE,

            .u8_diagEnabled         = FALSE,
            .u32_SPN                = 520193,
            .u16_dti                = 0,
        };


/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize Hardware Outputs
 *
 *  This funciton takes all the outputs described as modules global variables and adds them to the update list using
 *  add_hwOutput().  After all outputs are added to the update list, the handler (and all outputs in the update list) are initialized
 *  according to their set configuration.
 *
 *  \return s16_error Error Code
 */
sint16 init_hwOutputs(void)
{
    sint16 s16_return = C_NO_ERR;

    s16_return |= add_hwOutput(elevatorControlValve);
    s16_return |= init_outputHandler();

    return s16_return;
}

/** \brief Update Hardware Outputs
 *
 *  This funciton makes a single call to update_outputHandler (see corresponding documentation)takes all the outputs described as modules global variables and gathers
 *  the latest values and fault status'
 *
 *  \return update_outputHandler() - Pass through return code for indication of update_outputHandler execution.
 */
sint16 update_hwOutputs(void)
{
    return update_outputHandler();
}





//EOF
