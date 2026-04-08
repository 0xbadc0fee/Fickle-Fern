//-----------------------------------------------------------------------------
/*
 * Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Feb 4, 2026 STW Technic
 *
 * \file       hw_outputs.h
 * \brief      Interface for Hardware Outputs Module.
 *
 * \addtogroup System
 * @{
 * \addtogroup HwOutputs Hardware Outputs
 * @{
 */
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

