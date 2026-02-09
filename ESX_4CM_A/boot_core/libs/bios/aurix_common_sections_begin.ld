/* --------------------------------------------------------------------------------------------------------------------
   GNU GCC common sections linker script for Tricore AURIX Projects

   This file defines the standard linker script definitions and sections, which must be defined before any project
   specific sections.

   NOTE:
   Some symbols exported by the linker must adhere to certain address or alignement requirements.
   We use different pre- and postfixes to distinguish between different types of addresses.

      Linker Script Symbol naming conventions:
      * Postfixes:
         - LD_.._START              = symbol exports (absolute) start address of a certain memory range (e.g. section)
         - LD_.._END                = symbol exports (absolute) end address of a certain memory range (e.g. section)
         - LD_.._ADDR               = symbol exports arbitrary address (absolute) value

         -    .._STOP               = a symbol to reference the cursor inside a memory after all output sections were
                                      allocated to the memory

      *Prefixes:
      - SEC_...                     = Section address (absolute), used only internally by the linker
      - LD_...                      = Section address (absolute) that will be exported by a header file.
      - LD_MPU_...                  = Section address (absolute) that will be exported by a header file.
                                      The address is suitable to be used by CPU MPU
                                      (8-Byte Aligned, 8 Byte safety zone at start address after symbol address)
      - LD_BUS_MPU_...              = Section address (absolute) that will be exported by a header file.
                                      The address is suitable to be used by Bus MPU
                                      (32-Byte Aligned, 8 Byte safety zone at start address after symbol address)
                                      Note: Bus MPU addresses are also suitable for CPU MPU.

   Arguments (? = optional memory (ie. no_mem) or variable; + = Defaulted variable, only redefine if required):
      Name                             Type          Description
      MEM_BOOT_ROM                     Memory        Memory into which to place the boot code
                                                     (and per default the sig block)
      MEM_LOAD_ROM                     Memory        Memory used for load region of .data

      MEM_DEFAULT_ROM_0                Memory?       Segment of default rom (.text and .rodata)
      MEM_DEFAULT_ROM_1                Memory?       Segment of default rom (.text and .rodata)
      MEM_DEFAULT_ROM_2                Memory?       Segment of default rom (.text and .rodata)
      MEM_DEFAULT_ROM_3                Memory?       Segment of default rom (.text and .rodata)
      MEM_DEFAULT_ROM_4                Memory?       Segment of default rom (.text and .rodata)

      MEM_DEFAULT_RM_0                 Memory?        Segment of default ram (.data and .bss)
      MEM_DEFAULT_RM_1                 Memory?        Segment of default ram (.data and .bss)
      MEM_DEFAULT_RM_2                 Memory?        Segment of default ram (.data and .bss)
      MEM_DEFAULT_RM_3                 Memory?        Segment of default ram (.data and .bss)
      MEM_DEFAULT_RM_4                 Memory?        Segment of default ram (.data and .bss)

      MEM_LMU                          Memory?        LMU memory to use
      MEM_DSPR                         Memory         DSPR memory to use
      MEM_PSPR                         Memory         PSPR memory to use

      MEM_USER_HEAP_MIN_SIZE           Size           Minimum user heap size in bytes, assertion if smaller
      MEM_USER_HEAP_MAX_SIZE           Size?          If defined, limit the size of the user heap to this value,
                                                      otherwise fill up user heap memory

      MEM_SYSTEM_HEAP_MIN_SIZE         Size           Minimum system heap size in bytes, assertion if smaller
      MEM_SYSTEM_HEAP_MAX_SIZE         Size?          If defined, limit the size of the user heap to this value,
                                                      otherwise fill up system heap memory

      MEM_USE_SAFETLIB                 0/1            Whether to define special SafeTlib-Sections
      MEM_STL_SBST_DSPR_IUSTACK_SIZE   Size+          If MEM_USE_SAFETLIB: Size of SafeTlib-Stack in DSPR
      MEM_STL_SBST_PSPR_SIZE           Size+          If MEM_USE_SAFETLIB: Size of SafeTlib-Code in PSPR

      MEM_ISTACK_SIZE                  Size           Size of interrupt stack in bytes
      MEM_USTACK_SIZE                  Size           Size of startup thread stack in bytes

-------------------------------------------------------------------------------------------------------------------- */

