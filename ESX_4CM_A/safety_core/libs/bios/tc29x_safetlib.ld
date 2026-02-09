/* GNU GCC linker script for SafeTlib */

/* -- Declare Symbols for SBST DSPR memory -------------------------------------------------------------------------- */

/*stw-trace SM_AURIX_STL_120*/
__SBST_DSPR_BASE_ADDR = ((0x0FFFFFFF & SAFETLIB_SBST_DSPR_IUSTACK_BASE_ADDR) | 0xD0000000);  /* Need local DSPR memory address */
/*stw-trace SM_AURIX_STL_121*/
__SBST_PSPR_BASE_ADDR = ((0x000FFFFF & SAFETLIB_SBST_PSPR_BASE_ADDR) | 0xC0000000);          /* Need local PSPR memory address */
__SBST_CRC_RESULT = __SBST_DSPR_BASE_ADDR + 0x00FC;

_. = ASSERT ((__SBST_DSPR_BASE_ADDR == 0xD0001040), "Unexpected location of SBST DSPR memory (Interrupt Stack size is no longer 4k)! Please recalculate CRC value for SafeTLib CpuTst_CpuSbstPTst and adapt assertion!");
_. = ASSERT ((__SBST_PSPR_BASE_ADDR == 0xC0001100), "Unexpected location of SBST PSPR memory! Please recalculate CRC value for SafeTLib CpuTst_CpuSbstPTst and adapt assertion!");

/* -- Declare PMU test patterns as extern (otherwise, they will be discarded) --------------------------------------- */
EXTERN(PmuEccTst_TestPatternB0);
EXTERN(PmuEccTst_TestPatternB1);
EXTERN(PmuEccTst_TestPatternB2);
EXTERN(PmuEccTst_TestPatternB3);

/* -- Declare output symbols ---------------------------------------------------------------------------------------- */

MEM_STL_ROM_START = ORIGIN(MEM_STL_ROM);
MEM_STL_ROM_END   = ORIGIN(MEM_STL_ROM) + LENGTH(MEM_STL_ROM);
MEM_STL_ROM_SIZE  = LENGTH(MEM_STL_ROM);
MEM_STL_ROM_USED  = MEM_STL_ROM_STOP - MEM_STL_ROM_START;

LD_STL_ROM_START = MEM_STL_ROM_START;
LD_STL_ROM_END   = MEM_STL_ROM_END;
LD_STL_ROM_SIZE  = MEM_STL_ROM_SIZE;
LD_STL_ROM_USED  = MEM_STL_ROM_USED;

/* -- Linker definitions for SafeTlib SBST (TC299-specific) --------------------------------------------------------- */

/*stw-trace SM_AURIX_STL_015*/
__TC0_PCON2_VALUE     = 0x00200010;
__TC0_DCON2_VALUE     = 0x00780008;
__TC1_PCON2_VALUE     = 0x00200020;
__TC1_DCON2_VALUE     = 0x00F00008;
__TC2_PCON2_VALUE     = 0x00200020;
__TC2_DCON2_VALUE     = 0x00F00008;

SECTIONS
{
   /*stw-trace SM_AURIX_STL_119*/
   .text.ifx_opcode_test.asil_b._SBST_Test  : ALIGN(32768)
   {
      /* SBST Code Grouping is required by SafeTlib to be in this exact order */
      *(SafeTlib.text.ifx_opcode_test.asil_b.ISG_800B7000)
      *(SafeTlib.text.ifx_opcode_test.asil_b.Main)
      *(SafeTlib.text.ifx_opcode_test.asil_b.Functions)
      *(SafeTlib.text.ifx_opcode_test.asil_b.Kernel_Direct)
      *(SafeTlib.text.ifx_opcode_test.asil_b.TC16E_Direct)
      *(SafeTlib.text.ifx_opcode_test.asil_b.RelocTable)
      *(SafeTlib.text.ifx_opcode_test.asil_b.ISG_800D7000)
      *(SafeTlib.text.ifx_opcode_test.asil_b.TC16P_Main)
      *(SafeTlib.text.ifx_opcode_test.asil_b.TC16P_Direct)
   } > MEM_STL_SBST_CODE_ROM
}


/* -- Some SafeTlib Input Sections that need special ouput handling ------------------------------------------------- */

SECTIONS
{
   /* Post-Build configuration data */
  .CONFIGURATION_PB :
  {
    KEEP(*(SafeTlib.rodata.CONFIGURATION_PB))
  }> MEM_STL_ROM
}


