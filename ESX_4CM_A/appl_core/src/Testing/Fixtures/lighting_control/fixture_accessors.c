//----------------------------------------------------------------------------------------------------------------------
/*
 * fixture_accessors.c  (lighting_control fixture)
 *
 *  REFERENCE TEMPLATE. Seam-2 round-trip proven 2026-08-11 (see fixture_signals.h).
 *
 *  HAND-WRITTEN accessors - the DLL's exported signal surface. This is a LOCKED
 *  project decision: accessors are hand-written per fixture, NOT generated from a
 *  third .def macro expansion. Rationale: the export surface is the contract the
 *  Python bridge's cffi layer binds against; keeping it hand-written and plainly
 *  readable keeps that contract debuggable, and a broken copy-paste fails loudly
 *  at compile rather than obscurely inside a macro expansion.
 *
 *  HOW TO ADD A SIGNAL (the whole task, for the next developer):
 *  For each row in fixture.def, write ONE accessor, in the same order as the .def:
 *    - SVG2CNTRL row (bridge input)  -> a  void harness_set_<tag>(<width> v)
 *                                        that writes the gt_svg member.
 *    - CNTRL2SVG row (bridge output) -> a  <width> harness_get_<tag>(void)
 *                                        that returns the gt_svg member.
 *  Match <width> to the gt_svg member type (uint8 / uint16 / uint32).
 *  Accessors touch gt_svg ONLY - never gt_can_devs directly. The copy functions
 *  (harness_svg_copy.c) are what bridge gt_svg <-> gt_can_devs.
 *  Values cross RAW - no scaling here (that is the Python bridge's job).
 *
 *  Keep the .def row's friendly-name tag in the comment above each accessor so a
 *  visual scan down .def and this file confirms 1:1 coverage.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------

#ifndef SVG_HARNESS
#error "fixture_accessors.c is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "fixture_signals.h"   // gt_svg
#include "harness_api.h"       // export declarations (dllexport prototypes)

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//-- .def row: SVG2CNTRL(button04_state, ...) ---------------------------------------------------------------------------
// Bridge INPUT. uint8. Writes the soft-key press into the fixture mirror.
// 0 = not pressed, 1 = pressed, 3 = fault (any nonzero treated as pressed by the logic).
void harness_set_button04_state(const uint8 ou8_value)
{
    gt_svg.u8_svgKeypad01_b04_state = ou8_value;
}

//-- .def row: CNTRL2SVG(button04_lights, ...) -------------------------------------------------------------------------
// Bridge OUTPUT. uint8. Returns the decoded LED-indicator byte from the fixture mirror.
// Packed 2 bits per color (RED 0-1, AMBER 2-3, GREEN 4-5, BLUE 6-7; 00 off/01 on/10 flash).
// Crosses RAW - the SVG/bridge decodes the packing, not this accessor.
uint8 harness_get_button04_lights(void)
{
    return gt_svg.u8_svgKeypad01_b04_lights;
}

//EOF
