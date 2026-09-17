/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

////==============================================================================
//TidalChorusAudioProcessor::TidalChorusAudioProcessor()
//#ifndef JucePlugin_PreferredChannelConfigurations
//     : AudioProcessor (BusesProperties()
//                     #if ! JucePlugin_IsMidiEffect
//                      #if ! JucePlugin_IsSynth
//                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
//                      #endif
//                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
//                     #endif
//                       )
//#endif
//{
//}
//
//TidalChorusAudioProcessor::~TidalChorusAudioProcessor()
//{
//}
//
////==============================================================================
//const juce::String TidalChorusAudioProcessor::getName() const
//{
//    return JucePlugin_Name;
//}
//
//bool TidalChorusAudioProcessor::acceptsMidi() const
//{
//   #if JucePlugin_WantsMidiInput
//    return true;
//   #else
//    return false;
//   #endif
//}
//
//bool TidalChorusAudioProcessor::producesMidi() const
//{
//   #if JucePlugin_ProducesMidiOutput
//    return true;
//   #else
//    return false;
//   #endif
//}
//
//bool TidalChorusAudioProcessor::isMidiEffect() const
//{
//   #if JucePlugin_IsMidiEffect
//    return true;
//   #else
//    return false;
//   #endif
//}
//
//double TidalChorusAudioProcessor::getTailLengthSeconds() const
//{
//    return 0.0;
//}
//
//int TidalChorusAudioProcessor::getNumPrograms()
//{
//    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
//                // so this should be at least 1, even if you're not really implementing programs.
//}
//
//int TidalChorusAudioProcessor::getCurrentProgram()
//{
//    return 0;
//}
//
//void TidalChorusAudioProcessor::setCurrentProgram (int index)
//{
//}
//
//const juce::String TidalChorusAudioProcessor::getProgramName (int index)
//{
//    return {};
//}
//
//void TidalChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
//{
//}
//
////==============================================================================
//void TidalChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
//{
//    // Use this method as the place to do any pre-playback
//    // initialisation that you need..
//}
//
//void TidalChorusAudioProcessor::releaseResources()
//{
//    // When playback stops, you can use this as an opportunity to free up any
//    // spare memory, etc.
//}
//
//#ifndef JucePlugin_PreferredChannelConfigurations
//bool TidalChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
//{
//  #if JucePlugin_IsMidiEffect
//    juce::ignoreUnused (layouts);
//    return true;
//  #else
//    // This is the place where you check if the layout is supported.
//    // In this template code we only support mono or stereo.
//    // Some plugin hosts, such as certain GarageBand versions, will only
//    // load plugins that support stereo bus layouts.
//    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
//     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//        return false;
//
//    // This checks if the input layout matches the output layout
//   #if ! JucePlugin_IsSynth
//    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//        return false;
//   #endif
//
//    return true;
//  #endif
//}
//#endif
//
//void TidalChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
//{
//    juce::ScopedNoDenormals noDenormals;
//    auto totalNumInputChannels  = getTotalNumInputChannels();
//    auto totalNumOutputChannels = getTotalNumOutputChannels();
//
//    // In case we have more outputs than inputs, this code clears any output
//    // channels that didn't contain input data, (because these aren't
//    // guaranteed to be empty - they may contain garbage).
//    // This is here to avoid people getting screaming feedback
//    // when they first compile a plugin, but obviously you don't need to keep
//    // this code if your algorithm always overwrites all the output channels.
//    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//        buffer.clear (i, 0, buffer.getNumSamples());
//
//    // This is the place where you'd normally do the guts of your plugin's
//    // audio processing...
//    // Make sure to reset the state if your inner loop is processing
//    // the samples and the outer loop is handling the channels.
//    // Alternatively, you can process the samples with the channels
//    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
//}
//
////==============================================================================
//bool TidalChorusAudioProcessor::hasEditor() const
//{
//    return true; // (change this to false if you choose to not supply an editor)
//}
//
//juce::AudioProcessorEditor* TidalChorusAudioProcessor::createEditor()
//{
//    return new TidalChorusAudioProcessorEditor (*this);
//}
//
////==============================================================================
//void TidalChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
//{
//    // You should use this method to store your parameters in the memory block.
//    // You could do that either as raw data, or use the XML or ValueTree classes
//    // as intermediaries to make it easy to save and load complex data.
//}
//
//void TidalChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
//{
//    // You should use this method to restore your parameters from this memory block,
//    // whose contents will have been created by the getStateInformation() call.
//}
//
////==============================================================================
//// This creates new instances of the plugin..
//juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
//{
//    return new TidalChorusAudioProcessor();
//}

//==============================================================================
TidalChorusAudioProcessor::TidalChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

