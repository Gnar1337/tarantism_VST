#include "PluginProcessor.h"
#include "PluginEditor.h"

TarantismAudioProcessor::TarantismAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("TarantismVST"),
                 {
                     std::make_unique<juce::AudioParameterFloat>(
                         "gain",
                         "Gain",
                         juce::NormalisableRange<float>(0.0f, 1.0f),
                         0.5f)
                 })
{
    gainParameter = parameters.getRawParameterValue("gain");
}

TarantismAudioProcessor::~TarantismAudioProcessor()
{
}

const juce::String TarantismAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TarantismAudioProcessor::acceptsMidi() const
{
    return false;
}

bool TarantismAudioProcessor::producesMidi() const
{
    return false;
}

bool TarantismAudioProcessor::isMidiEffect() const
{
    return false;
}

double TarantismAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TarantismAudioProcessor::getNumPrograms()
{
    return 1;
}

int TarantismAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TarantismAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String TarantismAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void TarantismAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void TarantismAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void TarantismAudioProcessor::releaseResources()
{
}

bool TarantismAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void TarantismAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Apply gain to all channels
    float gain = gainParameter->load();
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= gain;
        }
    }
}

bool TarantismAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TarantismAudioProcessor::createEditor()
{
    return new TarantismAudioProcessorEditor(*this);
}

void TarantismAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TarantismAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TarantismAudioProcessor();
}
