# Tarantism VST3 - Architecture & Workflow

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    DAW (Host Application)                    │
│                                                               │
│  ┌────────────────────────────────────────────────────────┐ │
│  │              Tarantism VST3 Plugin                      │ │
│  │                                                          │ │
│  │  ┌─────────────────┐        ┌──────────────────┐      │ │
│  │  │   Processor     │        │   Controller     │      │ │
│  │  │  (MIDI Engine)  │◄──────►│   (GUI/Params)   │      │ │
│  │  │                 │        │                  │      │ │
│  │  │  • Collect      │        │  • 37-Key        │      │ │
│  │  │  • Calculate    │        │    Keyboard      │      │ │
│  │  │  • Transpose    │        │  • Parameter     │      │ │
│  │  │  • Output       │        │    Knobs         │      │ │
│  │  └─────────────────┘        └──────────────────┘      │ │
│  │          ▲                           │                  │ │
│  │          │                           │                  │ │
│  │    MIDI Events              Parameter Changes          │ │
│  └──────────┼───────────────────────────┼─────────────────┘ │
│             │                           │                    │
└─────────────┼───────────────────────────┼────────────────────┘
              │                           │
         MIDI Track               User Interaction
```

## MIDI Processing Flow

```
     MIDI Input (Note On Events)
              ▼
    ┌─────────────────┐
    │  Input Buffer   │
    │  [Wait for N    │
    │   notes...]     │
    └─────────────────┘
              ▼
    ┌─────────────────┐
    │ Check Complete? │◄─── Measure Length × Note Duration
    └─────────────────┘
         YES │  NO (wait)
             ▼
    ┌─────────────────┐
    │   Transpose     │◄─── Target Octave Parameter
    │   to Octave     │
    └─────────────────┘
              ▼
    ┌─────────────────┐
    │  Output MIDI    │
    │   Events        │
    └─────────────────┘
              ▼
         Synthesizer/
         Instrument
```

## Note Collection Example

### Scenario: 1 Measure, 8th Notes, Octave 3

```
Time: ─────────────────────────────────────────────►

Input Notes:
  t0    t1    t2    t3    t4    t5    t6    t7
  C5    D3    E6    F2    G4    A5    B3    C4
  │     │     │     │     │     │     │     │
  └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
                    ▼
             [Collecting: 8/8 notes]
                    ▼
              [Transpose to Octave 3]
                    ▼
Output Notes (all at once):
  t0    t1    t2    t3    t4    t5    t6    t7
  C3    D3    E3    F3    G3    A3    B3    C3
```

## Parameter Mapping

### Measure Length (1-4 measures)
```
Normalized Value:    0.0      0.33     0.67     1.0
                      │         │        │        │
Actual Measures:      1         2        3        4
```

### Note Duration
```
Normalized:     0.0    0.25    0.5     0.75    1.0
                 │       │       │       │       │
Duration:      Whole   Half  Quarter  8th    16th
Value:           1       2       4       8      16
Notes/Bar:       1       2       4       8      16
```

### Octave (0-10)
```
Normalized:  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0
              │    │    │    │    │    │    │    │    │    │    │
MIDI Octave:  0    1    2    3    4    5    6    7    8    9   10
Note Range:  C-1  C0   C1   C2   C3   C4   C5   C6   C7   C8   C9
             to   to   to   to   to   to   to   to   to   to   to
             B-1  B0   B1   B2   B3   B4   B5   B6   B7   B8   G10
```

## Octave Transposition Logic

### Input Note Decomposition
```
MIDI Note Number: 67 (G4)
        ▼
   ┌─────────┐
   │  Octave │ = Note ÷ 12 = 67 ÷ 12 = 5
   └─────────┘
   ┌─────────┐
   │  Pitch  │ = Note % 12 = 67 % 12 = 7 (G)
   └─────────┘
```

### Output Note Construction
```
Target Octave: 3
Pitch Class: 7 (G)
        ▼
   ┌─────────────────────┐
   │  New MIDI Note =    │
   │  (Octave + 1) × 12  │
   │  + Pitch            │
   │                     │
   │  = (3 + 1) × 12 + 7 │
   │  = 48 + 7           │
   │  = 55 (G3)          │
   └─────────────────────┘
```

## GUI Layout

```
┌──────────────────────────────────────────────────────────┐
│  Tarantism - MIDI Manipulation VST                       │
├──────────────────────────────────────────────────────────┤
│                                                           │
│  ┌───────────────────────────────────────────────────┐  │
│  │  37-Key Keyboard Display                          │  │
│  │  [W][B][W][B][W][W][B][W][B][W][B][W]...         │  │
│  │  White and Black keys showing 3 octaves           │  │
│  └───────────────────────────────────────────────────┘  │
│                                                           │
│  Measure Length:    Note Duration:     Octave:          │
│      ┌───┐             ┌───┐            ┌───┐          │
│      │ ◉ │             │ ◉ │            │ ◉ │          │
│      └───┘             └───┘            └───┘          │
│      [1-4]             [♩ ♪ ♫]          [0-10]         │
│                                                           │
│  Play MIDI notes to collect them.                        │
│  They will be transposed to the selected octave.         │
│  Number of notes = Measure × Duration                    │
└──────────────────────────────────────────────────────────┘
```

## State Management

### Processor State
```
┌────────────────────────┐
│ Processor State        │
├────────────────────────┤
│ • measureLength: 1.0   │
│ • noteDuration: 8.0    │
│ • targetOctave: 3      │
│ • bypass: false        │
│ • collectedNotes: []   │
│ • notesNeeded: 8       │
└────────────────────────┘
         │
         │ Save/Load
         ▼
