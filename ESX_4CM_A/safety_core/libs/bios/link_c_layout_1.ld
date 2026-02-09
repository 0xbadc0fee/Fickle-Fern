/*!
   \file
   \brief       Linker script for C applications

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/

/* Include platform memories and core interop sections */
INCLUDE tc299_osy_memdef.ld

/* -- Memory partitioning (flash, lmu ram, emem) -------------------------------------------------------------------- */
MEMORY_LAYOUT_IDENTIFIER = 1;                 /* The memory layout identifier must match across all core applications */

MEMORY
{
/* Flash Partitioning */
   core0_pflash          (rx) :   org = 0x80080400, len = 0x0013FC00        /* [0x80080400, 0x801C0000[    (1279 KiB) */

   core1_pflash_stl      (rx) :   org = 0x801C0000, len = 0x00040000        /* [0x801C0000, 0x80200000[     (256 KiB) */
   core1_pflash_seg0     (rx) :   org = 0x80200000, len = 0x001FFC00        /* [0x80200000, 0x803FFC00[    (2047 KiB) */
   core1_pflash_seg1     (rx) :   org = 0x80400400, len = 0x001FFC00        /* [0x80400400, 0x80600000[    (2047 KiB) */
   core2_pflash          (rx) :   org = 0x80600000, len = 0x001FFC00        /* [0x80600000, 0x807FFC00[    (2047 KiB) */


/* LMU Partitioning */
   core0_lmu             (wx) :   org = 0x90000000, len = 0x00000800        /* [0x90000000, 0x90000800[       (2 KiB) */
   core1_lmu             (wx) :   org = 0x90000800, len = 0x00007000        /* [0x90000800, 0x90007800[      (28 KiB) */
   core2_lmu             (wx) :   org = 0x90007800, len = 0x00000800        /* [0x90007800, 0x90008000[       (2 KiB) */

/* EMEM Partitioning */
   core1_emem            (wx) :   org = 0x9F000000, len = 0x00170000        /* [0x9F000000, 0x9F170000[    (1472 KiB) */
   core2_emem            (wx) :   org = 0x9F170000, len = 0x00079000        /* [0x9F170000, 0x9F1E9000[     (484 KiB) */
   core2_emem_eth        (wx) :   org = 0x9F1E9000, len = 0x00007000        /* [0x9F1E9000, 0x9F1F0000[      (28 KiB) */
   core0_emem            (wx) :   org = 0x9F1F0000, len = 0x00010000        /* [0x9F1F0000, 0x9F200000[      (64 KiB) */

   var_flash             (rx) :   org = ORIGIN(dflash0), len = 4K  	    /* Data Flash          (4 KiB) */
}

/* Define uncached counterparts where applicable */
MEMORY
{
   core0_pflash_uc       (rx) :   org = ORIGIN(core0_pflash)      | 0x20000000, len = LENGTH(core0_pflash)
   core1_pflash_stl_uc   (rx) :   org = ORIGIN(core1_pflash_stl)  | 0x20000000, len = LENGTH(core1_pflash_stl_uc)
   core1_pflash_seg0_uc  (rx) :   org = ORIGIN(core1_pflash_seg0) | 0x20000000, len = LENGTH(core1_pflash_seg0)
   core1_pflash_seg1_uc  (rx) :   org = ORIGIN(core1_pflash_seg1) | 0x20000000, len = LENGTH(core1_pflash_seg1)
   core2_pflash_uc       (rx) :   org = ORIGIN(core2_pflash)      | 0x20000000, len = LENGTH(core2_pflash)

   core0_lmu_uc          (wx) :   org = ORIGIN(core0_lmu)         | 0x20000000, len = LENGTH(core0_lmu)
   core1_lmu_uc          (wx) :   org = ORIGIN(core1_lmu)         | 0x20000000, len = LENGTH(core1_lmu)
   core2_lmu_uc          (wx) :   org = ORIGIN(core2_lmu)         | 0x20000000, len = LENGTH(core2_lmu)
   core1_emem_uc         (wx) :   org = ORIGIN(core1_emem)        | 0x20000000, len = LENGTH(core1_emem)
   core2_emem_uc         (wx) :   org = ORIGIN(core2_emem)        | 0x20000000, len = LENGTH(core2_emem)
   core2_emem_eth_uc     (wx) :   org = ORIGIN(core2_emem_eth)    | 0x20000000, len = LENGTH(core2_emem_eth)
   core0_emem_uc         (wx) :   org = ORIGIN(core0_emem)        | 0x20000000, len = LENGTH(core0_emem)
}

REGION_MIRROR("core0_pflash_uc",      "core0_pflash")
REGION_MIRROR("core1_pflash_stl_uc",  "core1_pflash_stl")
REGION_MIRROR("core1_pflash_seg0_uc", "core1_pflash_seg0")
REGION_MIRROR("core1_pflash_seg1_uc", "core1_pflash_seg1")
REGION_MIRROR("core2_pflash",         "core2_pflash")

