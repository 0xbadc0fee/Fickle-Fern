//-----------------------------------------------------------------------------
/*!
   \file
   \brief ESX-4CM-A Template (safety_core)
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "x_memtypes.h"
#include "x_sys.h"
#include "x_sys_rst.h"
#include "x_sys_defs.h"
#include "x_icc_barrier.h"
#include "osy_srv.h"
#include "x_os.h"

#include "x_in_server.h"
#include "x_out_server.h"

/* -- Defines ------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------- */
const X_MEM_APPLICATION_INFO T_x_sys_application_information gt_ApplicationInformation =
{
   .acn_Magic              = X_SYS_INFO_APPL_MAGIC,
   .u8_StructVersion       = X_SYS_STRUCT_VERSION,
   .acn_Devicename         = X_SYS_DEVICE_NAME,
   .acn_Date               = __DATE__,
   .acn_Time               = __TIME__,
   .acn_ApplicationName    = "ESX-4CM-A Template (safe_core)",
   .acn_ApplicationVersion = "V2.00r2",
   .u8_LenAdditionalInfo   = OSY_FL_LEN_ADDITIONAL_INFO,
   .acn_AdditionalInfo     = " "
};

// Definition of application heap
MEM_APPL_STATIC_HEAP_INT_DSPR uint8 gau8_DPR_HEAP[1024 * 50];
MEM_APPL_STATIC_HEAP_EMEM_SRAM uint8 gau8_EMEM_HEAP[1024 * 50];

// Global variables for main task configuration
const uint32 gu32_TaskTimerTick_us = 500u;              // Scheduler tick in microseconds, range 500 - 1000000
const uint32 gu32_TaskMainMemClass = X_OS_HEAP_ID_FAST; // Memory class used by this task (target specific:
                                                        // X_OS_HEAP_ID_FAST,X_OS_HEAP_ID_SRAM)
const uint32 gu32_TaskMainStackSize = 5000u;            // Task stack size in byte (8..FFFFFFF8)
const uint8 gu8_CpuCacheDisable = 0u;                   // Disable/enable CPU cache (default: enabled)

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Main routine. */
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
   sint16 s16_Error;
   sint16 s16_Return;
   uint8 u8_ResetRequest;
   uint8 u8_OnOff;
   const T_x_sys_rst_reason t_Reset =
   {
      .u8_Core = X_SYS_ID_CORE1,
      .u32_Reason = X_SYS_RST_REASON_SW,
      .u32_SwReason = X_SYS_RST_SW_REASON_APPL1,
      .u32_Information = 0u
   };

   s16_Error  = x_in_server_lock_allocations();
   s16_Error += x_out_server_lock_allocations();

   // Install openSYDE RTOS tasks
   s16_Error += osy_srv_init();

   // Call this to avoid deadlock in case other cores want to use x_icc_barrier_wait_for()
   // NOTE: this must be placed after creating tasks which will use barriers
   s16_Return = x_icc_barrier_wait_for(X_ICC_BARRIER_ID_MAX, X_ICC_BARRIER_TIMEOUT_NOWAIT);
   if (s16_Return != C_BUSY)
   {
      s16_Error += s16_Return;
   }

   s16_Error += x_sys_stay_alive(X_ON);

   do
   {
      // On safety core the running task has to release the CPU for a certain amount of time to do the low prior ICC
      // signal handling. It is not allowed to have a task which never releases the CPU (e.g. an infinite empty loop).
      x_os_sleep(10u);

      u8_ResetRequest = x_sys_rst_ongoing();

      s16_Error = x_sys_get_ignition(&u8_OnOff);
      if(u8_OnOff == X_OFF)
      {
    	  s16_Error = x_sys_close();
      }
   }
   while (u8_ResetRequest == FALSE);

   s16_Error += x_sys_rst_force_v2(&t_Reset);

   (void)s16_Error;

   return 0;
}