┌────────────────────────┐
│ IBStream (Binary)      │
├────────────────────────┤
│ measureLength (double) │
│ noteDuration (double)  │
│ targetOctave (int32)   │
│ bypass (int32)         │
└────────────────────────┘
```

## Plugin Lifecycle

```
┌─────────────┐
│   DAW       │
│  Startup    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Initialize  │ ← Create processor & controller
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Set Active  │ ← Reset buffers, prepare for audio
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Process    │ ◄─┐
│   Loop      │   │ Continuous MIDI processing
└──────┬──────┘   │
       │          │
       └──────────┘
       │ (Stop)
       ▼
┌─────────────┐
│ Set Inactive│ ← Clean up
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Terminate  │ ← Destroy plugin
└─────────────┘
```

## File Structure

```
tarantism_VST/
│
├── CMakeLists.txt              # Build configuration
├── README.md                   # Main documentation
├── QUICKSTART.md              # Quick setup guide
├── FEATURES.md                 # Feature descriptions
├── ARCHITECTURE.md             # This file
├── build.sh                    # Unix build script
├── build.bat                   # Windows build script
│
├── source/                     # C++ source code
│   ├── factory.cpp            # Plugin entry point
│   ├── pluginprocessor.h      # Processor interface
│   ├── pluginprocessor.cpp    # MIDI processing logic
│   ├── plugincontroller.h     # Controller interface
│   ├── plugincontroller.cpp   # GUI implementation
│   ├── pluginids.h            # Unique identifiers
│   └── version.h              # Version information
│
└── resource/                   # Resources
    ├── plugin.uidesc          # GUI layout (XML)
    └── plugin.rc              # Windows resources
```

## Build Process

```
┌──────────────┐
│  CMake       │
│  Configure   │
└──────┬───────┘
       │
       ▼
┌──────────────┐      ┌─────────────┐
│  Compile     │ ──► │   source/   │
│  C++ Files   │      │   *.cpp     │
└──────┬───────┘      └─────────────┘
       │
       ▼
┌──────────────┐      ┌─────────────┐
│    Link      │ ──► │  VST3 SDK   │
│  with SDK    │      │  Libraries  │
└──────┬───────┘      └─────────────┘
       │
       ▼
┌──────────────┐      ┌─────────────┐
│   Bundle     │ ──► │  Resources  │
│  Resources   │      │  .uidesc    │
└──────┬───────┘      └─────────────┘
       │
       ▼
┌──────────────┐
│ Tarantism    │
│   .vst3      │
│  (Plugin)    │
└──────────────┘
```

## Performance Characteristics

### CPU Usage
- **Idle**: Minimal (parameter monitoring only)
- **Active**: Low (simple calculations per MIDI event)
- **Peak**: Negligible (no audio processing)

### Memory Usage
- **Base**: ~1-2 MB (plugin code + GUI)
- **Per Note**: ~12 bytes (MidiNote struct)
- **Maximum**: Depends on note collection buffer size

### Latency
- **Processing**: 0 samples (direct MIDI)
- **Buffer**: None (event-based)
- **Total**: Host-dependent only

## Threading Model

```
┌──────────────┐
│  Audio Thread│  ← Processor::process()
│  (Real-time) │     • MIDI processing
└──────┬───────┘     • Note collection
       │              • Transposition
       │
       ▼
┌──────────────┐
│  UI Thread   │  ← Controller GUI
│  (Non-RT)    │     • Parameter changes
└──────────────┘     • Visual updates
                      • User interaction
```

## Error Handling

```
Parameter Changes:
  → Validate range (clamp 0.0-1.0)
  → Update internal values
  → Recalculate derived values

MIDI Processing:
  → Check event type
  → Validate note number (0-127)
  → Buffer size management
  → Output validation

State Load/Save:
  → Check stream validity
  → Validate data types
  → Return success/failure
```

## Extension Points

### Future Enhancements

1. **Multiple Output Modes**
   - Current: All notes same octave
   - Future: Spread across octaves, harmonize, etc.

2. **Timing Options**
   - Current: Preserve input timing
   - Future: Quantize, delay, arpeggiate

3. **Velocity Modification**
   - Current: Preserve velocity
   - Future: Scale, normalize, randomize

4. **Pattern Modes**
   - Current: Simple collection
   - Future: Sequences, chord detection

5. **MIDI CC Control**
   - Current: Basic MIDI CC assignment
   - Future: Learn mode, more mappings
