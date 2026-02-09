/* --------------------------------------------------------------------------------------------------------------------
   GNU GCC standard linker script for Tricore AURIX Projects

   This file defines the standard linker script definitions and sections, which must be defined before any project
   specific sections.

   NOTE:
   Some symbols exported by the linker must adhere to certain address or alignment requirements.
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
-------------------------------------------------------------------------------------------------------------------- */


/* Emit symbols for GCC sbrk (standard malloc) */
__HEAP     = LD_USER_HEAP_START;
__HEAP_END = LD_USER_HEAP_END;

/*
 *  Output sections required by crtstuff.c and for C++ programming
 */
SECTIONS
{
   /*
    * C++ Constructors and destructors.
    */
   .ctors : ALIGN(8) FLAGS(arl)
   {
      __CTOR_LIST__ = . ;
      LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2);
      KEEP(*(.ctors))
      LONG(0);
      __CTOR_END__ = . ;
   } AT> MEM_LOAD_ROM = 0

   .dtors : ALIGN(8) FLAGS(arl)
   {
      __DTOR_LIST__ = . ;
      LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2);
      KEEP(*(.dtors))
      LONG(0);
      __DTOR_END__ = . ;
   } AT> MEM_LOAD_ROM = 0


   /*
    * C++ exception handling tables.  NOTE: gcc emits .eh_frame
    * sections when compiling C sources with debugging enabled (-g).
    * If you can be sure that your final application consists
    * exclusively of C objects (i.e., no C++ objects), you may use
    * the -R option of the "strip" and "objcopy" utilities to remove
    * the .eh_frame section from the executable.
    */
   .eh_frame : ALIGN(8)
   {
      *(.gcc_except_table)
      __EH_FRAME_BEGIN__ = . ;
      KEEP (*(.eh_frame))
      __EH_FRAME_END__ = . ;
   } AT> MEM_LOAD_ROM = 0


   /*
    *   crtstuff.c sections (init/fini/jcr)
    */
   .init : ALIGN(8)
   {
      /* we have to keep all .init/.fini section
       * because the linker should not delete this
       * sections with --gc-sections
       */
      KEEP( *(SORT(.init*)) )
      KEEP( *(SORT(.fini*)) )
   } AT> MEM_LOAD_ROM = 0


   .jcr_ctors : ALIGN(8)
   {
      *(.jcr)
   } AT> MEM_LOAD_ROM = 0
}


/*
 *  We can define 4 small data areas.
 *  - 2 areas for STW small data/bss (ram) and rodata (rom)
 *  - 2 areas for application small data/bss (ram) and rodata (rom)
 *  Note: output sections must be named .sdata(x)/.sbss(x)
 *        the linker will automatically generate _SMALL_DATA(x)_ symbols
 *        that will point to the start address of the corresponding section
 */
SECTIONS
{
   /* small data and bss section, addressed by global register A3 */
   .sdata4 : ALIGN(8)
   {
      LD_MPU_SDATA_RAM_BIOS_BEGIN = .;
      *(.stw_bios.sdata)
      *(.stw_bios.sdata.*)
   } > MEM_SDATA_BIOS_RAM AT> MEM_LOAD_ROM

   .sbss4 (NOLOAD) : ALIGN(8)
   {
      *(.stw_bios.sbss)
      *(.stw_bios.sbss.*)
      . = ALIGN(8);
      LD_MPU_SDATA_RAM_BIOS_END = .;
   } > MEM_SDATA_BIOS_RAM


   /* small rodata section in for STW BIOS, addressed by global register A2 */
   .sdata3 : ALIGN(8)
   {
      LD_MPU_SDATA_ROM_BIOS_BEGIN = .;
      *(.stw_bios.sdata.rodata)
      *(.stw_bios.sdata.rodata.*)
      . = ALIGN(8);
      LD_MPU_SDATA_ROM_BIOS_END = .;
   } AT> MEM_SDATA_BIOS_ROM


   /* The sdata2 section should be used for small rodata */
   .sdata2 : ALIGN(8)
   {
      LD_MPU_SDATA_ROM_USER_BEGIN = .;
      *(.sdata.rodata)
      *(.sdata.rodata.*)
      . = ALIGN(8);
      LD_MPU_SDATA_ROM_USER_END   = .;
   } AT> MEM_SDATA_USER_ROM


   /* small data and bss section, addressed by global register A0 */
   /* This .sdata/sbss set can be used by the application         */
   .sdata : ALIGN(8)
   {
      LD_MPU_SDATA_RAM_USER_BEGIN = .;
      *(.sdata)
      *(.sdata.*)
      *(.gnu.linkonce.s.*)
   } > MEM_SDATA_USER_RAM AT> MEM_LOAD_ROM

   .sbss (NOLOAD) : ALIGN(8)
   {
      *(.sbss)
      *(.sbss.*)
      *(.gnu.linkonce.sb.*)
      . = ALIGN(8);
      LD_MPU_SDATA_RAM_USER_END = .;
   } > MEM_SDATA_USER_RAM


   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.sbss4));   LONG(SIZEOF(.sbss4));
      LONG(0 + ADDR(.sbss));    LONG(SIZEOF(.sbss));
   } AT> MEM_BOOT_ROM


   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.sdata4));  LONG(0 + ADDR(.sdata4));    LONG(SIZEOF(.sdata4));
      LONG(LOADADDR(.sdata));   LONG(0 + ADDR(.sdata));     LONG(SIZEOF(.sdata));
   } AT> MEM_BOOT_ROM
}