SECTIONS
{
   /*stw-trace SM_AURIX_STL_064*/
   .MTL_PMU_ECC_EDC_TST_PF0 (NOLOAD) :  ALIGN(32)
   {
      /*896 bytes need to be reserved for this section (576 if "Address buffer full" alarm test is disabled) */
      PROVIDE(LOC_START_PF0_PATTERNS = .);
      . += 896;
      . = ALIGN(32);
   } > ecctest_bank0

   /*stw-trace SM_AURIX_STL_064*/
   .MTL_PMU_ECC_EDC_TST_PF1 (NOLOAD) :  ALIGN(32)
   {
      /*896 bytes need to be reserved for this section (576 if "Address buffer full" alarm test is disabled) */
      PROVIDE(LOC_START_PF1_PATTERNS = .);
      . += 896;
      . = ALIGN(32);
   } > ecctest_bank1

   /*stw-trace SM_AURIX_STL_064*/
   .MTL_PMU_ECC_EDC_TST_PF2 (NOLOAD) :  ALIGN(32)
   {
      /*896 bytes need to be reserved for this section (576 if "Address buffer full" alarm test is disabled) */
      PROVIDE(LOC_START_PF2_PATTERNS = .);
      . += 896;
      . = ALIGN(32);
   } > ecctest_bank2

   /*stw-trace SM_AURIX_STL_064*/
   .MTL_PMU_ECC_EDC_TST_PF3 (NOLOAD) :  ALIGN(32)
   {
      /*896 bytes need to be reserved for this section (576 if "Address buffer full" alarm test is disabled) */
      PROVIDE(LOC_START_PF3_PATTERNS = .);
      . += 896;
      . = ALIGN(32);
   } > ecctest_bank3

   .flash_ecc_pattern_locations : ALIGN(4)
   {
      gau32_SafeTlibEccTestPatternLocations = .;
      LONG(LOC_START_PF0_PATTERNS);
      LONG(LOC_START_PF1_PATTERNS);
      LONG(LOC_START_PF2_PATTERNS);
      LONG(LOC_START_PF3_PATTERNS);
   } > MEM_STL_ROM

   /* Added for copytable implementation of PmuEccEdcTst Trap vector table that is executed from RAM*/
   /* Note that we alias this memory with the .pspr section. The testmanager integration handles swapping PSPR
      contents in and out when required for a test group */
   /* stw-trace: SM_AURIX_STL_066 */
   .PMUECCEDCTST_TVT_RAMCODE (ALIGN(ADDR(.pspr.default), 256)) :
   {
      LD_SAFETLIB_PSPR_OVERLAY_START = ABSOLUTE(.);
      LOC_START_PmuEccEdcTst_tvt_RAM_RUNCODE = ABSOLUTE (.);
      *(SafeTlib.PMUECCEDCTST_TVT_RAMCODE*)
      LOC_END_PmuEccEdcTst_tvt_RAM_RUNCODE = ABSOLUTE (.);
      LOC_START_PmuEccEdcTst_tvt_PFLASHCODE = LOADADDR (.PMUECCEDCTST_TVT_RAMCODE);
      LOC_END_PmuEccEdcTst_tvt_PFLASHCODE = LOC_START_PmuEccEdcTst_tvt_PFLASHCODE + SIZEOF(.PMUECCEDCTST_TVT_RAMCODE);
   }  AT> MEM_STL_ROM = 0

   /* Added for copytable implementation of PmuEccEdcTst, PFlashMonTst and SlFlsErrPtrn functions that are executed from RAM*/
   /* stw-trace: SM_AURIX_STL_065 */
   .PFLASH_MON_ECCEDC_RAMCODE (ALIGN(LOC_END_PmuEccEdcTst_tvt_RAM_RUNCODE, 4)) :
   {
      LOC_START_PflashMon_EccEdc_RAM_RUNCODE = ABSOLUTE (.);
      *(SafeTlib.PFLASH_MON_ECCEDC_RAMCODE*)
      *(.PFLASH_MON_ECCEDC_RAMCODE*)
      . = ALIGN(4);
      LOC_END_PflashMon_EccEdc_RAM_RUNCODE = ABSOLUTE (.);
      LOC_START_PflashMon_EccEdc_PFLASHCODE = LOADADDR (.PFLASH_MON_ECCEDC_RAMCODE);
      LOC_END_PflashMon_EccEdc_PFLASHCODE = LOC_START_PflashMon_EccEdc_PFLASHCODE + SIZEOF(.PFLASH_MON_ECCEDC_RAMCODE);
      LD_SAFETLIB_PSPR_OVERLAY_END = ABSOLUTE(.);
   } AT> MEM_STL_ROM = 0


   SafeTlib.text : ALIGN(4)
   {
      *(SafeTlib.text)
      *(SafeTlib.text.*)
   } AT> MEM_STL_ROM = 0

   .SafeTlib.rodata : ALIGN(8)
   {
      *(SafeTlib.rodata)
      *(SafeTlib.rodata.*)

      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.r.*)
   } AT> MEM_STL_ROM = 0

   .safetlib_overlay :
   {
      gt_SafeTlibOverlayInfo = ABSOLUTE(.);
      LONG(LD_SAFETLIB_PSPR_OVERLAY_START);
      LONG(LD_SAFETLIB_PSPR_OVERLAY_END - LD_SAFETLIB_PSPR_OVERLAY_START);
      LONG(LOADADDR(.PMUECCEDCTST_TVT_RAMCODE));
      /* the start address of the SafeTlib PSPR overlay might be shifted wrt. to the .pspr.default address due to different
         alignment requirements, thus we need to shift the restoration load address accordingly */
      LONG(LOADADDR(.pspr.default) + (LD_SAFETLIB_PSPR_OVERLAY_START - ADDR(.pspr.default)));
   } AT > MEM_BOOT_ROM
}

