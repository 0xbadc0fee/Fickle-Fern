//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       STW specific standard defines and types
                Definition of standard types according to STW C coding rules for TriCore/AURIX with HIGHTEC TriCore
                Development Platform
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef STWTYPESH
#define STWTYPESH

#ifdef STWTYPES_H_REPLACEMENT
#include STWTYPES_H_REPLACEMENT
#else

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define  STW_UINT8_MIN    (0U)
#define  STW_UINT8_MAX    (255U)

#define  STW_SINT8_MIN    (-128)
#define  STW_SINT8_MAX    (127)

#define  STW_UINT16_MIN   (0U)
#define  STW_UINT16_MAX   (65535U)

#define  STW_SINT16_MIN   (-32768)
#define  STW_SINT16_MAX   (32767)

#define  STW_UINT32_MIN   (0UL)
#define  STW_UINT32_MAX   (4294967295UL)

#define  STW_SINT32_MIN   (-2147483647L - 1L) //"-1" approach: increases compiler compatibility
#define  STW_SINT32_MAX   (2147483647L)

#define  STW_UINT64_MIN   (0ULL)
#define  STW_UINT64_MAX   (18446744073709551615ULL)

#define  STW_SINT64_MIN   (-9223372036854775807LL - 1LL) //"-1" approach: increases compiler compatibility
#define  STW_SINT64_MAX   (9223372036854775807LL)

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef unsigned char uint8;       // data type unsigned 8bit
typedef signed char sint8;         // data type signed 8bit
typedef unsigned short uint16;     // data type unsigned 16bit
typedef signed short sint16;       // data type signed 16bit
typedef unsigned long uint32;      // data type unsigned 32bit
typedef signed long sint32;        // data type signed 32bit
typedef unsigned long long uint64; // data type unsigned 64bit
typedef signed long long sint64;   // data type signed 64bit
typedef float float32;             // data type IEEE float 32bit
typedef double float64;            // data type IEEE float 64bit
typedef unsigned int uintn;        // native unsigned int
typedef signed int sintn;          // native signed int
typedef char charn;                // native char type

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif
#endif
