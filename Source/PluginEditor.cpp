/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

////==============================================================================
//TidalChorusAudioProcessorEditor::TidalChorusAudioProcessorEditor (TidalChorusAudioProcessor& p)
//    : AudioProcessorEditor (&p), audioProcessor (p)
//{
//    // Make sure that before the constructor has finished, you've set the
//    // editor's size to whatever you need it to be.
//    setSize (400, 300);
//}
//
//TidalChorusAudioProcessorEditor::~TidalChorusAudioProcessorEditor()
//{
//}
//
////==============================================================================
//void TidalChorusAudioProcessorEditor::paint (juce::Graphics& g)
//{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (juce::FontOptions (15.0f));
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
//}
//
//void TidalChorusAudioProcessorEditor::resized()
//{
//    // This is generally where you'll want to lay out the positions of any
//    // subcomponents in your editor..
//}

//==============================================================================
TidalChorusAudioProcessorEditor::TidalChorusAudioProcessorEditor (TidalChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // -------------------------------------------------------------------------
    // Sliders
    // -------------------------------------------------------------------------
    addAndMakeVisible(slowSlider);
    configureSlider(slowSlider, "Speed");

    addAndMakeVisible(fastSlider);
    configureSlider(fastSlider, "Speed");

    addAndMakeVisible(slowDepthSlider);
    configureSlider(slowDepthSlider, "Depth");

    addAndMakeVisible(fastDepthSlider);
    configureSlider(fastDepthSlider, "Depth");

    addAndMakeVisible(transitionSlider);
    configureSlider(transitionSlider, "Transition");

    // -------------------------------------------------------------------------
    // Mode labels
    // -------------------------------------------------------------------------
    configureModeLabels();

    // -------------------------------------------------------------------------
    // Buttons
    //
    // These are intentionally NOT ToggleButtons anymore. Their text describes
    // the current state, while clicking them requests the opposite state.
    // -------------------------------------------------------------------------
    addAndMakeVisible(slowFastButton);
    configureTextButton(slowFastButton);
    
    slowFastButton.setClickingTogglesState(false);
    slowFastButton.onClick = [this]
    {
        toggleSlowFastMode();
    };

    addAndMakeVisible(stereoButton);
    configureTextButton(stereoButton);
    
    stereoButton.setClickingTogglesState(false);
    stereoButton.onClick = [this]
    {
        toggleStereoMode();
    };

    // -------------------------------------------------------------------------
    // Slider attachments
    // -------------------------------------------------------------------------
    slowAttach = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "slow",
        slowSlider);

    fastAttach = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "fast",
        fastSlider);

    slowDepthAttach = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "slow_depth",
        slowDepthSlider);

    fastDepthAttach = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "fast_depth",
        fastDepthSlider);

    transitionAttach = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "transition",
        transitionSlider);

    // -------------------------------------------------------------------------
    // Mobile landscape default size
    // -------------------------------------------------------------------------
    setSize(800, 400);

    visualTransitionProgress =
        audioProcessor.getModeTransitionProgress();

    lastKnownModeProgress = visualTransitionProgress;

    updateSliderColours();
    updateModeButtonText();
    updateButtonColours();

    startTimerHz(30);
}

TidalChorusAudioProcessorEditor::~TidalChorusAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void TidalChorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(VisualStyle::background);

    g.setColour(VisualStyle::panelBackground);

    g.fillRoundedRectangle(
        chorusBox,
        VisualStyle::Geometry::componentCornerRadius);

    g.setColour(VisualStyle::panelBorder);

    g.drawRoundedRectangle(
        chorusBox,
        VisualStyle::Geometry::componentCornerRadius,
        VisualStyle::Geometry::componentBorderThickness);

    // -------------------------------------------------------------------------
    // Divider between main controls and right-hand controls
    // -------------------------------------------------------------------------
    const float dividerX =
        chorusBox.getRight()
        - chorusBox.getWidth() * 0.20f;

    g.setColour(
        VisualStyle::panelBorder.withAlpha(0.45f));

    g.drawVerticalLine(
        juce::roundToInt(dividerX),
        chorusBox.getY() + 10.0f,
        chorusBox.getBottom() - 10.0f);
}

