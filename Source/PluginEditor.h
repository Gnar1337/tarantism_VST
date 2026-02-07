#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class TarantismAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    TarantismAudioProcessorEditor(TarantismAudioProcessor&);
    ~TarantismAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    TarantismAudioProcessor& audioProcessor;

    juce::Slider gainSlider;
    juce::Label gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TarantismAudioProcessorEditor)
};