/*
 * Locate system heap.
 */
SECTIONS
{
   .int_system_heap :
   {
      /* locate space for SYSTEM heap */
      . = ALIGN(8);
      LD_MPU_SYSTEM_HEAP_START = ABSOLUTE(.);
      /* Conditional MPU protection boundary margin (See eg. UM Part 1 5.3.4.17 "Crossing protection boundaries") */
      . += (ORIGIN(MEM_SYSTEM_HEAP_RAM) == 0) ? 0 : 8;

      LD_SYSTEM_HEAP_START = ABSOLUTE(.);
      /* move location pointer to the end of the system heap memory region (or to defined maximum) to prevent any static data to be placed into the HEAP area */
       . = DEFINED(MEM_SYSTEM_HEAP_MAX_SIZE) ? . + MEM_SYSTEM_HEAP_MAX_SIZE : (ORIGIN(MEM_SYSTEM_HEAP_RAM) + LENGTH(MEM_SYSTEM_HEAP_RAM)) - LD_MPU_SYSTEM_HEAP_START;
      LD_SYSTEM_HEAP_END = ABSOLUTE(.);
      . = ALIGN(8);
      LD_MPU_SYSTEM_HEAP_END = ABSOLUTE(.);
      LD_SYSTEM_HEAP_SIZE = LD_SYSTEM_HEAP_END - LD_SYSTEM_HEAP_START;
   } >MEM_SYSTEM_HEAP_RAM

   /* Make sure enough memory is available for system heap.  */
   _. = ASSERT ((LD_SYSTEM_HEAP_START & 7) == 0 , "SRAM HEAP not doubleword aligned");
   _. = ASSERT ((LD_SYSTEM_HEAP_END - LD_SYSTEM_HEAP_START) >= MEM_SYSTEM_HEAP_MIN_SIZE , "not enough memory for SRAM HEAP");
}


/*
 * Locate application standard (RAM) data sections...
 */
