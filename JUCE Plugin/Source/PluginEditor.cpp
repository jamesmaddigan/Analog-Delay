/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Analog_Delay_AP2AudioProcessorEditor::Analog_Delay_AP2AudioProcessorEditor (Analog_Delay_AP2AudioProcessor& p, juce::AudioProcessorValueTreeState& vst)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vst)
{
    
    feedbackLabel.setText("FeedBack", juce::dontSendNotification);
    addAndMakeVisible(feedbackLabel);
    addAndMakeVisible(feedBackSlider);
    feedBackAttachment = std::make_unique<SliderAttachment> (valueTreeState, "FEEDBACK", feedBackSlider);
    
    delayLabel.setText("Delay Time", juce::dontSendNotification);
    addAndMakeVisible(delayLabel);
    addAndMakeVisible(delaySlider);
    delayAttachment = std::make_unique<SliderAttachment>(valueTreeState, "DELAYTIME", delaySlider);
    
    driveLabel.setText("Drive", juce::dontSendNotification);
    addAndMakeVisible(driveLabel);
    addAndMakeVisible(driveSlider);
    driveAttachment = std::make_unique<SliderAttachment>(valueTreeState, "DRIVE", driveSlider);
    
    
    breakUpLabel.setText("Break Up", juce::dontSendNotification);
    addAndMakeVisible(breakUpLabel);
    addAndMakeVisible(breakUpSlider);
    breakUpAttachment = std::make_unique<SliderAttachment>(valueTreeState, "BREAKUP", breakUpSlider);
    
    cutOffLabel.setText("Filter CutOff Frequency", juce::dontSendNotification);
    addAndMakeVisible(cutOffLabel);
    addAndMakeVisible(cutOffSlider);
    cutOffAttachment = std::make_unique<SliderAttachment>(valueTreeState, "CUTOFF", cutOffSlider);
    
    multiTapLabel.setText("Multi Tap Tempo", juce::dontSendNotification);
    addAndMakeVisible(multiTapLabel);
    addAndMakeVisible(multiTapSlider);
    multiTapAttachment = std::make_unique<SliderAttachment>(valueTreeState, "MULTITAP", multiTapSlider);
    
    multiTapButton.setButtonText("Multi Tap");
    addAndMakeVisible(multiTapButton);
    buttonAttachment = std::make_unique<ButtonAttachment>(valueTreeState, "TAPBUTTON", multiTapButton);

    
    multiTapIntensityLabel.setText("Multi Tap Intensity", juce::dontSendNotification);
    addAndMakeVisible(multiTapIntensityLabel);
    addAndMakeVisible(multiTapIntensitySlider);
    multiTapIntensityAttachment = std::make_unique<SliderAttachment>(valueTreeState, "TAPINTENSITY", multiTapIntensitySlider);

    //======================================================================================================

    acousticButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    acousticButton.onClick = [this] ()
    {
        audioProcessor.isAcousticPresetButtonPressed = 1;
    };
    addAndMakeVisible(acousticButton);
    
    stratButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    stratButton.onClick = [this] ()
    {
        audioProcessor.isStratPresetButtonPressed = 1;
    };
    addAndMakeVisible(stratButton);

    //======================================================================================================
    
    setSize (paramSliderWidth + paramLabelWidth, juce::jmax(100, totalHeight * 2));

}

Analog_Delay_AP2AudioProcessorEditor::~Analog_Delay_AP2AudioProcessorEditor()
{
}

//==============================================================================
void Analog_Delay_AP2AudioProcessorEditor::paint (juce::Graphics& g)
{
   
    g.fillAll (juce::Colours::blue);

    g.setColour (juce::Colours::white);
    
    //g.setColour (juce::Colours::white);
    g.setFont (30.0f);
    g.drawFittedText("Analog Delay", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
    
}

void Analog_Delay_AP2AudioProcessorEditor::resized()
{
    auto r = getLocalBounds();
    
    
    
    auto feedbackRect = r.removeFromTop(paramHeight + 50);
    feedbackLabel.setBounds(feedbackRect.removeFromLeft(paramLabelWidth));
    feedBackSlider.setBounds(feedbackRect);
    

    auto delayRect = r.removeFromTop(paramHeight );
    delayLabel.setBounds(delayRect.removeFromLeft(paramLabelWidth));
    delaySlider.setBounds(delayRect);
    
    
    auto driveRect = r.removeFromTop(paramHeight);
    driveLabel.setBounds(driveRect.removeFromLeft(paramLabelWidth));
    driveSlider.setBounds(driveRect);
    
    auto breakUpRect = r.removeFromTop(paramHeight);
    breakUpLabel.setBounds(breakUpRect.removeFromLeft(paramLabelWidth));
    breakUpSlider.setBounds(breakUpRect);
    
    auto cutOffRect = r.removeFromTop(paramHeight);
    cutOffLabel.setBounds(cutOffRect.removeFromLeft(paramLabelWidth));
    cutOffSlider.setBounds(cutOffRect);
    
    multiTapButton.setBounds(r.removeFromTop(paramHeight));
    
    auto multiTapRect = r.removeFromTop(paramHeight);
    multiTapLabel.setBounds(multiTapRect.removeFromLeft(paramLabelWidth));
    multiTapSlider.setBounds(multiTapRect);
    
    auto multiTapIntensityRect = r.removeFromTop(paramHeight);
    multiTapIntensityLabel.setBounds(multiTapIntensityRect.removeFromLeft(paramLabelWidth));
    multiTapIntensitySlider.setBounds(multiTapIntensityRect);
  
    acousticButton.setBounds(r.removeFromTop(paramHeight));
    stratButton.setBounds(r.removeFromTop(paramHeight));
    
  
    
    
    
}
