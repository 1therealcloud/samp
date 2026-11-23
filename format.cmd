@echo off
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%~dp0"

set "CF=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\bin\clang-format.exe"
if not exist "%CF%" (
    echo ERROR: clang-format.exe not found: %CF%
    pause
    exit /b 1
)

echo Directory: %CD%
"%CF%" --version
if errorlevel 1 goto :failed

set "FORMAT_COUNT=0"
for %%R in (source tools) do (
    for /f "delims=" %%F in ('dir /s /b /a-d "%%R\*" 2^>nul ^| findstr /v /i /r /c:"[\\]tools[\\]rcon[\\]" /c:"[\\]tools[\\]arctool2[\\]" /c:"[\\]tools[\\]exgui[\\]" /c:"[\\]tools[\\]dff[\\]Project1[\\]"') do (
        for %%E in (.cpp .c .h .hpp .inl) do if /i "%%~xF"=="%%E" (
            echo   %%F
            "%CF%" -i -style=file "%%F"
            if errorlevel 1 goto :failed
            set /a FORMAT_COUNT+=1 >nul
        )
    )
)

echo Formatted %FORMAT_COUNT% files.
pause
exit /b 0

:failed
echo ERROR: formatting failed.
pause
exit /b 1
