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

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types ----------------------------------------------------------------------------------------------------------- */
/* -- Module Global Variables --------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */
/* -- Implementation ------------------------------------------------------------------------------------------------ */

// Resets gt_can_devs to a known (all-zero) state, then runs the same I/O HAL init
// sequence main.c's real startup does for the in-scope HAL cluster (hw_inputs.c/
// hw_outputs.c) - everything still out of scope for the host build (CAN, fault
// handling, AgvChassis/AgvWork controls) is simply not called.
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

    return s16_Error;
}

// Runs one logic cycle: the same I/O HAL read/write pair main.c's cyclic loop runs
// each pass (update_hwInputs() then update_hwOutputs()), minus everything still out
// of scope for the host build.
sint16 harness_step(void)
{
    sint16 s16_Error;

    s16_Error  = update_hwInputs();
    s16_Error += update_hwOutputs();

    return s16_Error;
}

//======================================================================================================================
// Signal accessors - hand-written, one pair per signal, per CLAUDE.md's Bridge Interface
// (Approach B). First pair: gt_can_devs.t_buttonPanel.u8_b1_state, the confirmed
// injection target's first exposed field (0 = not pressed, 1 = pressed, 3 = fault -
// see hmi_8button_panel.h). RAW by contract - no scaling here, that is the Python
// bridge's job.
//======================================================================================================================

void harness_set_button1_state(const uint8 ou8_value)
{
    gt_can_devs.t_buttonPanel.u8_b1_state = ou8_value;
}

uint8 harness_get_button1_state(void)
{
    return gt_can_devs.t_buttonPanel.u8_b1_state;
}

//EOF
