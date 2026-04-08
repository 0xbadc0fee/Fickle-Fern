//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2025
 * License:   use only under terms of contract / confidential
 * Created:   Dec 9, 2025 kyle.boch
 *
 * \file       hw_inputs.h
 * \brief      Interface for Hardware Inputs Module.
 *
 * \addtogroup System
 * @{
 * \addtogroup HwInputs Hardware Inputs
 * @{
 */
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

