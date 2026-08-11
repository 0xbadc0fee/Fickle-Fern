//----------------------------------------------------------------------------------------------------------------------
/*
 * fixture_signals.h  (lighting_control fixture)
 *
 *  REFERENCE TEMPLATE for a per-fixture gt_svg mirror. Seam-2 round-trip proven
 *  2026-08-11 (lighting_control: harness_init -> 0, button04 press flips the
 *  decoded lights byte 0x01 -> 0x10 through the full bridge -> gt_svg ->
 *  gt_can_devs -> update_lightControl -> gt_svg -> bridge path). Copy this
 *  fixture folder as the starting point for the next fixture.
 *
 *  Per-fixture CONTENT for the lighting_control test fixture:
 *    - the fixture signal struct type (T_HarnessSignals)
 *    - the single struct instance gt_svg (defined in fixture_signals.c)
 *
 *  This is the "gt_svg mirror" half of Seam 2: the intermediate struct that sits
 *  between the Python bridge and gt_can_devs. The bridge reads/writes gt_svg; the
 *  harness copy functions move values between gt_svg and gt_can_devs via the .def.
 *
 *  STABLE-SYMBOL DISCIPLINE (load-bearing for the per-fixture-DLL model):
 *  every fixture, regardless of module, exposes the SAME symbol names -
 *    - struct instance : gt_svg
 *    - struct type     : T_HarnessSignals
 *    - copy functions  : update_harnessInputs / update_harnessOutputs
 *  The harness references these fixed names; the build selects which fixture
 *  folder supplies them. Do not rename per fixture.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------
#ifndef FIXTURE_LIGHTING_CONTROL_SIGNALS_H_
#define FIXTURE_LIGHTING_CONTROL_SIGNALS_H_

#ifndef SVG_HARNESS
#error "fixture_signals.h is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Types --------------------------------------------------------------------------------------------------------- */

// The fixture signal mirror. ONE member per signal that crosses the bridge<->DLL
// seam, RAW (no scaling here). Members correspond 1:1 to the rows in
// fixture_lighting_control.def.
//
// Capitalization is deliberately consistent (Keypad01) across both members -
// this fixes the known KeyPad/Keypad split from the button-panel-era .def files.
typedef struct
{
    // SVG2CNTRL signals (bridge writes -> flows into gt_can_devs)
    uint8 u8_svgKeypad01_b04_state;   //!< button 04 soft-key press from the SVG

    // CNTRL2SVG signals (flows out of gt_can_devs -> bridge reads)
    uint8 u8_svgKeypad01_b04_lights;  //!< decoded LED-indicator byte to the SVG

} T_HarnessSignals;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_HarnessSignals gt_svg;   //!< the one fixture signal mirror

#endif /* FIXTURE_LIGHTING_CONTROL_SIGNALS_H_ */
