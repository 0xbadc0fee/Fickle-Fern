/*!
   \file
   \brief       DSPR memory default P4 sections and symbols

   Conditional Memories (alias to "no_mem" if not required):

      MEM_DSPR_DATA_SC2:    Safety data

      MEM_DSPR_DATA_SC0:    Nonsafe data

      MEM_DSPR_CODE:        Code sections
      MEM_DSPR_STACKS:      Stack sections

   \copyright   Copyright 2022 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/

SECTIONS
{
    /* -- SC2 -------------------------------------------------------------------------------------------------------- */

   .dspr.data.sc2 : ALIGN(32)
   {
      LD_32_DSPR_DATA_SC2_START = ABSOLUTE(.);
      . += (LENGTH(MEM_DSPR_DATA_SC2) == 0) ? 0 : 8; /* Conditional MPU margin */
      KEEP(*(*.dspr.data.sc2))
      KEEP(*(*.dspr.data.sc2.*))
   } > MEM_DSPR_DATA_SC2 AT> MEM_LOAD_ROM

   .dspr.bss.sc2 (NOLOAD) : ALIGN(4)
   {
      KEEP(*(*.dspr.bss.sc2))
      KEEP(*(*.dspr.bss.sc2.*))
      . = ALIGN(32);
      LD_32_DSPR_DATA_SC2_END = ABSOLUTE(.);
   } > MEM_DSPR_DATA_SC2

   /* -- SC0 -------------------------------------------------------------------------------------------------------- */

   .dspr.data.sc0 : ALIGN(32)
   {
      LD_32_DSPR_DATA_SC0_START = ABSOLUTE(.);
      KEEP(*(*.dspr.data.sc0))
      KEEP(*(*.dspr.data.sc0.*))
   } > MEM_DSPR_DATA_SC0 AT> MEM_LOAD_ROM

   .dspr.bss.sc0 (NOLOAD) : ALIGN(4)
   {
      KEEP(*(*.dspr.bss.sc0))
      KEEP(*(*.dspr.bss.sc0.*))
      . = ALIGN(32);
      LD_32_DSPR_DATA_SC0_END = ABSOLUTE(.);
   } > MEM_DSPR_DATA_SC0

   /** -- SC0/SC2 Code ---------------------------------------------------------------------------------------------- */

   .dspr.code : ALIGN(32)
   {
      LD_32_DSPR_CODE_START = ABSOLUTE(.);
      LD_32_DSPR_CODE_SC2_START = ABSOLUTE(.);
      KEEP(*(*.dspr.code.sc2))
      KEEP(*(*.dspr.code.sc2.*))
      . = ALIGN(32);
      LD_32_DSPR_CODE_SC2_END = ABSOLUTE(.);
      LD_32_DSPR_CODE_SC0_START = ABSOLUTE(.);
      KEEP(*(*.dspr.code.sc0))
      KEEP(*(*.dspr.code.sc0.*))
      . = ALIGN(32);
      LD_32_DSPR_CODE_SC0_END = ABSOLUTE(.);
      LD_32_DSPR_CODE_END = ABSOLUTE(.);
   } > MEM_DSPR_CODE AT> MEM_LOAD_ROM

   /* -- Stack regions ---------------------------------------------------------------------------------------------- */

   .dspr.stacks (NOLOAD) : ALIGN(32)
   {
      LD_32_DSPR_STACKS_START = ABSOLUTE(.);
      . += (LENGTH(MEM_DSPR_STACKS) != 0) ? 128 : 0;  /* guard zone */
      LD_32_DSPR_STACKS_SC2_START = ABSOLUTE(.);
      KEEP(*(*.dspr.stacks.sc2))
      . = ALIGN(32);
      LD_32_DSPR_STACKS_SC2_END = ABSOLUTE(.);
      LD_32_DSPR_STACKS_SC0_START = ABSOLUTE(.);
      KEEP(*(*.dspr.stacks.sc0))
      . = ALIGN(32);
      LD_32_DSPR_STACKS_SC0_END = ABSOLUTE(.);
      . += (LENGTH(MEM_DSPR_STACKS) != 0) ? 128 : 0;  /* guard zone */
      LD_32_DSPR_STACKS_END     = ABSOLUTE(.);
   } > MEM_DSPR_STACKS

   /* -- Copy/Clear sections ---------------------------------------------------------------------------------------- */

   .clear_sec : ALIGN(4)
   {
      LONG(0 + ADDR(.dspr.bss.sc2));    LONG(SIZEOF(.dspr.bss.sc2));
      LONG(0 + ADDR(.dspr.bss.sc0));    LONG(SIZEOF(.dspr.bss.sc0));
   } AT> MEM_BOOT_ROM

   .copy_sec : ALIGN(4)
   {
      LONG(LOADADDR(.dspr.data.sc2));    LONG(0 + ADDR(.dspr.data.sc2));      LONG(SIZEOF(.dspr.data.sc2));
      LONG(LOADADDR(.dspr.data.sc0));    LONG(0 + ADDR(.dspr.data.sc0));      LONG(SIZEOF(.dspr.data.sc0));
      LONG(LOADADDR(.dspr.code));        LONG(0 + ADDR(.dspr.code));          LONG(SIZEOF(.dspr.code));
   } AT> MEM_BOOT_ROM

}