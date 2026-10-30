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

set "OUT=build\libs"
if not exist "%OUT%" mkdir "%OUT%"
if not exist "%OUT%" goto :failed
if exist "%OUT%\ready.txt" del /q "%OUT%\ready.txt"
if exist "%OUT%\ready.txt" goto :failed
if not exist "%OUT%\dxut-sdk" mkdir "%OUT%\dxut-sdk"
if not exist "%OUT%\bin" mkdir "%OUT%\bin"
rem Only untouched DXUT units come from the installed SDK.
rem The maintained GUI source/header and shared state are in source\dxut.
for %%F in (DXUT DXUTenum DXUTmisc DXUTMesh DXUTSettingsDlg DXUTsound) do (
    copy /y "%SAMP_DXSDK%\Samples\C++\Common\%%F.cpp" "%OUT%\dxut-sdk\%%F.cpp" >nul
    if errorlevel 1 goto :failed
    copy /y "%SAMP_DXSDK%\Samples\C++\Common\%%F.h" "%OUT%\dxut-sdk\%%F.h" >nul
    if errorlevel 1 goto :failed
)
for %%F in (DXUTgui.h dxstdafx.h) do (
    copy /y "source\dxut\%%F" "%OUT%\dxut-sdk\%%F" >nul
    if errorlevel 1 goto :failed
)

call :beginlib RakNet
if errorlevel 1 goto :failed
set "FLAGS=/nologo /c /MT /Gy /O2 /Ob1 /Oi /GF /Zp1 /GX /GR- /DNDEBUG /DWIN32"
set "INCLUDE=%CD%\third-party\RakNet\Include;%SYSTEM_INCLUDE%"
for %%F in (
    third-party\RakNet\Source\BitStream.cpp
    third-party\RakNet\Source\CheckSum.cpp
    third-party\RakNet\Source\DS_HuffmanEncodingTree.cpp
    third-party\RakNet\Source\GetTime.cpp
    third-party\RakNet\Source\InternalPacketPool.cpp
    third-party\RakNet\Source\NetworkTypes.cpp
    third-party\RakNet\Source\PluginInterface.cpp
    third-party\RakNet\Source\RakClient.cpp
    third-party\RakNet\Source\RakNetStatistics.cpp
    third-party\RakNet\Source\RakNetworkFactory.cpp
    third-party\RakNet\Source\RakPeer.cpp
    third-party\RakNet\Source\rakserver.cpp
    third-party\RakNet\Source\RakSleep.cpp
    third-party\RakNet\Source\Rand.cpp
    third-party\RakNet\Source\ReliabilityLayer.cpp
    third-party\RakNet\Source\RPCMap.cpp
    third-party\RakNet\Source\SimpleMutex.cpp
    third-party\RakNet\Source\SocketLayer.cpp
    third-party\RakNet\Source\StringCompressor.cpp
    third-party\RakNet\Source\StringTable.cpp
    third-party\RakNet\Source\TEABlockEncryptor.cpp
) do (
    call :compile "%%F"
    if errorlevel 1 goto :failed
)
call :archive
if errorlevel 1 goto :failed

call :beginlib zlib
if errorlevel 1 goto :failed
set "FLAGS=/nologo /c /MT /Gy /O2 /Oy-"
set "INCLUDE=%CD%\third-party\zlib;%SYSTEM_INCLUDE%"
for %%F in (
    third-party\zlib\adler32.c
    third-party\zlib\compress.c
    third-party\zlib\crc32.c
    third-party\zlib\deflate.c
    third-party\zlib\gzclose.c
    third-party\zlib\gzlib.c
    third-party\zlib\gzread.c
    third-party\zlib\gzwrite.c
    third-party\zlib\infback.c
    third-party\zlib\inffast.c
    third-party\zlib\inflate.c
    third-party\zlib\inftrees.c
    third-party\zlib\trees.c
    third-party\zlib\uncompr.c
    third-party\zlib\zutil.c
) do (
    call :compile "%%F"
    if errorlevel 1 goto :failed
)
call :archive
if errorlevel 1 goto :failed

