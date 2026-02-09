/*
 *   GNU GCC aurix platform tc299 common memory layout
 */

CPU_CACHE_LINE_SIZE = 32; /* TC299 cache line size in bytes*/

MEMORY
{
   no_mem                 (!x)  :   org = 0x00000000,             len =     0     /* empty region for unused sections */

/* RAMs */
   core2_dspr            (w!x)  :   org = 0x50000000,             len =  240K
   core2_pspr             (rx)  :   org = 0x50100000,             len =   32K

   core1_dspr            (w!x)  :   org = 0x60000000,             len =  240K-64
   core1_syms            (w!x)  :   org = 0x60000000 + 240K - 64, len =    64     /* safety exposed-if memory */
   core1_pspr             (rx)  :   org = 0x60100000,             len =   32K

   core0_dspr            (w!x)  :   org = 0x70000000,             len =  120K-64
   fl_persistent         (w!x)  :   org = 0x70000000 + 120K - 64, len =    56     /* persistent memory for flashloader */
   core0_sync            (w!x)  :   org = 0x70000000 + 120K -  8, len =     8     /* startup code synch barrier */
   core0_pspr             (rx)  :   org = 0x70100000,             len =   32K

   lmu                    (wx)  :   org = 0x90000000,             len =   32K
   emem                  (w!x)  :   org = 0x9F000000,             len =    2M

/* PFlash Memories */
   pflash0                (rx)  :   org = 0x80000000,             len =    2M
   pflash1                (rx)  :   org = 0x80200000,             len =    2M
   pflash2                (rx)  :   org = 0x80400000,             len =    2M
   pflash3                (rx)  :   org = 0x80600000,             len =    2M
   pflash                 (rx)  :   org = 0x80000000,             len =    8M        /* aggregate of pflash0 .. pflash3

/* Fixed application boot sectors */
   available_loader_bmi   (rx)  :   org = 0x80000000,             len =   48K        /* loader region with BMI */
   available_loader_bulk  (rx)  :   org = 0x80018000,             len =  288K        /* loader bulk region */
   core0_boot_rom         (rx)  :   org = 0x8000C000,             len =   16K        /* core 0 boot sector */
   core1_boot_rom         (rx)  :   org = 0x80010000,             len =   16K        /* core 1 boot sector */
   core2_boot_rom         (rx)  :   org = 0x80014000,             len =   16K        /* core 2 boot sector */
   hsm_reserved           (rx)  :   org = 0x80060000,             len =  128K        /* hsm reserved sectors */

/* Misc peripheral memories */
   mcs0_ram               (rx)  :   org = 0xF0138000,             len =    6K
   mcs1_ram               (rx)  :   org = 0xF0140000,             len =    6K
   mcs2_ram               (rx)  :   org = 0xF0148000,             len =    6K
   mcs3_ram               (rx)  :   org = 0xF0150000,             len =    6K
   mcs4_ram               (rx)  :   org = 0xF0158000,             len =    6K
   mcs5_ram               (rx)  :   org = 0xF0160000,             len =    6K

/* Misc ROM */
   dflash0                (rx)  :   org = 0xAF000000,             len =    1M + 16K
   dflash1                (rx)  :   org = 0xAF110000,             len =   64K

/* Flash ECC test pattern locations */ /* stw-trace: SM_AURIX_STL_064 */
   ecctest_bank0          (rx)  :   org = 0xA0000000 + 512K,      len =    1K
   ecctest_bank1          (rx)  :   org = 0xA0200000 + 2M - 1K,   len =    1K
   ecctest_bank2          (rx)  :   org = 0xA0400000,             len =    1K
   ecctest_bank3          (rx)  :   org = 0xA0600000 + 2M - 1K,   len =    1K

/* Flash regions available for application segmentation (for reference, target must split at flash sector borders) */
   available_pflash0      (rx)  :   org = 0x80000000 + 512K + 1K, len =    2M - 512K - 1K
   available_pflash1      (rx)  :   org = 0x80200000,             len =    2M - 1K
   available_pflash2      (rx)  :   org = 0x80400000 + 1K,        len =    2M - 1K
   available_pflash3      (rx)  :   org = 0x80600000,             len =    2M - 1K
}

