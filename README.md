# Tarantism VST3 - MIDI Manipulation Plugin

A VST3 plugin for MIDI manipulation that collects incoming MIDI notes and transposes them to a specified octave.

## Features

- **MIDI Note Collection**: Waits for a specified number of MIDI notes before processing
- **Octave Transposition**: Automatically transposes collected notes to the target octave
- **Flexible Measure Control**: Configure measure length (1-4 measures)
- **Note Duration Selection**: Choose from whole, half, quarter, 8th, or 16th notes
- **37-Key Visual Keyboard**: On-screen keyboard display for visual feedback
- **Real-time Parameter Control**: Adjust all parameters in real-time

## How It Works

The plugin calculates the number of notes needed based on:
- **Measure Length**: 1-4 measures
- **Note Duration**: Whole, half, quarter, 8th, or 16th notes

For example:
- 1 measure of 8th notes = 8 notes to collect
- 2 measures of quarter notes = 8 notes to collect
- 1 measure of 16th notes = 16 notes to collect

Once the required number of notes is collected, they are all output at once, transposed to the selected octave (0-10).

## Building the Plugin

### Prerequisites

1. **CMake** (version 3.15 or higher)
2. **C++ Compiler** with C++17 support
   - Windows: Visual Studio 2019 or later
   - macOS: Xcode 10 or later
   - Linux: GCC 7 or later
3. **VST3 SDK** from Steinberg

### Setup

1. Clone this repository:
```bash
git clone https://github.com/Gnar1337/tarantism_VST.git
cd tarantism_VST
```

2. Download and extract the VST3 SDK:
```bash
git clone https://github.com/steinbergmedia/vst3sdk.git
cd vst3sdk
git submodule update --init --recursive
cd ..
```

### Build Instructions

#### Windows

```bash
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk
cmake --build build --config Release
```

The plugin will be located in `build/VST3/Release/Tarantism.vst3`

#### macOS

```bash
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk
cmake --build build --config Release
```

The plugin will be located in `build/VST3/Release/Tarantism.vst3`

#### Linux

```bash
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk
cmake --build build --config Release
```

The plugin will be located in `build/VST3/Release/Tarantism.vst3`

### Installation

Copy the `Tarantism.vst3` bundle to your VST3 plugins folder:

- **Windows**: `C:\Program Files\Common Files\VST3\`
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/` or `/Library/Audio/Plug-Ins/VST3/`
- **Linux**: `~/.vst3/` or `/usr/lib/vst3/`

## Usage

1. Load the plugin in your DAW
2. Configure the parameters:
   - **Measure Length**: Set how many measures of notes to collect (1-4)
   - **Note Duration**: Choose the note duration (whole, half, quarter, 8th, 16th)
   - **Octave**: Select the target octave for transposition (0-10)
3. Play MIDI notes into the plugin
4. Once enough notes are collected, they will be output transposed to the target octave
5. The cycle repeats automatically

## GUI

The plugin features a custom GUI with:
- 37-key keyboard visualization (3 octaves)
- Knob controls for Measure Length, Note Duration, and Octave
- Visual feedback and parameter displays
- Clear instructions for usage

## Development

### Project Structure

```
tarantism_VST/
├── CMakeLists.txt           # Build configuration
├── source/
│   ├── pluginprocessor.h    # Audio/MIDI processor interface
│   ├── pluginprocessor.cpp  # MIDI processing logic
│   ├── plugincontroller.h   # GUI controller interface
│   ├── plugincontroller.cpp # GUI implementation
│   ├── pluginids.h          # Plugin and parameter IDs
│   ├── version.h            # Version information
│   └── factory.cpp          # Plugin factory entry point
└── resource/
    └── plugin.uidesc        # GUI description (VSTGUI)
```

### Technical Details

- Uses VST3 SDK for plugin architecture
- VSTGUI for graphical interface
- MIDI event processing for note collection
- Real-time octave transposition
- State serialization for project saving

## License

This project is provided as-is. Please ensure compliance with Steinberg's VST3 SDK license when distributing.

## Credits

Developed by Gnar
VST is a trademark of Steinberg Media Technologies GmbH