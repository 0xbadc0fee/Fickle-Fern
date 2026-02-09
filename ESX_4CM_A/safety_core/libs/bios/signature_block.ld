/*!
   \file
   \brief       Aurix Platform Signature Block

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/

MEM_SIGBLOCK_AREA0_START = DEFINED(MEM_SIGBLOCK_AREA0_START) ? MEM_SIGBLOCK_AREA0_START : MEM_BOOT_ROM_START;
MEM_SIGBLOCK_AREA0_SIZE  = DEFINED(MEM_SIGBLOCK_AREA0_SIZE)  ? MEM_SIGBLOCK_AREA0_SIZE  : MEM_BOOT_ROM_SIZE;

MEM_SIGBLOCK_AREA1_START = DEFINED(MEM_SIGBLOCK_AREA1_START) ? MEM_SIGBLOCK_AREA1_START : MEM_DEFAULT_ROM_0_START;
MEM_SIGBLOCK_AREA1_SIZE  = DEFINED(MEM_SIGBLOCK_AREA1_SIZE)  ? MEM_SIGBLOCK_AREA1_SIZE  : MEM_DEFAULT_ROM_0_USED;

MEM_SIGBLOCK_AREA2_START = DEFINED(MEM_SIGBLOCK_AREA2_START) ? MEM_SIGBLOCK_AREA2_START : MEM_DEFAULT_ROM_1_START;
MEM_SIGBLOCK_AREA2_SIZE  = DEFINED(MEM_SIGBLOCK_AREA2_SIZE)  ? MEM_SIGBLOCK_AREA2_SIZE  : MEM_DEFAULT_ROM_1_USED;

MEM_SIGBLOCK_AREA3_START = DEFINED(MEM_SIGBLOCK_AREA3_START) ? MEM_SIGBLOCK_AREA3_START : MEM_DEFAULT_ROM_2_START;
MEM_SIGBLOCK_AREA3_SIZE  = DEFINED(MEM_SIGBLOCK_AREA3_SIZE)  ? MEM_SIGBLOCK_AREA3_SIZE  : MEM_DEFAULT_ROM_2_USED;

MEM_SIGBLOCK_AREA4_START = DEFINED(MEM_SIGBLOCK_AREA4_START) ? MEM_SIGBLOCK_AREA4_START : MEM_DEFAULT_ROM_3_START;
MEM_SIGBLOCK_AREA4_SIZE  = DEFINED(MEM_SIGBLOCK_AREA4_SIZE)  ? MEM_SIGBLOCK_AREA4_SIZE  : MEM_DEFAULT_ROM_3_USED;

MEM_SIGBLOCK_AREA5_START = DEFINED(MEM_SIGBLOCK_AREA5_START) ? MEM_SIGBLOCK_AREA5_START : MEM_DEFAULT_ROM_4_START;
MEM_SIGBLOCK_AREA5_SIZE  = DEFINED(MEM_SIGBLOCK_AREA5_SIZE)  ? MEM_SIGBLOCK_AREA5_SIZE  : MEM_DEFAULT_ROM_4_USED;

MEM_SIGBLOCK_DESTINATION = DEFINED(MEM_SIGBLOCK_DESTINATION) ? MEM_SIGBLOCK_DESTINATION :
                                                         ORIGIN(MEM_BOOT_ROM) + MEM_APPLICATION_SIGNATURE_BLOCK_OFFSET;

SECTIONS
{
  /*
   *  This section provides a signature block that can be modified using the STWFlashSignatureGenerator
   *  tool in a post-build step. The linker will insert the start and end addresses of intern and
   *  external flash used by the application. The "QUAD(0);" entries are placeholder for the signature.
   *  This signature should be checked by the application to assure flash content integrity.
   */
   .block_signature MEM_SIGBLOCK_DESTINATION : AT(MEM_SIGBLOCK_DESTINATION) FLAGS(a)
   {
      /* Export variable name for C modules (variable declared in osy_fl_trg_engine_safe_if.h). */
      gt_ApplicationSignatureBlock = ABSOLUTE(.);

      /* magic key ";zwm2KgUZ!", zero terminated */
      LONG(0x6D777A3B);
      LONG(0x55674B32);
      SHORT(0x215A);
      BYTE(0x00);
      /* struct version */
      BYTE(0x04);
      /* struct size in byte */
      SHORT(0x0074 + (SIGNATURE_BLOCK_NEXT_LIST_END_ADDR - SIGNATURE_BLOCK_NEXT_LIST_START_ADDR));
      /* reserved */
      SHORT(0x0000);
      /* signature algorithm */
      SHORT(0x0003);
      /* number of next blocks */
      BYTE((SIGNATURE_BLOCK_NEXT_LIST_END_ADDR - SIGNATURE_BLOCK_NEXT_LIST_START_ADDR) / 4);
      /* number of memory areas to be checked */
      BYTE(0x06);
      /* memory area 0 start address and size */
      LONG(MEM_SIGBLOCK_AREA0_START);
      LONG(MEM_SIGBLOCK_AREA0_SIZE);
      /* memory area 1 start address and size */
      LONG(MEM_SIGBLOCK_AREA1_START);
      LONG(MEM_SIGBLOCK_AREA1_SIZE);
      /* memory area 2 start address and size */
      LONG(MEM_SIGBLOCK_AREA2_START);
      LONG(MEM_SIGBLOCK_AREA2_SIZE);
      /* memory area 3 start address and size */
      LONG(MEM_SIGBLOCK_AREA3_START);
      LONG(MEM_SIGBLOCK_AREA3_SIZE);
      /* memory area 4 start address and size */
      LONG(MEM_SIGBLOCK_AREA4_START);
      LONG(MEM_SIGBLOCK_AREA4_SIZE);
      /* memory area 5 start address and size */
      LONG(MEM_SIGBLOCK_AREA5_START);
      LONG(MEM_SIGBLOCK_AREA5_SIZE);
      /* signature for memory area 0 (filled in by signature tool) */
      QUAD(0);
      /* signature for memory area 1 (filled in by signature tool) */
      QUAD(0);
      /* signature for memory area 2 (filled in by signature tool) */
      QUAD(0);
      /* signature for memory area 3 (filled in by signature tool) */
      QUAD(0);
      /* signature for memory area 4 (filled in by signature tool) */
      QUAD(0);
      /* signature for memory area 5 (filled in by signature tool) */
      QUAD(0);
      /* Pointer to application defined Signature block */
      SIGNATURE_BLOCK_NEXT_LIST_START_ADDR = ABSOLUTE(.);
      KEEP(*(.next_signature_blocks))
      SIGNATURE_BLOCK_NEXT_LIST_END_ADDR = ABSOLUTE(.);
      KEEP(*(.application_info))
      SIGNATURE_BLOCK_APPLICATION_INFO_END_ADDR = ABSOLUTE(.);
   }
}

   _. = ASSERT(((SIGNATURE_BLOCK_NEXT_LIST_END_ADDR - SIGNATURE_BLOCK_NEXT_LIST_START_ADDR) % 4) == 0 , "List of next signature block pointers has invalid size (multiple of 4)!");
   _. = ASSERT((SIGNATURE_BLOCK_APPLICATION_INFO_END_ADDR - SIGNATURE_BLOCK_NEXT_LIST_END_ADDR) >= 102, "A valid application information block must be defined for each signature block instance");
