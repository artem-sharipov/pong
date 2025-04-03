@echo off
set EXE_NAME=pong.exe
set RES_DIR=resources
set EXE_DIR=build\Debug
set DEPLOY_DIR=deploy

if exist "%DEPLOY_DIR%" (
    rmdir /s /q "%DEPLOY_DIR%"
)

if not exist "%EXE_DIR%\%EXE_NAME%" (
    echo ERROR: executable not found.
    exit /b 1
)

if not exist "%DEPLOY_DIR%" (
    mkdir "%DEPLOY_DIR%"
)

copy "%EXE_DIR%\%EXE_NAME%" "%DEPLOY_DIR%\"

if exist "%RES_DIR%" (
    xcopy /E /I "%RES_DIR%" "%DEPLOY_DIR%\"%RES_DIR%"\"
) else (
    echo Folder "%RES_DIR%" not found.
)

pause
