//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_svg_copy.h   (HARNESS MECHANISM - not fixture-specific)
 *
 *  REFERENCE. Seam-2 round-trip proven 2026-08-11.
 *
 *  Prototypes for the gt_svg <-> gt_can_devs copy step. Called only by
 *  harness_step() in harness_api.c. Internal to the DLL - NOT exported.
 *
 *  Created on: Aug 11, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_TESTING_HARNESS_SVG_COPY_H_
#define APPL_CORE_SRC_TESTING_HARNESS_SVG_COPY_H_

#ifndef SVG_HARNESS
#error "harness_svg_copy.h is a host-DLL harness artifact; build only with -DSVG_HARNESS."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 update_harnessInputs(void);    // gt_svg      -> gt_can_devs   (bridge input)
sint16 update_harnessOutputs(void);   // gt_can_devs -> gt_svg        (bridge output)

#endif /* APPL_CORE_SRC_TESTING_HARNESS_SVG_COPY_H_ */
