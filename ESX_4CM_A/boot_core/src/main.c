//-----------------------------------------------------------------------------
/*!
   \file
   \brief ESX-4CM-A Template (boot_core)
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "x_memtypes.h"
#include "x_sys.h"
#include "x_sys_rst.h"
#include "x_sys_defs.h"
#include "x_os.h"
#include "x_icc_barrier.h"

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
   .acn_ApplicationName    = "ESX-4CM-A Template (boot_core)",
   .acn_ApplicationVersion = "V2.00r2",
   .u8_LenAdditionalInfo   = OSY_FL_LEN_ADDITIONAL_INFO,
   .acn_AdditionalInfo     = " "
};

// Definition of application heap
MEM_APPL_STATIC_HEAP_INT_DSPR uint8 gau8_DPR_HEAP[4 * 1024];
//MEM_APPL_STATIC_HEAP_EMEM_SRAM uint8 gau8_EMEM_HEAP[1024];    // no l2 heap required

// Global variables for main task configuration
const uint32 gu32_TaskTimerTick_us = 500u;              // Scheduler tick in microseconds, range 500 - 1000000
const uint32 gu32_TaskMainMemClass = X_OS_HEAP_ID_FAST; // Memory class used by this task (target specific:
                                                        // X_OS_HEAP_ID_FAST,X_OS_HEAP_ID_SRAM)
const uint32 gu32_TaskMainStackSize = 1000u;            // Task stack size in byte (8..FFFFFFF8)

/* -- Module Global Variables --------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------- */

/* -- Implementation ------------------------------------------------------ */

//-----------------------------------------------------------------------------
/*!  \brief   Main routine. */
//-----------------------------------------------------------------------------
int main(void)
{
   sint16 s16_Error = C_NO_ERR;
   sint16 s16_Return;
   uint8 u8_ResetRequest;

   // Call this to avoid deadlock in case other cores want to use x_icc_barrier_wait_for()
   // NOTE: this must be placed after creating tasks which will use barriers
   s16_Return = x_icc_barrier_wait_for(X_ICC_BARRIER_ID_MAX, X_ICC_BARRIER_TIMEOUT_NOWAIT);
   if (s16_Return != C_BUSY)
   {
      s16_Error += s16_Return;
   }

   (void)s16_Error;

   do
   {
      u8_ResetRequest = x_sys_rst_ongoing();
   }
   while (u8_ResetRequest == FALSE);

   return 0;
}
