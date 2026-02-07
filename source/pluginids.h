#pragma once

namespace Steinberg {
namespace Tarantism {

// Plugin UIDs
static const FUID kTarantismProcessorUID(0x12345678, 0x12345678, 0x12345678, 0x12345678);
static const FUID kTarantismControllerUID(0x87654321, 0x87654321, 0x87654321, 0x87654321);

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
