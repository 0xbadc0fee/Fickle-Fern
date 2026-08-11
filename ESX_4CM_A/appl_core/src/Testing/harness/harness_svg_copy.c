//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_svg_copy.c   (HARNESS MECHANISM - not fixture-specific)
 *
 *  REFERENCE. Seam-2 round-trip proven 2026-08-11 against the lighting_control
 *  fixture (see Fixtures/lighting_control/fixture_signals.h).
 *
 *  The gt_svg <-> gt_can_devs copy step of Seam 2. This is MECHANISM: it is
 *  identical for every fixture and lives in the harness, written ONCE. Only the
 *  CONTENT it expands (fixture.def) differs per fixture, and that is selected at
 *  BUILD time by pointing -I at the active fixture folder so that #include
 *  "fixture.def" resolves to the chosen fixture's table.
 *
 *  This file replaces the per-fixture-named update_harnessInputs/Outputs that used
 *  to live in harness_svg_lightControlTest.c. Moving them here is the fix for the
 *  "mechanism packaged as if fixture-specific" smell: the copy loop was never
 *  lighting-specific, only the .def rows were.
 *
 *  X-MACRO NOTE: the #define/#include/#undef blocks are evaluated ONCE by the
 *  preprocessor at compile time; each expands fixture.def's rows into straight-line
 *  assignments baked into the function body. At runtime these are just assignments,
 *  executed once per call. Run `x86_64-w64-mingw32-gcc -E -P` on this TU to see the
 *  expanded assignments with no macros present.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------

#ifndef SVG_HARNESS
#error "harness_svg_copy.c is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"          // C_NO_ERR

#include "SPN_definitions.h"    // VAR_ASSIGN row-expansion primitive
#include "harness_can_devs.h"   // gt_can_devs
#include "fixture_signals.h"    // gt_svg (from the active fixture folder via -I)
#include "harness_svg_copy.h"

/* -- Implementation ------------------------------------------------------------------------------------------------ */

// Bridge INPUT direction: copy each SVG2CNTRL row's SVG member -> CNTRL member.
// (gt_svg -> gt_can_devs). Runs at the top of harness_step(), before the logic.
sint16 update_harnessInputs(void)
{
    sint16 s16_error = C_NO_ERR;

    #define SVG2CNTRL(name, CNTRL_VALUE, SVG_VALUE) VAR_ASSIGN((CNTRL_VALUE), (SVG_VALUE));
    #define CNTRL2SVG(name, CNTRL_VALUE, SVG_VALUE) /* not this direction */
    #include "fixture.def"
    #undef SVG2CNTRL
    #undef CNTRL2SVG

    return s16_error;
}

// Bridge OUTPUT direction: copy each CNTRL2SVG row's CNTRL member -> SVG member.
// (gt_can_devs -> gt_svg). Runs at the bottom of harness_step(), after the logic.
sint16 update_harnessOutputs(void)
{
    sint16 s16_error = C_NO_ERR;

    #define SVG2CNTRL(name, CNTRL_VALUE, SVG_VALUE) /* not this direction */
    #define CNTRL2SVG(name, CNTRL_VALUE, SVG_VALUE) VAR_ASSIGN((SVG_VALUE), (CNTRL_VALUE));
    #include "fixture.def"
    #undef SVG2CNTRL
    #undef CNTRL2SVG

    return s16_error;
}

//EOF
