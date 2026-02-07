#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginids.h"
#include <vector>

namespace Steinberg {
namespace Tarantism {

//------------------------------------------------------------------------
// TarantismProcessor
//------------------------------------------------------------------------
class TarantismProcessor : public Vst::AudioEffect
{
public:
    TarantismProcessor();
    ~TarantismProcessor() SMTG_OVERRIDE;

    // Create function
    static FUnknown* createInstance(void* /*context*/) 
    { 
        return (Vst::IAudioProcessor*)new TarantismProcessor; 
    }

    // AudioEffect overrides
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
    tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

protected:
    // MIDI note structure
    struct MidiNote 
    {
        int32 noteNumber;
        int32 velocity;
        int32 sampleOffset;
    };

    // Process incoming MIDI events
    void processMidiInput(Vst::IEventList* events);
    
    // Process collected notes and output them
    void processCollectedNotes(Vst::IEventList* outputEvents, int32 numSamples);
    
    // Calculate number of notes needed based on measure and duration
    int32 calculateNotesNeeded();
    
    // Transpose note to target octave
    int32 transposeToOctave(int32 noteNumber, int32 targetOctave);

private:
    // Parameters
    double measureLength = 1.0;  // 1 = whole note, 2 = half, 4 = quarter, etc.
    double noteDuration = 8.0;   // 1 = whole, 2 = half, 4 = quarter, 8 = eighth, etc.
    int32 targetOctave = 3;      // Target octave (0-10)
    bool bypass = false;
    
    // Note collection buffer
    std::vector<MidiNote> collectedNotes;
    int32 notesNeeded = 0;
    bool waitingForNotes = true;
};

//------------------------------------------------------------------------
} // namespace Tarantism
} // namespace Steinberg
