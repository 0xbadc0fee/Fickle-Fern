//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief openSYDE Server Initialization Wrapper
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <string.h>
#include "osy_dph_init.h"
#include "osy_dph_data_set_handler.h"
#include "osy_srv.h"
#include "osy_init.h"

#include "x_icc_barrier.h"
#include "x_os.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define NUM_DATA_POOLS        (2u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- (Module) Global Variables ------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
static sint16 m_osy_srv_init_1(void);
static sint16 m_osy_srv_init_2(void);
static void m_osy_srv_task(void * const opv_TaskParameter);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//------------------------------------------------------------------------------
/*! \brief Installs a new RTOS task, that executes the DPD.                   */
//------------------------------------------------------------------------------
sint16 osy_srv_init(void)
{
   sint16 s16_Error;
   T_x_os_task * pt_TaskHandle;
   const T_x_os_task_config t_TaskConfig =
   {
      .u16_Priority = OSY_SRV_DPD_TASK_PRIO,
      .pr_Task = &m_osy_srv_task,
      .pv_Parameter = NULL,
      .u32_TimeDelay = OSY_SRV_DPD_TASK_DELAY | 1u, //0 means suspended
      .u32_TimeInterval = OSY_SRV_DPD_TASK_CYCLE,
      .u32_StackSize = 16uL * 1024u,
      .u32_MemClass = X_OS_HEAP_ID_SRAM,
      .u32_ConfigFlags = 0uL,
      .u16_SafetyLevel = X_OS_TASK_SAFETY_LEVEL_SIL2
   };

   s16_Error  = m_osy_srv_init_1();
   s16_Error += x_icc_barrier_wait_for(2u, X_ICC_BARRIER_TIMEOUT_INFINITE);
   s16_Error += m_osy_srv_init_2();
   s16_Error += x_os_install_task(&t_TaskConfig, NULL, &pt_TaskHandle);

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief 1st step of openSYDE server initialization.

   This function initialize the data pool access server.
   It exports its data pools to be imported by the diagnostic protocol driver.
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_osy_srv_init_1(void)
{
   sint16 s16_Error = C_NO_ERR;
   const T_osy_dpa_data_pool * const * const apt_DataPools = osy_dph_get_init_config();
   const uint8 u8_NumDP = osy_dph_get_num_data_pools();

   if (u8_NumDP > 0u)
   {
      // Init data pools
      s16_Error = osy_dph_init_data_pools(apt_DataPools, u8_NumDP);
   }

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief 2nd step of openSYDE server initialization.

   This function is intended for the diagnostic protocol driver DPD, to import the exported data pools from other cores.
   On all cores other than the one, that runs the DPD, this function is typically empty.
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_osy_srv_init_2(void)
{
   return C_NO_ERR;
}

//------------------------------------------------------------------------------
/*! \brief openSYDE main task.                                                */
//------------------------------------------------------------------------------
static void m_osy_srv_task(void * const opv_TaskParameter)
{
   //run until a reset request was issued
   while (1 /*!x_sys_rst_ongoing()*/)
   {
      //wait for next period
      x_os_wait_period();
   }
}