//==============================================================================
void TidalChorusAudioProcessorEditor::resized()
{
    auto area = getLocalBounds()
                    .toFloat()
                    .reduced(8.0f, 8.0f);

    chorusBox = area;

    // -------------------------------------------------------------------------
    // 80% main controls / 20% right-hand controls
    // -------------------------------------------------------------------------
    auto controlsArea = area;

    auto sideArea =
        controlsArea.removeFromRight(
            area.getWidth() * 0.20f);

    // -------------------------------------------------------------------------
    // Main area
    //
    // 40% slow / 20% transition / 40% fast
    //
    // The transition area is deliberately narrower, but the knob itself is
    // now substantially larger than in the previous version.
    // -------------------------------------------------------------------------
    const float transitionZoneWidth =
        controlsArea.getWidth() * 0.16f;

    auto slowAreaBounds =
        controlsArea.removeFromLeft(
            (controlsArea.getWidth()
             - transitionZoneWidth) * 0.5f);

    auto transitionArea =
        controlsArea.removeFromLeft(
            transitionZoneWidth);

    auto fastAreaBounds =
        controlsArea;

    slowArea = slowAreaBounds;
    fastArea = fastAreaBounds;

    // -------------------------------------------------------------------------
    // SLOW column
    // -------------------------------------------------------------------------
    auto slow = slowArea.reduced(8.0f);

    // Reserve some space at the top for the SLOW label.
    auto slowLabelArea =
        slow.removeFromTop(30.0f);

    slowLabel.setBounds(
        slowLabelArea.toNearestInt());

    // Remaining area is split between Speed and Depth.
    auto slowTop =
        slow.removeFromTop(
            slow.getHeight() * 0.50f);

    auto slowBottom = slow;

    slowSlider.setBounds(
        slowTop.reduced(8.0f).toNearestInt());

    slowDepthSlider.setBounds(
        slowBottom.reduced(8.0f).toNearestInt());

    // -------------------------------------------------------------------------
    // FAST column
    // -------------------------------------------------------------------------
    auto fast = fastAreaBounds.reduced(8.0f);

    auto fastLabelArea =
        fast.removeFromTop(30.0f);

    fastLabel.setBounds(
        fastLabelArea.toNearestInt());

    auto fastTop =
        fast.removeFromTop(
            fast.getHeight() * 0.50f);

    auto fastBottom = fast;

    fastSlider.setBounds(
        fastTop.reduced(8.0f).toNearestInt());

    fastDepthSlider.setBounds(
        fastBottom.reduced(8.0f).toNearestInt());

    // -------------------------------------------------------------------------
    // TRANSITION knob
    // -------------------------------------------------------------------------
    auto transition =
        transitionArea.reduced(2.0f);

    // The transition knob is now approximately 3/4 of the diameter of the
    // main knobs.
    //
    // We base its size on the main column width rather than the transition
    // column width, since the latter is intentionally narrow.
    const int mainKnobDiameter =
        juce::jmin(
            juce::roundToInt(slowTop.getWidth() * 0.72f),
            juce::roundToInt(slowTop.getHeight() * 0.72f));

    const int transitionSize =
        juce::roundToInt(
            static_cast<float>(mainKnobDiameter) * 0.75f);

    auto transitionBounds =
        transition.withSizeKeepingCentre(
            transitionSize,
            transitionSize);

    transitionSlider.setBounds(
        transitionBounds.toNearestInt());

    // -------------------------------------------------------------------------
    // RIGHT-HAND BUTTONS
    //
    // Deliberately very short buttons: approximately 1/4 of the previous
    // vertical allocation.
    // -------------------------------------------------------------------------
    auto side =
        sideArea.reduced(12.0f, 12.0f);
    
    const float initial_height = side.getHeight();

    const float buttonHeight =
        initial_height * 0.125f;

    auto firstButtonArea =
        side.removeFromTop(buttonHeight);

    // Give the two buttons some separation.
    side.removeFromTop(side.getHeight() * 0.20f);
    
    // TODO: make visualization
    auto visualizationArea =
        side.removeFromTop(initial_height - 4.0f * buttonHeight);

    auto secondButtonArea =
        side.removeFromTop(buttonHeight);

    slowFastButton.setBounds(
        firstButtonArea
            .reduced(4.0f, 2.0f)
            .toNearestInt());

    stereoButton.setBounds(
        secondButtonArea
            .reduced(4.0f, 2.0f)
            .toNearestInt());
}