call :beginlib libcurl
if errorlevel 1 goto :failed
set "FLAGS=/nologo /c /MT /Gy /O2 /Ob1 /Oi /GF /Zp1 /DBUILDING_LIBCURL /DCURL_STATICLIB /DUSE_WINDOWS_SSPI /DUSE_SCHANNEL /DHAVE_LIBZ /DHAVE_ZLIB_H"
set "INCLUDE=%CD%\third-party\curl\include;%CD%\third-party\curl\lib;%CD%\third-party\zlib;%SYSTEM_INCLUDE%"
for %%F in (
    third-party\curl\lib\file.c
    third-party\curl\lib\timeval.c
    third-party\curl\lib\base64.c
    third-party\curl\lib\hostip.c
    third-party\curl\lib\progress.c
    third-party\curl\lib\formdata.c
    third-party\curl\lib\cookie.c
    third-party\curl\lib\http.c
    third-party\curl\lib\sendf.c
    third-party\curl\lib\ftp.c
    third-party\curl\lib\url.c
    third-party\curl\lib\dict.c
    third-party\curl\lib\if2ip.c
    third-party\curl\lib\speedcheck.c
    third-party\curl\lib\ldap.c
    third-party\curl\lib\version.c
    third-party\curl\lib\getenv.c
    third-party\curl\lib\escape.c
    third-party\curl\lib\mprintf.c
    third-party\curl\lib\telnet.c
    third-party\curl\lib\netrc.c
    third-party\curl\lib\getinfo.c
    third-party\curl\lib\transfer.c
    third-party\curl\lib\strcase.c
    third-party\curl\lib\easy.c
    third-party\curl\lib\security.c
    third-party\curl\lib\curl_fnmatch.c
    third-party\curl\lib\fileinfo.c
    third-party\curl\lib\ftplistparser.c
    third-party\curl\lib\wildcard.c
    third-party\curl\lib\krb5.c
    third-party\curl\lib\memdebug.c
    third-party\curl\lib\http_chunks.c
    third-party\curl\lib\strtok.c
    third-party\curl\lib\connect.c
    third-party\curl\lib\llist.c
    third-party\curl\lib\hash.c
    third-party\curl\lib\multi.c
    third-party\curl\lib\content_encoding.c
    third-party\curl\lib\share.c
    third-party\curl\lib\http_digest.c
    third-party\curl\lib\md4.c
    third-party\curl\lib\md5.c
    third-party\curl\lib\http_negotiate.c
    third-party\curl\lib\inet_pton.c
    third-party\curl\lib\strtoofft.c
    third-party\curl\lib\strerror.c
    third-party\curl\lib\amigaos.c
    third-party\curl\lib\hostasyn.c
    third-party\curl\lib\hostip4.c
    third-party\curl\lib\hostip6.c
    third-party\curl\lib\hostsyn.c
    third-party\curl\lib\inet_ntop.c
    third-party\curl\lib\parsedate.c
    third-party\curl\lib\select.c
    third-party\curl\lib\tftp.c
    third-party\curl\lib\splay.c
    third-party\curl\lib\strdup.c
    third-party\curl\lib\socks.c
    third-party\curl\lib\ssh.c
    third-party\curl\lib\curl_addrinfo.c
    third-party\curl\lib\socks_gssapi.c
    third-party\curl\lib\socks_sspi.c
    third-party\curl\lib\curl_sspi.c
    third-party\curl\lib\slist.c
    third-party\curl\lib\nonblock.c
    third-party\curl\lib\curl_memrchr.c
    third-party\curl\lib\imap.c
    third-party\curl\lib\pop3.c
    third-party\curl\lib\smtp.c
    third-party\curl\lib\pingpong.c
    third-party\curl\lib\rtsp.c
    third-party\curl\lib\curl_threads.c
    third-party\curl\lib\warnless.c
    third-party\curl\lib\hmac.c
    third-party\curl\lib\curl_rtmp.c
    third-party\curl\lib\openldap.c
    third-party\curl\lib\curl_gethostname.c
    third-party\curl\lib\gopher.c
    third-party\curl\lib\idn_win32.c
    third-party\curl\lib\http_proxy.c
    third-party\curl\lib\non-ascii.c
    third-party\curl\lib\asyn-ares.c
    third-party\curl\lib\asyn-thread.c
    third-party\curl\lib\curl_gssapi.c
    third-party\curl\lib\http_ntlm.c
    third-party\curl\lib\curl_ntlm_wb.c
    third-party\curl\lib\curl_ntlm_core.c
    third-party\curl\lib\curl_sasl.c
    third-party\curl\lib\rand.c
    third-party\curl\lib\curl_multibyte.c
    third-party\curl\lib\hostcheck.c
    third-party\curl\lib\conncache.c
    third-party\curl\lib\pipeline.c
    third-party\curl\lib\dotdot.c
    third-party\curl\lib\x509asn1.c
    third-party\curl\lib\http2.c
    third-party\curl\lib\smb.c
    third-party\curl\lib\curl_endian.c
    third-party\curl\lib\curl_des.c
    third-party\curl\lib\system_win32.c
    third-party\curl\lib\mime.c
    third-party\curl\lib\sha256.c
    third-party\curl\lib\setopt.c
    third-party\curl\lib\vauth\vauth.c
    third-party\curl\lib\vauth\cleartext.c
    third-party\curl\lib\vauth\cram.c
    third-party\curl\lib\vauth\digest.c
    third-party\curl\lib\vauth\digest_sspi.c
    third-party\curl\lib\vauth\krb5_gssapi.c
    third-party\curl\lib\vauth\krb5_sspi.c
    third-party\curl\lib\vauth\ntlm.c
    third-party\curl\lib\vauth\ntlm_sspi.c
    third-party\curl\lib\vauth\oauth2.c
    third-party\curl\lib\vauth\spnego_gssapi.c
    third-party\curl\lib\vauth\spnego_sspi.c
    third-party\curl\lib\vtls\openssl.c
    third-party\curl\lib\vtls\gtls.c
    third-party\curl\lib\vtls\vtls.c
    third-party\curl\lib\vtls\nss.c
    third-party\curl\lib\vtls\polarssl.c
    third-party\curl\lib\vtls\polarssl_threadlock.c
    third-party\curl\lib\vtls\axtls.c
    third-party\curl\lib\vtls\cyassl.c
    third-party\curl\lib\vtls\schannel.c
    third-party\curl\lib\vtls\darwinssl.c
    third-party\curl\lib\vtls\gskit.c
    third-party\curl\lib\vtls\mbedtls.c
) do (
    call :compile "%%F"
    if errorlevel 1 goto :failed
)
call :archive
if errorlevel 1 goto :failed

