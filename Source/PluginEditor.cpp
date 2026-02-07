#include "PluginProcessor.h"
#include "PluginEditor.h"

TarantismAudioProcessorEditor::TarantismAudioProcessorEditor(TarantismAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set the size of the editor window
    setSize(400, 300);

    // Configure gain slider
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    gainSlider.setPopupDisplayEnabled(true, false, this);
    gainSlider.setTextValueSuffix(" Gain");
    addAndMakeVisible(gainSlider);

    // Configure gain label
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);
    addAndMakeVisible(gainLabel);

    // Attach slider to parameter
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "gain", gainSlider);
}

TarantismAudioProcessorEditor::~TarantismAudioProcessorEditor()
{
}

void TarantismAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill background with dark grey
    g.fillAll(juce::Colour(0xff2d2d2d));

    // Draw title
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(24.0f, juce::Font::bold));
    g.drawText("Tarantism VST", getLocalBounds().removeFromTop(60),
               juce::Justification::centred, true);

    // Draw border
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2);
}

void TarantismAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    // Reserve space for title
    area.removeFromTop(80);
    
    // Center the gain slider
    auto sliderArea = area.withSizeKeepingCentre(150, 150);
    gainSlider.setBounds(sliderArea);
}