SECTIONS
{
   /* Region 0 */

   .data0 : ALIGN(8)
   {
      LD_MPU_DATA_SEGMENT_0_START = ABSOLUTE(.);
      /* Conditional MPU protection boundary margin (See eg. UM Part 1 5.3.4.17 "Crossing protection boundaries") */
      . += (ORIGIN(MEM_DEFAULT_RAM_0) == 0) ? 0 : 8;

      . = ALIGN(32);                  /* -maligned-data-sections (and -fdata-sections) */
      *(SORT_BY_ALIGNMENT(.data.a8*)) /* >= 8-byte alignment    */
      *(.data.a4)                     /* 4-byte alignment       */
      *(.data.a2)                     /* 2-byte alignment       */
      *(.data.a1)                     /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data0*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data*))    /* generic .data (-fdata-sections and mem-holes) */

      *(.tm_clone_table)
      *(.gnu.linkonce.d.*)
   } > MEM_DEFAULT_RAM_0 AT> MEM_LOAD_ROM

   .bss0 (NOLOAD) : ALIGN(32)
   {
      SEC_BSS_0_START_ADDR = ABSOLUTE(.);
                                     /* -maligned-data-sections (and -fdata-section) */
      *(SORT_BY_ALIGNMENT(.bss.a8*)) /* >= 8-byte alignment    */
      *(.bss.a4*)                    /* 4-byte alignment       */
      *(.bss.a2*)                    /* 2-byte alignment       */
      *(.bss.a1*)                    /* 1-byte alignment       */

      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss0*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss*))    /* generic .bss (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.b.*)
      *(COMMON)
      . = ALIGN(8);
      SEC_BSS_0_END_ADDR = ABSOLUTE(.);
      BSS_0_SIZE = ABSOLUTE(.) - (SEC_BSS_0_START_ADDR);

      *(.noinit)

      . = ALIGN(8) ;
      LD_MPU_DATA_SEGMENT_0_END = ABSOLUTE(.);
   } > MEM_DEFAULT_RAM_0

   /* Region 1 */

   .data1 : ALIGN(8)
   {
      LD_MPU_DATA_SEGMENT_1_START = ABSOLUTE(.);
      /* Conditional 8 bytes MPU safety margin (See eg. UM Part 1 5.3.4.17 "Crossing protection boundaries") */
      . += (ORIGIN(MEM_DEFAULT_RAM_1) == 0) ? 0 : 8;

      . = ALIGN(32); /* -maligned-data-sections (and -fdata-sections) */
      *(SORT_BY_ALIGNMENT(.data.a8*)) /* >= 8-byte alignment    */
      *(.data.a4*)                    /* 4-byte alignment       */
      *(.data.a2*)                    /* 2-byte alignment       */
      *(.data.a1*)                    /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data1*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data*))    /* generic .data (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.d.*)
   } > MEM_DEFAULT_RAM_1 AT> MEM_LOAD_ROM

   .bss1 (NOLOAD) : ALIGN(32)
   {
      SEC_BSS_1_START_ADDR = ABSOLUTE(.);
                                     /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.bss.a8*)) /* >= 8-byte alignment    */
      *(.bss.a4*)                    /* 4-byte alignment       */
      *(.bss.a2*)                    /* 2-byte alignment       */
      *(.bss.a1*)                    /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss1*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss*))    /* generic .bss (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.b.*)
      *(COMMON)
      . = ALIGN(8);
      SEC_BSS_1_END_ADDR = ABSOLUTE(.);
      BSS_1_SIZE = ABSOLUTE(.) - (SEC_BSS_1_START_ADDR);

      *(.noinit)

      . = ALIGN(8);
      LD_MPU_DATA_SEGMENT_1_END = ABSOLUTE(.);
   } > MEM_DEFAULT_RAM_1

   /* Region 2 */

   .data2 : ALIGN(8)
   {
      LD_MPU_DATA_SEGMENT_2_START = ABSOLUTE(.);
      /* Conditional MPU protection boundary margin (See eg. UM Part 1 5.3.4.17 "Crossing protection boundaries") */
      . += (ORIGIN(MEM_DEFAULT_RAM_2) == 0) ? 0 : 8;

      . = ALIGN(32);                  /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.data.a8*)) /* >= 8-byte alignment    */
      *(.data.a4)                     /* 4-byte alignment       */
      *(.data.a2)                     /* 2-byte alignment       */
      *(.data.a1)                     /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data2*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data*))    /* generic .data (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.d.*)
   } > MEM_DEFAULT_RAM_2 AT> MEM_LOAD_ROM

   .bss2 (NOLOAD) : ALIGN(32)
   {
      SEC_BSS_2_START_ADDR = ABSOLUTE(.);
                                     /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.bss.a8*)) /* >= 8-byte alignment    */
      *(.bss.a4*)                    /* 4-byte alignment       */
      *(.bss.a2*)                    /* 2-byte alignment       */
      *(.bss.a1*)                    /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss2*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss*))    /* generic .bss (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.b.*)
      *(COMMON)
      . = ALIGN(8);
      SEC_BSS_2_END_ADDR = ABSOLUTE(.);
      BSS_2_SIZE = ABSOLUTE(.) - (SEC_BSS_2_START_ADDR);

      *(.noinit)

      . = ALIGN(8);
      LD_MPU_DATA_SEGMENT_2_END = ABSOLUTE(.);
   } > MEM_DEFAULT_RAM_2

   /* Region 3 */

   .data3 : ALIGN(8)
   {
      LD_MPU_DATA_SEGMENT_3_START = ABSOLUTE(.);
      /* Conditional MPU protection boundary margin (See eg. UM Part 1 5.3.4.17 "Crossing protection boundaries") */
      . += (ORIGIN(MEM_DEFAULT_RAM_3) == 0) ? 0 : 8;

      . = ALIGN(32);                  /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.data.a8*)) /* >= 8-byte alignment    */
      *(.data.a4)                     /* 4-byte alignment       */
      *(.data.a2)                     /* 2-byte alignment       */
      *(.data.a1)                     /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data3*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data*))    /* generic .data (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.d.*)
   } > MEM_DEFAULT_RAM_3 AT> MEM_LOAD_ROM

   .bss3 (NOLOAD) : ALIGN(32)
   {
      SEC_BSS_3_START_ADDR = ABSOLUTE(.);
                                     /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.bss.a8*)) /* >= 8-byte alignment    */
      *(.bss.a4*)                    /* 4-byte alignment       */
      *(.bss.a2*)                    /* 2-byte alignment       */
      *(.bss.a1*)                    /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss3*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss*))    /* default sections */

      *(.gnu.linkonce.b.*)
      *(COMMON)
      . = ALIGN(8);
      SEC_BSS_3_END_ADDR = ABSOLUTE(.);
      BSS_3_SIZE = ABSOLUTE(.) - (SEC_BSS_3_START_ADDR);

      *(.noinit)

      . = ALIGN(8);
      LD_MPU_DATA_SEGMENT_3_END = ABSOLUTE(.);
   } > MEM_DEFAULT_RAM_3

   /* Region 4 */

   .data4 : ALIGN(8)
   {
      . = ALIGN(8);
      LD_MPU_DATA_SEGMENT_4_START = ABSOLUTE(.);
      /* Conditional MPU protection boundary margin (See eg. UM Part 1 5.3.4.17 "Crossing protection boundaries") */
      . += (ORIGIN(MEM_DEFAULT_RAM_4) == 0) ? 0 : 8;

      . = ALIGN(32);                  /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.data.a8*)) /* >= 8-byte alignment    */
      *(.data.a4)                     /* 4-byte alignment       */
      *(.data.a2)                     /* 2-byte alignment       */
      *(.data.a1)                     /* 1-byte alignment       */
      *(.data.*)
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data4*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.data*))    /* generic .data (-fdata-sections and mem-holes) */

      *(.gnu.linkonce.d.*)
   } > MEM_DEFAULT_RAM_4 AT> MEM_LOAD_ROM

   .bss4 (NOLOAD) : ALIGN(32)
   {
      SEC_BSS_4_START_ADDR = ABSOLUTE(.);
                                     /* -maligned-data-sections (and -fdata-section */
      *(SORT_BY_ALIGNMENT(.bss.a8*)) /* >= 8-byte alignment    */
      *(.bss.a4*)                    /* 4-byte alignment       */
      *(.bss.a2*)                    /* 2-byte alignment       */
      *(.bss.a1*)                    /* 1-byte alignment       */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss4*))   /* pinned sections */
      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.bss*))    /* default sections */

      *(.gnu.linkonce.b.*)
      *(COMMON)
      . = ALIGN(8);
      SEC_BSS_4_END_ADDR = ABSOLUTE(.);
      BSS_4_SIZE = ABSOLUTE(.) - (SEC_BSS_4_START_ADDR);

      *(.noinit)

      . = ALIGN(8) ;
      LD_MPU_DATA_SEGMENT_4_END = ABSOLUTE(.);
   } > MEM_DEFAULT_RAM_4

   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.bss0));           LONG(BSS_0_SIZE);
      LONG(0 + ADDR(.bss1));           LONG(BSS_1_SIZE);
      LONG(0 + ADDR(.bss2));           LONG(BSS_2_SIZE);
      LONG(0 + ADDR(.bss3));           LONG(BSS_3_SIZE);
      LONG(0 + ADDR(.bss4));           LONG(BSS_4_SIZE);
   } AT> MEM_BOOT_ROM

   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.data0));          LONG(0 + ADDR(.data0));             LONG(SIZEOF(.data0));
      LONG(LOADADDR(.data1));          LONG(0 + ADDR(.data1));             LONG(SIZEOF(.data1));
      LONG(LOADADDR(.data2));          LONG(0 + ADDR(.data2));             LONG(SIZEOF(.data2));
      LONG(LOADADDR(.data3));          LONG(0 + ADDR(.data3));             LONG(SIZEOF(.data3));
      LONG(LOADADDR(.data4));          LONG(0 + ADDR(.data4));             LONG(SIZEOF(.data4));
   } AT> MEM_BOOT_ROM
}

