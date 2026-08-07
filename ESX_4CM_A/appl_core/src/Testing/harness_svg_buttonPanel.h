//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_svg_lib.h
 *
 *  Created on: Aug 7, 2026
 *      Author: silas.curfman
 */

//----------------------------------------------------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_TESTING_HARNESS_SVG_LIB_H_
#define APPL_CORE_SRC_TESTING_HARNESS_SVG_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include <stdbool.h>

#include "SPN_definitions.h"  // Only needed for VAR_ASSIGN macro

/* -- Defines ------------------------------------------------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_CANDevices gt_can_devs; //!< part of seam where DLL SVG connects
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

sint16 update_harnessInputs(void);
sint16 update_harnessOutputs(void);

#endif /* APPL_CORE_SRC_TESTING_HARNESS_SVG_LIB_H_ */
