@echo off
setlocal

REM Target directory
set "TARGET_DIR=output_HTML"
set "DOXYFILE=Doxyfile_HTML"
set "HTML_DIR=output_HTML\html\index.html"
set "HTML_CHM=output_CHM\html"

REM Run Doxygen HTML
REM Need Graphviz installed: https://graphviz.org/download/
echo Running HTML Doxygen...
doxygen "%DOXYFILE%"
if errorlevel 1 (
    echo ❌ Doxygen HTML failed.
    exit /b 1
)

REM Run Doxygen CHM (optional)
REM echo Running CHM Doxygen...
REM doxygen "Doxyfile_CHM"
REM if errorlevel 1 (
REM     echo ❌ Doxygen CHM failed.
REM     exit /b 1
REM )

REM Create redirect index.html
(
echo ^<!DOCTYPE html^>
echo ^<html^>
echo   ^<head^>
echo     ^<meta http-equiv="refresh" content="0; URL='html/index.html'" /^>
echo   ^</head^>
echo   ^<body^>
echo     ^<p^>If you are not redirected automatically, ^<a href='html/index.html'^>documentation^</a^> click here.^</p^>
echo   ^</body^>
echo ^</html^>
) > "%TARGET_DIR%\index.html"

echo ✅ Redirect index.html created in %TARGET_DIR%

endlocal