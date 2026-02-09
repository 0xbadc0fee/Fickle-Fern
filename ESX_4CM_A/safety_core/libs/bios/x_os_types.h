//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Generic x_os types used in all software layers
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_TYPES_H
#define X_OS_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_os_msg.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

// OS init states
//! Parts of OS state
#define X_OS_NOT_INIT               (0u) //!< OS is not initialized
#define X_OS_INIT                   (1u) //!< OS is initialized but not started
#define X_OS_RUNNING                (2u) //!< OS is initialized and running

#define X_OS_TASK_FLAGREG_NUM       (4u)                                             //!< number of task flag registers
#define X_OS_TASK_FLAGREG_SIZE      (32u)                                            //!< size of task flag register
#define X_OS_MAX_TASK               (X_OS_TASK_FLAGREG_NUM * X_OS_TASK_FLAGREG_SIZE) //!< max. number of tasks
#define X_OS_MAX_TASK_PRIO          (X_OS_MAX_TASK - 1)                              //!< max. task priority

#define X_OS_NOWAIT                 (0xFFFFFFFFu) //!< don't wait

// send options for x_os_mbox_send_message:
//! \brief Options for MBOX send (bit coded)
#define X_OS_MBOX_SEND_DEFAULT      (0u) //!< send message to waiting task with highest priority
#define X_OS_MBOX_SEND_BROADCAST    (1u) //!< send message broadcast to any waiting tasks

/*! \brief bit coded mode options for x_os_wait_for_signal:
   X_OS_SIGNAL_MODE_RESET may be ORed with the other settings
*/
#define X_OS_SIGNAL_MODE_ANY        (0u) //!< signal mode: any signal bits must be set
#define X_OS_SIGNAL_MODE_ALL        (1u) //!< signal mode: all signal bits must be set
#define X_OS_SIGNAL_MODE_RESET      (2u) //!< signal mode: reset all signals used by the calling task

//! \brief event bits are bit coded
#define X_OS_GLOBAL_EVENT_01               (0x00000001u)
#define X_OS_GLOBAL_EVENT_02               (0x00000002u)
#define X_OS_GLOBAL_EVENT_03               (0x00000004u)
#define X_OS_GLOBAL_EVENT_04               (0x00000008u)
#define X_OS_GLOBAL_EVENT_05               (0x00000010u)
#define X_OS_GLOBAL_EVENT_06               (0x00000020u)
#define X_OS_GLOBAL_EVENT_07               (0x00000040u)
#define X_OS_GLOBAL_EVENT_08               (0x00000080u)
#define X_OS_GLOBAL_EVENT_09               (0x00000100u)
#define X_OS_GLOBAL_EVENT_10               (0x00000200u)
#define X_OS_GLOBAL_EVENT_11               (0x00000400u)
#define X_OS_GLOBAL_EVENT_12               (0x00000800u)
#define X_OS_GLOBAL_EVENT_13               (0x00001000u)
#define X_OS_GLOBAL_EVENT_14               (0x00002000u)
#define X_OS_GLOBAL_EVENT_15               (0x00004000u)
#define X_OS_GLOBAL_EVENT_16               (0x00008000u)
#define X_OS_GLOBAL_EVENT_17               (0x00010000u)
#define X_OS_GLOBAL_EVENT_18               (0x00020000u)
#define X_OS_GLOBAL_EVENT_19               (0x00040000u)
#define X_OS_GLOBAL_EVENT_20               (0x00080000u)
#define X_OS_GLOBAL_EVENT_21               (0x00100000u)
#define X_OS_GLOBAL_EVENT_22               (0x00200000u)
#define X_OS_GLOBAL_EVENT_23               (0x00400000u)
#define X_OS_GLOBAL_EVENT_24               (0x00800000u)
#define X_OS_GLOBAL_EVENT_25               (0x01000000u)
#define X_OS_GLOBAL_EVENT_26               (0x02000000u)
#define X_OS_GLOBAL_EVENT_27               (0x04000000u)
#define X_OS_GLOBAL_EVENT_28               (0x08000000u)
#define X_OS_GLOBAL_EVENT_29               (0x10000000u)
#define X_OS_GLOBAL_EVENT_30               (0x20000000u)
#define X_OS_GLOBAL_EVENT_31               (0x40000000u)
#define X_OS_GLOBAL_EVENT_32               (0x80000000u)

