//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_api.c
 *
 *  Host-DLL harness entry points (Bridge Interface, Approach B - see CLAUDE.md).
 *  These are the ONLY symbols this DLL exports (enforced by -Wl,--exclude-all-symbols
 *  in host_harness/CMakeLists.txt) - everything else stays hidden from the Python
 *  bridge, which must go through these accessor functions rather than reading raw
 *  struct memory.
 *
 *  Hand-written, not generated - see CLAUDE.md's Bridge Interface section for why.
 *  Keep this file small: one accessor pair per signal actually needed for a test,
 *  added deliberately, not via a lookup table or macro layer.
 *
 *  Created on: Aug 10, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------

#ifndef SVG_HARNESS
#error "harness_api.c is a host-DLL harness artifact; build only with -DSVG_HARNESS (see CLAUDE.md)."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <string.h> // memset - harness_init() reset below

#include "harness_api.h"
#include "harness_can_devs.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "lighting_control.h"   // init_lightControl() / update_lightControl() - the AgvChassis logic under test

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types ----------------------------------------------------------------------------------------------------------- */
/* -- Module Global Variables --------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */
/* -- Implementation ------------------------------------------------------------------------------------------------ */

// Resets gt_can_devs to a known (all-zero) state, then runs the same init sequence
// main.c's real startup does for the subset of the application in scope for the host
// build: the in-scope I/O HAL cluster (hw_inputs.c/hw_outputs.c) plus the one AgvChassis
// control under test (lighting_control.c). Everything still out of scope for the host
// build (CAN, fault handling, the other AgvChassis/AgvWork controls) is simply not called.
//
// NOTE on init ordering vs. main.c: on target, init_lightControl() runs in main.c's init
// block and captures pointers INTO gt_can_devs (its RX pointer -> t_buttonPanel.u8_b4_state,
// its TX pointers -> t_buttonPanel.u8_b4_lights and t_display members). Because those are
// pointers into the struct - not copies - the memset MUST happen before init_lightControl(),
// otherwise init captures addresses into memory we then zero. Order below is deliberate:
// memset first, then init the HAL, then init the lighting control against the already-cleared
// struct. This matches target semantics (struct exists and is stable for the life of the run;
// init captures addresses once).
//
// NOT safe to call more than once per DLL load: init_hwInputs()/init_hwOutputs()
// register each hardware channel by calling add_hwInput()/add_hwOutput() against a
// monotonically-growing static counter in input_handler_lib.c/output_handler_lib.c
// that only ever increments, with no reset path in the application's own code - a
// second call re-registers the same hardware IDs, which add_hwInput()'s own
// duplicate-detection logic correctly rejects (confirmed: returns C_INPUT_INIT_HW_FAIL,
// -20, on a second call). This mirrors the real controller's own assumption that init
// runs exactly once per boot; it is not a harness-introduced limitation. To get a clean
// slate between test cases, reload the DLL (fresh process, or FreeLibrary + re-load);
// do not call harness_init() a second time against the same load.
sint16 harness_init(void)
{
    sint16 s16_Error;

    memset(&gt_can_devs, 0, sizeof(gt_can_devs));

    s16_Error  = init_hwInputs();
    s16_Error += init_hwOutputs();

    // Bring the AgvChassis lighting control online against the (now-zeroed) device struct.
    // Must run AFTER the memset (see note above) and after the output HAL init, since
    // update_lightControl() calls get_outputFaultStatus()/set_outputValue() on the named
    // "HEADLIGHTS"/"WORKLIGHTS"/"TAILLIGHTS" outputs.
    s16_Error += init_lightControl(&gt_can_devs);

    return s16_Error;
}

// Runs one logic cycle for the lighting-control test path:
//   1. (input already staged) - the Python bridge has written u8_b4_state via
//      harness_set_button4_state() before calling step.
//   2. update_hwInputs()      - same input HAL read main.c runs each pass.
//   3. update_lightControl()  - THE LOGIC UNDER TEST. Reads t_buttonPanel.u8_b4_state
//      (via its RX pointer captured at init), runs the toggle/mode state machine, and
//      writes the decoded LED byte back to t_buttonPanel.u8_b4_lights (via its TX pointer).
//   4. update_hwOutputs()     - same output HAL write main.c runs each pass.
// After step returns, the bridge reads the result via harness_get_button4_lights().
//
// This is the host-DLL analogue of main.c's superloop body, reduced to only what is in
// scope. It does NOT go through the gt_svg / .def seam used by harness_svg_lightControlTest.c
// - that seam is the ON-TARGET superloop's mechanism. On the host DLL, the accessor
// functions below ARE the seam; routing through gt_svg as well would double-copy the same
// signal. One seam, not two.
sint16 harness_step(void)
{
    sint16 s16_Error;

    s16_Error  = update_hwInputs();
    s16_Error += update_lightControl();
    s16_Error += update_hwOutputs();

    return s16_Error;
}

//======================================================================================================================
// Signal accessors - hand-written, one pair per signal, per CLAUDE.md's Bridge Interface
// (Approach B). RAW by contract - no scaling here, that is the Python bridge's job.
//======================================================================================================================

//-- Button 1 (free/unmapped) - original plumbing PoC pair. Kept for the bring-up smoke test.
//   0 = not pressed, 1 = pressed, 3 = fault (see hmi_8button_panel.h).
void harness_set_button1_state(const uint8 ou8_value)
{
    gt_can_devs.t_buttonPanel.u8_b1_state = ou8_value;
}

uint8 harness_get_button1_state(void)
{
    return gt_can_devs.t_buttonPanel.u8_b1_state;
}

//-- Button 4 (J1939 8-button HMI, lighting control) - functional-test pair.
//   Input : u8_b4_state  - the soft-key press the lighting state machine reads
//           (0 = not pressed, 1 = pressed, 3 = fault; the logic treats any nonzero as pressed).
//   Output: u8_b4_lights - the decoded LED-indicator byte the lighting logic writes.
//           Packed 2 bits per color per hmi_8button_panel.h: RED bits0-1, AMBER bits2-3,
//           GREEN bits4-5, BLUE bits6-7 (00 off / 01 on / 10 flash). Decode is the
//           bridge/SVG's job - RAW byte crosses here.
void harness_set_button4_state(const uint8 ou8_value)
{
    gt_can_devs.t_buttonPanel.u8_b4_state = ou8_value;
}

uint8 harness_get_button4_lights(void)
{
    return gt_can_devs.t_buttonPanel.u8_b4_lights;
}

//EOF
