//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Header file for Startup
   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef STARTUP_H
#define STARTUP_H

// Check GCC version at preprocessing. Do not edit!
#if ((__GNUC__ != 4) || (__GNUC_MINOR__ != 9) || (__GNUC_PATCHLEVEL__ != 4))
#error This Library shall only be used with TriCore/AURIX GCC V4.9.4
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
//! \brief global variables for task system start; to be defined by application
extern const uint32 gu32_TaskTimerTick_us;  //!< scheduler tick in micro seconds, range 500 - 1000000
extern const uint32 gu32_TaskMainMemClass;  /*!< memory class used by this task (target specific:
                                                 X_OS_STACKS_DSPR, X_OS_HEAP_ID_SRAM)*/
extern const uint32 gu32_TaskMainStackSize; //!< task stack size in byte (8..FFFFFFF8)

extern const uint8 gu8_CpuCacheDisable;      /*!< global variable to disable CPU cache
                                                 - 0:  CPU cache will be enabled (default)
                                                 - >0: CPU cache keep disabled (e.g. for debug purpose)*/
extern sint32 gs32_MainReturnValue;          //!< Variable to save the return value of main task
extern const uint8 gu8_IccSigPollFuncEnable; /*!< global variable to configure Rx ICC polling
                                                  - 0: RX signal polling is done automatically by the C-BIOS on the
                                                       lowest priority level (default)
                                                  - 1: RX signal polling is done by the application using
                                                       "x_icc_sig_poll_one"*/

extern int main(void); //!< needed for first task

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

void idle_main(void);         // idle main task
void startup_core_init(void); // initialization of local core

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
