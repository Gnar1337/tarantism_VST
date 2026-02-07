# Tarantism VST

A basic VST3 audio plugin built with JUCE framework.

## Features

- VST3 format support
- Simple gain control with GUI
- Stereo audio processing
- Cross-platform (Windows, macOS, Linux)

## Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler
  - Windows: Visual Studio 2019 or newer
  - macOS: Xcode 10 or newer
  - Linux: GCC 7 or newer

## Building

### Clone and Build

```bash
# Clone the repository
git clone https://github.com/Gnar1337/tarantism_VST.git
cd tarantism_VST

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --config Release
```

### Platform-Specific Notes

#### Windows
- The VST3 plugin will be automatically copied to: `C:\Program Files\Common Files\VST3\`
- You may need to run as administrator for the automatic copy

#### macOS
- The VST3 plugin will be automatically copied to: `~/Library/Audio/Plug-Ins/VST3/`
- The Standalone app will be in: `build/TarantismVST_artefacts/Release/Standalone/`

#### Linux
- The VST3 plugin will be automatically copied to: `~/.vst3/`
- You may need to create this directory first

## Using the Plugin

1. After building, the VST3 plugin will be copied to your system's VST3 directory
2. Open your DAW (e.g., Reaper, FL Studio, Ableton Live, etc.)
3. Scan for new plugins or rescan your VST3 directory
4. Load "Tarantism VST" from your plugin list
5. Use the gain knob to control the output level

## Development

The plugin consists of:
- `Source/PluginProcessor.cpp/h` - Audio processing logic
- `Source/PluginEditor.cpp/h` - GUI implementation
- `CMakeLists.txt` - Build configuration

## License

This project is open source and available under the MIT License.