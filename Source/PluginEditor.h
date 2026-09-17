/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Style/VisualStyle.h"

//==============================================================================
/**
*/
class TidalChorusAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      private juce::Timer
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    TidalChorusAudioProcessorEditor (TidalChorusAudioProcessor&);
    ~TidalChorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    TidalChorusAudioProcessor& audioProcessor;

    juce::Rectangle<float> chorusBox;
    juce::Rectangle<float> slowArea;
    juce::Rectangle<float> fastArea;

    juce::Slider slowSlider;
    juce::Slider fastSlider;
    juce::Slider slowDepthSlider;
    juce::Slider fastDepthSlider;
    juce::Slider transitionSlider;

    juce::TextButton slowFastButton;
    juce::TextButton stereoButton;

    juce::Label slowLabel;
    juce::Label fastLabel;

    std::unique_ptr<SliderAttachment> slowAttach;
    std::unique_ptr<SliderAttachment> fastAttach;
    std::unique_ptr<SliderAttachment> slowDepthAttach;
    std::unique_ptr<SliderAttachment> fastDepthAttach;
    std::unique_ptr<SliderAttachment> transitionAttach;

    float visualTransitionProgress = 0.0f;
    float lastKnownModeProgress = 0.0f;

    void timerCallback() override;

    void configureSlider(juce::Slider& slider,
                         const juce::String& suffix);

    void configureTextButton(juce::TextButton& button);

    void configureModeLabels();

    void updateSliderColours();
    void updateModeButtonText();
    void updateButtonColours();

    void toggleSlowFastMode();
    void toggleStereoMode();

    juce::Colour getInterpolatedStateColour(
        const VisualStyle::ColorSet& set,
        float activity) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TidalChorusAudioProcessorEditor)
};
