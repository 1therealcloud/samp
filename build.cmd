@echo off
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%~dp0"
if not "%~1"=="" if /i not "%~1"=="--rebuild" goto :usage
if not "%~2"=="" goto :usage
if not defined SAMP_VC set "SAMP_VC=C:\Program Files (x86)\Microsoft Visual C++ Toolkit 2003"
if not defined SAMP_PSDK set "SAMP_PSDK=C:\Program Files\Microsoft Platform SDK"
if not defined SAMP_DXSDK set "SAMP_DXSDK=C:\Program Files (x86)\Microsoft DirectX 9.0 SDK (April 2005)"
if not exist "%SAMP_VC%\bin\cl.exe" goto :missing_tools
if not exist "%SAMP_VC%\bin\link.exe" goto :missing_tools
if not exist "%SAMP_PSDK%\Include\windows.h" goto :missing_tools
if not exist "%SAMP_DXSDK%\Include\d3dx9.h" goto :missing_tools
set "PATH=%SAMP_VC%\bin;%SAMP_PSDK%\Bin;%PATH%"
set "CL="
set "_CL_="
set "LIB=%SAMP_VC%\lib;%SAMP_PSDK%\Lib;%SAMP_DXSDK%\Lib\x86;%CD%\build\libs"
set "SYSTEM_INCLUDE=%SAMP_VC%\include;%SAMP_DXSDK%\Include;%SAMP_PSDK%\Include"

if not exist "build\libs\ready.txt" goto :missing_libs
findstr /l /x /c:"VC=%SAMP_VC%" "build\libs\ready.txt" >nul
if errorlevel 1 goto :missing_libs
findstr /l /x /c:"PSDK=%SAMP_PSDK%" "build\libs\ready.txt" >nul
if errorlevel 1 goto :missing_libs
findstr /l /x /c:"DXSDK=%SAMP_DXSDK%" "build\libs\ready.txt" >nul
if errorlevel 1 goto :missing_libs
for %%L in (RakNet zlib libcurl lzma dxut bass detours) do (
    if not exist "build\libs\%%L.lib" goto :missing_libs
)
if not exist "build\libs\dxut-sdk\DXUTgui.h" goto :missing_libs
set "OUT=build\client"
set "OBJDIR=%OUT%\obj"
set "LOGDIR=%OUT%\logs"
set "OBJECTS=%OUT%\objects.rsp"
if not exist "%OBJDIR%" mkdir "%OBJDIR%"
if not exist "%LOGDIR%" mkdir "%LOGDIR%"
if not exist "%OBJDIR%" goto :failed
if not exist "%LOGDIR%" goto :failed
type nul >"%OBJECTS%"
set "FLAGS=/nologo /c /O2 /Ob2 /Oi /GF /MT /Zp1 /Gy /GX /GR- /DNDEBUG /DWIN32 /DUNICODE /D_UNICODE /DSTRSAFE_NO_DEPRECATE /DCURL_STATICLIB /DSAMP_DIRECT_GTA_GLOBALS /FI"%CD%\build\libs\dxut-sdk\dxstdafx.h""
set "INCLUDE=%CD%\build\libs\dxut-sdk;%SYSTEM_INCLUDE%;%CD%\source;%CD%\source\gui;%CD%\source\dxut;%CD%\third-party\RakNet\Include;%CD%\third-party\bass;%CD%\third-party\detours;%CD%\third-party\curl\include;%CD%\third-party\zlib;%CD%\third-party\lzma\C;%CD%\third-party\ttmath"
for %%F in (
    source\runutil.cpp
    source\gui\chat_dependencies.cpp
    source\cmdwindow.cpp
    source\fontrender.cpp
    source\gui\font_wrapper.cpp
    source\config.cpp
    source\gui\input_text.cpp
    source\gui\input_activity.cpp
    source\gui\input_password.cpp
    source\game\keystuff.cpp
    source\game\aimstuff.cpp
    source\game\player_colors.cpp
    source\game\animation.cpp
    source\game\scripting.cpp
    source\game\entity.cpp
    source\game\building_removal.cpp
    source\filesystem.cpp
    source\archive\ArchiveFS.cpp
    source\archive\CryptoContext.cpp
    source\archive\CryptoFns.cpp
    source\archive\Hasher.cpp
    source\archive\KeyPair.cpp
    source\archive\Signer.cpp
    source\archive\TinyEncrypt.cpp
    source\filehooks.cpp
    source\d3dhook\IDirect3DDevice9Hook.cpp
    source\gui\device_objects.cpp
    source\game\device_integrity.cpp
    source\main.cpp
    source\objectedit.cpp
    source\game\model_preview.cpp
    source\game\object.cpp
    source\game\textdraw.cpp
    source\game\font.cpp
    source\game\util.cpp
    source\game\quaternion.cpp
    source\game\weapon_backend.cpp
    source\game\game.cpp
    source\game\pad.cpp
    source\net\pool_frame.cpp
    source\game\vehicle.cpp
    source\net\vehiclepool.cpp
    source\game\custommodels.cpp
    source\game\camera.cpp
    source\game\audio.cpp
    source\chatwindow.cpp
    source\gui\input_ime.cpp
    source\game\patches.cpp
    source\game\hooks.cpp
    source\archive\name_digest.cpp
    source\exceptions.cpp
    source\gui\gui.cpp
    source\audiostream.cpp
    source\gui\gui_events.cpp
    source\cmdprocs.cpp
    source\subclass.cpp
    source\events.cpp
    source\gui\device_lifecycle.cpp
    source\game\playerped.cpp
    source\net\event_lookup.cpp
    source\game\task.cpp
    source\gui\render_state.cpp
    source\net\local_rpc.cpp
    source\dllmain.cpp
    source\helpdialog.cpp
    source\label.cpp
    source\dialog.cpp
    source\game\actor.cpp
    source\menu.cpp
    source\net\gangzonepool.cpp
    source\net\menupool.cpp
    source\net\pickuppool.cpp
    source\net\actorpool.cpp
    source\textdrawselection.cpp
    source\net\textdrawpool.cpp
    source\net\objectpool.cpp
) do (
    call :compile "%%F"
    if errorlevel 1 goto :failed
)