/*
 * Locate MCS code and data sections and add them to the copy table
 */
SECTIONS
{
   /* MCS code and data sections */
   .mcs0_text :
   {
      /* KEEP keyword will keep also unreferenced input sections. */
      KEEP(*(.mcs0.text*))
   } > mcs0_ram AT> MEM_LOAD_ROM

   .mcs0_data :
   {
      KEEP(*(.mcs0.data*))
   } > mcs0_ram AT> MEM_LOAD_ROM

   .mcs1_text :
   {
      KEEP(*(.mcs1.text*))
   } > mcs1_ram AT> MEM_LOAD_ROM

   .mcs1_data :
   {
      KEEP(*(.mcs1.data*))
   } > mcs1_ram AT> MEM_LOAD_ROM

   .mcs2_text :
   {
      KEEP(*(.mcs2.text*))
   } > mcs2_ram AT> MEM_LOAD_ROM

   .mcs2_data :
   {
      KEEP(*(.mcs2.data*))
   } > mcs2_ram AT> MEM_LOAD_ROM

   .mcs3_text :
   {
      KEEP(*(.mcs3.text*))
   } > mcs3_ram AT> MEM_LOAD_ROM

   .mcs3_data :
   {
      KEEP(*(.mcs3.data*))
   } > mcs3_ram AT> MEM_LOAD_ROM

   .mcs4_text :
   {
      KEEP(*(.mcs4.text*))
   } > mcs4_ram AT> MEM_LOAD_ROM

   .mcs4_data :
   {
      KEEP(*(.mcs4.data*))
   } > mcs4_ram AT> MEM_LOAD_ROM

   .mcs5_text :
   {
      KEEP(*(.mcs5_text*))
   } > mcs5_ram AT> MEM_LOAD_ROM

   .mcs5_data :
   {
      KEEP(*(.mcs5.data*))
   } > mcs5_ram AT> MEM_LOAD_ROM


   .copy_mcs_sec :
   {
      LD_MCS_COPY_TABLE_START = ABSOLUTE(.);
      LONG(LOADADDR(.mcs0_text));      LONG(0 + ADDR(.mcs0_text));      LONG(SIZEOF(.mcs0_text));
      LONG(LOADADDR(.mcs0_data));      LONG(0 + ADDR(.mcs0_data));      LONG(SIZEOF(.mcs0_data));
      LONG(LOADADDR(.mcs1_text));      LONG(0 + ADDR(.mcs1_text));      LONG(SIZEOF(.mcs1_text));
      LONG(LOADADDR(.mcs1_data));      LONG(0 + ADDR(.mcs1_data));      LONG(SIZEOF(.mcs1_data));
      LONG(LOADADDR(.mcs2_text));      LONG(0 + ADDR(.mcs2_text));      LONG(SIZEOF(.mcs2_text));
      LONG(LOADADDR(.mcs2_data));      LONG(0 + ADDR(.mcs2_data));      LONG(SIZEOF(.mcs2_data));
      LONG(LOADADDR(.mcs3_text));      LONG(0 + ADDR(.mcs3_text));      LONG(SIZEOF(.mcs3_text));
      LONG(LOADADDR(.mcs3_data));      LONG(0 + ADDR(.mcs3_data));      LONG(SIZEOF(.mcs3_data));
      LONG(LOADADDR(.mcs4_text));      LONG(0 + ADDR(.mcs4_text));      LONG(SIZEOF(.mcs4_text));
      LONG(LOADADDR(.mcs4_data));      LONG(0 + ADDR(.mcs4_data));      LONG(SIZEOF(.mcs4_data));
      LONG(LOADADDR(.mcs5_text));      LONG(0 + ADDR(.mcs5_text));      LONG(SIZEOF(.mcs5_text));
      LONG(LOADADDR(.mcs5_data));      LONG(0 + ADDR(.mcs5_data));      LONG(SIZEOF(.mcs5_data));
      LONG(-1);                        LONG(-1);                        LONG(-1);
   } AT> MEM_BOOT_ROM
}