call :beginlib lzma
if errorlevel 1 goto :failed
set "FLAGS=/nologo /c /MT /Gy /O2 /Ob1 /Oi /GF /Zp1 /GS- /DNDEBUG"
set "INCLUDE=%CD%\third-party\lzma\C;%SYSTEM_INCLUDE%"
for %%F in (
    third-party\lzma\C\7zAlloc.c
    third-party\lzma\C\7zArcIn.c
    third-party\lzma\C\7zBuf.c
    third-party\lzma\C\7zBuf2.c
    third-party\lzma\C\7zCrc.c
    third-party\lzma\C\7zCrcOpt.c
    third-party\lzma\C\7zDec.c
    third-party\lzma\C\7zFile.c
    third-party\lzma\C\7zStream.c
    third-party\lzma\C\Aes.c
    third-party\lzma\C\AesOpt.c
    third-party\lzma\C\Alloc.c
    third-party\lzma\C\Bcj2.c
    third-party\lzma\C\Bcj2Enc.c
    third-party\lzma\C\Bra.c
    third-party\lzma\C\Bra86.c
    third-party\lzma\C\BraIA64.c
    third-party\lzma\C\CpuArch.c
    third-party\lzma\C\Delta.c
    third-party\lzma\C\DllSecur.c
    third-party\lzma\C\LzFind.c
    third-party\lzma\C\LzFindMt.c
    third-party\lzma\C\Lzma2Dec.c
    third-party\lzma\C\Lzma2Enc.c
    third-party\lzma\C\Lzma86Dec.c
    third-party\lzma\C\Lzma86Enc.c
    third-party\lzma\C\LzmaDec.c
    third-party\lzma\C\LzmaEnc.c
    third-party\lzma\C\LzmaLib.c
    third-party\lzma\C\MtCoder.c
    third-party\lzma\C\Ppmd7.c
    third-party\lzma\C\Ppmd7Dec.c
    third-party\lzma\C\Ppmd7Enc.c
    third-party\lzma\C\Sha256.c
    third-party\lzma\C\Sort.c
    third-party\lzma\C\Threads.c
    third-party\lzma\C\Xz.c
    third-party\lzma\C\XzCrc64.c
    third-party\lzma\C\XzCrc64Opt.c
    third-party\lzma\C\XzDec.c
    third-party\lzma\C\XzEnc.c
    third-party\lzma\C\XzIn.c
) do (
    call :compile "%%F"
    if errorlevel 1 goto :failed
)
call :archive
if errorlevel 1 goto :failed