OUTPUT_FORMAT("elf32-tricore")
OUTPUT_ARCH(tricore)
ENTRY(_start)

EXTERN(_start)

/*
 * memory statistics
 */
MEM_BOOT_ROM_START      = ORIGIN(MEM_BOOT_ROM);
MEM_BOOT_ROM_END        = ORIGIN(MEM_BOOT_ROM) + LENGTH(MEM_BOOT_ROM);
MEM_BOOT_ROM_SIZE       = LENGTH(MEM_BOOT_ROM);
MEM_BOOT_ROM_USED       = MEM_BOOT_ROM_USED_END_ADDR - MEM_BOOT_ROM_START;

MEM_DEFAULT_ROM_0_START = ORIGIN(MEM_DEFAULT_ROM_0);
MEM_DEFAULT_ROM_0_END   = ORIGIN(MEM_DEFAULT_ROM_0) + LENGTH(MEM_DEFAULT_ROM_0);
MEM_DEFAULT_ROM_0_SIZE  = LENGTH(MEM_DEFAULT_ROM_0);
MEM_DEFAULT_ROM_0_USED  = MEM_DEFAULT_ROM_0_STOP - MEM_DEFAULT_ROM_0_START;

MEM_DEFAULT_ROM_1_START = ORIGIN(MEM_DEFAULT_ROM_1);
MEM_DEFAULT_ROM_1_END   = ORIGIN(MEM_DEFAULT_ROM_1) + LENGTH(MEM_DEFAULT_ROM_1);
MEM_DEFAULT_ROM_1_SIZE  = LENGTH(MEM_DEFAULT_ROM_1);
MEM_DEFAULT_ROM_1_USED  = MEM_DEFAULT_ROM_1_STOP - MEM_DEFAULT_ROM_1_START;

MEM_DEFAULT_ROM_2_START = ORIGIN(MEM_DEFAULT_ROM_2);
MEM_DEFAULT_ROM_2_END   = ORIGIN(MEM_DEFAULT_ROM_2) + LENGTH(MEM_DEFAULT_ROM_2);
MEM_DEFAULT_ROM_2_SIZE  = LENGTH(MEM_DEFAULT_ROM_2);
MEM_DEFAULT_ROM_2_USED  = MEM_DEFAULT_ROM_2_STOP - MEM_DEFAULT_ROM_2_START;

MEM_DEFAULT_ROM_3_START = ORIGIN(MEM_DEFAULT_ROM_3);
MEM_DEFAULT_ROM_3_END   = ORIGIN(MEM_DEFAULT_ROM_3) + LENGTH(MEM_DEFAULT_ROM_3);
MEM_DEFAULT_ROM_3_SIZE  = LENGTH(MEM_DEFAULT_ROM_3);
MEM_DEFAULT_ROM_3_USED  = MEM_DEFAULT_ROM_3_STOP - MEM_DEFAULT_ROM_3_START;

MEM_DEFAULT_ROM_4_START = ORIGIN(MEM_DEFAULT_ROM_4);
MEM_DEFAULT_ROM_4_END   = ORIGIN(MEM_DEFAULT_ROM_4) + LENGTH(MEM_DEFAULT_ROM_4);
MEM_DEFAULT_ROM_4_SIZE  = LENGTH(MEM_DEFAULT_ROM_4);
MEM_DEFAULT_ROM_4_USED  = MEM_DEFAULT_ROM_4_STOP - MEM_DEFAULT_ROM_4_START;

MEM_DEFAULT_RAM_0_START = ORIGIN(MEM_DEFAULT_RAM_0);
MEM_DEFAULT_RAM_0_END   = ORIGIN(MEM_DEFAULT_RAM_0) + LENGTH(MEM_DEFAULT_RAM_0);
MEM_DEFAULT_RAM_0_SIZE  = LENGTH(MEM_DEFAULT_RAM_0);
MEM_DEFAULT_RAM_0_USED  = MEM_DEFAULT_RAM_0_STOP - MEM_DEFAULT_RAM_0_START;

