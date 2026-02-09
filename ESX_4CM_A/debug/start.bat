@echo off
rem ********************************************************************************************************************
rem Start Lauterbach debug software TRACE32 for Tricore applications
rem ********************************************************************************************************************

rem Path of boot core ELF file: can be absolute or relative to trace32 folder
set "ELF_File_BootCore=..\..\boot_core_debug\boot_core.elf"
rem Path of safety core ELF file: can be absolute or relative to trace32 folder
set "ELF_File_SafetyCore=..\..\safety_core_debug\safety_core.elf"
rem Path of application core ELF file: can be absolute or relative to trace32 folder
set "ELF_File_ApplCore=..\..\appl_core_debug\appl_core.elf"

rem Start generic batch script
"%~dp0trace32\start_t32mtc.bat" -Core0_elf "%ELF_File_BootCore%" -Core1_elf "%ELF_File_SafetyCore%" -Core2_elf "%ELF_File_ApplCore%" "ebu^=FALSE"