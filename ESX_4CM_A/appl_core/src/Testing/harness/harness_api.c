//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_api.c   (Seam 2)
 *
 *  REFERENCE. Seam-2 round-trip proven 2026-08-11 against the lighting_control
 *  fixture (see Fixtures/lighting_control/fixture_signals.h and
 *  reqs/EDR_seam2_lighting_control.md).
 *
 *  Lifecycle + step MECHANISM. Fixture-agnostic: this file contains no signal
 *  names and does not change per fixture. Signal specifics live in the fixture
 *  folder (fixture.def, fixture_signals.*, fixture_accessors.c).
 *
 *  SEAM 2, not Seam 1: harness_step() routes signals through the gt_svg mirror via
 *  update_harnessInputs()/update_harnessOutputs(). The accessors write/read gt_svg
 *  (fixture_accessors.c); the copy functions bridge gt_svg <-> gt_can_devs
 *  (harness_svg_copy.c). This file just sequences them.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------

#ifndef SVG_HARNESS
#error "harness_api.c is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <string.h>             // memset

#include "harness_api.h"
#include "harness_can_devs.h"   // gt_can_devs
#include "harness_svg_copy.h"   // update_harnessInputs / update_harnessOutputs
#include "hw_inputs.h"          // init_hwInputs / update_hwInputs        VERIFY name vs repo
#include "hw_outputs.h"         // init_hwOutputs / update_hwOutputs      VERIFY name vs repo
#include "lighting_control.h"   // init_lightControl / update_lightControl  (logic under test)

/* -- Implementation ------------------------------------------------------------------------------------------------ */

// See the current-tree harness_api.c for the full init-ordering rationale. Summary:
// memset gt_can_devs FIRST (init_lightControl captures pointers INTO the struct, so
// it must run against already-cleared memory), then HAL init, then logic init.
// Not safe to call twice per load (HAL registration has no reset path).
sint16 harness_init(void)
{
    sint16 s16_Error;

    memset(&gt_can_devs, 0, sizeof(gt_can_devs));

    s16_Error  = init_hwInputs();
    s16_Error += init_hwOutputs();
    s16_Error += init_lightControl(&gt_can_devs);

    return s16_Error;
}

// One host logic cycle (Seam 2). See harness_api.h for the read->execute->write
// shape. NOTE: this is the PROOF-STAGE reduced body - it runs the in-scope HAL and
// the single logic-under-test (update_lightControl). The full-superloop version
// (all in-scope update_*() calls, discovered by which symbols link) is a SEPARATE,
// LATER step - do not expand it here until the round trip is green with this
// minimal body.
sint16 harness_step(void)
{
    sint16 s16_Error;

    s16_Error  = update_harnessInputs();    // gt_svg -> gt_can_devs  (stage bridge inputs)

    s16_Error += update_hwInputs();         // in-scope input HAL read
    s16_Error += update_lightControl();     // THE LOGIC UNDER TEST
    s16_Error += update_hwOutputs();        // in-scope output HAL write

    s16_Error += update_harnessOutputs();   // gt_can_devs -> gt_svg  (publish bridge outputs)

    return s16_Error;
}

//EOF
