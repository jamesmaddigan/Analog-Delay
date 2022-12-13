/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"



typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//==============================================================================
/**
*/
class Analog_Delay_AP2AudioProcessorEditor  : public juce::AudioProcessorEditor

{
public:
    
    enum
        {
            paramHeight = 50,
            totalHeight = 300,
            paramLabelWidth = 80,
            paramSliderWidth = 300
        };

    
    Analog_Delay_AP2AudioProcessorEditor (Analog_Delay_AP2AudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~Analog_Delay_AP2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Analog_Delay_AP2AudioProcessor& audioProcessor;
    
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    
    juce::Label feedbackLabel;
    juce::Slider feedBackSlider;
    std::unique_ptr<SliderAttachment> feedBackAttachment;
    
    juce::Label delayLabel;
    juce::Slider delaySlider;
    std::unique_ptr<SliderAttachment> delayAttachment;
    
    juce::Label driveLabel;
    juce::Slider driveSlider;
    std::unique_ptr<SliderAttachment> driveAttachment;
    
    juce::Label breakUpLabel;
    juce::Slider breakUpSlider;
    std::unique_ptr<SliderAttachment> breakUpAttachment;
    
    juce::Label cutOffLabel;
    juce::Slider cutOffSlider;
    std::unique_ptr<SliderAttachment> cutOffAttachment;
    
    juce::Label multiTapLabel;
    juce::ToggleButton multiTapButton;
    juce::Slider multiTapSlider;
    std::unique_ptr<SliderAttachment> multiTapAttachment;
    std::unique_ptr<ButtonAttachment> buttonAttachment;
    
    juce::Label multiTapIntensityLabel;
    juce::Slider multiTapIntensitySlider;
    std::unique_ptr<SliderAttachment> multiTapIntensityAttachment;
    
    //======================================================================================================
    
    juce::TextButton acousticButton {"Save Acousic Preset"};
    std::unique_ptr<ButtonAttachment> acousticButtonAttachment;
    
    juce::TextButton stratButton {"Save Stratocaster Preset"};
    std::unique_ptr<ButtonAttachment> stratButtonAttachment;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Analog_Delay_AP2AudioProcessorEditor)
};