//==============================================================================
void TidalChorusAudioProcessorEditor::configureSlider(juce::Slider& s,
                                                    const juce::String& suffix)
{
    s.setSliderStyle(
        juce::Slider::RotaryVerticalDrag);

    s.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        82,
        20);

    s.setColour(
        juce::Slider::textBoxTextColourId,
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            true));

    s.setColour(
        juce::Slider::textBoxOutlineColourId,
        juce::Colours::transparentBlack);

    s.setColour(
        juce::Slider::rotarySliderFillColourId,
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            true));

    s.setColour(
        juce::Slider::rotarySliderOutlineColourId,
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            false,
            false));

    s.setColour(
        juce::Slider::thumbColourId,
        juce::Colours::transparentBlack);

    s.setTextValueSuffix(
        " " + suffix);
}

//==============================================================================
void TidalChorusAudioProcessorEditor::configureModeLabels()
{
    // -------------------------------------------------------------------------
    // slowLabel
    // -------------------------------------------------------------------------
    slowLabel.setText(
        "SLOW",
        juce::dontSendNotification);

    slowLabel.setFont(
        VisualStyle::getDefaultFont(18.0f));

    slowLabel.setColour(
        juce::Label::textColourId,
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            true));

    slowLabel.setJustificationType(
        juce::Justification::centred);

    addAndMakeVisible(slowLabel);

    // -------------------------------------------------------------------------
    // fastLabel
    // -------------------------------------------------------------------------
    fastLabel.setText(
        "FAST",
        juce::dontSendNotification);

    fastLabel.setFont(
        VisualStyle::getDefaultFont(18.0f));

    fastLabel.setColour(
        juce::Label::textColourId,
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            true));

    fastLabel.setJustificationType(
        juce::Justification::centred);

    addAndMakeVisible(fastLabel);
}

void TidalChorusAudioProcessorEditor::configureTextButton(juce::TextButton& button)
{
    // These are ordinary buttons, not toggle buttons.
        button.setClickingTogglesState(false);

        button.setColour(
            juce::TextButton::buttonColourId,
            VisualStyle::panelBackground);

        button.setColour(
            juce::TextButton::buttonOnColourId,
            VisualStyle::panelBackground);

        // Always highlight these buttons.
        const auto highlight =
            VisualStyle::getStateColor(
                VisualStyle::Palette::blue,
                true);

        button.setColour(
            juce::TextButton::textColourOffId,
            highlight);

        button.setColour(
            juce::TextButton::textColourOnId,
            highlight);

        button.setColour(
            juce::ComboBox::outlineColourId,
            VisualStyle::panelBorder);

//    button.setFont(VisualStyle::getDefaultFont(16.0f));
}

//==============================================================================
void TidalChorusAudioProcessorEditor::toggleSlowFastMode()
{
//    auto* parameter =
//        audioProcessor.parameters.getParameter("slow_fast");
//
//    if (parameter == nullptr)
//        return;
//
//    const bool currentlyFast =
//        parameter->getValue() > 0.5f;
//
//    // Explicitly request the opposite state.
//    //
//    // We deliberately don't rely on the button's own toggle state.
//    parameter->setValueNotifyingHost(
//        currentlyFast ? 0.0f : 1.0f);
    auto* parameter = audioProcessor.parameters.getParameter("slow_fast");

    if (parameter != nullptr)
    {
        const float current = parameter->getValue();
        parameter->setValueNotifyingHost(current > 0.5f ? 0.0f : 1.0f);
    }

    updateModeButtonText();
}

