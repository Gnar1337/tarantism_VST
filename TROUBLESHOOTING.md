# Tarantism VST3 - Troubleshooting Guide

## Build Issues

### Problem: "CMake not found"

**Symptoms:**
```
bash: cmake: command not found
```

**Solution:**
Install CMake:
- **Windows**: Download from https://cmake.org/download/
- **macOS**: `brew install cmake`
- **Linux**: `sudo apt-get install cmake` or `sudo yum install cmake`

---

### Problem: "Cannot find VST3 SDK"

**Symptoms:**
```
CMake Error: Could not find VST3_SDK_ROOT
```

**Solution:**
1. Clone the SDK: `git clone https://github.com/steinbergmedia/vst3sdk.git`
2. Initialize submodules: `cd vst3sdk && git submodule update --init --recursive`
3. Make sure `-DVST3_SDK_ROOT=./vst3sdk` points to the correct path

---

### Problem: "Submodule errors"

**Symptoms:**
```
fatal: no submodule mapping found in .gitmodules
```

**Solution:**
```bash
cd vst3sdk
git submodule update --init --recursive
```

If still failing:
```bash
cd vst3sdk
rm -rf *
git reset --hard HEAD
git submodule update --init --recursive
```

---

### Problem: "Compiler not found" (Windows)

**Symptoms:**
```
CMake Error: No CMAKE_CXX_COMPILER could be found
```

**Solution:**
Install Visual Studio 2019 or later with C++ development tools:
1. Download Visual Studio Community (free)
2. Select "Desktop development with C++"
3. Install and restart
4. Run CMake from "Developer Command Prompt for VS"

---

### Problem: "Compiler not found" (macOS)

**Symptoms:**
```
xcrun: error: invalid active developer path
```

**Solution:**
Install Xcode Command Line Tools:
```bash
xcode-select --install
```

---

### Problem: "Compiler not found" (Linux)

**Symptoms:**
```
No CMAKE_CXX_COMPILER found
```

**Solution:**
Install GCC/G++:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# Fedora/RHEL
sudo yum groupinstall "Development Tools"

# Arch
sudo pacman -S base-devel
```

---

### Problem: "VSTGUI errors during build"

**Symptoms:**
```
fatal error: vstgui/vstgui.h: No such file or directory
```

**Solution:**
This means VST3 SDK submodules weren't initialized properly:
```bash
cd vst3sdk
git submodule update --init --recursive
cd ..
cmake -B build -S . -DVST3_SDK_ROOT=./vst3sdk
```

---

### Problem: "Build succeeds but no .vst3 file"

**Symptoms:**
Build completes but can't find the plugin

**Solution:**
Check these locations:
- `build/VST3/Release/Tarantism.vst3`
- `build/VST3/Tarantism.vst3`
- `build/Release/Tarantism.vst3`

On Linux, the file might be in:
- `build/VST3/Tarantism.so`

---

## Installation Issues

### Problem: "Plugin not showing in DAW"

**Symptoms:**
Installed plugin but DAW doesn't detect it

**Solutions:**

1. **Verify installation location:**
   - Windows: `C:\Program Files\Common Files\VST3\`
   - macOS: `~/Library/Audio/Plug-Ins/VST3/` or `/Library/Audio/Plug-Ins/VST3/`
   - Linux: `~/.vst3/` or `/usr/lib/vst3/`

2. **Copy the entire .vst3 bundle/folder:**
   - Don't copy individual files
   - Copy the whole `Tarantism.vst3` directory

3. **Rescan plugins in DAW:**
   - Most DAWs: Preferences → Plugins → Rescan
   - Some require restart

4. **Check DAW's VST3 search paths:**
   - Verify the installation location is in DAW's search paths
   - Add path manually if needed

---

### Problem: "Plugin crashes on load" (macOS)

**Symptoms:**
```
"Tarantism.vst3" is damaged and can't be opened
```

**Solution:**
Remove quarantine attribute:
```bash
xattr -cr ~/Library/Audio/Plug-Ins/VST3/Tarantism.vst3
```

Or allow in System Preferences:
1. System Preferences → Security & Privacy
2. Click "Open Anyway" for Tarantism.vst3

---

### Problem: "Permission denied" (Linux)

**Symptoms:**
```
cp: cannot create directory: Permission denied
```

**Solution:**
```bash
# Install to user directory (no sudo needed)
mkdir -p ~/.vst3
cp -r build/VST3/Release/Tarantism.vst3 ~/.vst3/