SECTIONS
{
   /* data that must be placed in dspr, alignment must be 4 at least */
   .SafeTlib.data.dspr : ALIGN(32)
   {
      LD_BUS_MPU_SAFETLIB_DSPR_START = ABSOLUTE(.);
      *(SafeTlib.data.DEFAULT_RAM_32BIT_NONZERO_INIT)
   } > MEM_STL_DSPR_RAM AT> MEM_STL_ROM

   /* bss that must be placed in dspr */
   .SafeTlib.bss.dspr (NOLOAD) : ALIGN(4)
   {
      *(SafeTlib.bss.Lmu.CPU0_DSPR_RAM_32BIT)
      *(SafeTlib.bss.Lmu.CPU0_DSPR_RAM_8BIT)
      . = ALIGN(32);
      LD_BUS_MPU_SAFETLIB_DSPR_END = ABSOLUTE(.);
   } > MEM_STL_DSPR_RAM

   .SafeTlib.data.default : ALIGN(32)
   {
      LD_BUS_MPU_SAFETLIB_DEFAULT_RAM_START = ABSOLUTE(.);
      *(SafeTlib.data*)
      . = ALIGN(4);
   } > MEM_STL_DEFAULT_RAM AT> MEM_STL_ROM

   .SafeTlib.bss.default (NOLOAD) : ALIGN(8)
   {
      *(SafeTlib.bss*)
      . = ALIGN(32);
      LD_BUS_MPU_SAFETLIB_DEFAULT_RAM_END = ABSOLUTE(.);
   } > MEM_STL_DEFAULT_RAM

   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.SafeTlib.data.dspr));     LONG(0 + ADDR(.SafeTlib.data.dspr));     LONG(SIZEOF(.SafeTlib.data.dspr));
      LONG(LOADADDR(.SafeTlib.data.default));  LONG(0 + ADDR(.SafeTlib.data.default));  LONG(SIZEOF(.SafeTlib.data.default));
   } AT> MEM_BOOT_ROM

   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.SafeTlib.bss.dspr));      LONG(SIZEOF(.SafeTlib.bss.dspr));
      LONG(0 + ADDR(.SafeTlib.bss.default));   LONG(SIZEOF(.SafeTlib.bss.default));
   } AT> MEM_BOOT_ROM
}

SECTIONS
{
   .SafeTlib.rom.info : {
      MEM_STL_ROM_STOP = .;
   } > MEM_STL_ROM
}

/*
 * DWARF debug sections.
 * Symbols in the DWARF debugging sections are relative to the
 * beginning of the section, so we begin them at 0.
 */
SECTIONS
{
   /*
    * DWARF 1
    */
   .comment         0 : { *(SafeTlib.comment)}
   .debug           0 : { *(SafeTlib.debug) }
   .line            0 : { *(SafeTlib.line) }
   /*
    * GNU DWARF 1 extensions
    */
   .debug_srcinfo   0 : { *(SafeTlib.debug_srcinfo) }
   .debug_sfnames   0 : { *(SafeTlib.debug_sfnames) }
   /*
    * DWARF 1.1 and DWARF 2
    */
   .debug_aranges   0 : { *(SafeTlib.debug_aranges) }
   .debug_pubnames  0 : { *(SafeTlib.debug_pubnames) }
   /*
    * DWARF 2
    */
   .debug_info      0 : { *(SafeTlib.debug_info) }
   .debug_abbrev    0 : { *(SafeTlib.debug_abbrev) }
   .debug_line      0 : { *(SafeTlib.debug_line) }
   .debug_frame     0 : { *(SafeTlib.debug_frame) }
   .debug_str       0 : { *(SafeTlib.debug_str) }
   .debug_loc       0 : { *(SafeTlib.debug_loc) }
   .debug_macinfo   0 : { *(SafeTlib.debug_macinfo)}
   .debug_ranges    0 : { *(SafeTlib.debug_ranges)}
   /*
    * SGI/MIPS DWARF 2 extensions
    */
   .debug_weaknames 0 : { *(SafeTlib.debug_weaknames)}
   .debug_funcnames 0 : { *(SafeTlib.debug_funcnames)}
   .debug_typenames 0 : { *(SafeTlib.debug_typenames)}
   .debug_varnames  0 : { *(SafeTlib.debug_varnames)}
   /*
    * Optional sections that may only appear when relocating.
    */
   /*
    * Optional sections that may appear regardless of relocating.
    */
   .version_info    0 : { *(SafeTlib.version_info*) }
   .boffs           0 : { KEEP (*(SafeTlib.boffs))}
}
