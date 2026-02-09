//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Architecture specific definitions for lwIP stack
   \details     Header defines architecture specific macros and types
   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef ARCH_CC_H
#define ARCH_CC_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_intrinsics.h"
#include "lwipopts.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define LWIP_ERR_T               sint16

// Define (sn)printf formatters for these lwIP types
#define U16_F                    "hu"
#define S16_F                    "hd"
#define X16_F                    "hx"
#define U32_F                    "lu"
#define S32_F                    "ld"
#define X32_F                    "lx"
#ifndef SZT_F
#define SZT_F                    "lu"
#endif // SZT_F

// Host byte order definition
#define BYTE_ORDER               LITTLE_ENDIAN

// Compiler hints for packing structures
#define PACK_STRUCT_FIELD(x)     x
#define PACK_STRUCT_STRUCT       __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

// Plaform specific diagnostic output
#define LWIP_PLATFORM_DIAG(m)       do {lwip_sys_arch_printf m;} while (0)
#define LWIP_PLATFORM_ASSERT(x)     do {LWIP_PLATFORM_DIAG((x)); debug();} while (0)

/* -- Types --------------------------------------------------------------------------------------------------------- */

#define LWIP_NO_STDINT_H   1  // Use stwtypes instead of stdint types to comply with P4 platform
#define LWIP_HAVE_INT64    1  // P4 platform supports 64 bit types
typedef uint8 u8_t;
typedef sint8 s8_t;
typedef uint16 u16_t;
typedef sint16 s16_t;
typedef uint32 u32_t;
typedef sint32 s32_t;
typedef uint64 u64_t;
typedef sint64 s64_t;
typedef uint32 mem_ptr_t;

typedef uint32 sys_prot_t;
#if !NO_SYS
typedef void * sys_sem_t;
typedef void * sys_mutex_t;

struct T_sys_mbox;

typedef struct T_sys_mbox * sys_mbox_t;
typedef LWIP_ERR_T sys_thread_t;
#endif

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

#if defined NETIF_X_ETHNC_NUM && NETIF_X_ETHNC_NUM > 0u
// Place malloc mempools in memory region specified by LWIP_MALLOC_MEMPOOL_MEM_ATTRIBUTE.
// (Details see example in memp.h, line 92)
#define LWIP_MALLOC_MEMPOOL_START
#define LWIP_MALLOC_MEMPOOL_END
#define LWIP_MALLOC_MEMPOOL(num, size)  \
   extern LWIP_MALLOC_MEMPOOL_MEM_ATTRIBUTE u8_t memp_memory_POOL_ ## size ## _base[];
#include "lwippools.h"
#undef LWIP_MALLOC_MEMPOOL_START
#undef LWIP_MALLOC_MEMPOOL_END
#undef LWIP_MALLOC_MEMPOOL
#endif

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Print debug and statistics information an a terminal.

   An empty implementation of this function is defined in sys_arch.c with weak linkage.
   The application can overwrite it by an own implementation to print the information wherever it wants.
*/
//----------------------------------------------------------------------------------------------------------------------
extern void lwip_sys_arch_printf(const charn * opcn_Fmt, ...);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // ARCH_CC_H