"%SAMP_VC%\bin\link.exe" /LIB /NOLOGO /MACHINE:X86 /OUT:"%OUT%\samp-recovered.lib" @"%OBJECTS%" >"%LOGDIR%\archive.log" 2>&1
if errorlevel 1 (
    type "%LOGDIR%\archive.log"
    goto :failed
)
if exist "%OUT%\samp-link.dll" del /q "%OUT%\samp-link.dll"
"%SAMP_VC%\bin\link.exe" /NOLOGO /DLL /MACHINE:X86 /OPT:REF /OUT:"%OUT%\samp-link.dll" /MAP:"%OUT%\samp.map" @"%OBJECTS%" ^
    build\libs\RakNet.lib build\libs\zlib.lib build\libs\libcurl.lib ^
    build\libs\lzma.lib build\libs\dxut.lib build\libs\bass.lib build\libs\detours.lib ^
    libcmt.lib libcpmt.lib oldnames.lib kernel32.lib user32.lib gdi32.lib advapi32.lib ws2_32.lib winmm.lib imm32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib shell32.lib wldap32.lib crypt32.lib secur32.lib version.lib usp10.lib dsound.lib d3d9.lib d3dx9.lib dxguid.lib dxerr9.lib >"%OUT%\link.log" 2>&1
if errorlevel 1 (
    type "%OUT%\link.log"
    if exist "%OUT%\samp-link.dll" del /q "%OUT%\samp-link.dll"
    echo ERROR: No new samp.dll produced. See build\client\link.log.
    exit /b 1
)
copy /y "%OUT%\samp-link.dll" "%OUT%\samp.dll" >nul
if errorlevel 1 goto :failed
del /q "%OUT%\samp-link.dll"
copy /y "third-party\bass\bass.dll" "%OUT%\bass.dll" >nul
if errorlevel 1 goto :failed
echo Built build\client\samp.dll
exit /b 0

:missing_libs
echo ERROR: Build dependencies first with build-libs.cmd using the same SDK paths.
exit /b 1

:failed
echo ERROR: SA-MP compilation or archive build failed. See build\client\logs.
exit /b 1

:usage
echo Usage: build.cmd [--rebuild]
echo Builds and links SA-MP using VC++ 2003 and Windows CMD only.
exit /b 1

:compile
set "SRC=%~1"
if not exist "%SRC%" (
    echo ERROR: Missing source "%SRC%".
    exit /b 1
)
set "UNIT=%SRC:\=_%"
for %%U in ("%UNIT%") do set "UNIT=%%~nU"
echo [CL] %SRC%
"%SAMP_VC%\bin\cl.exe" %FLAGS% /Fo"%OBJDIR%\%UNIT%.obj" "%SRC%" >"%LOGDIR%\%UNIT%.log" 2>&1
if errorlevel 1 (
    type "%LOGDIR%\%UNIT%.log"
    exit /b 1
)
>>"%OBJECTS%" echo "%OBJDIR%\%UNIT%.obj"
exit /b 0

:missing_tools
echo ERROR: VC++ Toolkit 2003, Platform SDK or DirectX April 2005 SDK is missing.
echo SAMP_VC=%SAMP_VC%
echo SAMP_PSDK=%SAMP_PSDK%
echo SAMP_DXSDK=%SAMP_DXSDK%
exit /b 1