TidalChorusAudioProcessor::~TidalChorusAudioProcessor()
{
    parameters.removeParameterListener("slow", this);
    parameters.removeParameterListener("fast", this);
    parameters.removeParameterListener("slow_depth", this);
    parameters.removeParameterListener("fast_depth", this);
    parameters.removeParameterListener("transition", this);
    parameters.removeParameterListener("stereo", this);
    parameters.removeParameterListener("slow_fast", this);
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout
TidalChorusAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"slow", 1},
        "Slow",
        juce::NormalisableRange<float>(0.01f, 3.0f, 0.01f),
        0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"fast", 1},
        "Fast",
        juce::NormalisableRange<float>(3.0f, 10.0f, 0.01f),
        5.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"slow_depth", 1},
        "Slow Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"fast_depth", 1},
        "Fast Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f));

    // Transition time in seconds. 0 = instantaneous.
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"transition", 1},
        "Transition",
        juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f),
        0.25f));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{"slow_fast", 1},
        "Slow/Fast",
        false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{"stereo", 1},
        "Stereo",
        false));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String TidalChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TidalChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TidalChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TidalChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TidalChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TidalChorusAudioProcessor::getNumPrograms()
{
    return 1;
}

int TidalChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TidalChorusAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String TidalChorusAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void TidalChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void TidalChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);

    currentSampleRate = sampleRate;

    choruses.resize(getTotalNumInputChannels());

    for (auto& c : choruses)
        c.prepare(sampleRate);

    targetFastSpeed = *parameters.getRawParameterValue("fast");
    targetSlowSpeed = *parameters.getRawParameterValue("slow");
    targetSlowDepth = *parameters.getRawParameterValue("slow_depth");
    targetFastDepth = *parameters.getRawParameterValue("fast_depth");
    transitionTimeSeconds = *parameters.getRawParameterValue("transition");
    stereoMode = *parameters.getRawParameterValue("stereo");

    const bool fastMode = *parameters.getRawParameterValue("slow_fast") > 0.5f;

    modeTransitionProgress = fastMode ? 1.0f : 0.0f;
    targetModeTransitionProgress = modeTransitionProgress;

    currentSpeed = juce::jmap(modeTransitionProgress,
                              targetSlowSpeed, targetFastSpeed);
    currentDepth = juce::jmap(modeTransitionProgress,
                              targetSlowDepth, targetFastDepth);

    float i = 0.0f;
    for (auto& chorus : choruses)
    {
        chorus.setSpeed(currentSpeed);
        chorus.setDepth(currentDepth);
        chorus.setPhase(stereoMode ? i * 0.5f : 0.0f);
        ++i;
    }

    parameters.addParameterListener("fast", this);
    parameters.addParameterListener("slow", this);
    parameters.addParameterListener("slow_depth", this);
    parameters.addParameterListener("fast_depth", this);
    parameters.addParameterListener("transition", this);
    parameters.addParameterListener("stereo", this);
    parameters.addParameterListener("slow_fast", this);
}

void TidalChorusAudioProcessor::parameterChanged(const juce::String& id, float newValue)
{
    // Parameter callbacks should only update targets/state. The actual Chorus
    // parameters are changed from processBlock so that audio processing remains
    // deterministic and the mode change can be smoothed.

    if (id == "slow")
    {
        targetSlowSpeed = newValue;
    }
    else if (id == "fast")
    {
        targetFastSpeed = newValue;
    }
    else if (id == "slow_depth")
    {
        targetSlowDepth = newValue;
    }
    else if (id == "fast_depth")
    {
        targetFastDepth = newValue;
    }
    else if (id == "transition")
    {
        transitionTimeSeconds = newValue;
    }
    else if (id == "slow_fast")
    {
        targetModeTransitionProgress = newValue > 0.5f ? 1.0f : 0.0f;
    }
    else if (id == "stereo")
    {
        stereoMode = newValue > 0.5f;

        float i = 0.0f;
        for (auto& chorus : choruses)
        {
            chorus.setPhase(stereoMode ? i * 0.5f : 0.0f);
            ++i;
        }
    }
}

//==============================================================================
void TidalChorusAudioProcessor::releaseResources()
{
}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool TidalChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
void TidalChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // ------------------------------------------------------------
    // Move the slow/fast mode towards its target.
    // ------------------------------------------------------------
    const float transitionTime = transitionTimeSeconds;

    if (transitionTime <= 0.0f)
    {
        modeTransitionProgress = targetModeTransitionProgress;
    }
    else
    {
        const float distance = targetModeTransitionProgress - modeTransitionProgress;

        // Exact enough for a parameter ramp while remaining extremely cheap.
        const float step = static_cast<float>(buffer.getNumSamples())
                         / static_cast<float>(currentSampleRate * transitionTime);

        if (std::abs(distance) <= step)
            modeTransitionProgress = targetModeTransitionProgress;
        else
            modeTransitionProgress += (distance > 0.0f ? step : -step);
    }
    
    // Interpolate both speed and depth between the two modes.
    currentSpeed = juce::jmap(modeTransitionProgress,
                              targetSlowSpeed, targetFastSpeed);

    currentDepth = juce::jmap(modeTransitionProgress,
                              targetSlowDepth, targetFastDepth);

    for (auto& chorus : choruses)
    {
        chorus.setSpeed(currentSpeed);
        chorus.setDepth(currentDepth);
    }

    // ------------------------------------------------------------
    // Process audio.
    // ------------------------------------------------------------
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto& chorusChannel = choruses[channel];

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            channelData[sample] = chorusChannel.process_sample(channelData[sample]);
    }
}

//==============================================================================
bool TidalChorusAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TidalChorusAudioProcessor::createEditor()
{
    return new TidalChorusAudioProcessorEditor (*this);
}

//==============================================================================
void TidalChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TidalChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TidalChorusAudioProcessor();
}
