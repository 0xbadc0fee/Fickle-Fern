/*
 *   GNU GCC aurix platform tc299 openSYDE memory layout
 */

/* Include platform memories and core interop sections */
INCLUDE "tc299_memdef.ld"

MEM_APPLICATION_SIGNATURE_BLOCK_OFFSET = 0x200; /* Signature block offset relative to start address */

MEMORY
{
/* Fixed sectors for openSYDE flashloader */
   loader_boot_rom    (rx) :   org = 0x80000000, len = 0x0000C000  /* flash loader boot rom with BMI   (48 KiB) */
   loader_rom_0       (rx) :   org = 0x80018000, len = 0x00020000  /* flash loader main rom 0         (128 KiB) */
   loader_rom_1       (rx) :   org = 0x80038000, len = 0x00028000  /* 2nd level reserved space        (160 KiB) */
}

/* Convert specific Application start labels to a generic one that is understood by the startup code */
MEM_LOADER_START_ADDR = ORIGIN(loader_boot_rom) + 0x20;

/*
Signature block addresses must match with MEM_SIGBLOCK_DESTINATION from signature_block.ld. It is not possible to check
this with an ASSERT because there can be additional target specific signature block addresses which we do not know here!
*/
MEM_CORE0_SIGBLOCK_ADDR = ORIGIN(core0_boot_rom) + MEM_APPLICATION_SIGNATURE_BLOCK_OFFSET;
MEM_CORE1_SIGBLOCK_ADDR = ORIGIN(core1_boot_rom) + MEM_APPLICATION_SIGNATURE_BLOCK_OFFSET;
MEM_CORE2_SIGBLOCK_ADDR = ORIGIN(core2_boot_rom) + MEM_APPLICATION_SIGNATURE_BLOCK_OFFSET;

/* Application start address must match with globally defined start addresses!  */
_. = ASSERT ((((MEM_START_ADDR | 0x20000000) == (MEM_CORE0_START_ADDR  | 0x20000000)) ||
              ((MEM_START_ADDR | 0x20000000) == (MEM_CORE1_START_ADDR  | 0x20000000)) ||
              ((MEM_START_ADDR | 0x20000000) == (MEM_CORE2_START_ADDR  | 0x20000000)) ||
              ((MEM_START_ADDR | 0x20000000) == (MEM_LOADER_START_ADDR | 0x20000000))),
              "Application does not start at an expected start address (cf. MEM_START_ADDR vs. MEM_COREx_START_ADDR)!");
