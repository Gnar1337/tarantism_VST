# Tarantism VST3 - Feature Overview

## Core Concept

Tarantism is a MIDI transformation plugin that collects incoming MIDI notes and outputs them transposed to a specific octave. It's like a musical "note collector" that waits for a certain number of notes before processing them all at once.

## Key Features

### 1. Smart Note Collection

The plugin calculates how many notes it needs to collect based on two parameters:

- **Measure Length** (1-4 measures)
- **Note Duration** (whole, half, quarter, 8th, 16th notes)

**Formula**: `Notes to collect = Measure Length × Note Duration`

**Examples**:
```
1 measure × 8 (eighth notes) = 8 notes
2 measures × 4 (quarter notes) = 8 notes
1 measure × 16 (sixteenth notes) = 16 notes
4 measures × 2 (half notes) = 8 notes
```

### 2. Octave Transposition

Once the required number of notes is collected, ALL notes are transposed to the target octave:

- **Input**: Any MIDI notes (any octave)
- **Process**: Preserve the note within octave (C, C#, D, etc.)
- **Output**: Same notes, but all in the target octave

**Example**:
```
Input:  C3, E4, G5, B2  (scattered across octaves)
Target Octave: 3
Output: C3, E3, G3, B3  (all in octave 3)
```

### 3. Visual Keyboard Display

The plugin features a 37-key virtual keyboard (3 octaves) showing:
- White keys (C, D, E, F, G, A, B)
- Black keys (C#, D#, F#, G#, A#)
- Visual reference for octave ranges

### 4. Parameter Controls

#### Measure Length Knob
- **Range**: 1-4 measures
- **Use**: Determines how many measures worth of notes to collect
- **Default**: 1 measure

#### Note Duration Knob
- **Options**: 
  - Whole note (1)
  - Half note (2)
  - Quarter note (4)
  - Eighth note (8)
  - Sixteenth note (16)
- **Use**: Sets the rhythmic division for note counting
- **Default**: Eighth notes

#### Octave Knob
- **Range**: 0-10 (MIDI octaves)
- **Use**: Target octave for transposition
- **Default**: Octave 3 (middle range)

### 5. Real-time Processing

- **Immediate Response**: Parameters update in real-time
- **No Latency**: Direct MIDI processing
- **Automation Ready**: All parameters can be automated in your DAW

## Use Cases

### Creative Composition

**Bass Line Generator**
- Set octave to 0 or 1 (low)
- Play melody in comfortable range
- Output as deep bass notes

**Harmony Creation**
- Set measure length to match your progression
- Play notes in any octave
- Output creates consistent octave harmony

**Melodic Variation**
- Change octave mid-performance
- Create octave jumps automatically
- Transform existing melodies to new ranges

### Live Performance

**Octave Switching**
- Quickly jump between octave ranges
- No need to move hands on keyboard
- Smooth transitions between sections

**Pattern Transformation**
- Play same pattern, different octaves
- Create call-and-response effects
- Build layered arrangements

### Music Production

**MIDI Editing Alternative**
- Faster than manual octave edits
- Consistent results across sections
- Non-destructive processing (bypass available)

**Layering Tool**
- Create octave doubles easily
- Build thick arrangements
- Experimental sound design

## Technical Specifications

### MIDI Processing
- **Input**: MIDI Note On events
- **Processing**: Note collection and octave transposition
- **Output**: Modified MIDI Note On events
- **Latency**: Zero samples (direct processing)

### Octave Calculation
```
Original Note: MIDI note number (0-127)
Note in Octave: Note % 12
Target Octave: User selected (0-10)
Output Note: (Target Octave + 1) × 12 + Note in Octave
Clamped: 0-127 (valid MIDI range)
```

### Note Duration Values
```
In 4/4 time:
- Whole note: 4 beats (1 per measure)
- Half note: 2 beats (2 per measure)
- Quarter note: 1 beat (4 per measure)
- Eighth note: 0.5 beats (8 per measure)
- Sixteenth note: 0.25 beats (16 per measure)
```

### Parameter Ranges
```
Measure Length: 1.0 - 4.0 (normalized: 0.0 - 1.0)
Note Duration: 1, 2, 4, 8, 16 (normalized: 0.0 - 1.0 in 5 steps)
Octave: 0 - 10 (normalized: 0.0 - 1.0)
Bypass: Off/On (normalized: 0.0 / 1.0)
```

## Plugin Architecture

### Processor (MIDI Engine)
- Receives MIDI input events
- Collects notes in buffer
- Calculates required note count
- Transposes to target octave
- Outputs transformed MIDI

### Controller (GUI)
- Displays 37-key keyboard
- Provides parameter controls
- Updates visual feedback
- Manages state saving/loading

### Communication Flow
```
[MIDI Input] 
    ↓
[Processor: Collect Notes]
    ↓
[Count == Target?]
    ↓ (Yes)
[Transpose to Octave]
    ↓
[MIDI Output]
    ↓
[Instrument/Synth]
```

## Workflow Integration

### DAW Setup
1. Insert Tarantism on a MIDI track
2. Route MIDI from input or clip
3. Route Tarantism output to instrument
4. Configure parameters
5. Record or play

### Parameter Automation
All parameters support DAW automation:
- Create evolving octave changes
- Automate measure length for variations
- Dynamic note duration switching

### Preset Management
- State saved with DAW project
- All parameter values preserved
- Quick recall of favorite settings

## Tips and Tricks

### Tip 1: Consistent Layering
Set the same octave value across multiple instances for consistent doubling.

### Tip 2: Rhythmic Fills
Use short measure lengths (1 measure, 16th notes) for quick fill patterns.

### Tip 3: Build Progression
Automate octave changes to create rising or falling progressions.

### Tip 4: Experimental Sound Design
Combine with other MIDI effects for unique transformations.

### Tip 5: Live Loop Recording
Record into a looper while changing octaves for textured loops.

## Limitations

- Assumes 4/4 time signature
- Collects complete sets of notes (no partial outputs)
- Octave transposition only (no other transformations)
- No velocity modification (preserved from input)
- No timing modification (preserved from input)

## Future Enhancement Ideas

- Variable time signatures
- Velocity scaling options
- Note timing quantization
- Multiple octave output (create chords)
- Note range limiting
- Swing/groove templates
- MIDI CC modulation mapping
