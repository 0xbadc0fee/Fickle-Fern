//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief openSYDE Server Initialization Wrapper
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_SRV_H
#define OSY_SRV_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define OSY_SRV_DPD_TASK_PRIO         (5u)    //task priority of DPD task
#define OSY_SRV_DPD_TASK_CYCLE        (10u)   //task cycle time (in RTOS ticks) of DPD task
#define OSY_SRV_DPD_TASK_DELAY        (4000u) //task delay

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

//install openSYDE RTOS task - requires an initialized RTOS.
sint16 osy_srv_init(void);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
