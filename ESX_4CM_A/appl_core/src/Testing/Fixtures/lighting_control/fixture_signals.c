//----------------------------------------------------------------------------------------------------------------------
/*
 * fixture_signals.c  (lighting_control fixture)
 *
 *  REFERENCE TEMPLATE. Seam-2 round-trip proven 2026-08-11 (see fixture_signals.h).
 *
 *  Defines the single gt_svg instance. Nothing else lives here - the copy logic
 *  is harness mechanism (harness_svg_copy.c), and the accessors are in
 *  fixture_accessors.c. This file is ONLY the storage for the mirror struct.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------

#ifndef SVG_HARNESS
#error "fixture_signals.c is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "fixture_signals.h"

/* -- Module Global Variables --------------------------------------------------------------------------------------- */
T_HarnessSignals gt_svg;   //!< zero-initialized at load; harness_init() does not touch it

//EOF
