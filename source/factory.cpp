#include "pluginprocessor.h"
#include "plugincontroller.h"
#include "pluginids.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "Tarantism"

using namespace Steinberg::Vst;
using namespace Steinberg::Tarantism;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("Gnar",
                   "https://www.gnar.com",
                   "mailto:info@gnar.com")

    //---First Plug-in included in this factory-------
    // Its kVstAudioEffectClass component
    DEF_CLASS2 (INLINE_UID_FROM_FUID(kTarantismProcessorUID),
                PClassInfo::kManyInstances,
                kVstAudioEffectClass,
                stringPluginName,
                Vst::kDistributable,
                Vst::PlugType::kFxInstrument,
                VERSION_STR,
                kVstVersionString,
                TarantismProcessor::createInstance)

    // Its kVstComponentControllerClass component
    DEF_CLASS2 (INLINE_UID_FROM_FUID (kTarantismControllerUID),
                PClassInfo::kManyInstances,
                kVstComponentControllerClass,
                stringPluginName "Controller",
                0,
                "",
                VERSION_STR,
                kVstVersionString,
                TarantismController::createInstance)

END_FACTORY
