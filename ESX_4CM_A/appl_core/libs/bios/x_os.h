//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      RTOS API
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_H
#define X_OS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_os_types.h"
#include "x_os_mbox.h"
#include "x_os_mutex.h"
#include "x_os_sema.h"
#include "x_os_signal.h"
#include "x_os_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define X_OS_CFG_FLAG_ICC_BARRIER       (0x04000000u) /*!< Task uses x_icc_barrier() for synchronization between
                                                        tasks on local and remote cores.
                                                        Implementation hint: icc_barrier uses the upper 6 bits of
                                                        TCB member u32_CfgFlags, see XB_ICC_BARRIER_CFG_FLAG_ID_MASK */

/* -- Types --------------------------------------------------------------------------------------------------------- */
//! \brief Initial task configuration
typedef struct
{
   uint16 u16_Priority;     //!< Task priority (0 is used for main()) [1..X_OS_MAX_TASK_PRIO]
   PR_x_os_task pr_Task;    //!< Task function pointer
   void * pv_Parameter;     //!< Parameter passed to task function
   uint32 u32_TimeDelay;    /*!< Delay time in task ticks before 1st call
                                 - 0 = suspend task if u32_TimeInterval == 0 (otherwise u32_TimeDelay will be set to 1
                                       internally)
                                 - 1..FFFFFFFF*/
   uint32 u32_TimeInterval; //!< Interval time after 1st task call (0 = one shot timer, 1..FFFFFFFF)
   uint32 u32_StackSize;    //!< Task stack size in byte (8..FFFFFFF8)
   uint32 u32_MemClass;     /*!< Task memory class used for stack allocation (target specific: e.g. X_OS_STACKS_DSPR,
                                 X_OS_HEAP_ID_SRAM)*/
   uint32 u32_ConfigFlags;  //!< Task configuration flags: e.g. X_OS_CFG_FLAG_ICC_BARRIER
   uint16 u16_SafetyLevel;  /*!< Safety Level of the Task (target-specific, e.g. default:
                                 X_OS_SAFETY_LEVEL_QM)*/
   uint16 u16_UserTimer;    /*!< Number of additional user timers, each user timer can produce an arbitrary combination
                                 of the available user events and adds 12 byte of stack memory requirements (on top of
                                 the requested stack memory size)
                                 - [0 .. STW_UINT16_MAX-1]
                                  */
} T_x_os_task_config;
/*STW Deviation Coding Rules v4*/
/*Violated Rule: No rule available*/
/*Circumstances: SW Architecture*/
/*Reason: Dir 4.8 The pointer to the structure is never dereferenced, thus implementation of the object is hidden*/
/*Potential risks: none*/
/*Prevention of risks:  opaque type, nothing to do*/
/*PRQA S 3313 1*/
typedef struct T_x_os_task_struct T_x_os_task; //!< Task handler

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_os_install_task(const T_x_os_task_config * const opt_TaskConfig,
                         const T_x_os_task_wd_cfg * const opt_WatchdogConfig, T_x_os_task ** const oppt_Task);
sint16 x_os_set_task_timer(T_x_os_task * const opt_Task, const uint32 ou32_TimeInterval, const uint32 ou32_TimeDelay);
sint16 x_os_suspend_task(T_x_os_task * const opt_Task);
sint16 x_os_resume_task(T_x_os_task * const opt_Task);
void x_os_wait_for_events(void);
void x_os_wait_period(void);
void x_os_sleep(const uint32 ou32_TimeDelay);

void x_os_set_global_event_mask(const uint32 ou32_NewEventMask);
void x_os_get_global_event_mask(uint32 * const opu32_EventMask);
void x_os_set_global_events(const uint32 ou32_NewEvents);
uint32 x_os_get_global_events(void);

sint16 x_os_set_task_event_timer(T_x_os_task * const opt_Task, const uint16 ou16_UserTimer,
                                 const uint32 ou32_TaskEvents, const uint32 ou32_TimeInterval,
                                 const uint32 ou32_TimeDelay);
void   x_os_set_task_event_mask(const uint32 ou32_TaskEventMask, const uint32 ou32_ModifyMask);
void   x_os_get_task_event_mask(uint32 * const opu32_TaskEventMask);
sint16 x_os_set_task_events(T_x_os_task * const opt_Task, const uint32 ou32_TaskEvents);
uint32 x_os_get_task_events(const uint32 ou32_ClearTaskEvents);
T_x_os_wait_result x_os_wait_task_event(const uint32 ou32_TaskEventMask, const uint32 ou32_Timeout);

sint16 x_os_check_task_stack(T_x_os_task * const opt_Task, uint32 * const opu32_UnusedStackSize);
sint16 x_os_get_status(T_x_os_status * const opt_Status);
sint16 x_os_get_task_safety_level(T_x_os_task * const opt_Task, uint16 * const opu16_SafetyLevel);
sint16 x_os_get_task_priority(T_x_os_task * const opt_Task, uint16 * const opu16_Priority);
sint16 x_os_get_task_handle(T_x_os_task ** const oppt_Task);
sint16 x_os_task_watchdog_trigger(void);
sint16 x_os_task_watchdog_check(T_x_os_task * const opt_Task, uint16 * const opu16_TriggerStatus);
sint16 x_os_task_watchdog_get_status(T_x_os_task * const opt_Task, T_x_os_task_wd_status * const opt_Status);
sint16 x_os_task_watchdog_restart(T_x_os_task * const opt_Task);
uint32 x_os_enter_critical(void);
void x_os_leave_critical(const uint32 ou32_PreviousState);

// Backwards compatibility (deprecated)

DEPRECATED // Use x_os_set_global_event_mask() instead
void x_os_set_event_mask(const uint32 ou32_NewEventMask);

DEPRECATED // Use x_os_get_global_event_mask() instead
void x_os_get_event_mask(uint32 * const opu32_EventMask);

DEPRECATED // Use x_os_set_global_events() instead
void x_os_set_events(const uint32 ou32_NewEvents);

DEPRECATED // Use x_os_get_global_events() instead
uint32 x_os_get_events(void);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