MEM_DEFAULT_RAM_1_START = ORIGIN(MEM_DEFAULT_RAM_1);
MEM_DEFAULT_RAM_1_END   = ORIGIN(MEM_DEFAULT_RAM_1) + LENGTH(MEM_DEFAULT_RAM_1);
MEM_DEFAULT_RAM_1_SIZE  = LENGTH(MEM_DEFAULT_RAM_1);
MEM_DEFAULT_RAM_1_USED  = MEM_DEFAULT_RAM_1_STOP - MEM_DEFAULT_RAM_1_START;

MEM_DEFAULT_RAM_2_START = ORIGIN(MEM_DEFAULT_RAM_2);
MEM_DEFAULT_RAM_2_END   = ORIGIN(MEM_DEFAULT_RAM_2) + LENGTH(MEM_DEFAULT_RAM_2);
MEM_DEFAULT_RAM_2_SIZE  = LENGTH(MEM_DEFAULT_RAM_2);
MEM_DEFAULT_RAM_2_USED  = MEM_DEFAULT_RAM_2_STOP - MEM_DEFAULT_RAM_2_START;

MEM_DEFAULT_RAM_3_START = ORIGIN(MEM_DEFAULT_RAM_3);
MEM_DEFAULT_RAM_3_END   = ORIGIN(MEM_DEFAULT_RAM_3) + LENGTH(MEM_DEFAULT_RAM_3);
MEM_DEFAULT_RAM_3_SIZE  = LENGTH(MEM_DEFAULT_RAM_3);
MEM_DEFAULT_RAM_3_USED  = MEM_DEFAULT_RAM_3_STOP - MEM_DEFAULT_RAM_3_START;

MEM_DEFAULT_RAM_4_START = ORIGIN(MEM_DEFAULT_RAM_4);
MEM_DEFAULT_RAM_4_END   = ORIGIN(MEM_DEFAULT_RAM_4) + LENGTH(MEM_DEFAULT_RAM_4);
MEM_DEFAULT_RAM_4_SIZE  = LENGTH(MEM_DEFAULT_RAM_4);
MEM_DEFAULT_RAM_4_USED  = MEM_DEFAULT_RAM_4_STOP - MEM_DEFAULT_RAM_4_START;

MEM_DEFAULT_LMU_START  = ORIGIN(MEM_LMU);
MEM_DEFAULT_LMU_END    = ORIGIN(MEM_LMU) + LENGTH(MEM_LMU);
MEM_DEFAULT_LMU_SIZE   = LENGTH(MEM_LMU);
MEM_DEFAULT_LMU_USED   = MEM_LMU_STOP - MEM_DEFAULT_LMU_START;

MEM_DSPR_START = ORIGIN(MEM_DSPR);
MEM_DSPR_END   = ORIGIN(MEM_DSPR) + LENGTH(MEM_DSPR);
MEM_DSPR_SIZE  = LENGTH(MEM_DSPR);
MEM_DSPR_USED  = MEM_DSPR_USED_END_ADDR - MEM_DSPR_START;

MEM_PSPR_START = ORIGIN(MEM_PSPR);
MEM_PSPR_END   = ORIGIN(MEM_PSPR) + LENGTH(MEM_PSPR);
MEM_PSPR_SIZE  = LENGTH(MEM_PSPR);
MEM_PSPR_USED  = MEM_PSPR_USED_END_ADDR - MEM_PSPR_START;

LD_CSA_END     = ORIGIN(MEM_DSPR) + LENGTH(MEM_DSPR);  /* end of CSA area */
LD_CSA_SIZE    = LD_CSA_END - LD_CSA_START;  /* define CSA size */

LD_ISTACK   = LD_MPU_ISTACK_END; /* export initial ISTACK value for startup code */
LD_USTACK   = LD_MPU_USTACK_END; /* export initial USTACK value for startup code */

/*
 * The heap is the memory between LD_USER_HEAP_START_ADDR and LD_USER_HEAP_END
 * programs can dynamically allocate space in this area using malloc()
 * and various other functions.
 */