REGION_MIRROR("core0_lmu_uc",         "core0_lmu")
REGION_MIRROR("core1_lmu_uc",         "core1_lmu")
REGION_MIRROR("core2_lmu_uc",         "core2_lmu")
REGION_MIRROR("core0_emem_uc",        "core0_emem")
REGION_MIRROR("core1_emem_uc",        "core1_emem")
REGION_MIRROR("core2_emem_uc",        "core2_emem")

/* -- Define aliases for "our" memories to be referenced later ------------------------------------------------------ */
REGION_ALIAS("MEM_MY_DSPR",           core1_dspr);
REGION_ALIAS("MEM_MY_PSPR",           core1_pspr);
REGION_ALIAS("MEM_MY_LMU",            core1_lmu);
REGION_ALIAS("MEM_MY_LMU_UC",         core1_lmu_uc);
REGION_ALIAS("MEM_MY_EMEM",           core1_emem);
REGION_ALIAS("MEM_MY_EMEM_UC",        core1_emem_uc);
REGION_ALIAS("MEM_MY_BOOT_ROM",       core1_boot_rom);
REGION_ALIAS("MEM_MY_PFLASH_SEG0",    core1_pflash_seg0);
REGION_ALIAS("MEM_MY_PFLASH_SEG0_UC", core1_pflash_seg0_uc);
REGION_ALIAS("MEM_MY_PFLASH_SEG1",    core1_pflash_seg1);
REGION_ALIAS("MEM_MY_PFLASH_SEG1_UC", core1_pflash_seg1_uc);
REGION_ALIAS("MEM_MY_PFLASH_SEG2",    no_mem);
REGION_ALIAS("MEM_MY_PFLASH_SEG2_UC", no_mem);
REGION_ALIAS("MEM_MY_PFLASH_SEG3",    no_mem);
REGION_ALIAS("MEM_MY_PFLASH_SEG3_UC", no_mem);
REGION_ALIAS("MEM_MY_PFLASH_SEG4",    core1_pflash_stl);
REGION_ALIAS("MEM_MY_PFLASH_SEG4_UC", core1_pflash_stl_uc);

/* -- Map aurix_common_sections_begin.ld and aurix_common_sections_end.ld ------------------------------------------- */
MEM_ISTACK_SIZE  =  4K;                                    /* Interrupt stack size in bytes                           */
MEM_USTACK_SIZE  = 20K;                                    /* User stack size in bytes                                */
MEM_CSA_MIN_SIZE = 24K;                                    /* Minimum size of CSA area in bytes                       */
MEM_LCX_HEADROOM =   6;                                    /* Number of reserved lower contexts for trap handling     */
MEM_USE_SAFETLIB =   1;                                    /* Use SafeTlib (use defaults for dpsr and pspr sizes)     */

REGION_ALIAS("MEM_BOOT_ROM",       core1_boot_rom);        /* Startup code and data placed at static address          */
REGION_ALIAS("MEM_LOAD_ROM",       core1_pflash_seg0);     /* Data/Code load address area                             */
REGION_ALIAS("MEM_PSPR",           core1_pspr);            /* PSPR coupled with this core                             */
REGION_ALIAS("MEM_DSPR",           core1_dspr);            /* DSPR coupled with this core                             */
REGION_ALIAS("MEM_LMU",            core1_lmu);             /* LMU SRAM area assigned to this core                     */
REGION_ALIAS("MEM_LMU_UC",         core1_lmu_uc);          /* LMU SRAM area assigned to this core (uncached)          */
REGION_ALIAS("MEM_HANDLES",        core1_dspr);            /* Memory region for system object handles                 */
REGION_ALIAS("MEM_SDATA_BIOS_RAM", core1_lmu);             /* Small data for internal usage                           */
REGION_ALIAS("MEM_SDATA_BIOS_ROM", core1_pflash_seg0);     /* Small rom for internal usage                            */
REGION_ALIAS("MEM_SDATA_USER_RAM", core1_emem);            /* Small ram data reserved for user                        */
REGION_ALIAS("MEM_SDATA_USER_ROM", core1_pflash_seg0);     /* Small rom reserved for user                             */

REGION_ALIAS("MEM_DEFAULT_ROM_0",  core1_pflash_seg0);     /* Application default .text and .rodata section           */
REGION_ALIAS("MEM_DEFAULT_ROM_1",  core1_pflash_seg1);     /* Application default .text and .rodata (continued)       */
REGION_ALIAS("MEM_DEFAULT_ROM_2",  no_mem);                /* Application default .text and .rodata (continued)       */
REGION_ALIAS("MEM_DEFAULT_ROM_3",  no_mem);                /* Application default .text and .rodata (continued)       */
REGION_ALIAS("MEM_DEFAULT_ROM_4",  core1_pflash_stl);      /* Application default .text and .rodata (continued)       */

