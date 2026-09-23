@echo off
setlocal EnableDelayedExpansion

set "ROOT_FILE=.\Scripts\Win32\CopyDLLPerProject.ps1"
set "IS_INVALID=0"

set "DEPENDENCIES=cmake python make"

for %%D in (%DEPENDENCIES%) do (
    where %%D >nul 2>nul
    if errorlevel 1 (
        echo error: Missing dependency '%%D'
        set "IS_INVALID=1"
    )
)

if not exist "%ROOT_FILE%" (
    echo error: Please call the script from the root directory of the engine.
    set "IS_INVALID=1"
)

if "%~1"=="" (
    echo error: Invalid amount of arguments.
    echo.
    call :printHelp
    set "IS_INVALID=1"
)

if "!IS_INVALID!"=="1" (
    exit /b 1
)

python .\Scripts\BuildManagment.py %*

if errorlevel 1 (
    call :printHelp
)

exit /b 0

:printHelp
echo Help manual
echo.
echo      command:
echo          %~nx0 [build-switches] --All
echo          %~nx0 [build-switches] --Tests
echo          %~nx0 [build-switches] --Project ^<project-name^>
echo.
echo.
echo The scripts defaults to debug build type.
echo.
echo.
echo Build switches:
echo.
echo   -r :  release build type
echo   -d :  debug build type
echo   -v :  verbose mode
echo.
goto :eof
