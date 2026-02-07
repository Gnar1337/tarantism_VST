#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "pluginids.h"

namespace Steinberg {
namespace Tarantism {

//------------------------------------------------------------------------
// TarantismController
//------------------------------------------------------------------------
class TarantismController : public Vst::EditControllerEx1, public VSTGUI::VST3EditorDelegate
{
public:
    TarantismController() = default;
    ~TarantismController() SMTG_OVERRIDE = default;

    // Create function
    static FUnknown* createInstance(void* /*context*/) 
    { 
        return (Vst::IEditController*)new TarantismController; 
    }

    // IPluginBase
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;

    // EditController
    tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
    IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel,
        Vst::CtrlNumber midiControllerNumber, Vst::ParamID& id) SMTG_OVERRIDE;

    // VST3EditorDelegate
    VSTGUI::CView* createCustomView(VSTGUI::UTF8StringPtr name,
        const VSTGUI::UIAttributes& attributes,
        const VSTGUI::IUIDescription* description,
        VSTGUI::VST3Editor* editor) SMTG_OVERRIDE;

    OBJ_METHODS(TarantismController, EditControllerEx1)
    DEFINE_INTERFACES
        DEF_INTERFACE(VSTGUI::VST3EditorDelegate)
    END_DEFINE_INTERFACES(EditControllerEx1)
    REFCOUNT_METHODS(EditControllerEx1)
};

//------------------------------------------------------------------------
} // namespace Tarantism
} // namespace Steinberg