REGION_ALIAS("MEM_DEFAULT_RAM_0",  core1_emem);            /* Application default .data and .bss section              */
REGION_ALIAS("MEM_DEFAULT_RAM_1",  no_mem);                /* Application default .data and .bss section (continued)  */
REGION_ALIAS("MEM_DEFAULT_RAM_2",  no_mem);                /* Application default .data and .bss section (continued)  */
REGION_ALIAS("MEM_DEFAULT_RAM_3",  no_mem);                /* Application default .data and .bss section (continued)  */
REGION_ALIAS("MEM_DEFAULT_RAM_4",  no_mem);                /* Application default .data and .bss section (continued)  */

/* System Heap */
MEM_SYSTEM_HEAP_MIN_SIZE = 10K;                            /* Minimum system heap size in bytes                       */
MEM_SYSTEM_HEAP_MAX_SIZE = 32K;                            /* [optional] limit system heap to size in bytes           */
REGION_ALIAS("MEM_SYSTEM_HEAP_RAM", core1_emem);           /* System heap location (e.g. communication buffers)       */

/* User Heap */
MEM_USER_HEAP_MIN_SIZE   = 20K;                            /* Minimum user heap size in bytes (used by malloc)        */
/*MEM_USER_HEAP_MAX_SIZE = 10K; */                         /* [optional] User heap maximum size to size in bytes      */
REGION_ALIAS("MEM_USER_HEAP_RAM",   core1_emem);           /* User heap memory location                               */

/* -- Map tc29x_safetlib.ld ----------------------------------------------------------------------------------------- */
REGION_ALIAS("MEM_STL_SBST_CODE_ROM", core1_pflash_stl);   /* SafeTlib SBST code                                      */
REGION_ALIAS("MEM_STL_ROM",           core1_pflash_stl);   /* SafeTlib rom data                                       */
REGION_ALIAS("MEM_STL_DSPR_RAM",      core1_dspr)          /* Write-accessible by all cores, dspr memory              */
REGION_ALIAS("MEM_STL_DEFAULT_RAM",   core1_lmu)           /* Write-accessible by all cores, any ram                  */

/* -- Target specific optional sections and memories ---------------------------------------------------------------- */

/* xll_memtypes_trg.ld */
REGION_ALIAS("MEM_TRG_ICC_RESOURCES",  MEM_MY_DSPR);

/* xll_memtype_dspr.ld */
REGION_ALIAS("MEM_DSPR_STACKS",        MEM_MY_DSPR);
REGION_ALIAS("MEM_DSPR_DATA_SC2",      MEM_MY_DSPR);
REGION_ALIAS("MEM_DSPR_DATA_SC0",      MEM_MY_DSPR);
REGION_ALIAS("MEM_DSPR_CODE",          MEM_MY_DSPR);

/* xll_memtype_emem.ld */
REGION_ALIAS("MEM_EMEM_STACKS",        MEM_MY_EMEM);
REGION_ALIAS("MEM_EMEM_DATA_SC2",      MEM_MY_EMEM);
REGION_ALIAS("MEM_EMEM_DATA_SC2_UC",   MEM_MY_EMEM_UC);
REGION_ALIAS("MEM_EMEM_DATA_SC0",      MEM_MY_EMEM);
REGION_ALIAS("MEM_EMEM_DATA_SC0_UC",   MEM_MY_EMEM_UC);
REGION_ALIAS("MEM_EMEM_CODE",          MEM_MY_EMEM);

/* -- Support stdlib malloc/realloc/free: force linking of stdlib malloc replacements ------------------------------- */
EXTERN(malloc);
EXTERN(calloc);
EXTERN(realloc);
EXTERN(free);

/* -- Application start address ------------------------------------------------------------------------------------- */
MEM_START_ADDR = ORIGIN(MEM_BOOT_ROM);

/* -- Add SafeTlib area to signature block -------------------------------------------------------------------------- */
MEM_SIGBLOCK_AREA5_START = LD_STL_ROM_START;
MEM_SIGBLOCK_AREA5_SIZE  = LD_STL_ROM_USED;

/* -- Emit output sections ------------------------------------------------------------------------------------------ */
INCLUDE aurix_common_sections_begin.ld
INCLUDE signature_block.ld
INCLUDE tc29x_safetlib.ld
INCLUDE xll_memtypes_trg.ld
INCLUDE xll_memtype_dspr.ld
INCLUDE xll_memtype_emem.ld
INCLUDE aurix_common_sections_end.ld

/*  -- Check for any unallocated input sections --------------------------------------------------------------------- */
SECTIONS
{
   .unallocated : { *(*) }

   /* Make sure that the .unallocated output section is empty  */
   _. = ASSERT (SIZEOF(.unallocated) == 0, "there are unallocated input sections");
}

/*  -- Set Variant Structure ---------------------------------------------------------------------------------------- */
gt_XbVar4cma = ORIGIN(var_flash);