# OR install system-wide (requires sudo)
sudo mkdir -p /usr/lib/vst3
sudo cp -r build/VST3/Release/Tarantism.vst3 /usr/lib/vst3/
```

---

## Runtime Issues

### Problem: "MIDI not passing through"

**Symptoms:**
Notes going in, but no sound coming out

**Solutions:**

1. **Check MIDI routing:**
   - Ensure plugin input receives MIDI
   - Ensure plugin output routes to instrument
   - Test with MIDI monitor in DAW

2. **Check bypass state:**
   - Bypass parameter should be OFF
   - Look for bypass button in GUI or DAW

3. **Verify note collection:**
   - Plugin waits for N notes before output
   - Example: 8th notes = 8 notes needed
   - Play enough notes to trigger output

4. **Check octave range:**
   - Very high/low octaves may be outside instrument range
   - Try octave 3-5 for testing

---

### Problem: "GUI not displaying"

**Symptoms:**
Plugin loads but GUI window is blank or crashes

**Solutions:**

1. **Check VSTGUI resources:**
   - Ensure `plugin.uidesc` is bundled with plugin
   - Rebuild with `cmake --build build --config Release`

2. **Graphics driver update:**
   - Update graphics drivers to latest version
   - Some older GPUs may have issues

3. **Try different DAW:**
   - Test in another DAW to isolate issue
   - Some DAWs have better VST3 GUI support

---

### Problem: "Parameters not responding"

**Symptoms:**
Knobs move but don't affect sound

**Solutions:**

1. **Restart plugin:**
   - Remove and re-add plugin in DAW
   - Some DAWs cache parameter states

2. **Check automation:**
   - Disable parameter automation if enabled
   - Automation can override manual changes

3. **Verify MIDI input:**
   - Ensure MIDI is actually reaching plugin
   - Use MIDI monitor to confirm

---

### Problem: "Notes all outputting at once"

**Symptoms:**
All collected notes play simultaneously instead of in sequence

**Explanation:**
This is the intended behavior! The plugin:
1. Collects N notes
2. Outputs them all together
3. Resets and waits for next N notes

**If you want sequential output:**
This plugin may not suit your needs. Consider:
- Using bypass mode
- Different MIDI effect
- Smaller measure length for faster output

---

### Problem: "Unexpected number of notes required"

**Symptoms:**
Plugin waits for different number of notes than expected

**Solution:**
Remember the formula: `Notes = Measure Length × Note Duration`

Examples:
- 1 measure × 8 (eighth) = 8 notes
- 2 measures × 4 (quarter) = 8 notes
- 1 measure × 16 (sixteenth) = 16 notes

Check both parameters are set correctly!

---

### Problem: "Plugin causes DAW to crash"

**Symptoms:**
DAW crashes when loading or using plugin

**Solutions:**

1. **Check DAW logs:**
   - Look for error messages
   - Identify crash location

2. **Test in isolation:**
   - Create new project
   - Load only Tarantism
   - Minimal MIDI input

3. **Update DAW:**
   - Ensure DAW is latest version
   - Check DAW's VST3 compatibility

4. **Rebuild plugin:**
   - Clean build: `./build.sh clean` or `build.bat clean`
   - Rebuild from scratch

5. **Report issue:**
   - If crash persists, report on GitHub
   - Include DAW version, OS, crash logs

---

## Development Issues

### Problem: "Modifications not reflected"

**Symptoms:**
Changed code but plugin behaves the same

**Solution:**
1. Clean build: `rm -rf build`
2. Rebuild: `./build.sh` or `build.bat`
3. Copy new .vst3 to plugin folder
4. Restart DAW (some cache plugins)

---

### Problem: "Debugging not working"

**Symptoms:**
Can't attach debugger to plugin

**Solution:**

**Visual Studio (Windows):**
1. Build Debug configuration
2. Debug → Attach to Process
3. Find DAW process
4. Set breakpoints in code

**Xcode (macOS):**
1. Product → Scheme → Edit Scheme
2. Run → Executable → Choose DAW
3. Build and Run (launches DAW)

**GDB (Linux):**
```bash
gdb /path/to/daw
run
# Load plugin in DAW
# Plugin will break at issues
```

---

## Performance Issues

### Problem: "High CPU usage"

**Symptoms:**
Plugin uses significant CPU

**Analysis:**
This shouldn't happen - plugin is very lightweight!

**Solutions:**
1. Check for MIDI feedback loops
2. Disable plugin to confirm it's the cause
3. Monitor MIDI event rate (excessive events?)
4. Report issue if plugin is definitely the cause

---

### Problem: "GUI updates slowly"

**Symptoms:**
Knobs lag when turning, visual updates delayed

**Solutions:**
1. This is usually DAW-related, not plugin
2. Increase DAW's GUI refresh rate if available
3. Close other plugins to free CPU
4. Update graphics drivers

---

## Getting Help

### Before Asking for Help

Please provide:
1. **Operating System**: Windows/macOS/Linux + version
2. **DAW**: Name and version
3. **Plugin Version**: From About/Info
4. **Issue Description**: What you expected vs. what happened
5. **Steps to Reproduce**: Exact steps to trigger issue
6. **Logs/Screenshots**: Any error messages or screenshots

### Where to Get Help

1. **GitHub Issues**: https://github.com/Gnar1337/tarantism_VST/issues
2. **Check existing issues first**: Your problem might already be solved!
3. **Be specific**: More details = faster solution

### Useful Diagnostic Commands

**Check plugin file:**
```bash
# macOS/Linux
file Tarantism.vst3/Contents/MacOS/Tarantism
ls -lR Tarantism.vst3/