MEM_USER_HEAP_MIN_SIZE   = DEFINED (MEM_USER_HEAP_MIN_SIZE) ? MEM_USER_HEAP_MIN_SIZE : 2K;
MEM_SYSTEM_HEAP_MIN_SIZE = DEFINED (MEM_SYSTEM_HEAP_MIN_SIZE) ? MEM_SYSTEM_HEAP_MIN_SIZE : 2K;

/* Define platform defaults for SafeTlib DSPR and PSPR regions */
MEM_STL_SBST_DSPR_IUSTACK_SIZE = DEFINED(MEM_STL_SBST_DSPR_IUSTACK_SIZE) ? MEM_STL_SBST_DSPR_IUSTACK_SIZE : 0x100;
MEM_STL_SBST_PSPR_SIZE         = DEFINED(MEM_STL_SBST_PSPR_SIZE) ? MEM_STL_SBST_PSPR_SIZE :   0x40;

/* check user / interrupt stack address */
_. = ASSERT ((LD_ISTACK & 7) == 0 , "Interrupt stack not doubleword aligned");
_. = ASSERT ((LD_USTACK & 7) == 0 , "User stack not doubleword aligned");

/* Make sure CSA, stack and heap addresses are properly aligned.  */
_. = ASSERT ((LD_CSA_START & 0x3f) == 0 , "illegal CSA start address");
_. = ASSERT ((LD_CSA_SIZE & 0x3f) == 0 , "illegal CSA size");
_. = ASSERT (LD_CSA_SIZE >= MEM_CSA_MIN_SIZE , "not enough memory for CSA");
_. = ASSERT ((LD_CSA_SIZE >> 6) > MEM_LCX_HEADROOM , "LCX does not fit in CSA area");
_. = ASSERT (MEM_LCX_HEADROOM >= 6 , "MEM_LCX_HEADROOM should be >= 6");

/*
 *    LD_MPU_ISTACK_START must be set to the start address of the memory region so that a stack overflow
 *    will result in invalid address trap
 */
_. = ASSERT (LD_MPU_ISTACK_START == (ORIGIN(MEM_DSPR)), "Interrupt stack (LD_MPU_ISTACK_START) must start with first memory segment address!");

/* -- Boot Rom Symbols ---------------------------------------------------------------------------------------------- */

/* -- Default Ram Symbols ------------------------------------------------------------------------------------------- */

/* -- Default Rom Symbols ------------------------------------------------------------------------------------------- */

/* Verify CPU- and Bus-MPU alignment requirements */

_. = ASSERT ((LD_MPU_LMU_DEFAULT_START     & 0x7) == 0, "LMU Default SRAM start (cached)  misaligned (8-Byte alignment)!");
_. = ASSERT ((LD_MPU_LMU_DEFAULT_END       & 0x7) == 0, "LMU Default SRAM end (cached)    misaligned (8-Byte alignment)!");
_. = ASSERT ((LD_MPU_LMU_DEFAULT_UC_START  & 0x7) == 0, "LMU Default SRAM start           misaligned (8-Byte alignment)!");
_. = ASSERT ((LD_MPU_LMU_DEFAULT_UC_END    & 0x7) == 0, "LMU Default SRAM end             misaligned (8-Byte alignment)!");

_. = ASSERT ((MEM_DSPR_START          & 0x1F) == 0, "DSPR start          misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DSPR_END            & 0x1F) == 0, "DSPR end            misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_PSPR_START          & 0x1F) == 0, "PSPR start          misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_PSPR_END            & 0x1F) == 0, "PSPR end            misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_0_START & 0x1F) == 0, "DEFAULT_RAM_0 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_0_END   & 0x1F) == 0, "DEFAULT_RAM_0 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_1_START & 0x1F) == 0, "DEFAULT_RAM_1 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_1_END   & 0x1F) == 0, "DEFAULT_RAM_1 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_2_START & 0x1F) == 0, "DEFAULT_RAM_2 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_2_END   & 0x1F) == 0, "DEFAULT_RAM_2 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_3_START & 0x1F) == 0, "DEFAULT_RAM_3 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_3_END   & 0x1F) == 0, "DEFAULT_RAM_3 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_4_START & 0x1F) == 0, "DEFAULT_RAM_4 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_RAM_4_END   & 0x1F) == 0, "DEFAULT_RAM_4 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_0_START & 0x1F) == 0, "DEFAULT_ROM_0 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_0_END   & 0x1F) == 0, "DEFAULT_ROM_0 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_1_START & 0x1F) == 0, "DEFAULT_ROM_1 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_1_END   & 0x1F) == 0, "DEFAULT_ROM_1 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_2_START & 0x1F) == 0, "DEFAULT_ROM_2 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_2_END   & 0x1F) == 0, "DEFAULT_ROM_2 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_3_START & 0x1F) == 0, "DEFAULT_ROM_3 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_3_END   & 0x1F) == 0, "DEFAULT_ROM_3 end   misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_4_START & 0x1F) == 0, "DEFAULT_ROM_4 start misaligned (32-Byte alignment)!");
_. = ASSERT ((MEM_DEFAULT_ROM_4_END   & 0x1F) == 0, "DEFAULT_ROM_4 end   misaligned (32-Byte alignment)!");

