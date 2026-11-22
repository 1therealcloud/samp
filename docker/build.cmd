@echo off
setlocal

for %%I in ("%~dp0.") do set "DOCKER_DIR=%%~fI"
for %%I in ("%~dp0..") do set "PROJECT_DIR=%%~fI"
set "OUTPUT_DIR=%CD%"
set "IMAGE=samp-gcc34:latest"
set "CONTAINER="

docker info >nul 2>&1
if errorlevel 1 (
    echo Docker is not available.
    exit /b 1
)

docker image inspect "%IMAGE%" >nul 2>&1
if errorlevel 1 (
    docker build --platform linux/386 -t "%IMAGE%" -f "%DOCKER_DIR%\Dockerfile" "%DOCKER_DIR%"
    if errorlevel 1 exit /b 1
)

for /f "delims=" %%I in ('docker run -d --platform linux/386 -w /work "%IMAGE%" /bin/sh -c "while :; do sleep 3600; done"') do set "CONTAINER=%%I"
if not defined CONTAINER exit /b 1

docker exec "%CONTAINER%" mkdir -p /work/tools /work/source /work/third-party
if errorlevel 1 goto failed

docker cp "%PROJECT_DIR%\tools\announce" "%CONTAINER%:/work/tools/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\tools\plugin-sdk" "%CONTAINER%:/work/tools/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\server" "%CONTAINER%:/work/"
if errorlevel 1 goto failed

docker cp "%PROJECT_DIR%\source\httpclient.cpp" "%CONTAINER%:/work/source/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\source\httpclient.h" "%CONTAINER%:/work/source/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\source\static_assert.h" "%CONTAINER%:/work/source/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\source\runutil.cpp" "%CONTAINER%:/work/source/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\source\runutil.h" "%CONTAINER%:/work/source/"
if errorlevel 1 goto failed

docker cp "%PROJECT_DIR%\third-party\amx" "%CONTAINER%:/work/third-party/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\third-party\lzma" "%CONTAINER%:/work/third-party/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\third-party\RakNet" "%CONTAINER%:/work/third-party/"
if errorlevel 1 goto failed
docker cp "%PROJECT_DIR%\third-party\sqlite" "%CONTAINER%:/work/third-party/"
if errorlevel 1 goto failed

docker exec -w /work/tools/announce "%CONTAINER%" make -B CC=gcc-3.4 CXX=g++-3.4
if errorlevel 1 goto failed
docker exec -w /work/server "%CONTAINER%" make -B CC=gcc-3.4 CXX=g++-3.4
if errorlevel 1 goto failed

docker cp "%CONTAINER%:/work/tools/announce/announce" "%OUTPUT_DIR%\announce"
if errorlevel 1 goto failed
docker cp "%CONTAINER%:/work/server/samp03svr" "%OUTPUT_DIR%\samp03svr"
if errorlevel 1 goto failed

echo Built: %OUTPUT_DIR%\announce and %OUTPUT_DIR%\samp03svr
docker rm -f "%CONTAINER%" >nul 2>&1
endlocal & exit /b 0

:failed
docker rm -f "%CONTAINER%" >nul 2>&1
endlocal & exit /b 1
