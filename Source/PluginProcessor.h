/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/Chorus.h"

//==============================================================================
/**
*/
class TidalChorusAudioProcessor  : public juce::AudioProcessor,
                                 public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    TidalChorusAudioProcessor();
    ~TidalChorusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;

    // Returns the current visual/audio transition progress:
    // 0.0 = slow, 1.0 = fast.
    float getModeTransitionProgress() const noexcept { return modeTransitionProgress; }

private:
    //==============================================================================
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::vector<Chorus> choruses;

    void parameterChanged(const juce::String& id, float newValue) override;

    double currentSampleRate = 44100.0;

    // These are targets received from the parameter system. The actual Chorus
    // objects are moved towards them from the audio thread.
    float targetSlowSpeed = 0.5f;
    float targetFastSpeed = 5.0f;
    float targetSlowDepth = 0.5f;
    float targetFastDepth = 0.5f;

    // Current state of the continuous slow <-> fast transition.
    float modeTransitionProgress = 0.0f;
    float targetModeTransitionProgress = 0.0f;

    // Smoothed values used by the audio thread.
    float currentSpeed = 0.5f;
    float currentDepth = 0.5f;

    // Transition time in seconds.
    float transitionTimeSeconds = 0.25f;

    bool stereoMode = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TidalChorusAudioProcessor)
};