/* Define a default symbol for address 0.  */
NULL = DEFINED (NULL) ? NULL : 0;

/* force linking of library version information */
/* even if not referenced by the application    */
EXTERN(gt_x_lib_version);


/*
 *   Now we start with the definition of the output sections.
 *   The interrupt vector table needs no load memory (flash).
 *   As it shall be located at the first PSPR memory region, we can assign it first.
 */
SECTIONS
{
   /* Interrupt vector table shall be fist in PSPR RAM */
   .cpux_ivect (NOLOAD) :
   {
      KEEP(*(.cpu_intvect))
   } > MEM_PSPR

   _. = ASSERT((ADDR (.cpux_ivect) & 0x7FE0) == 0 ,
               "Cannot locate Interrupt Vector Table at this location (check output address of .cpux_ivect)!");
}


/*
 *   Now we start with the definition of the output sections.
 *   The following output sections shall locate flash memory in a ordered fashion.
 */
SECTIONS
{
   /*
    *   We start with the boot code that must be the first address of
    *   application rom. It is expected that the first address of the
    *   boot rom will be execued due to a cpu reset or a jump from
    *   another application.
    */

   .boot (MEM_START_ADDR) :
   {
      KEEP(*(.boot_code))
   } > MEM_BOOT_ROM = 0

   /*
    * This magic key signals a valid application to the flash loader
    */
   .validprogramkeys (MEM_START_ADDR + 0x18) :
   {
      gt_LinkerInfoBlock = .;
      /* This magic key signals a valid application to the flash loader */
      LONG(0x900dc0de);
      /*
       *  This memory layout identifier key defines which (RAM/ROM) layout has been used during linking.
       *  it must be the same for all CORE applications of a multi-core project.
       */
      LONG(MEMORY_LAYOUT_IDENTIFIER);
   } > MEM_BOOT_ROM= 0

   /*
    * allocate the default trap vector table code at address offset 0x100 (load address)
    * the run time location is the scratchpad ram memory (int_tvram)
    * the code is loaded by the startup code (see also __copy_table)
    */
   .cpux_tvect : AT(ORIGIN(MEM_BOOT_ROM) + 0x100)
   {
      KEEP(*(.traptab))
   } > MEM_PSPR

   ._ = ASSERT((ADDR(.cpux_tvect) & 0xFF) == 0,
               "The trap table should be located at least to a 256 Byte boundary address!");

   /*
    * export the trap vector table load address for tc core library
    */
   gu32_Ld_DefaultTrapVect = LOADADDR(.cpux_tvect);

   /*
    * Allocate SafeTlib PSPR code afet interrupt and trap vector table (but within first 16k memory window)
    */
   /*stw-trace SM_AURIX_STL_121*/
   .safetlib_cpu_sbst_pspr : ALIGN(64)
   {
      /* Export (local) base address for SafeTLib SBST PSPR memory */
      SAFETLIB_SBST_PSPR_BASE_ADDR = ABSOLUTE(.);

      /* Reserve memroy for SafeTLib CPU SBST (If configured, either zero or 0x100) */
      . += (MEM_USE_SAFETLIB != 0) ? MEM_STL_SBST_PSPR_SIZE : 0;
   } AT> MEM_PSPR

   ._ = ASSERT((ADDR(.safetlib_cpu_sbst_pspr) & 0xFFFFC000) == ORIGIN(MEM_PSPR),
               "SafeTLib SBST memory must be within first 16k of local PSPR!");

   /*
    * allocate block signature data at address offset 0x20
    * NOTE: This will be done in the target specific "signature block" linker script
    */


   /*
   * provide the end address of the used ROM area (1st. unused address)
   * C usage (external definition):
   * extern const uint32 gu32_Ld_EndOfUsedPFlash0;
   */
   .romendinfo (ORIGIN(MEM_BOOT_ROM) + 0x400): ALIGN(4)
   {
      gu32_Ld_EndOfUsedRom0 = .;             /* provide this symbol for HLL access        */
      LONG(MEM_DEFAULT_ROM_0_STOP); /* 1st unused ROM0 address                   */
      gu32_Ld_EndOfUsedRom1 = .;             /* provide this symbol for HLL access        */
      LONG(MEM_DEFAULT_ROM_1_STOP); /* 1st unused ROM1 address                   */
      gu32_Ld_EndOfUsedRom2 = .;             /* provide this symbol for HLL access        */
      LONG(MEM_DEFAULT_ROM_2_STOP); /* 1st unused ROM2 address                   */
      gu32_Ld_EndOfUsedRom3 = .;             /* provide this symbol for HLL access        */
      LONG(MEM_DEFAULT_ROM_3_STOP); /* 1st unused ROM3 address                   */
      gu32_Ld_EndOfUsedRom4 = .;             /* provide this symbol for HLL access        */
      LONG(MEM_DEFAULT_ROM_4_STOP); /* 1st unused ROM3 address                   */
      *(.romendinfo)                         /* add target specific .romendinfo sections  */
   } > MEM_BOOT_ROM = 0

   /*
    * allocate the ECU (mainboard) library information data and external libraries information data
    */
   .libversion  :
   {
      KEEP(*(.libversion))
      KEEP(*(.extlibinfo))
   } AT> MEM_BOOT_ROM= 0

   /*
    * Place rest of startup code in boot rom right after the fixed locations
    */
   .startup_code : ALIGN(4)
   {
      KEEP(*(.startup_code))
   } AT> MEM_BOOT_ROM = 0

   /*
    * Create the clear and copy tables that tell the startup code
    * which memory areas to clear and to copy, respectively.
    */
   .clear_sec : ALIGN(4)
   {
      __clear_table = .;
      LONG(0 + ADDR(.cpux_ivect));   LONG(SIZEOF(.cpux_ivect));

      /* include additional clear sections */
      *(.clear_sec)
   } AT> MEM_BOOT_ROM = 0

    /*
    * The copy table is used for sections that are special, because their
    * initial code or data (if any) must also be stored in the ROM part of an
    * executable, but they "live" at completely different addresses
    * at runtime -- usually in RAM areas.  NOTE: This is not really
    * necessary if you use a special program loader (e.g., a debugger)
    * to load a complete executable consisting of code, data, BSS, etc.
    * into the RAM of some target hardware or a simulator, but it *is*
    * necessary if you want to burn your application into non-volatile
    * memories such as EPROM or FLASH.
    */
   .copy_sec : ALIGN(4)
   {
      __copy_table = .;
      LONG(LOADADDR(.cpux_tvect));   LONG(0 + ADDR(.cpux_tvect));     LONG(SIZEOF(.cpux_tvect));
      /* include additional copy sections */
      *(.copy_sec)
   } AT> MEM_BOOT_ROM = 0
}

