#include "plugincontroller.h"
#include "pluginids.h"
#include "pluginprocessor.h"
#include "version.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"

#include "vstgui/plugin-bindings/vst3editor.h"
#include "vstgui/vstgui.h"
#include "vstgui/uidescription/uiattributes.h"

namespace Steinberg {
namespace Tarantism {

//------------------------------------------------------------------------
// Custom Keyboard View
//------------------------------------------------------------------------
class KeyboardView : public VSTGUI::CView
{
public:
    KeyboardView(const VSTGUI::CRect& size) : VSTGUI::CView(size)
    {
        setMouseEnabled(true);
    }
    
    void draw(VSTGUI::CDrawContext* context) SMTG_OVERRIDE
    {
        // Draw 37 keys (3 octaves + 1 key, starting from C)
        // White keys: C, D, E, F, G, A, B (7 per octave)
        // Black keys: C#, D#, F#, G#, A# (5 per octave)
        
        VSTGUI::CRect viewSize = getViewSize();
        double whiteKeyWidth = viewSize.getWidth() / 22.0; // 22 white keys in 37 keys
        double whiteKeyHeight = viewSize.getHeight();
        double blackKeyWidth = whiteKeyWidth * 0.6;
        double blackKeyHeight = whiteKeyHeight * 0.6;
        
        // Pattern for 12 notes: W=white, B=black, N=no black key after
        // C, C#, D, D#, E, F, F#, G, G#, A, A#, B
        // W  B   W  B   W  W  B   W  B   W  B   W
        const bool hasBlackKey[] = {true, false, true, false, false, true, false, true, false, true, false, false};
        
        // Draw white keys first
        context->setFillColor(VSTGUI::CColor(255, 255, 255));
        context->setFrameColor(VSTGUI::CColor(0, 0, 0));
        context->setLineWidth(1.0);
        
        int whiteKeyIndex = 0;
        for (int i = 0; i < 37; i++)
        {
            int noteInOctave = i % 12;
            // White keys: 0(C), 2(D), 4(E), 5(F), 7(G), 9(A), 11(B)
            bool isWhiteKey = (noteInOctave == 0 || noteInOctave == 2 || noteInOctave == 4 || 
                             noteInOctave == 5 || noteInOctave == 7 || noteInOctave == 9 || noteInOctave == 11);
            
            if (isWhiteKey)
            {
                VSTGUI::CRect keyRect(
                    viewSize.left + whiteKeyIndex * whiteKeyWidth,
                    viewSize.top,
                    viewSize.left + (whiteKeyIndex + 1) * whiteKeyWidth,
                    viewSize.top + whiteKeyHeight
                );
                context->drawRect(keyRect, VSTGUI::kDrawFilledAndStroked);
                whiteKeyIndex++;
            }
        }
        
        // Draw black keys on top
        context->setFillColor(VSTGUI::CColor(0, 0, 0));
        whiteKeyIndex = 0;
        for (int i = 0; i < 37; i++)
        {
            int noteInOctave = i % 12;
            bool isWhiteKey = (noteInOctave == 0 || noteInOctave == 2 || noteInOctave == 4 || 
                             noteInOctave == 5 || noteInOctave == 7 || noteInOctave == 9 || noteInOctave == 11);
            
            if (isWhiteKey)
            {
                // Check if there's a black key after this white key
                if (i < 36 && hasBlackKey[noteInOctave])
                {
                    VSTGUI::CRect blackKeyRect(
                        viewSize.left + whiteKeyIndex * whiteKeyWidth + whiteKeyWidth * 0.7,
                        viewSize.top,
                        viewSize.left + whiteKeyIndex * whiteKeyWidth + whiteKeyWidth * 0.7 + blackKeyWidth,
                        viewSize.top + blackKeyHeight
                    );
                    context->drawRect(blackKeyRect, VSTGUI::kDrawFilledAndStroked);
                }
                whiteKeyIndex++;
            }
        }
    }
    
    CLASS_METHODS(KeyboardView, VSTGUI::CView)
};

//------------------------------------------------------------------------
// TarantismController
//------------------------------------------------------------------------
tresult PLUGIN_API TarantismController::initialize(FUnknown* context)
{
    tresult result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    // Add parameters
    parameters.addParameter(STR16("Measure Length"), STR16("bars"), 0, 0.0,
        Vst::ParameterInfo::kCanAutomate, kMeasureLengthParam, 0,
        STR16("Measure"));
    
    parameters.addParameter(STR16("Note Duration"), STR16(""), 0, 0.4,
        Vst::ParameterInfo::kCanAutomate, kNoteDurationParam, 0,
        STR16("Duration"));
    
    parameters.addParameter(STR16("Octave"), STR16(""), 0, 0.3,
        Vst::ParameterInfo::kCanAutomate, kOctaveParam, 0,
        STR16("Octave"));
    
    parameters.addParameter(STR16("Bypass"), STR16(""), 0, 0.0,
        Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass, 
        kBypassParam, 0, STR16("Bypass"));

    return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismController::terminate()
{
    return EditControllerEx1::terminate();
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismController::setComponentState(IBStream* state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);
    
    double savedMeasure = 0.0;
    if (streamer.readDouble(savedMeasure) == false)
        return kResultFalse;
    setParamNormalized(kMeasureLengthParam, (savedMeasure - 1.0) / 3.0);
    
    double savedDuration = 0.0;
    if (streamer.readDouble(savedDuration) == false)
        return kResultFalse;
    // Map duration back to normalized value
    double durations[] = {1.0, 2.0, 4.0, 8.0, 16.0};
    for (int i = 0; i < 5; i++)
    {
        if (std::abs(savedDuration - durations[i]) < 0.001)
        {
            setParamNormalized(kNoteDurationParam, i / 4.0);
            break;
        }
    }
    
    int32 savedOctave = 0;
    if (streamer.readInt32(savedOctave) == false)
        return kResultFalse;
    setParamNormalized(kOctaveParam, savedOctave / 10.0);
    
    int32 savedBypass = 0;
    if (streamer.readInt32(savedBypass) == false)
        return kResultFalse;
    setParamNormalized(kBypassParam, savedBypass > 0 ? 1.0 : 0.0);

    return kResultOk;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API TarantismController::createView(FIDString name)
{
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
    {
        // Create the editor with UI description
        return new VSTGUI::VST3Editor(this, "view", "plugin.uidesc");
    }
    return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismController::setState(IBStream* state)
{
    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismController::getState(IBStream* state)
{
    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TarantismController::getMidiControllerAssignment(
    int32 busIndex, int16 /*channel*/,
    Vst::CtrlNumber midiControllerNumber, Vst::ParamID& id)
{
    // Example MIDI CC assignments
    if (busIndex == 0)
    {
        switch (midiControllerNumber)
        {
            case Vst::kCtrlModWheel:
                id = kMeasureLengthParam;
                return kResultTrue;
        }
    }
    return kResultFalse;
}

//------------------------------------------------------------------------
VSTGUI::CView* TarantismController::createCustomView(
    VSTGUI::UTF8StringPtr name,
    const VSTGUI::UIAttributes& attributes,
    const VSTGUI::IUIDescription* /*description*/,
    VSTGUI::VST3Editor* /*editor*/)
{
    if (strcmp(name, "KeyboardView") == 0)
    {
        // Get size from attributes or use default
        VSTGUI::CRect size(0, 0, 600, 100);
        return new KeyboardView(size);
    }
    return nullptr;
}

//------------------------------------------------------------------------
} // namespace Tarantism
} // namespace Steinberg