/* Define uncached counterparts where applicable */
MEMORY
{
   pflash0_uc             (rx) :   org = ORIGIN(pflash0)         | 0x20000000, len = LENGTH(pflash0)
   pflash1_uc             (rx) :   org = ORIGIN(pflash1)         | 0x20000000, len = LENGTH(pflash1)
   pflash2_uc             (rx) :   org = ORIGIN(pflash2)         | 0x20000000, len = LENGTH(pflash2)
   pflash3_uc             (rx) :   org = ORIGIN(pflash3)         | 0x20000000, len = LENGTH(pflash3)
   pflash_uc              (rx) :   org = ORIGIN(pflash)          | 0x20000000, len = LENGTH(pflash)

   core0_boot_rom_uc      (rx) :   org = ORIGIN(core0_boot_rom)  | 0x20000000, len = LENGTH(core0_boot_rom)
   core1_boot_rom_uc      (rx) :   org = ORIGIN(core1_boot_rom)  | 0x20000000, len = LENGTH(core1_boot_rom)
   core2_boot_rom_uc      (rx) :   org = ORIGIN(core2_boot_rom)  | 0x20000000, len = LENGTH(core2_boot_rom)

   lmu_uc                 (wx) :   org = ORIGIN(lmu)             | 0x20000000, len = LENGTH(lmu)
   emem_uc                (wx) :   org = ORIGIN(emem)            | 0x20000000, len = LENGTH(emem)
}

REGION_MIRROR("pflash0_uc",           "pflash0")
REGION_MIRROR("pflash1_uc",           "pflash1")
REGION_MIRROR("pflash2_uc",           "pflash2")
REGION_MIRROR("pflash3_uc",           "pflash3")
REGION_MIRROR("pflash_uc",            "pflash")
REGION_MIRROR("core0_boot_rom_uc",    "core0_boot_rom")
REGION_MIRROR("core1_boot_rom_uc",    "core1_boot_rom")
REGION_MIRROR("core2_boot_rom_uc",    "core2_boot_rom")

/* Convert specific Application start labels to a generic one that is understood by the startup code */
MEM_CORE0_START_ADDR  = ORIGIN(core0_boot_rom);
MEM_CORE1_START_ADDR  = ORIGIN(core1_boot_rom);
MEM_CORE2_START_ADDR  = ORIGIN(core2_boot_rom);

/* -- Coupled memories symbols -------------------------------------------------------------------------------------- */

MEM_CORE0_DSPR_START = ORIGIN(core0_dspr);
MEM_CORE0_DSPR_END   = ORIGIN(core0_dspr) + LENGTH(core0_dspr);
MEM_CORE0_PSPR_START = ORIGIN(core0_pspr);
MEM_CORE0_PSPR_END   = ORIGIN(core0_pspr) + LENGTH(core0_pspr);

MEM_CORE1_DSPR_START = ORIGIN(core1_dspr);
MEM_CORE1_DSPR_END   = ORIGIN(core1_dspr) + LENGTH(core1_dspr);
MEM_CORE1_PSPR_START = ORIGIN(core1_pspr);
MEM_CORE1_PSPR_END   = ORIGIN(core1_pspr) + LENGTH(core1_pspr);

MEM_CORE2_DSPR_START = ORIGIN(core2_dspr);
MEM_CORE2_DSPR_END   = ORIGIN(core2_dspr) + LENGTH(core2_dspr);
MEM_CORE2_PSPR_START = ORIGIN(core2_pspr);
MEM_CORE2_PSPR_END   = ORIGIN(core2_pspr) + LENGTH(core2_pspr);

/* -- LMU Boundaries ------------------------------------------------------------------------------------------------ */
MEM_LMU_START            = ORIGIN(lmu);
MEM_LMU_END              = ORIGIN(lmu) + LENGTH(lmu);
MEM_LMU_UC_START         = ORIGIN(lmu_uc);
MEM_LMU_UC_END           = ORIGIN(lmu_uc) + LENGTH(lmu_uc);