/*
 * Emit definition of interrupt and trap table sizes and base addresses for startup code
 */
SECTIONS
{
   LD_INTVECT_SIZE = SIZEOF(.cpux_ivect);
   LD_TRAPVECT_SIZE = SIZEOF(.cpux_tvect);

   LD_INTVECT_BASE_ADDR = (0 + ADDR(.cpux_ivect));
   LD_TRAPVECT_BASE_ADDR = (0 + ADDR(.cpux_tvect));
}

/* Allocate dedicated rom sections */

SECTIONS
{
   .dedicated_rom_segment_0 : ALIGN(4)
   {
      KEEP(*(*.text0))
      KEEP(*(*.text0.*))
      . = ALIGN(8);
      KEEP(*(*.rodata0))
      KEEP(*(*.rodata0.*))
   } AT > MEM_DEFAULT_ROM_0

   .dedicated_rom_segment_1 : ALIGN(4)
   {
      KEEP(*(*.text1))
      KEEP(*(*.text1.*))
      . = ALIGN(8);
      KEEP(*(*.rodata1))
      KEEP(*(*.rodata1.*))
   } AT > MEM_DEFAULT_ROM_1

   .dedicated_rom_segment_2 : ALIGN(4)
   {
      KEEP(*(*.text2))
      KEEP(*(*.text2.*))
      . = ALIGN(8);
      KEEP(*(*.rodata2))
      KEEP(*(*.rodata2.*))
   } AT > MEM_DEFAULT_ROM_2

   .dedicated_rom_segment_3 : ALIGN(4)
   {
      KEEP(*(*.text3))
      KEEP(*(*.text3.*))
      . = ALIGN(8);
      KEEP(*(*.rodata3))
      KEEP(*(*.rodata3.*))
   } AT > MEM_DEFAULT_ROM_3

   .dedicated_rom_segment_4 : ALIGN(4)
   {
      KEEP(*(*.text4))
      KEEP(*(*.text4.*))
      . = ALIGN(8);
      KEEP(*(*.rodata4))
      KEEP(*(*.rodata4.*))
   } AT > MEM_DEFAULT_ROM_4
}

