@echo off
setlocal
rem ********************************************************************************************************************
rem Start Lauterbach debug software TRACE32 for Tricore applications
rem
rem With this batch script you can start TRACE32 to debug Tricore applications.
rem
rem How to use the batch file:
rem start_t32mtc.bat -Core0_elf "..\core0.elf" -Core1_elf "..\core1.elf" -Core2_elf "..\core2.elf"
rem
rem Additional parameters:
rem - Change debug port type: debugporttype=DAP2
rem - Flashloader:            fblmode=true
rem - 2nd level Flashlader:   fbl2mode=true
rem ********************************************************************************************************************

set "BatchFileDirectory=%~dp0"
set "WorkingDirectory=%cd%"

rem Change working directory to batch file directory
%~d0 & cd %BatchFileDirectory%

rem ********************************************************************************************************************
rem Parse arguments
rem ********************************************************************************************************************
set "Parameter_Core0_elf=undefined"
set "Parameter_Core1_elf=undefined"
set "Parameter_Core2_elf=undefined"
set "Parameter_Additional="

rem We cannot use a for loop here due to the variable expansion before execution
:loop_1
if not "%1" == "" (
   if "%1" == "-Core0_elf" (
      rem Also remove the quotes here (if there are some)
      set "Parameter_Core0_elf=%~2"
      shift & shift
      goto :loop_1
   )
   if "%1" == "-Core1_elf" (
      rem Also remove the quotes here (if there are some)
      set "Parameter_Core1_elf=%~2"
      shift & shift
      goto :loop_1
   )
   if "%1" == "-Core2_elf" (
      rem Also remove the quotes here (if there are some)
      set "Parameter_Core2_elf=%~2"
      shift & shift
   ) else (
      rem Also remove the quotes here (if there are some)
      set "Parameter_Additional=%Parameter_Additional%%~1=%~2 "
      shift & shift
   )
   goto :loop_1
)

rem ********************************************************************************************************************
rem Generate canonicalized absolute paths
rem ********************************************************************************************************************
set /a NumberOfCores=0

if not "%Parameter_Core0_elf%" == "undefined" (
   rem ELF file of Core0
   for /F "delims=" %%i in ("%Parameter_Core0_elf%") do (
      set "ELF_File_Core0_Abs=%%~dpnxi"
   )
   set /a NumberOfCores=1
   if not "%Parameter_Core1_elf%" == "undefined" (
      rem Safety core ELF file
      for /F "delims=" %%i in ("%Parameter_Core1_elf%") do (
         set "ELF_File_Core1_Abs=%%~dpnxi"
      )
      set /a NumberOfCores=2
      if not "%Parameter_Core2_elf%" == "undefined" (
         rem Application core ELF file
         for /F "delims=" %%i in ("%Parameter_Core2_elf%") do (
            set "ELF_File_Core2_Abs=%%~dpnxi"
         )
         set /a NumberOfCores=3
      )
   )
)

rem ********************************************************************************************************************
rem Check parameters
rem ********************************************************************************************************************
set "TRACE32_PathAbs=%T32_AURIX%\bin\windows64\t32mtc.exe"
set "StartDebugger=true"

echo ##################
echo # START DEBUGGER #
echo ##################

if "%T32_AURIX%" == "" (
   echo Environment variable T32_AURIX not set!
   echo T32_AURIX must contain the installation path of the TRACE32 software for AURIX!
   echo "T32_AURIX\bin\windows64\t32mtc.exe"
   set "StartDebugger=false"
) else (
   if not exist "%TRACE32_PathAbs%" (
      echo Path of TRACE32 executable is wrong!
      echo "%TRACE32_PathAbs%"
      set "StartDebugger=false"
   )
)

if %NumberOfCores% EQU 0 (
   echo "ELF file of Core0 is not provided!
   set "StartDebugger=false"
)
if %NumberOfCores% GEQ 1 (
   if not exist "%ELF_File_Core0_Abs%" (
      echo ELF file of Core0 does not exist!
      echo "%ELF_File_Core0_Abs%"
      set "StartDebugger=false"
   ) else (
      set "T32_ELF_File_Parameters=Core0_elf=%ELF_File_Core0_Abs% "
   )
)
if %NumberOfCores% GEQ 2 (
   if not exist "%ELF_File_Core1_Abs%" (
      echo ELF file of Core1 does not exist!
      echo "%ELF_File_Core1_Abs%"
      set "StartDebugger=false"
   ) else (
      set "T32_ELF_File_Parameters=%T32_ELF_File_Parameters%Core1_elf=%ELF_File_Core1_Abs% "
   )
)
if %NumberOfCores% GEQ 3 (
   if not exist "%ELF_File_Core2_Abs%" (
      echo ELF file of Core2 does not exist!
      echo "%ELF_File_Core2_Abs%"
      set "StartDebugger=false"
   ) else (
      set "T32_ELF_File_Parameters=%T32_ELF_File_Parameters%Core2_elf=%ELF_File_Core2_Abs% "
   )
)

rem ********************************************************************************************************************
rem Start debugger
rem ********************************************************************************************************************

if "%StartDebugger%" == "true" (
   echo TRACE32 is running...
   %TRACE32_PathAbs% -c stw/config.t32 -s startup.cmm %T32_ELF_File_Parameters%%Parameter_Additional%
   set "TerminateDebugger=true"
)

if not "%TerminateDebugger%" == "true" (
   pause > nul
)

rem ********************************************************************************************************************
rem Terminate debugger
rem ********************************************************************************************************************

rem Terminate all TRACE32 processes (necessary for call from logi.CAD 3)
if "%TerminateDebugger%" == "true" (
   taskkill /im t32mtc.exe > nul 2>&1
)

rem Wait for termination of all TRACE32 processes (necessary for call from logi.CAD 3)
:loop_2
tasklist /fi "imagename eq t32mtc.exe" | findstr /I "t32mtc.exe" > nul
if "%ERRORLEVEL%" == "0" (
   if "%TerminateDebugger%" == "true" (
      goto :loop_2
   )
)

if "%TerminateDebugger%" == "true" (
   echo TRACE32 stopped!
)

rem Change working directory to initial working directory
%WorkingDirectory:~0,2% & cd %WorkingDirectory%

rem End localisation of environment variables
endlocal

exit 0
