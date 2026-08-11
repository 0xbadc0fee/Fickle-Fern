//----------------------------------------------------------------------------------------------------------------------

/*
 * harness_svg_lib.c
 *
 *  Created on: Aug 7, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <stdbool.h>

//STW
#include "stwtypes.h"
#include "stwerrors.h"

#include "harness_svg_buttonPanel.h"


/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

// Definition of application heap

// Global variables for main task configuration


/* -- Module Global Variables --------------------------------------------------------------------------------------- */
T_HarnessSignalsButtonPanel gt_svg;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

sint16 update_harnessInputs(void)
{
    sint16 s16_error = C_NO_ERR;
    gt_svg.u8_testerCounter = 0;
    gt_svg.u8_testerReset = 0;

    #define SVG2CNTRL(name, CNTRL_VALUE, SVG_VALUE) VAR_ASSIGN((CNTRL_VALUE), (SVG_VALUE));
    #include "Fixtures/lighting_control/fixture.def"
    #undef SVG2CNTRL

    return s16_error;
}

sint16 update_harnessOutputs(void)
{
    sint16 s16_error = C_NO_ERR;

    #define CNTRL2SVG(name, CNTRL_VALUE, SVG_VALUE) VAR_ASSIGN((SVG_VALUE), (CNTRL_VALUE));
    #include "Fixtures/lighting_control/fixture.def"
    #undef CNTRL2SVG

    return s16_error;
}


//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Main routine */
//----------------------------------------------------------------------------------------------------------------------


