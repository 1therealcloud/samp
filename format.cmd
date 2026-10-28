@echo off
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%~dp0"
set "FORMAT_PAUSE=1"
if /i "%~1"=="--no-pause" set "FORMAT_PAUSE=0"
if not "%~1"=="" if /i not "%~1"=="--no-pause" goto :usage
if not "%~2"=="" goto :usage

set "CF=%CLANG_FORMAT%"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not defined CF if exist "%VSWHERE%" (
    for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -property installationPath`) do (
        if exist "%%I\VC\Tools\Llvm\x64\bin\clang-format.exe" (
            set "CF=%%I\VC\Tools\Llvm\x64\bin\clang-format.exe"
        ) else if exist "%%I\VC\Tools\Llvm\bin\clang-format.exe" (
            set "CF=%%I\VC\Tools\Llvm\bin\clang-format.exe"
        )
    )
)
if not defined CF if exist "%ProgramFiles%\LLVM\bin\clang-format.exe" set "CF=%ProgramFiles%\LLVM\bin\clang-format.exe"
if not defined CF for %%I in (clang-format.exe) do set "CF=%%~$PATH:I"
if not defined CF goto :missing_formatter
if not exist "%CF%" goto :missing_formatter

echo Directory: %CD%
echo Formatter: %CF%
"%CF%" --version
if errorlevel 1 goto :failed
set "FORMAT_COUNT=0"
for /r "%CD%\source" %%F in (*) do (
    for %%E in (.cpp .c .h .hpp .inl) do if /i "%%~xF"=="%%E" (
        echo   %%F
        "%CF%" -i -style=file "%%F"
        if errorlevel 1 goto :failed
        set /a FORMAT_COUNT+=1 >nul
    )
)
echo Formatted %FORMAT_COUNT% files.
set "FORMAT_EXIT=0"
goto :finish

:usage
echo Usage: format.cmd [--no-pause]
set "FORMAT_EXIT=1"
goto :finish

:missing_formatter
echo ERROR: clang-format.exe not found. Install LLVM or set CLANG_FORMAT to its full path.
set "FORMAT_EXIT=1"
goto :finish

:failed
echo ERROR: formatting failed.
set "FORMAT_EXIT=1"

:finish
if "%FORMAT_PAUSE%"=="1" pause
exit /b %FORMAT_EXIT%
