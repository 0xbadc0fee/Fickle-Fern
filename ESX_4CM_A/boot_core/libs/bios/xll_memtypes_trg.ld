/* -- System resource regions --------------------------------------------------------------------------------------- */
SECTIONS
{

   .trg.bss.icc_resources (NOLOAD) : ALIGN(32)
   {
      LD_32_TRG_ICC_MSG_PROXIES_BEGIN = ABSOLUTE(.);
      KEEP(*(*.icc_msg_proxies))
      . = ALIGN(32);
      LD_32_TRG_ICC_MSG_PROXIES_END = ABSOLUTE(.);
   } > MEM_TRG_ICC_RESOURCES

}

_. = ASSERT (SIZEOF(.BMI_Header) == 0x20, "BMI Header must be defined for this target build!");

/* -- Target memories ----------------------------------------------------------------------------------------------- */

MEM_TRG_CORE2_EMEM_ETH_UC_START = ORIGIN(core2_emem_eth_uc);
MEM_TRG_CORE2_EMEM_ETH_UC_END   = ORIGIN(core2_emem_eth_uc) + LENGTH(core2_emem_eth_uc);

/* -- Private aliases ----------------------------------------------------------------------------------------------- */
MEM_MY_DSPR_START           = ORIGIN(MEM_MY_DSPR);
MEM_MY_DSPR_END             = ORIGIN(MEM_MY_DSPR) + LENGTH(MEM_MY_DSPR);

MEM_MY_PSPR_START           = ORIGIN(MEM_MY_PSPR);
MEM_MY_PSPR_END             = ORIGIN(MEM_MY_PSPR) + LENGTH(MEM_MY_PSPR);

MEM_MY_LMU_START            = ORIGIN(MEM_MY_LMU);
MEM_MY_LMU_END              = ORIGIN(MEM_MY_LMU)    + LENGTH(MEM_MY_LMU);
MEM_MY_LMU_UC_START         = ORIGIN(MEM_MY_LMU_UC);
MEM_MY_LMU_UC_END           = ORIGIN(MEM_MY_LMU_UC) + LENGTH(MEM_MY_LMU_UC);

MEM_MY_EMEM_START           = ORIGIN(MEM_MY_EMEM);
MEM_MY_EMEM_END             = ORIGIN(MEM_MY_EMEM)    + LENGTH(MEM_MY_EMEM);
MEM_MY_EMEM_UC_START        = ORIGIN(MEM_MY_EMEM_UC);
MEM_MY_EMEM_UC_END          = ORIGIN(MEM_MY_EMEM_UC) + LENGTH(MEM_MY_EMEM_UC);

/* -- Core 0 Memories ----------------------------------------------------------------------------------------------- */
MEM_TRG_CORE0_LMU_START     = ORIGIN(core0_lmu);
MEM_TRG_CORE0_LMU_END       = ORIGIN(core0_lmu) + LENGTH(core0_lmu);
MEM_TRG_CORE0_LMU_UC_START  = ORIGIN(core0_lmu_uc);
MEM_TRG_CORE0_LMU_UC_END    = ORIGIN(core0_lmu_uc) + LENGTH(core0_lmu_uc);
MEM_TRG_CORE0_EMEM_START    = ORIGIN(core0_emem);
MEM_TRG_CORE0_EMEM_END      = ORIGIN(core0_emem) + LENGTH(core0_emem);
MEM_TRG_CORE0_EMEM_UC_START = ORIGIN(core0_emem_uc);
MEM_TRG_CORE0_EMEM_UC_END   = ORIGIN(core0_emem_uc) + LENGTH(core0_emem_uc);

/* -- Core 1 Memories ----------------------------------------------------------------------------------------------- */
MEM_TRG_CORE1_LMU_START     = ORIGIN(core1_lmu);
MEM_TRG_CORE1_LMU_END       = ORIGIN(core1_lmu) + LENGTH(core1_lmu);
MEM_TRG_CORE1_LMU_UC_START  = ORIGIN(core1_lmu_uc);
MEM_TRG_CORE1_LMU_UC_END    = ORIGIN(core1_lmu_uc) + LENGTH(core1_lmu_uc);
MEM_TRG_CORE1_EMEM_START    = ORIGIN(core1_emem);
MEM_TRG_CORE1_EMEM_END      = ORIGIN(core1_emem) + LENGTH(core1_emem);
MEM_TRG_CORE1_EMEM_UC_START = ORIGIN(core1_emem_uc);
MEM_TRG_CORE1_EMEM_UC_END   = ORIGIN(core1_emem_uc) + LENGTH(core1_emem_uc);

/* -- Core 2 Memories ----------------------------------------------------------------------------------------------- */
MEM_TRG_CORE2_LMU_START     = ORIGIN(core2_lmu);
MEM_TRG_CORE2_LMU_END       = ORIGIN(core2_lmu) + LENGTH(core2_lmu);
MEM_TRG_CORE2_LMU_UC_START  = ORIGIN(core2_lmu_uc);
MEM_TRG_CORE2_LMU_UC_END    = ORIGIN(core2_lmu_uc) + LENGTH(core2_lmu_uc);
MEM_TRG_CORE2_EMEM_START    = ORIGIN(core2_emem);
MEM_TRG_CORE2_EMEM_END      = ORIGIN(core2_emem) + LENGTH(core2_emem);


MEM_TRG_CORE2_EMEM_UC_START = ORIGIN(core2_emem_uc);
MEM_TRG_CORE2_EMEM_UC_END   = ORIGIN(core2_emem_uc) + LENGTH(core2_emem_uc);
