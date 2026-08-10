//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_api.h
 *
 *  Host-DLL harness entry points (Bridge Interface, Approach B - see CLAUDE.md).
 *
 *  Created on: Aug 10, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_TESTING_HARNESS_API_H_
#define APPL_CORE_SRC_TESTING_HARNESS_API_H_

#ifndef SVG_HARNESS
#error "harness_api.h is a host-DLL harness artifact; build only with -DSVG_HARNESS (see CLAUDE.md)."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

// Lifecycle. harness_init() must be called exactly once per DLL load (the
// application's own input/output registration has no reset path and rejects a
// second call - see harness_api.c). To reset state between test cases, reload the
// DLL rather than calling harness_init() twice against the same load.
__declspec(dllexport) sint16 harness_init(void);
__declspec(dllexport) sint16 harness_step(void);

// Signal accessors - hand-written, one pair per signal (see CLAUDE.md's Bridge
// Interface section for why this isn't a generated/table-driven lookup). RAW by
// contract: no scaling here, that is the Python bridge's job.
__declspec(dllexport) void  harness_set_button1_state(uint8 ou8_value);
__declspec(dllexport) uint8 harness_get_button1_state(void);

#ifdef __cplusplus
}
#endif

#endif /* APPL_CORE_SRC_TESTING_HARNESS_API_H_ */