/*
 *  Stack, Interrupt vector SafeTLib SBST RAM, absolute addressable data and user stack
 *  NOTE: All this areas must be located in DSPR, exactly in this order!
 */
SECTIONS
{
   /*
    * allocate interrupt stack
    */
   .istack :
   {
      . = ALIGN(8);
      LD_MPU_ISTACK_START = ABSOLUTE (.);
      /* we don't need extra 8 byte MPU safety zone for ISTACK as this section contains no static data */
      . += MEM_ISTACK_SIZE;
      /* ISTACK must be 64 Bit aligned */
      . = ALIGN(8);
      LD_MPU_ISTACK_END = ABSOLUTE(.);
      /* Add an 32 Byte gap for the MPU. This is needed to protect the ISTACK from USTACK overflow */
      . += 32;
   } > MEM_DSPR

   /*
    *    LD_MPU_ISTACK_START must be set to the start address of the memory region so that a stack overflow
    *    will result in invalid address trap
    */
   _. = ASSERT (LD_MPU_ISTACK_START == (ORIGIN(MEM_DSPR)), "Interrupt stack (LD_MPU_ISTACK_START) must start with first memory segment address!");

   /*stw-trace SM_AURIX_STL_120*/
   .safetlib_sbst_dspr :
   {
      /* Align to next 64 Byte Boundary for SafeTLib DSPR RAM section */
      . = ALIGN(64);
      /* Export base address for SafeTLib SBST DSPR Memory */
      SAFETLIB_SBST_DSPR_IUSTACK_BASE_ADDR = ABSOLUTE(.);
      /* Reserve memroy for SafeTLib CPU SBST (If configured, either zero or 0x100) */
      . += (MEM_USE_SAFETLIB != 0) ? MEM_STL_SBST_DSPR_IUSTACK_SIZE : 0;
   } > MEM_DSPR


   /*
    * Allocate space for absolute addressable sections; this requires that the
    * zdata and zbss section starts at a TriCore segment (256M) and points to
    * some RAM area! This condition can only be met in DSPR RAM for all Cores.
    * Therefore, we define a z - section after interrupt stack and safetlib sbst
    * data section. The remaining memory of the first 16k DSPR ram can be used
    * for absolute addressable data.
    */
   .zdata : ALIGN(8) FLAGS(awzl)
   {
      SEC_ZDATA_START_ADDR = ABSOLUTE(.);
      . = ALIGN(32);
      LD_BUS_MPU_ZDATA_START = ABSOLUTE(.);
      /* Add 8 Byte safety gap before start of zdata */
      . += 8;
      *(.zdata)
      *(.zdata.*)
      *(.gnu.linkonce.z.*)
      *(.bdata)
      *(.bdata.*)
      . = ALIGN(8);
      SEC_ZDATA_END_ADDR = ABSOLUTE(.);
   } > MEM_DSPR AT> MEM_LOAD_ROM

   /* make sure, that zdata is placed at an absolute addressable memory region */
   _. = ASSERT(((SEC_ZDATA_START_ADDR & 0x0FFFC000) == 0) &&
               ((SEC_ZDATA_END_ADDR   & 0x0FFFC000) == 0) &&
               ((SEC_ZDATA_START_ADDR & 0xF0000000) == (SEC_ZDATA_END_ADDR & 0xF0000000)),
               "zdata section must be inside an absolute addressable memory region!");

   .zbss (NOLOAD) : ALIGN(8) FLAGS(awzl)
   {
      SEC_ZBSS_START_ADDR = ABSOLUTE(.);
      *(.zbss)
      *(.zbss.*)
      *(.gnu.linkonce.zb.*)
      *(.bbss)
      *(.bbss.*)
      . = ALIGN(32);
      SEC_ZBSS_END_ADDR = ABSOLUTE(.);
      LD_BUS_MPU_ZDATA_END = ABSOLUTE(.);
   } > MEM_DSPR

   /* make sure that zbss is placed at an absolute addressable memory region */
   _. = ASSERT(((SEC_ZBSS_START_ADDR & 0x0FFFC000) == 0) &&
               ((SEC_ZBSS_END_ADDR   & 0x0FFFC000) == 0) &&
               ((SEC_ZBSS_START_ADDR & 0xF0000000) == (SEC_ZBSS_END_ADDR & 0xF0000000)),
               "zbss section must be inside an absolute addressable memory region!");

   /*
    * allocate user stack
    */

   .ustack :
   {
      . = ALIGN(8);
      . += 8;
      LD_MPU_USTACK_START = ABSOLUTE(.);
      . += MEM_USTACK_SIZE;
      /* USTACK must be 64 Bit aligned */
      . = ALIGN(8);
      LD_MPU_USTACK_END = ABSOLUTE(.);
      /* Add an 32 Byte gap to make a USTACK underflow better detectable by the MPU */
      . += 32;
   } > MEM_DSPR


   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.zbss));    LONG(SIZEOF(.zbss));
   } AT> MEM_BOOT_ROM


   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.zdata));   LONG(0 + ADDR(.zdata));     LONG(SIZEOF(.zdata));
   } AT> MEM_BOOT_ROM

}



