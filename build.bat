@echo off
REM Build script for Tarantism VST3 plugin (Windows)
REM Usage: build.bat [clean]

setlocal enabledelayedexpansion

echo ================================
echo Tarantism VST3 Build Script
echo ================================
echo.

REM Check if clean build is requested
if "%1"=="clean" (
    echo Cleaning build directory...
    if exist build rmdir /s /q build
    echo [OK] Build directory cleaned
    echo.
)

REM Check if VST3 SDK exists
if not exist "vst3sdk" (
    echo VST3 SDK not found. Downloading...
    git clone https://github.com/steinbergmedia/vst3sdk.git
    cd vst3sdk
    echo Initializing submodules ^(this may take a few minutes^)...
    git submodule update --init --recursive
    cd ..
    echo [OK] VST3 SDK downloaded and initialized
    echo.
) else (
    echo [OK] VST3 SDK found
)

REM Create build directory
if not exist "build" mkdir build

REM Configure CMake
echo.
echo Configuring CMake...
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk

if %errorlevel% equ 0 (
    echo [OK] CMake configuration successful
) else (
    echo [ERROR] CMake configuration failed
    exit /b 1
)

REM Build
echo.
echo Building plugin...
cmake --build build --config Release

if %errorlevel% equ 0 (
    echo.
    echo ================================
    echo [OK] Build successful!
    echo ================================
    echo.
    echo Plugin location:
    if exist "build\VST3\Release\Tarantism.vst3" (
        echo   build\VST3\Release\Tarantism.vst3
    ) else (
        echo   ^(Check build directory for .vst3 bundle^)
    )
    echo.
    echo Installation instructions:
    echo   Copy the Tarantism.vst3 folder to:
    echo   C:\Program Files\Common Files\VST3\
    echo.
) else (
    echo [ERROR] Build failed
    exit /b 1
)

endlocal