call :beginlib dxut
if errorlevel 1 goto :failed
set "FLAGS=/nologo /c /O2 /Ob2 /Oi /GF /MT /Zp1 /Gy /GX /GR- /DNDEBUG /DWIN32 /DUNICODE /D_UNICODE /DSTRSAFE_NO_DEPRECATE /DCURL_STATICLIB /DSAMP_DIRECT_GTA_GLOBALS /FI"%CD%\build\libs\dxut-sdk\dxstdafx.h""
set "INCLUDE=%CD%\build\libs\dxut-sdk;%SYSTEM_INCLUDE%;%CD%\source;%CD%\source\dxut;%CD%\third-party\RakNet\Include;%CD%\third-party\bass;%CD%\third-party\detours;%CD%\third-party\curl\include;%CD%\third-party\zlib;%CD%\third-party\lzma\C;%CD%\third-party\ttmath"
for %%F in (
    build\libs\dxut-sdk\DXUT.cpp
    source\dxut\DXUTgui.cpp
    build\libs\dxut-sdk\DXUTmisc.cpp
    build\libs\dxut-sdk\DXUTenum.cpp
    build\libs\dxut-sdk\DXUTMesh.cpp
    build\libs\dxut-sdk\DXUTSettingsDlg.cpp
    build\libs\dxut-sdk\DXUTsound.cpp
    source\dxut\dxut_hybrid_native.cpp
    source\dxut\dxut_dialog_native.cpp
    source\dxut\dxut_texture_native.cpp
    source\dxut\dxut_media_native.cpp
    source\dxut\dxut_theme_native.cpp
    source\dxut\dxut_text_native.cpp
    source\dxut\dxut_manager_native.cpp
    source\dxut\dxut_listbox_native.cpp
) do (
    call :compile "%%F"
    if errorlevel 1 goto :failed
)
call :archive
if errorlevel 1 goto :failed

copy /y "third-party\bass\bass.lib" "%OUT%\bass.lib" >nul
if errorlevel 1 goto :failed
copy /y "third-party\bass\bass.dll" "%OUT%\bin\bass.dll" >nul
if errorlevel 1 goto :failed
copy /y "third-party\detours\detours.lib" "%OUT%\detours.lib" >nul
if errorlevel 1 goto :failed
if not exist "third-party\ttmath\ttmath\ttmath.h" goto :missing_submodules
>"%OUT%\ready.txt" echo VC=%SAMP_VC%
>>"%OUT%\ready.txt" echo PSDK=%SAMP_PSDK%
>>"%OUT%\ready.txt" echo DXSDK=%SAMP_DXSDK%
if not exist "%OUT%\ready.txt" goto :failed
echo.
echo Libraries built in build\libs. TTMath is header-only.
exit /b 0

:beginlib
set "NAME=%~1"
set "OBJDIR=%OUT%\obj\%NAME%"
set "LOGDIR=%OUT%\logs\%NAME%"
set "OBJECTS=%OUT%\%NAME%-objects.rsp"
if not exist "%OBJDIR%" mkdir "%OBJDIR%"
if not exist "%LOGDIR%" mkdir "%LOGDIR%"
if not exist "%OBJDIR%" exit /b 1
if not exist "%LOGDIR%" exit /b 1
type nul >"%OBJECTS%"
echo.
echo Building %NAME%...
exit /b 0

:archive
"%SAMP_VC%\bin\link.exe" /LIB /NOLOGO /MACHINE:X86 /OUT:"%OUT%\%NAME%.lib" @"%OBJECTS%" >"%LOGDIR%\archive.log" 2>&1
if errorlevel 1 (
    type "%LOGDIR%\archive.log"
    exit /b 1
)
echo Built %OUT%\%NAME%.lib
exit /b 0

:missing_submodules
echo ERROR: Initialize dependencies with git submodule update --init --recursive.
exit /b 1

:failed
echo ERROR: Dependency build failed. See build\libs\logs.
exit /b 1

:usage
echo Usage: build-libs.cmd [--rebuild]
echo Builds all dependencies using VC++ 2003 and Windows CMD only.
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