/* -- EMEM Boundaries ----------------------------------------------------------------------------------------------- */
MEM_EMEM_START           = ORIGIN(emem);
MEM_EMEM_END             = ORIGIN(emem) + LENGTH(emem);
MEM_EMEM_UC_START        = ORIGIN(emem_uc);
MEM_EMEM_UC_END          = ORIGIN(emem_uc) + LENGTH(emem_uc);
MEM_EMEM_SIZE            = LENGTH(emem_uc);

/* -- PFLASH Boundaries --------------------------------------------------------------------------------------------- */
MEM_PFLASH_START         = ORIGIN(pflash);
MEM_PFLASH_END           = ORIGIN(pflash)    + LENGTH(pflash);
MEM_PFLASH_UC_START      = ORIGIN(pflash_uc);
MEM_PFLASH_UC_END        = ORIGIN(pflash_uc) + LENGTH(pflash_uc);

/* -- DFLASH Boundaries --------------------------------------------------------------------------------------------- */
MEM_DFLASH_START         = ORIGIN(dflash0);
MEM_DFLASH_END           = ORIGIN(dflash1) + LENGTH(dflash1);


/* -- Exposed-If ---------------------------------------------------------------------------------------------------- */
/*
 *    Define global visible Table for all cores.
 *    The safe table can only be set by the safety master core. Non-interfering cores have only read-only access.
 *    The non-interfering table can be set by all cores.
 *    Target integration needs to define core1_syms memory (or aliases)
 */
SECTIONS
{
   /* define an uninitialized list of global symbols */
   .safe_master_syms_table (NOLOAD) :
   {
      PROVIDE(gpt_ExposedIf = .);
   } AT> core1_syms
}

/* -- Bootloader interop -------------------------------------------------------------------------------------------- */
/*
 *   Define symbol for information exchange between bootloader, itself and application. Interface is defined in the
 *   generic bootloader project.
 */

SECTIONS
{
   .bootloader_boot_params (NOLOAD) :
   {
      PROVIDE(gt_BootloaderBootParams = .);
   } AT> fl_persistent
}

/* -- Barrier Memory ------------------------------------------------------------------------------------------------ */
STARTUP_CODE_SYNC_ADDR = ORIGIN(core0_sync);

/* -- Sanity Checks ------------------------------------------------------------------------------------------------- */

_. = ASSERT ((MEM_EMEM_START       & 0x1F) == 0, "EMEM start (cached) misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_EMEM_END         & 0x1F) == 0, "EMEM end (cached)   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_EMEM_UC_START    & 0x1F) == 0, "EMEM start          misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_EMEM_UC_END      & 0x1F) == 0, "EMEM end            misaligned (32-Byte alignment)!");

_. = ASSERT ((MEM_CORE0_DSPR_START & 0x1F) == 0, "CORE0 DSPR start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE0_DSPR_END   & 0x1F) == 0, "CORE0 DSPR end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE0_PSPR_START & 0x1F) == 0, "CORE0 PSPR start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE0_PSPR_END   & 0x1F) == 0, "CORE0 PSPR end   misaligned (32-Byte alignment)!")
;
_. = ASSERT ((MEM_CORE1_DSPR_START & 0x1F) == 0, "CORE1 DSPR start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE1_DSPR_END   & 0x1F) == 0, "CORE1 DSPR end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE1_PSPR_START & 0x1F) == 0, "CORE1 PSPR start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE1_PSPR_END   & 0x1F) == 0, "CORE1 PSPR end   misaligned (32-Byte alignment)!");

_. = ASSERT ((MEM_CORE2_DSPR_START & 0x1F) == 0, "CORE2 DSPR start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE2_DSPR_END   & 0x1F) == 0, "CORE2 DSPR end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE2_PSPR_START & 0x1F) == 0, "CORE2 PSPR start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_CORE2_PSPR_END   & 0x1F) == 0, "CORE2 PSPR end   misaligned (32-Byte alignment)!");