//! \brief defines for task watchdog status value
#define X_OS_TASK_WD_OVERFLOW       (0x0001u) //!< task watchdog trigger time overflow
#define X_OS_TASK_WD_UNDERFLOW      (0x0002u) //!< task watchdog trigger time underflow
#define X_OS_TASK_WD_FIRST_TRIGGER  (0x0004u) /*!< task watchdog first trigger time overflow
                                                   (cf. T_x_os_task_wd_cfg.u16_WDFirstTrigger_ms*/

//! \brief defines for user event flags
#define X_OS_TASK_EVENT_MAX         (24u)         //!< max. number of user event flags
#define X_OS_TASK_EVENT_MASK        (0x00FFFFFFu) //!< user event flag mask
#define X_OS_TASK_EVENT_01          (0x00000001u)
#define X_OS_TASK_EVENT_02          (0x00000002u)
#define X_OS_TASK_EVENT_03          (0x00000004u)
#define X_OS_TASK_EVENT_04          (0x00000008u)
#define X_OS_TASK_EVENT_05          (0x00000010u)
#define X_OS_TASK_EVENT_06          (0x00000020u)
#define X_OS_TASK_EVENT_07          (0x00000040u)
#define X_OS_TASK_EVENT_08          (0x00000080u)
#define X_OS_TASK_EVENT_09          (0x00000100u)
#define X_OS_TASK_EVENT_10          (0x00000200u)
#define X_OS_TASK_EVENT_11          (0x00000400u)
#define X_OS_TASK_EVENT_12          (0x00000800u)
#define X_OS_TASK_EVENT_13          (0x00001000u)
#define X_OS_TASK_EVENT_14          (0x00002000u)
#define X_OS_TASK_EVENT_15          (0x00004000u)
#define X_OS_TASK_EVENT_16          (0x00008000u)
#define X_OS_TASK_EVENT_17          (0x00010000u)
#define X_OS_TASK_EVENT_18          (0x00020000u)
#define X_OS_TASK_EVENT_19          (0x00040000u)
#define X_OS_TASK_EVENT_20          (0x00080000u)
#define X_OS_TASK_EVENT_21          (0x00100000u)
#define X_OS_TASK_EVENT_22          (0x00200000u)
#define X_OS_TASK_EVENT_23          (0x00400000u)
#define X_OS_TASK_EVENT_24          (0x00800000u)
#define X_OS_TASK_EVENT_TIMER       (0x01000000u) //!< reserved user event used for the default task timer
#define X_OS_TASK_EVENT_TIMEOUT     (0x02000000u) //!< reserved user event used for the timeout timer
#define X_OS_TASK_EVENT_MESSAGE     (0x04000000u) //!< reserved user event used for message queues

//! \brief defines for wait for user event result type
#define X_OS_WAIT_RESULT_NONE       (0x00000000u)
#define X_OS_WAIT_RESULT_TIMEOUT    (0x00000001u)
#define X_OS_WAIT_RESULT_EVENT      (0x00000002u)
#define X_OS_WAIT_RESULT_MESSAGE    (0x00000004u)

// Deprecated macros

/*PRQA S 2217  ++*/ /*STW_MD4_2217_FORMATTING_RULE_SUPPRESSION_BEGIN*/
//! \brief event bits are bit coded
#define X_OS_EVENT_01   _Pragma("GCC warning \"'X_OS_EVENT_01' macro is deprecated, use 'X_OS_GLOBAL_EVENT_01'\"") X_OS_GLOBAL_EVENT_01
#define X_OS_EVENT_02   _Pragma("GCC warning \"'X_OS_EVENT_02' macro is deprecated, use 'X_OS_GLOBAL_EVENT_02'\"") X_OS_GLOBAL_EVENT_02
#define X_OS_EVENT_03   _Pragma("GCC warning \"'X_OS_EVENT_03' macro is deprecated, use 'X_OS_GLOBAL_EVENT_03'\"") X_OS_GLOBAL_EVENT_03
#define X_OS_EVENT_04   _Pragma("GCC warning \"'X_OS_EVENT_04' macro is deprecated, use 'X_OS_GLOBAL_EVENT_04'\"") X_OS_GLOBAL_EVENT_04
#define X_OS_EVENT_05   _Pragma("GCC warning \"'X_OS_EVENT_05' macro is deprecated, use 'X_OS_GLOBAL_EVENT_05'\"") X_OS_GLOBAL_EVENT_05
#define X_OS_EVENT_06   _Pragma("GCC warning \"'X_OS_EVENT_06' macro is deprecated, use 'X_OS_GLOBAL_EVENT_06'\"") X_OS_GLOBAL_EVENT_06
#define X_OS_EVENT_07   _Pragma("GCC warning \"'X_OS_EVENT_07' macro is deprecated, use 'X_OS_GLOBAL_EVENT_07'\"") X_OS_GLOBAL_EVENT_07
#define X_OS_EVENT_08   _Pragma("GCC warning \"'X_OS_EVENT_08' macro is deprecated, use 'X_OS_GLOBAL_EVENT_08'\"") X_OS_GLOBAL_EVENT_08
#define X_OS_EVENT_09   _Pragma("GCC warning \"'X_OS_EVENT_09' macro is deprecated, use 'X_OS_GLOBAL_EVENT_09'\"") X_OS_GLOBAL_EVENT_09
#define X_OS_EVENT_10   _Pragma("GCC warning \"'X_OS_EVENT_10' macro is deprecated, use 'X_OS_GLOBAL_EVENT_10'\"") X_OS_GLOBAL_EVENT_10
#define X_OS_EVENT_11   _Pragma("GCC warning \"'X_OS_EVENT_11' macro is deprecated, use 'X_OS_GLOBAL_EVENT_11'\"") X_OS_GLOBAL_EVENT_11
#define X_OS_EVENT_12   _Pragma("GCC warning \"'X_OS_EVENT_12' macro is deprecated, use 'X_OS_GLOBAL_EVENT_12'\"") X_OS_GLOBAL_EVENT_12
#define X_OS_EVENT_13   _Pragma("GCC warning \"'X_OS_EVENT_13' macro is deprecated, use 'X_OS_GLOBAL_EVENT_13'\"") X_OS_GLOBAL_EVENT_13
#define X_OS_EVENT_14   _Pragma("GCC warning \"'X_OS_EVENT_14' macro is deprecated, use 'X_OS_GLOBAL_EVENT_14'\"") X_OS_GLOBAL_EVENT_14
#define X_OS_EVENT_15   _Pragma("GCC warning \"'X_OS_EVENT_15' macro is deprecated, use 'X_OS_GLOBAL_EVENT_15'\"") X_OS_GLOBAL_EVENT_15
#define X_OS_EVENT_16   _Pragma("GCC warning \"'X_OS_EVENT_16' macro is deprecated, use 'X_OS_GLOBAL_EVENT_16'\"") X_OS_GLOBAL_EVENT_16
#define X_OS_EVENT_17   _Pragma("GCC warning \"'X_OS_EVENT_17' macro is deprecated, use 'X_OS_GLOBAL_EVENT_17'\"") X_OS_GLOBAL_EVENT_17
#define X_OS_EVENT_18   _Pragma("GCC warning \"'X_OS_EVENT_18' macro is deprecated, use 'X_OS_GLOBAL_EVENT_18'\"") X_OS_GLOBAL_EVENT_18
#define X_OS_EVENT_19   _Pragma("GCC warning \"'X_OS_EVENT_19' macro is deprecated, use 'X_OS_GLOBAL_EVENT_19'\"") X_OS_GLOBAL_EVENT_19
#define X_OS_EVENT_20   _Pragma("GCC warning \"'X_OS_EVENT_20' macro is deprecated, use 'X_OS_GLOBAL_EVENT_20'\"") X_OS_GLOBAL_EVENT_20
#define X_OS_EVENT_21   _Pragma("GCC warning \"'X_OS_EVENT_21' macro is deprecated, use 'X_OS_GLOBAL_EVENT_21'\"") X_OS_GLOBAL_EVENT_21
#define X_OS_EVENT_22   _Pragma("GCC warning \"'X_OS_EVENT_22' macro is deprecated, use 'X_OS_GLOBAL_EVENT_22'\"") X_OS_GLOBAL_EVENT_22
#define X_OS_EVENT_23   _Pragma("GCC warning \"'X_OS_EVENT_23' macro is deprecated, use 'X_OS_GLOBAL_EVENT_23'\"") X_OS_GLOBAL_EVENT_23
#define X_OS_EVENT_24   _Pragma("GCC warning \"'X_OS_EVENT_24' macro is deprecated, use 'X_OS_GLOBAL_EVENT_24'\"") X_OS_GLOBAL_EVENT_24
#define X_OS_EVENT_25   _Pragma("GCC warning \"'X_OS_EVENT_25' macro is deprecated, use 'X_OS_GLOBAL_EVENT_25'\"") X_OS_GLOBAL_EVENT_25
#define X_OS_EVENT_26   _Pragma("GCC warning \"'X_OS_EVENT_26' macro is deprecated, use 'X_OS_GLOBAL_EVENT_26'\"") X_OS_GLOBAL_EVENT_26
#define X_OS_EVENT_27   _Pragma("GCC warning \"'X_OS_EVENT_27' macro is deprecated, use 'X_OS_GLOBAL_EVENT_27'\"") X_OS_GLOBAL_EVENT_27
#define X_OS_EVENT_28   _Pragma("GCC warning \"'X_OS_EVENT_28' macro is deprecated, use 'X_OS_GLOBAL_EVENT_28'\"") X_OS_GLOBAL_EVENT_28
#define X_OS_EVENT_29   _Pragma("GCC warning \"'X_OS_EVENT_29' macro is deprecated, use 'X_OS_GLOBAL_EVENT_29'\"") X_OS_GLOBAL_EVENT_29
#define X_OS_EVENT_30   _Pragma("GCC warning \"'X_OS_EVENT_30' macro is deprecated, use 'X_OS_GLOBAL_EVENT_30'\"") X_OS_GLOBAL_EVENT_30
#define X_OS_EVENT_31   _Pragma("GCC warning \"'X_OS_EVENT_31' macro is deprecated, use 'X_OS_GLOBAL_EVENT_31'\"") X_OS_GLOBAL_EVENT_31
#define X_OS_EVENT_32   _Pragma("GCC warning \"'X_OS_EVENT_32' macro is deprecated, use 'X_OS_GLOBAL_EVENT_32'\"") X_OS_GLOBAL_EVENT_32
/*PRQA S 2217  --*/ /*FORMATTING_RULE_SUPPRESSION_END*/

/* -- Types --------------------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Signature of a task function that can be installed into the task system.

   RTOS user tasks can be used to implement parallel multi tasking. There are two possible structures for a user task.

   Standard RTOS task
   This is recommended for new applications. The basic characteristic is the endless loop. As the task is started only
   once the overhead of the functions prologue and epilogue can be neglected.
   All local variables keeps their value as the scope of the task is never left. The task can be suspended at any
   position.

   void rtos_task(void * const opv_TaskParameter)
   {
      place for local variable declarations - as we never leave the task function  all
      local variables behave like static variables

      do task initialization stuff here...

      while (TRUE) this is the endless loop
      {
         do the job...

         suspend task ... with one of the available function (x_os_sleep, x_os_wait_period, x_os_wait_for_events)
         x_os_sleep(1000);
      }
   }


   Classic timer task
   This task structure is for compatibility only. The basic characteristic is that this function has no endless loop
   and returns at the end of the function.
   When the function returns the task is suspended implicit by x_os_wait_for_events. If the task shall be resumed, the
   function is started again from the beginning.
   Thus the functions prologue and epilogue is executed every time. Furthermore local variables can not be used to store
   information that must be present for several task calls as the scope of the task is left every time. If such
   variables are needed anyway they must be declared as static.

   void timer_task(void * const opv_TaskParameter)
   {
      place for local variable declarations - if a variable should hold the value
      from the last task call it must be declared static
      static uint32 u32_FirstRun = TRUE;

      if (u32_FirstRun == TRUE)
      {
         u32_FirstRun = FALSE;

         do task initialization stuff here...
      }
      else
      {
         do the job...
      }

      leave the task function and dispatch to next task ready to run
   }

   There is no way to set the value of opv_TaskParameter. It's value is always NULL.

   \param[in]     opv_TaskParameter          parameter passed to the task function.

   \return
   void
*/
//----------------------------------------------------------------------------------------------------------------------
typedef void (* PR_x_os_task)(void * const opv_TaskParameter);

//! \brief status information about the OS
typedef struct
{
   uint16 u16_State;       //!< state of task system (see defines X_OS_NOT_INIT; X_OS_INIT; X_OS_RUNNING)
   uint16 u16_TaskCount;   //!< number of installed tasks
   uint32 u32_TimerTickUs; //!< scheduler tick in micro sec, range 500 - 1000000
} T_x_os_status;

/*STW Deviation Coding Rules v4*/
/*Violated Rule: No rule available*/
/*Circumstances: SW Architecture*/
/*Reason: Dir 4.8 The pointer to the structure is never dereferenced, thus implementation of the object is hidden*/
/*Potential risks: none*/
/*Prevention of risks:  opaque type, nothing to do*/
/*PRQA S 3313 4*/
typedef struct T_x_os_mbox_struct T_x_os_mbox;     //!< message box object type
typedef struct T_x_os_mutex_struct T_x_os_mutex;   //!< mutex object type
typedef struct T_x_os_sema_struct T_x_os_sema;     //!< semaphore object type
typedef struct T_x_os_signal_struct T_x_os_signal; //!< signal object type

//! \brief types used by task watchdog module
typedef struct
{
   uint16 u16_WDmin_ms;          //!< min. watchdog time [ms]
   uint16 u16_WDmax_ms;          //!< max. watchdog time [ms]
   uint16 u16_WDFirstTrigger_ms; /*!< 1st trigger timeout after initialization [ms],
                                     set to zero, if this feature is not required*/
} T_x_os_task_wd_cfg;

//! \brief watchdog configuration and status
typedef struct
{
   T_x_os_task_wd_cfg t_WDCfg;   //!< task watchdog configuration
   uint32 u32_MaxTriggerDiff_ms; //!< max. measured trigger difference [ms]
   uint32 u32_MinTriggerDiff_ms; //!< min. measured trigger difference [ms]
} T_x_os_task_wd_status;

//! \brief types used by wait for user event function
typedef struct
{
   uint32 u32_Result;             /*! Wait operation result:
                                     - X_OS_TASK_EVENT_TIMEOUT: Wait operation timed out
                                     - otherwise combination of
                                         o X_OS_WAIT_RESULT_EVENT:   Any of the selected events occurred
                                         o X_OS_WAIT_RESULT_MESSAGE: Message was received */
   uint32 u32_Events;             /*! Mask of selected events that occurred*/
   const T_x_os_msg * pt_Message; //!< Received message if X_OS_WAIT_RESULT_MESSAGE is set in u32_Result
} T_x_os_wait_result;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* X_OS_TYPES_H */