# Windows (PowerShell)
Get-ChildItem -Recurse Tarantism.vst3
```

**Verify VST3 SDK version:**
```bash
cd vst3sdk
git log -1 --oneline
```

**Check build configuration:**
```bash
cat build/CMakeCache.txt | grep VST3
```

---

## Known Limitations

These are by design, not bugs:

1. **4/4 Time Only**: Plugin assumes 4/4 time signature
2. **Complete Sets**: Won't output partial note collections
3. **No Note Hold**: Doesn't track note-off events
4. **Timing Preserved**: Original note timing maintained
5. **Octave Only**: No other transformations (velocity, etc.)

---

## Common Misunderstandings

### "Nothing happens when I play notes"

Remember: Plugin waits for N notes! 
- Check measure length and duration
- Play enough notes to trigger
- Default: 8 notes for 1 measure of 8th notes

### "Notes are in wrong octave"

Check octave parameter:
- Octave 0-2: Low bass range
- Octave 3-5: Middle range (most instruments)
- Octave 6-10: High range

### "Can't hear any sound"

Plugin is MIDI-only:
- Outputs MIDI, not audio
- Needs instrument/synth after plugin
- Check MIDI routing to instrument

---

## Quick Fixes

### Reset Everything

**Option 1: Remove and re-add plugin in DAW**

**Option 2: Reset parameters to default**
- Measure Length: 1
- Note Duration: 8th notes (0.4)
- Octave: 3
- Bypass: Off

**Option 3: Clean reinstall**
```bash
# Remove old plugin
rm -rf /path/to/VST3/Tarantism.vst3

# Rebuild
./build.sh clean
./build.sh

# Reinstall
cp -r build/VST3/Release/Tarantism.vst3 /path/to/VST3/

# Restart DAW
```

---

## Still Having Issues?

Open an issue on GitHub with:
- Full description of problem
- Your system info
- Build/install steps you followed
- What you've tried so far
- Screenshots or error logs

We'll help you get it working!
