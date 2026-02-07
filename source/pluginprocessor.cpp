#include "pluginprocessor.h"
#include "pluginids.h"
#include "plugincontroller.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#include <algorithm>
#include <cmath>

namespace Steinberg {
namespace Tarantism {

//------------------------------------------------------------------------
// TarantismProcessor
//------------------------------------------------------------------------
TarantismProcessor::TarantismProcessor()
{
    setControllerClass(kTarantismControllerUID);
}

//------------------------------------------------------------------------
TarantismProcessor::~TarantismProcessor()
{
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismProcessor::initialize(FUnknown* context)
{
    tresult result = AudioEffect::initialize(context);
    if (result != kResultOk)
        return result;

    // Add MIDI input and output
    addEventInput(STR16("MIDI In"));
    addEventOutput(STR16("MIDI Out"));

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismProcessor::terminate()
{
    return AudioEffect::terminate();
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismProcessor::setActive(TBool state)
{
    if (state)
    {
        // Reset when activated
        collectedNotes.clear();
        notesNeeded = calculateNotesNeeded();
        waitingForNotes = true;
    }
    return AudioEffect::setActive(state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismProcessor::process(Vst::ProcessData& data)
{
    // Read parameter changes
    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
        for (int32 i = 0; i < numParamsChanged; i++)
        {
            Vst::IParamValueQueue* paramQueue = 
                data.inputParameterChanges->getParameterData(i);
            if (paramQueue)
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount();
                
                if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                {
                    switch (paramQueue->getParameterId())
                    {
                        case kMeasureLengthParam:
                            measureLength = 1.0 + (value * 3.0); // 1-4 measures
                            notesNeeded = calculateNotesNeeded();
                            break;
                        case kNoteDurationParam:
                            // Map to standard note durations: whole, half, quarter, 8th, 16th
                            {
                                int durIndex = (int)(value * 4.999);
                                double durations[] = {1.0, 2.0, 4.0, 8.0, 16.0};
                                noteDuration = durations[durIndex];
                                notesNeeded = calculateNotesNeeded();
                            }
                            break;
                        case kOctaveParam:
                            targetOctave = (int32)(value * 10.0); // 0-10 octave range
                            break;
                        case kBypassParam:
                            bypass = (value > 0.5);
                            break;
                    }
                }
            }
        }
    }

    // Process MIDI events
    if (data.inputEvents)
    {
        processMidiInput(data.inputEvents);
    }

    // Output collected notes when we have enough
    if (data.outputEvents && !bypass)
    {
        processCollectedNotes(data.outputEvents, data.numSamples);
    }

    return kResultOk;
}

//------------------------------------------------------------------------
void TarantismProcessor::processMidiInput(Vst::IEventList* events)
{
    if (!events || bypass)
        return;

    int32 numEvents = events->getEventCount();
    for (int32 i = 0; i < numEvents; i++)
    {
        Vst::Event event;
        if (events->getEvent(i, event) == kResultOk)
        {
            if (event.type == Vst::Event::kNoteOnEvent)
            {
                // Collect note if we're waiting for more
                if (waitingForNotes && collectedNotes.size() < (size_t)notesNeeded)
                {
                    MidiNote note;
                    note.noteNumber = event.noteOn.pitch;
                    note.velocity = (int32)(event.noteOn.velocity * 127.0f);
                    note.sampleOffset = event.sampleOffset;
                    collectedNotes.push_back(note);
                    
                    // Check if we have all notes
                    if (collectedNotes.size() >= (size_t)notesNeeded)
                    {
                        waitingForNotes = false;
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------
void TarantismProcessor::processCollectedNotes(Vst::IEventList* outputEvents, int32 numSamples)
{
    if (waitingForNotes || collectedNotes.empty())
        return;

    // Output all collected notes, transposed to target octave
    for (size_t i = 0; i < collectedNotes.size(); i++)
    {
        MidiNote& note = collectedNotes[i];
        
        // Create note on event
        Vst::Event noteOn;
        memset(&noteOn, 0, sizeof(Vst::Event));
        noteOn.type = Vst::Event::kNoteOnEvent;
        noteOn.sampleOffset = std::min(note.sampleOffset, numSamples - 1);
        noteOn.noteOn.channel = 0;
        noteOn.noteOn.pitch = transposeToOctave(note.noteNumber, targetOctave);
        noteOn.noteOn.velocity = note.velocity / 127.0f;
        noteOn.noteOn.length = 0;
        noteOn.noteOn.tuning = 0.0f;
        noteOn.noteOn.noteId = -1;
        
        outputEvents->addEvent(noteOn);
    }
    
    // Clear collected notes and start waiting again
    collectedNotes.clear();
    waitingForNotes = true;
}

//------------------------------------------------------------------------
int32 TarantismProcessor::calculateNotesNeeded()
{
    // Calculate how many notes fit in the measure based on duration
    // For example: 1 measure of 8th notes = 8 notes
    // 2 measures of quarter notes = 8 notes
    double notesPerMeasure = 4.0 * (noteDuration / 4.0); // Assuming 4/4 time
    int32 totalNotes = (int32)(measureLength * notesPerMeasure);
    return std::max(1, totalNotes);
}

//------------------------------------------------------------------------
int32 TarantismProcessor::transposeToOctave(int32 noteNumber, int32 targetOctave)
{
    // Get the note within octave (0-11, C=0)
    int32 noteInOctave = noteNumber % 12;
    
    // Calculate new MIDI note number at target octave
    // MIDI octaves: C-1 = 0, C0 = 12, C1 = 24, etc.
    // So octave N starts at (N+1)*12
    int32 newNote = (targetOctave + 1) * 12 + noteInOctave;
    
    // Clamp to valid MIDI range (0-127)
    return std::max(0, std::min(127, newNote));
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismProcessor::setState(IBStream* state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);
    
    double savedMeasure = 0.0;
    if (streamer.readDouble(savedMeasure) == false)
        return kResultFalse;
    measureLength = savedMeasure;
    
    double savedDuration = 0.0;
    if (streamer.readDouble(savedDuration) == false)
        return kResultFalse;
    noteDuration = savedDuration;
    
    int32 savedOctave = 0;
    if (streamer.readInt32(savedOctave) == false)
        return kResultFalse;
    targetOctave = savedOctave;
    
    int32 savedBypass = 0;
    if (streamer.readInt32(savedBypass) == false)
        return kResultFalse;
    bypass = savedBypass > 0;
    
    notesNeeded = calculateNotesNeeded();
    
    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismProcessor::getState(IBStream* state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);
    
    streamer.writeDouble(measureLength);
    streamer.writeDouble(noteDuration);
    streamer.writeInt32(targetOctave);
    streamer.writeInt32(bypass ? 1 : 0);
    
    return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Tarantism
} // namespace Steinberg
