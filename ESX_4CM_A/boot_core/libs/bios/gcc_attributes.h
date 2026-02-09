//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      GNU GCC attribute definitions
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef GNU_ATTRIBUTES_H
#define GNU_ATTRIBUTES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */

// Add optimization attribute only on non-optimized builds (rely on specified global setting on release builds)
#ifndef NDEBUG
#define ALWAYS_OPTIMIZE      __attribute__((optimize("Os")))
#else
#define ALWAYS_OPTIMIZE
#endif

// alignment directive for variables
#define ALIGNED(align)       __attribute__((aligned(align))) // locate a variable aligned to byte boundary

/*STW Deviation Coding Rules v4*/
/*Violated Rule: N/A (MISRA C:2012 Dir-1.1 is applicable)*/
/*Circumstances: SW Architecture */
/*Reason: Avoid __attribute__ constructs in client code*/
/*Potential risks: N/A*/
/*Prevention of risks: N/A*/
/*PRQA S 1030 ++*/

// section
#define SECTION(...)         __attribute__((asection(__VA_ARGS__)))

/*PRQA S 1030 --*/

// define attribute for weak symbols
#define WEAK                 __attribute__((weak))

// define function attribute to designate functions as deprecated
#define DEPRECATED           __attribute__((deprecated))

// define function attribute for trap and interrupt handler (called by jump and link instruction)
#define TRAP_HANDLER         __attribute__((interrupt)) // function attribute for trap handler
#define INTERRUPT_HANDLER    TRAP_HANDLER               // function attribute for interrupt handler

// define function attributes for inline code
#define NO_INLINE            __attribute__((noinline))      // function attribute to suppress inline code
#define ALWAYS_INLINE        __attribute__((always_inline)) // function attribute to force inline code

// INLINE functions shall always be static
#define INLINE               static inline ALWAYS_INLINE

// function attribute for functions that don't return (eg. helps if transitioning into cpu_halt() on unreachable cases)
#define NORETURN             __attribute__((noreturn))

// function attribute for long calls (call over 256MB segment borders)
#define LONGCALL             __attribute__((longcall))

// ram code (note: NO_INLINE is controversial, however it keeps code size between debug and release closer together)
#define RAMCODE(section)     SECTION(section, "a=4", "f=x") LONGCALL NO_INLINE ALWAYS_OPTIMIZE

#endif /* GNU_ATTRIBUTES_H */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */
