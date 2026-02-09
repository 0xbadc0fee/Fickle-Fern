/*!
   \file
   \brief       EMEM memory default P4 sections and symbols

   Conditional Memories (alias to "no_mem" if not required):

      MEM_EMEM_DATA_SC2:    Safety data
      MEM_EMEM_DATA_SC2_UC: Safety data uncached

      MEM_EMEM_DATA_SC0:    Nonsafe data
      MEM_EMEM_DATA_SC0_UC: Nonsafe data uncached

      MEM_EMEM_CODE:        Code sections
      MEM_EMEM_STACKS:      Stack sections

   \copyright   Copyright 2022 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/

SECTIONS {

   _. = ASSERT ((LENGTH(MEM_EMEM_DATA_SC0) == 0 || LENGTH(MEM_EMEM_DATA_SC0_UC) != 0), "You must map both cached and uncached emem");
   _. = ASSERT ((LENGTH(MEM_EMEM_DATA_SC2) == 0 || LENGTH(MEM_EMEM_DATA_SC2_UC) != 0), "You must map both cached and uncached emem");

   /* emem partition gap: in TC2xx targets no bus MPU is available for the emem to provide freedom of interference, thus
      we rely on the CPU MPU which on accesses only checks the start address of the access, not if it reaches into
      forbidden space, eg. uint64 access to the last word of a range. We add the gap if any of the optional destination
      regions is assigned to anything but no_mem and does NOT begin on the beginning of the emem or emem_uc region.  As
      we provide at most 4 gaps with sum of 32 bytes, due to the alignment of 32 byte of all payload sections, we
      will not incur more overhead than necessary.
      In practice the safety core emem partition will start at the beginning of the emem, so we do not have to provide
      a gap - accesses before the beginning of the memory will trigger a bus error. */

   .emem.gap.code (NOLOAD) : {
      /* we might map the code emem region to either cached or the uncached emem memory */
      . += ((ORIGIN(MEM_EMEM_CODE) != ORIGIN(emem)) && (ORIGIN(MEM_EMEM_CODE) != ORIGIN(emem_uc)) && (ORIGIN(MEM_EMEM_CODE) != 0)) ? 8 : 0;
   } > MEM_EMEM_CODE

   .emem.gap.stacks (NOLOAD) : {
      /* we might map the stack emem region to either cached or the uncached emem memory */
      . +=  ((ORIGIN(MEM_EMEM_STACKS) != ORIGIN(emem)) && (ORIGIN(MEM_EMEM_STACKS) != ORIGIN(emem_uc)) && (ORIGIN(MEM_EMEM_STACKS) != 0)) ? 8 : 0;
   } > MEM_EMEM_STACKS

   .emem.gap.data_default (NOLOAD) : {
      /* we must map the cached emem region to the cached emem memory! */
      . += ((ORIGIN(MEM_EMEM_DATA_SC2) != ORIGIN(emem)) && (ORIGIN(MEM_EMEM_DATA_SC2) != 0)) ? 8 : 0;
   } > MEM_EMEM_DATA_SC2

   .emem.gap.data_uncached (NOLOAD) : {
      /* we must map the uncached emem region to the uncached emem memory! */
      . +=  ((ORIGIN(MEM_EMEM_DATA_SC2_UC) != ORIGIN(emem_uc)) && (ORIGIN(MEM_EMEM_DATA_SC2_UC) != 0)) ? 8 : 0;
   } > MEM_EMEM_DATA_SC2_UC

   /* -- SC0/SC2 Code ----------------------------------------------------------------------------------------------- */

   .emem.code : ALIGN(32)
   {
      LD_32_EMEM_CODE_START = ABSOLUTE(.);
      LD_32_EMEM_CODE_SC2_START = ABSOLUTE(.);
      KEEP(*(*.emem.code.sc2))
      KEEP(*(*.emem.code.sc2.*))
      . = ALIGN(32);
      LD_32_EMEM_CODE_SC2_END = ABSOLUTE(.);
      LD_32_EMEM_CODE_SC0_START = ABSOLUTE(.);
      KEEP(*(*.emem.code.sc0))
      KEEP(*(*.emem.code.sc0.*))
      . = ALIGN(32);
      LD_32_EMEM_CODE_SC0_END = ABSOLUTE(.);
      LD_32_EMEM_CODE_END = ABSOLUTE(.);
   } > MEM_EMEM_CODE AT> MEM_LOAD_ROM

   LD_32_EMEM_CODE_UC_START = LD_32_EMEM_CODE_START | 0x20000000;
   LD_32_EMEM_CODE_UC_END   = LD_32_EMEM_CODE_END   | 0x20000000;

    /* -- Stack regions --------------------------------------------------------------------------------------------- */

   .emem.stacks (NOLOAD) : ALIGN(32)
   {
      LD_32_EMEM_STACKS_START = ABSOLUTE(.);
      . += (LENGTH(MEM_EMEM_STACKS) != 0) ? 128 : 0;  /* guard zone */
      LD_32_EMEM_STACKS_SC2_START = ABSOLUTE(.);
      KEEP(*(*.emem.stacks.sc2))
      . = ALIGN(32);
      LD_32_EMEM_STACKS_SC2_END = ABSOLUTE(.);
      LD_32_EMEM_STACKS_SC0_START = ABSOLUTE(.);
      KEEP(*(*.emem.stacks.sc0))
      . = ALIGN(32);
      LD_32_EMEM_STACKS_SC0_END = ABSOLUTE(.);
      . += (LENGTH(MEM_EMEM_STACKS) != 0) ? 128 : 0;  /* guard zone */
      LD_32_EMEM_STACKS_END     = ABSOLUTE(.);
   } > MEM_EMEM_STACKS

   LD_32_EMEM_STACKS_UC_START = LD_32_EMEM_STACKS_START | 0x20000000;
   LD_32_EMEM_STACKS_UC_END   = LD_32_EMEM_STACKS_END   | 0x20000000;

    /* -- SC2 ------------------------------------------------------------------------------------------------------- */

   .emem.data.sc2.c : ALIGN(32)
   {
      LD_32_EMEM_DATA_SC2_START = ABSOLUTE(.);
      . += (LENGTH(MEM_EMEM_DATA_SC2) == 0) ? 0 : 8; /* Conditional MPU margin */
      KEEP(*(*.emem.data.sc2.c))
      KEEP(*(*.emem.data.sc2.c.*))
   } > MEM_EMEM_DATA_SC2 AT> MEM_LOAD_ROM

   .emem.bss.sc2.c (NOLOAD) : ALIGN(4)
   {
      KEEP(*(*.emem.bss.sc2.c))
      KEEP(*(*.emem.bss.sc2.c.*))
   } > MEM_EMEM_DATA_SC2

   .emem.data.sc2.uc : ALIGN(32)
   {
      KEEP(*(*.emem.data.sc2.uc))
      KEEP(*(*.emem.data.sc2.uc.*))
   } > MEM_EMEM_DATA_SC2_UC AT> MEM_LOAD_ROM

   .emem.bss.sc2.uc (NOLOAD) : ALIGN(4)
   {
      KEEP(*(*.emem.bss.sc2.uc))
      KEEP(*(*.emem.bss.sc2.uc.*))
      . = ALIGN(32);
      LD_32_EMEM_DATA_SC2_UC_END = ABSOLUTE(.);
   } > MEM_EMEM_DATA_SC2_UC

   LD_32_EMEM_DATA_SC2_UC_START = LD_32_EMEM_DATA_SC2_START  |   0x20000000;
   LD_32_EMEM_DATA_SC2_END      = LD_32_EMEM_DATA_SC2_UC_END & ~ 0x20000000;

   /* -- SC0 -------------------------------------------------------------------------------------------------------- */

   .emem.data.sc0.c : ALIGN(32)
   {
      LD_32_EMEM_DATA_SC0_START = ABSOLUTE(.);
      KEEP(*(*.emem.data.sc0.c))
      KEEP(*(*.emem.data.sc0.c.*))
   } > MEM_EMEM_DATA_SC0 AT> MEM_LOAD_ROM

   .emem.bss.sc0.c (NOLOAD) : ALIGN(4)
   {
      KEEP(*(*.emem.bss.sc0.c))
      KEEP(*(*.emem.bss.sc0.c.*))
      . = ALIGN(32);
   } > MEM_EMEM_DATA_SC0

   .emem.data.sc0.uc : ALIGN(32)
   {
      KEEP(*(*.emem.data.sc0.uc))
      KEEP(*(*.emem.data.sc0.uc.*))
   } > MEM_EMEM_DATA_SC0_UC AT> MEM_LOAD_ROM

   .emem.bss.sc0.uc (NOLOAD) : ALIGN(4)
   {
      KEEP(*(*.emem.bss.sc0.uc))
      KEEP(*(*.emem.bss.sc0.uc.*))
   } > MEM_EMEM_DATA_SC0_UC

   .emem.gcov.data : ALIGN(4)
   {
      KEEP(*(.data.__gcov_.*))
   } > MEM_EMEM_DATA_SC0_UC AT> MEM_LOAD_ROM

   .emem.gcov.bss (NOLOAD) : ALIGN(4)
   {
      KEEP(*(.bss.__gcov0.*))
   } > MEM_EMEM_DATA_SC0_UC

   .emem.sc0.end : ALIGN(4)
   {
      . = ALIGN(32);
      LD_32_EMEM_DATA_SC0_UC_END = ABSOLUTE(.);
   } > MEM_EMEM_DATA_SC0_UC

   LD_32_EMEM_DATA_SC0_UC_START = LD_32_EMEM_DATA_SC0_START  |   0x20000000;
   LD_32_EMEM_DATA_SC0_END      = LD_32_EMEM_DATA_SC0_UC_END & ~ 0x20000000;

   /* -- Combined data block ---------------------------------------------------------------------------------------- */

   LD_32_EMEM_DATA_START        = LD_32_EMEM_DATA_SC2_START;
   LD_32_EMEM_DATA_END          = LD_32_EMEM_DATA_SC0_END;
   LD_32_EMEM_DATA_UC_START     = LD_32_EMEM_DATA_SC2_UC_START;
   LD_32_EMEM_DATA_UC_END       = LD_32_EMEM_DATA_SC0_UC_END;

   /* -- Copy/Clear sections ---------------------------------------------------------------------------------------- */

   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.emem.bss.sc0.c));  LONG(SIZEOF(.emem.bss.sc0.c));
      LONG(0 + ADDR(.emem.bss.sc2.c));  LONG(SIZEOF(.emem.bss.sc2.c));
      LONG(0 + ADDR(.emem.bss.sc0.uc)); LONG(SIZEOF(.emem.bss.sc0.uc));
      LONG(0 + ADDR(.emem.gcov.bss));   LONG(SIZEOF(.emem.gcov.bss));
      LONG(0 + ADDR(.emem.bss.sc2.uc)); LONG(SIZEOF(.emem.bss.sc2.uc));
   } AT> MEM_BOOT_ROM

   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.emem.data.sc2.c));  LONG(0 + ADDR(.emem.data.sc2.c));  LONG(SIZEOF(.emem.data.sc2.c));
      LONG(LOADADDR(.emem.data.sc0.c));  LONG(0 + ADDR(.emem.data.sc0.c));  LONG(SIZEOF(.emem.data.sc0.c));
      LONG(LOADADDR(.emem.data.sc2.uc)); LONG(0 + ADDR(.emem.data.sc2.uc)); LONG(SIZEOF(.emem.data.sc2.uc));
      LONG(LOADADDR(.emem.data.sc0.uc)); LONG(0 + ADDR(.emem.data.sc0.uc)); LONG(SIZEOF(.emem.data.sc0.uc));
      LONG(LOADADDR(.emem.gcov.data));   LONG(0 + ADDR(.emem.gcov.data));   LONG(SIZEOF(.emem.gcov.data));
      LONG(LOADADDR(.emem.code));        LONG(0 + ADDR(.emem.code));        LONG(SIZEOF(.emem.code));
   } AT> MEM_BOOT_ROM

}
