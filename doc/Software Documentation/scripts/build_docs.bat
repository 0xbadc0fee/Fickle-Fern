@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "DOXY_DIR=%SCRIPT_DIR%.."
set "DOXYFILE=Doxyfile_HTML"
set "TARGET_DIR=%DOXY_DIR%\output_HTML"

echo Running HTML Doxygen...

pushd "%DOXY_DIR%"
doxygen "%DOXYFILE%"
if errorlevel 1 (
    echo ERROR: Doxygen HTML failed.
    popd
    pause
    exit /b 1
)

if not exist "%TARGET_DIR%" mkdir "%TARGET_DIR%"

(
echo ^<!DOCTYPE html^>
echo ^<html^>
echo   ^<head^>
echo     ^<meta http-equiv="refresh" content="0; url=html/index.html"^>
echo   ^</head^>
echo   ^<body^>
echo     ^<p^>If you are not redirected automatically, ^<a href="html/index.html"^>click here^</a^>.^</p^>
echo   ^</body^>
echo ^</html^>
) > "%TARGET_DIR%\index.html"

popd

echo Redirect index.html created in %TARGET_DIR%
pause
endlocal