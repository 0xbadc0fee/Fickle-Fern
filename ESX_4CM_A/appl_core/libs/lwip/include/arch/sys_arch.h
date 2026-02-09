//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Target specific glue layer for lwip stack
   \details     Header defines macros and functions that are used throughout the lwIP stack
   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  SYS_ARCH_H
#define  SYS_ARCH_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "lwip/opt.h"
#include "x_sys.h"
#if !NO_SYS
#include "x_os.h"
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#if !NO_SYS

#define VALID                    1
#define INVALID                  0

extern const uint32 gu32_TaskTimerTick_us;
#define SYS_ARCH_MS_TO_TICKS(ms)          ((ms * 1000UL) / gu32_TaskTimerTick_us)

#define sys_msleep               sys_arch_msleep // Define target specific msleep function

// For tcpip_init are 3 semaphores needed and for each socket 4 semaphores are needed!
#define SYS_SEMA_NUMBER          (3u + (MEMP_NUM_NETCONN * 4u))

#if !MEM_USE_POOLS
// For tcpip_init 3 semaphores are needed and for each socket 4 semaphores are needed!
// Three mutexes are required for netif transmit function, netif transmit
// time stamp function and for TCPIP_CORE_LOCKING
#define SYS_MUTEX_NUMBER         (SYS_SEMA_NUMBER + 3u)
#else
#define SYS_MUTEX_NUMBER         3u // Three mutexes are required for netif transmit function, netif transmit
                                    // time stamp function and for TCPIP_CORE_LOCKING
#endif

#define SYS_MBOX_SIZE            64u
#define SYS_MBOX_NUMBER          (MEMP_NUM_NETCONN + 1u) // for each socket one mbox + 1 for tcpip_init

#endif // !NO_SYS

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
#if !NO_SYS
void sys_mutex_init(void); // Implemented in sys_arch_mutex.c
void sys_sem_init(void);   // Implemented in sys_arch_sem.c

void sys_mark_tcpip_thread(void);   // Implemented in sys_arch.c
void sys_check_core_locked(void);  // Implemented in sys_arch.c
void sys_tcpip_core_lock(sys_mutex_t * opt_Mutex);
void sys_tcpip_core_unlock(sys_mutex_t * opt_Mutex);

#endif // !NO_SYS

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get system time in ms

   \return   Time in ms
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE u32_t sys_now(void)
{
   return x_sys_get_time_ms();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get system time in 10ms

   \return   Time in 10ms
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE u32_t sys_jiffies(void)
{
   return (u32_t)(x_sys_get_time_us() / 10000ULL);
}

#if !NO_SYS

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Releases machine time and suspends current task for a timespan of ou32_SleepMs

   \param[in]     ou32_SleepMs         Sleep time in milliseconds
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void sys_arch_msleep(const uint32 ou32_SleepMs)
{
   x_os_sleep(SYS_ARCH_MS_TO_TICKS(ou32_SleepMs));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check the message box if it's valid or not

   \param[in] opt_Mbox       Mbox to check

   \retval    1              Valid
   \retval    0              Invalid
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE int sys_mbox_valid(sys_mbox_t * opt_Mbox)
{
   return ((opt_Mbox != NULL) && (*opt_Mbox != NULL)) ? VALID : INVALID;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the mbox invalid

   \param[in] opt_Mbox       Mbox to set invalid
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void sys_mbox_set_invalid(sys_mbox_t * opt_Mbox)
{
   *opt_Mbox = NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check the semaphore if it's valid or not

   \param[in] opt_Sem       semaphore to check

   \retval    1             Valid
   \retval    0             Invalid
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE int sys_sem_valid(sys_sem_t * opt_Sem)
{
   return ((opt_Sem != NULL) && (*opt_Sem != NULL)) ? VALID : INVALID;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the semaphore invalid

   \param[in] opt_Sem       Semaphore to set invalid
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void sys_sem_set_invalid(sys_sem_t * opt_Sem)
{
   *opt_Sem = NULL;
}

#if SYS_LIGHTWEIGHT_PROT
//----------------------------------------------------------------------------------------------------------------------
/*! \brief   SYS_ARCH_PROTECT

   Perform a "fast" protect. This could be implemented by
   disabling interrupts for an embedded system or by using a semaphore or
   mutex. The implementation should allow calling SYS_ARCH_PROTECT when
   already protected. The old protection level is returned in the variable
   "lev". This macro will default to calling the sys_arch_protect() function
   which should be implemented in sys_arch.c. If a particular port needs a
   different implementation, then this macro may be defined in sys_arch.h

   \return cpu state
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE sys_prot_t sys_arch_protect(void)
{
   return x_os_enter_critical();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   SYS_ARCH_UNPROTECT

   Perform a "fast" set of the protection level to "lev". This could be
   implemented by setting the interrupt level to "lev" within the MACRO or by
   using a semaphore or mutex.  This macro will default to calling the
   sys_arch_unprotect() function which should be implemented in
   sys_arch.c. If a particular port needs a different implementation, then
   this macro may be defined in sys_arch.h
*/
//----------------------------------------------------------------------------------------------------------------------
//PRQA S 3227 1 // Parameter can not declared const, because declaration is done inside lwIP stack
INLINE void sys_arch_unprotect(sys_prot_t ou32_Pval)
{
   x_os_leave_critical(ou32_Pval);
}

#endif // SYS_LIGHTWEIGHT_PROT

#endif // !NO_SYS

#endif // SYS_ARCH_H
