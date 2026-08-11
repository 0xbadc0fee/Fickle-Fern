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
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
        //Local variable
        uint8 u8_testerCounter;
        uint8 u8_testerReset;

        // SVG2CNTRL Signals
        uint8 u8_svgKeypad01_b04_state;

        // CNTRL2SVG Signals
        uint8 u8_svgKeypad01_b04_lights;
}T_HarnessSignalsButtonPanel;


/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_HarnessSignalsButtonPanel gt_svg; //!< part of seam where DLL SVG connects
extern T_CANDevices gt_can_devs;
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

sint16 update_harnessInputs(void);
sint16 update_harnessOutputs(void);

#endif /* APPL_CORE_SRC_TESTING_HARNESS_SVG_LIB_H_ */
