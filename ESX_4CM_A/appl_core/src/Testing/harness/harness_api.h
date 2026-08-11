//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_api.h   (Seam 2)
 *
 *  REFERENCE. Seam-2 round-trip proven 2026-08-11.
 *
 *  Host-DLL harness entry points. Under Seam 2 the exported surface is:
 *    - lifecycle : harness_init, harness_step            (mechanism, fixed)
 *    - accessors : harness_set_<tag> / harness_get_<tag> (per fixture, hand-written)
 *
 *  The accessor DECLARATIONS live here but their DEFINITIONS live in the fixture
 *  folder (fixture_accessors.c). This header therefore has a fixture-specific
 *  section that changes per fixture. That is the one place harness_api is not
 *  fully fixture-agnostic under the per-fixture-DLL model - acceptable because each
 *  DLL is built for exactly one fixture.
 *
 *  ALL exports are dllexport; -Wl,--exclude-all-symbols keeps everything else hidden.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_TESTING_HARNESS_API_H_
#define APPL_CORE_SRC_TESTING_HARNESS_API_H_

#ifndef SVG_HARNESS
#error "harness_api.h is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

//== Lifecycle (MECHANISM - identical for every fixture) ===============================================================

// Call exactly once per DLL load. memset gt_can_devs, init the in-scope I/O HAL,
// init the logic-under-test against the cleared struct. No reset path - to reset
// between test cases, reload the DLL. (init_hwInputs/Outputs reject a second call.)
__declspec(dllexport) sint16 harness_init(void);

// One logic cycle (the host analogue of main.c's superloop body, in scope only):
//   update_harnessInputs()  gt_svg   -> gt_can_devs   (stage bridge inputs)
//   <the in-scope update_*() logic body>              (THE LOGIC UNDER TEST)
//   update_harnessOutputs() gt_can_devs -> gt_svg     (publish bridge outputs)
// The bridge stages inputs via harness_set_* before calling, and reads results via
// harness_get_* after it returns. The bridge owns the cyclic cadence - there is no
// do-while in the DLL.
__declspec(dllexport) sint16 harness_step(void);

//== Accessors (PER FIXTURE - hand-written in fixture_accessors.c) ======================================================
// One accessor per fixture.def row. SVG2CNTRL -> set; CNTRL2SVG -> get. RAW, no
// scaling. See fixture_accessors.c for the add-a-signal recipe.
//
// --- lighting_control fixture ---
__declspec(dllexport) void  harness_set_button04_state(uint8 ou8_value);   // SVG2CNTRL(button04_state)
__declspec(dllexport) uint8 harness_get_button04_lights(void);             // CNTRL2SVG(button04_lights)

#ifdef __cplusplus
}
#endif

#endif /* APPL_CORE_SRC_TESTING_HARNESS_API_H_ */