/*
 * All dedicated rom sections have been located. Now we collect remaining flash data and distribute it across
 * remaining flash memory. We start by locating default application flash data to the according memory.
 * After that, we fill up the flash memories starting with pflash0 and ending with external flash memory.
 */

SECTIONS
{
   .text0 : ALIGN(32)
   {
      *(SORT_BY_ALIGNMENT(.text*))
      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.t.*)
      /* .gnu.warning sections are handled specially by elf32.em.   */
      *(.gnu.warning)
   } AT> MEM_DEFAULT_ROM_0 = 0

   .rodata0 : ALIGN(32)
   {
      /* sort rodata according to their alignment (only needed with -maligned-data-sections compiler switch) */
      *(SORT_BY_ALIGNMENT(.rodata.a8*))   /* >= 8-byte alignment             */
      *(.rodata.a4*)                      /* 4-byte alignment                */
      *(.rodata.a2*)                      /* 2-byte alignment                */
      *(.rodata.a1*)                      /* 1-byte alignment                */

      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.rodata*))

      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.r.*)
   } AT> MEM_DEFAULT_ROM_0 = 0

   .text1 : ALIGN(32)
   {
      *(SORT_BY_ALIGNMENT(.text*))
      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.t.*)
      /* .gnu.warning sections are handled specially by elf32.em.   */
      *(.gnu.warning)
   } AT> MEM_DEFAULT_ROM_1 = 0

   .rodata1 : ALIGN(32)
   {
      /* sort rodata according to their alignment (only needed with -maligned-data-sections compiler switch) */
      *(SORT_BY_ALIGNMENT(.rodata.a8*))   /* >= 8-byte alignment             */
      *(.rodata.a4*)                      /* 4-byte alignment                */
      *(.rodata.a2*)                      /* 2-byte alignment                */
      *(.rodata.a1*)                      /* 1-byte alignment                */

      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.rodata*))

      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.r.*)
   } AT> MEM_DEFAULT_ROM_1 = 0

   .text2 : ALIGN(32)
   {
      *(SORT_BY_ALIGNMENT(.text*))
      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.t.*)
      /* .gnu.warning sections are handled specially by elf32.em.   */
      *(.gnu.warning)
   } AT> MEM_DEFAULT_ROM_2 = 0

   .rodata2 : ALIGN(32)
   {
      /* sort rodata according to their alignment (only needed with -maligned-data-sections compiler switch) */
      *(SORT_BY_ALIGNMENT(.rodata.a8*))   /* >= 8-byte alignment             */
      *(.rodata.a4*)                      /* 4-byte alignment                */
      *(.rodata.a2*)                      /* 2-byte alignment                */
      *(.rodata.a1*)                      /* 1-byte alignment                */

      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.rodata*))

      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.r.*)
   } AT> MEM_DEFAULT_ROM_2 = 0

   .text3 : ALIGN(32)
   {
      *(SORT_BY_ALIGNMENT(.text*))
      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.t.*)
      /* .gnu.warning sections are handled specially by elf32.em.   */
      *(.gnu.warning)
   } AT> MEM_DEFAULT_ROM_3 = 0

   .rodata3 : ALIGN(32)
   {
      /* sort rodata according to their alignment (only needed with -maligned-data-sections compiler switch) */
      *(SORT_BY_ALIGNMENT(.rodata.a8*))   /* >= 8-byte alignment             */
      *(.rodata.a4*)                      /* 4-byte alignment                */
      *(.rodata.a2*)                      /* 2-byte alignment                */
      *(.rodata.a1*)                      /* 1-byte alignment                */

      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.rodata*))

      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.r.*)
   } AT> MEM_DEFAULT_ROM_3 = 0

   .text4 : ALIGN(32)
   {
      *(SORT_BY_ALIGNMENT(.text*))
      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.t.*)
      /* .gnu.warning sections are handled specially by elf32.em.   */
      *(.gnu.warning)
   } AT> MEM_DEFAULT_ROM_3 = 0

   .rodata4 : ALIGN(32)
   {
      /* sort rodata according to their alignment (only needed with -maligned-data-sections compiler switch) */
      *(SORT_BY_ALIGNMENT(.rodata.a8*))   /* >= 8-byte alignment             */
      *(.rodata.a4*)                      /* 4-byte alignment                */
      *(.rodata.a2*)                      /* 2-byte alignment                */
      *(.rodata.a1*)                      /* 1-byte alignment                */

      . = ALIGN(32);
      *(SORT_BY_ALIGNMENT(.rodata*))

      /* .gnu.linkonce sections are used by gcc for vague linking   */
      *(.gnu.linkonce.r.*)
   } AT> MEM_DEFAULT_ROM_4 = 0
}


