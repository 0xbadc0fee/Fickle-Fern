//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Target specific memory attributes
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_MEMTYPES_H
#define X_MEMTYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "gcc_attributes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

// Variables placed into these sections define the space available for dynamic task stacks inside the respective memory
#define X_MEM_STACKS_DSPR                       SECTION(".dspr.stacks.sc0", "f=wB")
#define X_MEM_STACKS_EMEM                       SECTION(".emem.stacks.sc0", "f=wB")
#define X_MEM_STACKS_DEFAULT                    X_MEM_STACKS_EMEM

// X_MEM_RESOURCES_MAKE() references these
#define X_MEM_SYSTEM_HANDLES                    SECTION(".handles",         "f=wB")
#define X_MEM_ICC_MSG_PROXIES                   SECTION(".icc_msg_proxies", "f=wB")

// Define memory section attribute to place library information on a fixed address location
#define X_MEM_LIB_VERSION                       SECTION(".libversion", "f=a")

// Define memory attribute for library information
#define X_MEM_LIB_INFO                          SECTION(".extlibinfo",            "f=a")
#define X_MEM_APPLICATION_INFO                  SECTION(".application_info",      "f=a")
#define X_MEM_APPLICATION_SIGNATURE_BLOCK_LIST  SECTION(".next_signature_blocks", "f=a")

// Nonsafe data manual placement (sorted in order of decreasing performance)
#define X_MEM_DSPR_DATA_NONSAFE                 SECTION(".dspr.data.sc0",    "f=w") // extremely fast
#define X_MEM_DSPR_BSS_NONSAFE                  SECTION(".dspr.bss.sc0",     "f=wB")
#define X_MEM_EMEM_DATA_NONSAFE                 SECTION(".emem.data.sc0.c",  "f=w") // pretty fast 2
#define X_MEM_EMEM_BSS_NONSAFE                  SECTION(".emem.bss.sc0.c",   "f=wB")
#define X_MEM_EMEM_DATA_UC_NONSAFE              SECTION(".emem.data.sc0.uc", "f=w")
#define X_MEM_EMEM_BSS_UC_NONSAFE               SECTION(".emem.bss.sc0.uc",  "f=wB")

// Map default memory types to this core safety level (we're non-safe core)
#define X_MEM_DSPR_DATA                         X_MEM_DSPR_DATA_NONSAFE
#define X_MEM_DSPR_BSS                          X_MEM_DSPR_BSS_NONSAFE
#define X_MEM_EMEM_DATA                         X_MEM_EMEM_DATA_NONSAFE
#define X_MEM_EMEM_BSS                          X_MEM_EMEM_BSS_NONSAFE
#define X_MEM_EMEM_DATA_UC                      X_MEM_EMEM_DATA_UC_NONSAFE
#define X_MEM_EMEM_BSS_UC                       X_MEM_EMEM_BSS_UC_NONSAFE

// Public data is guaranteed to be readable by all safety levels, writes by lower safety levels are blocked
#define X_MEM_DSPR_DATA_PUBLIC                  X_MEM_DSPR_DATA_NONSAFE
#define X_MEM_DSPR_BSS_PUBLIC                   X_MEM_DSPR_BSS_NONSAFE
#define X_MEM_EMEM_DATA_PUBLIC                  X_MEM_EMEM_DATA_UC_NONSAFE
#define X_MEM_EMEM_BSS_PUBLIC                   X_MEM_EMEM_BSS_UC_NONSAFE

// Shared data is guaranteed to be readable and writable by all safety levels
#define X_MEM_DSPR_DATA_SHARED                  X_MEM_DSPR_DATA_NONSAFE
#define X_MEM_DSPR_BSS_SHARED                   X_MEM_DSPR_BSS_NONSAFE
#define X_MEM_EMEM_DATA_SHARED                  X_MEM_EMEM_DATA_UC_NONSAFE
#define X_MEM_EMEM_BSS_SHARED                   X_MEM_EMEM_BSS_UC_NONSAFE

// Public defaults, guaranteed to be readable by all safety levels, writes by lower safety levels are blocked
#define X_MEM_DATA_PUBLIC                       X_MEM_DSPR_DATA_PUBLIC
#define X_MEM_BSS_PUBLIC                        X_MEM_DSPR_BSS_PUBLIC

// Shared defaults, guaranteed to be readable and writable by all safety levels
#define X_MEM_DATA_SHARED                       X_MEM_DSPR_DATA_SHARED
#define X_MEM_BSS_SHARED                        X_MEM_DSPR_BSS_SHARED

// Deprecated
#define X_MEM_SHARED_DATA                       X_MEM_DATA_SHARED
#define X_MEM_SHARED_BSS                        X_MEM_BSS_SHARED
#define MEM_APPL_STATIC_HEAP_INT_DSPR           X_MEM_STACKS_DSPR
#define MEM_APPL_STATIC_HEAP_EMEM_SRAM          X_MEM_STACKS_EMEM

#endif /* X_MEMTYPES_H */
