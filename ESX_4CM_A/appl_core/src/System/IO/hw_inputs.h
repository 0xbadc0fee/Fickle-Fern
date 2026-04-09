//-----------------------------------------------------------------------------
/**
 * \file       hw_inputs.h
 * \brief      System - Hardware Inputs Module
 *
 * \addtogroup System
 * @{
 * \addtogroup HwInputs Hardware Inputs
 *
 * The Hardware Inputs module manages the physical pin assignments and
 * direct hardware-level reading for the controller's inputs. It serves
 * as the low-level interface connecting physical sensors, switches,
 * and signals to the logical input handlers within the system.
 *
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2025
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Dec 9, 2025 kyle.boch
 *
 * @{
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_SYSTEM_IO_HW_INPUTS_H_
#define APPL_CORE_SRC_SYSTEM_IO_HW_INPUTS_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "input_handler_lib.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_hwInputs(void);
sint16 update_hwInputs(void);

#endif /* APPL_CORE_SRC_SYSTEM_IO_HW_INPUTS_H_ */