/*
 * Statistic variables (must be declared after all sections has been filled with static data)
 * NOTE: We use named memory regions instead of ALIASES as we want to get the fill statistics for each
 *       CPU memory type
 */
SECTIONS
{
   /* set marker for end of used boot rom segment*/
   .boot_rom :
   {
      /* end of used internal rom will be exported as uint32 to the application and should be aligned */
      MEM_BOOT_ROM_USED_END_ADDR = ABSOLUTE(ALIGN(4));
   } AT> MEM_BOOT_ROM


   /* set marker for end of used rom0 segment*/
   .rom0_segment :
   {
      /* end of used internal rom will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_ROM_0_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_ROM_0


   /* set marker for end of used rom1 segment*/
   .rom1_segment :
   {
      /* end of used internal rom will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_ROM_1_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_ROM_1


   /* set marker for end of used rom2 segment*/
   .rom2_segment :
   {
      /* end of used external rom will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_ROM_2_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_ROM_2

   /* set marker for end of used rom3 segment*/
   .rom3_segment :
   {
      /* end of used external rom will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_ROM_3_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_ROM_3

   /* set marker for end of used rom2 segment*/
   .rom4_segment :
   {
      /* end of used external rom will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_ROM_4_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_ROM_4

   /* set marker for end of used ram0 segment*/
   .ram0_segment :
   {
      /* end of used internal ram will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_RAM_0_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_RAM_0


   /* set marker for end of used ram1 segment*/
   .ram1_segment :
   {
      /* end of used internal ram will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_RAM_1_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_RAM_1


   /* set marker for end of used ram2 segment*/
   .ram2_segment :
   {
      /* end of used external ram will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_RAM_2_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_RAM_2


   /* set marker for end of used ram3 segment*/
   .ram3_segment :
   {
      /* end of used external ram will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_RAM_3_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_RAM_3


   /* set marker for end of used ram3 segment*/
   .ram4_segment :
   {
      /* end of used external ram will be exported as uint32 to the application and should be aligned */
      MEM_DEFAULT_RAM_4_STOP = ABSOLUTE(ALIGN(4));
   } AT> MEM_DEFAULT_RAM_4


   /* define end of lmu_sram data; All memory left after this marker will be used as internal system heap memory */
   .lmu_sram_stat :
   {
      MEM_LMU_STOP = ABSOLUTE(.);
   } > MEM_LMU


   /* define end of core0_dspr data; All memory left after this marker will be used as CSA */
   .dspr_stat :
   {
      MEM_DSPR_USED_END_ADDR = ABSOLUTE(.);
   } > MEM_DSPR


   /* define end of core0_pspr memory (program scratch pad RAM = fast code memory) */
   .pspr_stat :
   {
      MEM_PSPR_USED_END_ADDR = ABSOLUTE(.);
   } > MEM_PSPR
}


/*
 * Static data statistics have been set.
 * Now we use the rest of the remaining memory for dynamic data. (Heap and CSA)
 */
SECTIONS
{
   .application_heap :
   {
      /* locate space for heap behind last static RAM section */
      /* symbols edata and end are used by newlib malloc function */
      . = ALIGN(8);
      LD_MPU_USER_HEAP_START = ABSOLUTE(.);
      . += 8;
      LD_USER_HEAP_START = ABSOLUTE(.);
      PROVIDE(edata = LD_USER_HEAP_START);
      /* move location pointer to the end of the system heap memory region (or to defined maximum) to prevent any static data to be placed into the HEAP area */
      . = DEFINED(MEM_USER_HEAP_MAX_SIZE) ? . + MEM_USER_HEAP_MAX_SIZE : (ORIGIN(MEM_USER_HEAP_RAM) + LENGTH(MEM_USER_HEAP_RAM) - ABSOLUTE(.));
      LD_USER_HEAP_END = ABSOLUTE(.);
      LD_MPU_USER_HEAP_END = ABSOLUTE(.);
      PROVIDE(end = LD_USER_HEAP_END);
      LD_USER_HEAP_SIZE = LD_USER_HEAP_END - LD_USER_HEAP_START;
   } > MEM_USER_HEAP_RAM

   .handles (NOLOAD) : ALIGN(8)
   {
      /* MPU gap */
      . += 8;

      /* Protected region for user handles */
      LD_HANDLES_BEGIN = ABSOLUTE(.);
      KEEP(*(*.handles))
      . = ALIGN(8);
      LD_HANDLES_END = ABSOLUTE(.);
   } > MEM_HANDLES

   .dram_csa : ALIGN(0x40)
   {
      /* CSA starts after DRAM end address */
      LD_CSA_START = ABSOLUTE(.);
   } > MEM_DSPR

   /* Make sure enough memory is available for heap.  */
   _. = ASSERT ((LD_USER_HEAP_START & 7) == 0 , "HEAP not doubleword aligned");
   _. = ASSERT ((LD_USER_HEAP_END - LD_USER_HEAP_START) >= MEM_USER_HEAP_MIN_SIZE , "not enough memory for HEAP");
}

/*
 * All relocatable sections have been defined. We can now terminate the clear and copy table
 */
SECTIONS
{
   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.handles)); LONG(SIZEOF(.handles));
      LONG(-1);                 LONG(-1);
   } AT> MEM_BOOT_ROM

   .copy_sec : ALIGN(4)
   {
      LONG(-1);                 LONG(-1);                  LONG(-1);
   } AT> MEM_BOOT_ROM
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
   .comment         0 : { *(.comment)           *(SafeTLib.comment)}
   .debug           0 : { *(.debug)             *(SafeTLib.debug) }
   .line            0 : { *(.line)              *(SafeTLib.line) }
   /*
    * GNU DWARF 1 extensions
    */
   .debug_srcinfo   0 : { *(.debug_srcinfo)     *(SafeTLib.debug_srcinfo) }
   .debug_sfnames   0 : { *(.debug_sfnames)     *(SafeTLib.debug_sfnames) }
   /*
    * DWARF 1.1 and DWARF 2
    */
   .debug_aranges   0 : { *(.debug_aranges)     *(SafeTLib.debug_aranges) }
   .debug_pubnames  0 : { *(.debug_pubnames)    *(SafeTLib.debug_pubnames) }
   /*
    * DWARF 2
    */
   .debug_info      0 : { *(.debug_info)        *(SafeTLib.debug_info) }
   .debug_abbrev    0 : { *(.debug_abbrev)      *(SafeTLib.debug_abbrev) }
   .debug_line      0 : { *(.debug_line)        *(SafeTLib.debug_line) }
   .debug_frame     0 : { *(.debug_frame)       *(SafeTLib.debug_frame) }
   .debug_str       0 : { *(.debug_str)         *(SafeTLib.debug_str) }
   .debug_loc       0 : { *(.debug_loc)         *(SafeTLib.debug_loc) }
   .debug_macinfo   0 : { *(.debug_macinfo)     *(SafeTLib.debug_macinfo)}
   .debug_ranges    0 : { *(.debug_ranges)      *(SafeTLib.debug_ranges)}
   /*
    * SGI/MIPS DWARF 2 extensions
    */
   .debug_weaknames 0 : { *(.debug_weaknames)   *(SafeTLib.debug_weaknames)}
   .debug_funcnames 0 : { *(.debug_funcnames)   *(SafeTLib.debug_funcnames)}
   .debug_typenames 0 : { *(.debug_typenames)   *(SafeTLib.debug_typenames)}
   .debug_varnames  0 : { *(.debug_varnames)    *(SafeTLib.debug_varnames)}
   .debug_macro     0 : { *(.debug_macro)       *(SafeTLib.debug_macro)}
   /*
    * Optional sections that may only appear when relocating.
    */
   /*
    * Optional sections that may appear regardless of relocating.
    */
   .version_info    0 : { *(.version_info*)     *(SafeTLib.version_info*) }
   .boffs           0 : { KEEP (*(.boffs))      KEEP (*(SafeTLib.boffs))}
}