/*
 *   We locate special data RAM locations for variables that need fast access.
 *   There are different memory areas for data with different safety requirements (safety classes).
 *   Safety classes are not really associated with a certain safety level, but we should use the
 *   convention that the highest safety class number shall relate with the highest safety level.
 *   (meaning: safety class 2 contains the data with the highest safety level)
 */
SECTIONS
{
/** ---------------------------------- Default Code and Dataram Sections ------------------------------------------ **/

/* Note: Sections with no safety classification are considered to have the highest safety class */
   .dspr.noinit (NOLOAD) : ALIGN(32)
   {
      . += (ORIGIN(MEM_DSPR) == 0) ? 0 : 8;  /* Conditional MPU margin */
      LD_MPU_DSPR_NOINIT_START = ABSOLUTE(.);
      KEEP(*(*.dspr.noinit))
      KEEP(*(*.dspr.noinit.*))
      . = ALIGN(8);
      LD_MPU_DSPR_NOINIT_END   = ABSOLUTE(.);
   } > MEM_DSPR


   .dspr.default : ALIGN(8)
   {
      LD_MPU_DSPR_DEFAULT_START   = ABSOLUTE(.);
      KEEP(*(*.dspr.default))
      KEEP(*(*.dspr.default.*))
      *xb_mem_server_tc2xx_emem_diag.c.obj(*.data *.data.* *.bss *.bss.*)
      . = ALIGN(8);
      LD_MPU_DSPR_DEFAULT_END = ABSOLUTE(.);
   } > MEM_DSPR AT> MEM_LOAD_ROM

   .lmu.default.uc : ALIGN(32)
   {
      . = ALIGN(CPU_CACHE_LINE_SIZE);
      LD_MPU_LMU_DEFAULT_UC_START = ABSOLUTE(.);
      . += ((LENGTH(MEM_LMU) == 0) && (LENGTH(MEM_LMU_UC) == 0)) ? 0 : 8; /* Conditional MPU margin */
      KEEP(*(*.lmu.default.uc))
      KEEP(*(*.lmu.default.uc.*))
      . = ALIGN(32);
      LD_MPU_LMU_DEFAULT_UC_END = ABSOLUTE(.);
   } > MEM_LMU_UC AT> MEM_LOAD_ROM

   .lmu.default.c : ALIGN(32)
   {
      . = ALIGN(CPU_CACHE_LINE_SIZE);
      LD_MPU_LMU_DEFAULT_START = ABSOLUTE(.);
      KEEP(*(*.lmu.default.c))
      KEEP(*(*.lmu.default.c.*))
      . = ALIGN(CPU_CACHE_LINE_SIZE);  /* MAX function is broken on hightec ld */
      . = ALIGN(32);
      LD_MPU_LMU_DEFAULT_END = ABSOLUTE(.);
   } > MEM_LMU AT> MEM_LOAD_ROM

   .pspr.default : ALIGN(8)
   {
      LD_MPU_PSPR_DEFAULT_START = ABSOLUTE(.);
      KEEP(*(*.pspr))
      KEEP(*(*.pspr.*))
      . = ALIGN(8);
      LD_MPU_PSPR_DEFAULT_END = ABSOLUTE(.);
   } > MEM_PSPR AT> MEM_LOAD_ROM

   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.pspr.default));   LONG(0 + ADDR(.pspr.default));   LONG(SIZEOF(.pspr.default));
      LONG(LOADADDR(.dspr.default));   LONG(0 + ADDR(.dspr.default ));  LONG(SIZEOF(.dspr.default));
      LONG(LOADADDR(.lmu.default.c));  LONG(0 + ADDR(.lmu.default.c));  LONG(SIZEOF(.lmu.default.c));
      LONG(LOADADDR(.lmu.default.uc)); LONG(0 + ADDR(.lmu.default.uc)); LONG(SIZEOF(.lmu.default.uc));
   }  AT> MEM_BOOT_ROM
}
