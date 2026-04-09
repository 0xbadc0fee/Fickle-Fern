//-----------------------------------------------------------------------------
/**
 * \file       fault_handler.h
 * \brief      AgvDiag - Fault Handler
 *
 * \addtogroup AgvDiag
 * @{
 * \addtogroup FaultHandler Fault Handler
 *
 * The Fault Handler manages the detection, logging, and state management of
 * system-wide faults and errors. It interfaces with the diagnostic systems
 * to ensure the machine enters safe operational states when critical
 * issues arise and provides data for troubleshooting.
 *
 * @par Project
 * Flory_8772-4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Feb 20, 2026 kyle.boch
 *
 * @{
 */
#ifndef APPL_CORE_SRC_AGVDIAG_FAULT_HANDLER_H_
#define APPL_CORE_SRC_AGVDIAG_FAULT_HANDLER_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "alarm_handler_lib.h"
#include "input_handler_lib.h"
#include "output_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_faultHandler(void);
sint16 update_faultHandler(void);
sint16 clear_machineFaults(void);

#endif /* APPL_CORE_SRC_AGVDIAG_FAULT_HANDLER_H_ */

