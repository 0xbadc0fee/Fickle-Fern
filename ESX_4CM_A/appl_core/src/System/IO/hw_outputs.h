//-----------------------------------------------------------------------------
/**
 * \file       hw_outputs.h
 * \brief      System - Hardware Outputs Module
 *
 * \addtogroup System
 * @{
 * \addtogroup HwOutputs Hardware Outputs
 *
 * The Hardware Outputs module manages the physical pin assignments and
 * direct hardware-level configuration for the controller's outputs.
 * It serves as the low-level interface connecting the logical output
 * handlers to the physical hardware pins on the device.
 *
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Feb 4, 2026 STW Technic
 *
 * @{
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_SYSTEM_IO_HW_OUTPUTS_H_
#define APPL_CORE_SRC_SYSTEM_IO_HW_OUTPUTS_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "output_handler_lib.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_hwOutputs(void);
sint16 update_hwOutputs(void);

#endif /* APPL_CORE_SRC_SYSTEM_IO_HW_OUTPUTS_H_ */

