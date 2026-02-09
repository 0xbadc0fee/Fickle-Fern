@echo off

rem This is just a post build command for STW internal purposes
set "CRF_File=%~dp0..\..\..\internals\safety_core\bat\crf.bat"
if exist "%CRF_File%" (
   %CRF_File%
)