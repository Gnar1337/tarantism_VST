# Quick Start Guide - Tarantism VST3

## Getting Started with Building

### Step 1: Clone the Repository

```bash
git clone https://github.com/Gnar1337/tarantism_VST.git
cd tarantism_VST
```

### Step 2: Get the VST3 SDK

```bash
# Clone the VST3 SDK
git clone https://github.com/steinbergmedia/vst3sdk.git

# Initialize submodules (required!)
cd vst3sdk
git submodule update --init --recursive
cd ..
```

### Step 3: Build the Plugin

**Windows (Visual Studio):**
```bash
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk
cmake --build build --config Release
```

**macOS:**
```bash
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk -G Xcode
cmake --build build --config Release
```

**Linux:**
```bash
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk
cmake --build build --config Release
```

### Step 4: Install the Plugin

The built plugin will be at: `build/VST3/Release/Tarantism.vst3`

Copy it to your VST3 folder:
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
- **Linux**: `~/.vst3/`

## How to Use

1. **Load in DAW**: Open the plugin in your favorite DAW (Ableton, FL Studio, Reaper, etc.)

2. **Set Parameters**:
   - **Measure Length**: Choose 1-4 measures (knob on left)
   - **Note Duration**: Select note type - whole, half, quarter, 8th, or 16th (middle knob)
   - **Octave**: Pick target octave 0-10 (right knob)

3. **Play Notes**: Play MIDI notes into the plugin
   - The plugin will wait for the correct number of notes
   - Example: 1 measure of 8th notes = waits for 8 notes
   - Example: 2 measures of quarter notes = waits for 8 notes

4. **Output**: Once enough notes are collected, they're all output at once, transposed to your selected octave

## Example Scenarios

### Scenario 1: Creating Bass Lines
- Set Measure Length: 1
- Set Note Duration: 8th notes
- Set Octave: 1 (low octave)
- Play 8 high notes → They output as 8 low bass notes

### Scenario 2: Creating Chord Progressions
- Set Measure Length: 2
- Set Note Duration: Quarter notes
- Set Octave: 4 (middle octave)
- Play 8 scattered notes → They output as a sequence in the middle range

### Scenario 3: Melodic Transformation
- Set Measure Length: 1
- Set Note Duration: 16th notes
- Set Octave: 6 (high octave)
- Play 16 notes → They output as rapid high-pitched notes

## Troubleshooting

### Build Issues

**"Cannot find VST3 SDK"**
- Make sure you set `-DVST3_SDK_ROOT=./vst3sdk` correctly
- Ensure the path points to where you cloned the SDK

**"Missing submodules"**
- Run `git submodule update --init --recursive` in the vst3sdk directory

### Plugin Not Showing in DAW

- Make sure you copied the `.vst3` bundle/folder (not just files inside it)
- Rescan plugins in your DAW
- Check DAW's VST3 search paths include the installation location

### MIDI Not Working

- Ensure the plugin is on a MIDI track (not audio track)
- Route MIDI to the plugin in your DAW
- Check that MIDI output from the plugin is routed to an instrument

## Technical Notes

- **Time Signature**: Assumes 4/4 time (4 quarter notes per measure)
- **Note Preservation**: Original note timing and velocity are preserved
- **Octave Range**: MIDI octaves 0-10 (notes 0-127)
- **Real-time**: All parameters can be automated in your DAW

## Support

For issues, please visit: https://github.com/Gnar1337/tarantism_VST/issues
