#!/bin/bash
# Build script for Tarantism VST3 plugin
# Usage: ./build.sh [clean]

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}================================${NC}"
echo -e "${GREEN}Tarantism VST3 Build Script${NC}"
echo -e "${GREEN}================================${NC}"
echo ""

# Check if clean build is requested
if [ "$1" = "clean" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf build
    echo -e "${GREEN}✓ Build directory cleaned${NC}"
    echo ""
fi

# Check if VST3 SDK exists
if [ ! -d "vst3sdk" ]; then
    echo -e "${YELLOW}VST3 SDK not found. Downloading...${NC}"
    git clone https://github.com/steinbergmedia/vst3sdk.git
    cd vst3sdk
    echo -e "${YELLOW}Initializing submodules (this may take a few minutes)...${NC}"
    git submodule update --init --recursive
    cd ..
    echo -e "${GREEN}✓ VST3 SDK downloaded and initialized${NC}"
    echo ""
else
    echo -e "${GREEN}✓ VST3 SDK found${NC}"
fi

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

# Configure CMake
echo ""
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk -DCMAKE_BUILD_TYPE=Release

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ CMake configuration successful${NC}"
else
    echo -e "${RED}✗ CMake configuration failed${NC}"
    exit 1
fi

# Build
echo ""
echo -e "${YELLOW}Building plugin...${NC}"
cmake --build build --config Release

if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}================================${NC}"
    echo -e "${GREEN}✓ Build successful!${NC}"
    echo -e "${GREEN}================================${NC}"
    echo ""
    echo "Plugin location:"
    
    # Find the VST3 bundle
    if [ -d "build/VST3/Release/Tarantism.vst3" ]; then
        echo -e "${GREEN}  build/VST3/Release/Tarantism.vst3${NC}"
    elif [ -d "build/VST3/Tarantism.vst3" ]; then
        echo -e "${GREEN}  build/VST3/Tarantism.vst3${NC}"
    else
        echo -e "${YELLOW}  (Check build directory for .vst3 bundle)${NC}"
    fi
    
    echo ""
    echo "Installation instructions:"
    case "$OSTYPE" in
        darwin*)  echo "  sudo cp -r build/VST3/Release/Tarantism.vst3 /Library/Audio/Plug-Ins/VST3/" ;;
        linux*)   echo "  mkdir -p ~/.vst3 && cp -r build/VST3/Release/Tarantism.vst3 ~/.vst3/" ;;
        msys*|cygwin*)    echo "  Copy build/VST3/Release/Tarantism.vst3 to C:\\Program Files\\Common Files\\VST3\\" ;;
    esac
    echo ""
else
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
fi
