#pragma once

namespace Steinberg {
namespace Tarantism {

// Plugin UIDs - Unique identifiers for this plugin
// Generated using random values to ensure uniqueness
static const FUID kTarantismProcessorUID(0xA7B3C1D4, 0xE5F67890, 0x1A2B3C4D, 0x5E6F7A8B);
static const FUID kTarantismControllerUID(0x9C8B7A65, 0x4321FEDC, 0xBA987654, 0x3210ABCD);

// Parameter IDs
enum TarantismParams : Vst::ParamID
{
    kMeasureLengthParam = 100,
    kNoteDurationParam = 101,
    kOctaveParam = 102,
    kBypassParam = 103
};

} // namespace Tarantism
} // namespace Steinberg