//==============================================================================
void TidalChorusAudioProcessorEditor::toggleStereoMode()
{
    auto* parameter = audioProcessor.parameters.getParameter("stereo");

    if (parameter != nullptr)
    {
        const float current = parameter->getValue();
        parameter->setValueNotifyingHost(current > 0.5f ? 0.0f : 1.0f);
    }

    updateModeButtonText();
}

//==============================================================================
juce::Colour TidalChorusAudioProcessorEditor::getInterpolatedStateColour(
    const VisualStyle::ColorSet& set, float activity) const
{
    activity =
        juce::jlimit(
            0.0f,
            1.0f,
            activity);

    return set.disabled.interpolatedWith(
        set.highlight,
        activity);
}

void TidalChorusAudioProcessorEditor::updateSliderColours()
{
    // 1 = fully highlighted
    // 0 = fully dimmed
    const float slowActivity =
        1.0f - visualTransitionProgress;

    const float fastActivity =
        visualTransitionProgress;

    const auto slowColour =
        getInterpolatedStateColour(
            VisualStyle::Palette::blue,
            slowActivity);

    const auto fastColour =
        getInterpolatedStateColour(
            VisualStyle::Palette::blue,
            fastActivity);

    auto applyColours =
        [](juce::Slider& slider,
           juce::Colour colour)
    {
        slider.setColour(
            juce::Slider::textBoxTextColourId,
            colour);

        slider.setColour(
            juce::Slider::rotarySliderFillColourId,
            colour);

        slider.setColour(
            juce::Slider::rotarySliderOutlineColourId,
            colour.withAlpha(0.55f));
    };

    applyColours(
        slowSlider,
        slowColour);

    applyColours(
        slowDepthSlider,
        slowColour);

    applyColours(
        fastSlider,
        fastColour);

    applyColours(
        fastDepthSlider,
        fastColour);

    // Transition is always active/highlighted.
    const auto transitionColour =
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            true);

    applyColours(
        transitionSlider,
        transitionColour);

    // The labels follow their corresponding mode.
    slowLabel.setColour(
        juce::Label::textColourId,
        slowColour);

    fastLabel.setColour(
        juce::Label::textColourId,
        fastColour);
}

//==============================================================================
void TidalChorusAudioProcessorEditor::updateButtonColours()
{
    // Unlike the sliders, these buttons are always highlighted.
    const auto highlight =
        VisualStyle::getStateColor(
            VisualStyle::Palette::blue,
            true);

    slowFastButton.setColour(
        juce::TextButton::textColourOffId,
        highlight);

    slowFastButton.setColour(
        juce::TextButton::textColourOnId,
        highlight);

    stereoButton.setColour(
        juce::TextButton::textColourOffId,
        highlight);

    stereoButton.setColour(
        juce::TextButton::textColourOnId,
        highlight);
}

void TidalChorusAudioProcessorEditor::updateModeButtonText()
{
    const bool fast =
        audioProcessor.parameters.getRawParameterValue("slow_fast")->load() > 0.5f;

    slowFastButton.setButtonText(fast ? "FAST" : "SLOW");

    const bool stereo =
        audioProcessor.parameters.getRawParameterValue("stereo")->load() > 0.5f;

    stereoButton.setButtonText(stereo ? "STEREO" : "MONO");

    updateButtonColours();
}

//==============================================================================
void TidalChorusAudioProcessorEditor::timerCallback()
{
    const float processorProgress =
        audioProcessor.getModeTransitionProgress();

    // The processor is authoritative for the actual audio transition.
    visualTransitionProgress =
        processorProgress;

    const bool modeChangedEnough =
        std::abs(
            visualTransitionProgress
            - lastKnownModeProgress) > 0.001f;

    if (modeChangedEnough)
    {
        updateSliderColours();
        updateModeButtonText();

        lastKnownModeProgress =
            visualTransitionProgress;
    }
    else
    {
        // Keep colour state synchronized with any host-side parameter change.
        updateSliderColours();
    }

    repaint();
}
