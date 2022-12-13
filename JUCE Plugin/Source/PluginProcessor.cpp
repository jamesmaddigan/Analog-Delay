/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DelayLine.h"
#include "AudioUtilities.h"
#include "Preset.h"

//==============================================================================
Analog_Delay_AP2AudioProcessor::Analog_Delay_AP2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, juce::Identifier("delayValueTree"),
                                   {
                                        std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 1.0f, 0.5f),
                                        std::make_unique<juce::AudioParameterFloat>("DELAYTIME", "Delay Time", 0.05f, 0.6f, 0.3f),
                                        std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.0f, 1.5f, 0.7f),
                                        std::make_unique<juce::AudioParameterFloat>("BREAKUP", "BreakUp", 1.f, 2.f, 1.5f),
                                        std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cut Off", 200.f, 2500.f, 1000.f),
                                        std::make_unique<juce::AudioParameterFloat>("MULTITAP", "Multi Tap", 0.1f, 0.5f, 0.25f),
                                        std::make_unique<juce::AudioParameterBool>("TAPBUTTON", "Multi Tap Button", false),
                                        std::make_unique<juce::AudioParameterFloat>("TAPINTENSITY", "Multi Tap Intensity", 0.1f, 3.f, 1.f),
                       })
#endif
{
    analogDelayLine = new Delay;
    
    delayTimeParameter = parameters.getRawParameterValue("DELAYTIME");
    feedbackParameter = parameters.getRawParameterValue("FEEDBACK");
    driveParameter = parameters.getRawParameterValue("DRIVE");
    breakUpParameter = parameters.getRawParameterValue("BREAKUP");
    cutOffParameter = parameters.getRawParameterValue("CUTOFF");
    multiTapButtonState = parameters.getRawParameterValue("TAPBUTTON");
    multiTapParameter = parameters.getRawParameterValue("MULTITAP");
    multiTapIntensityParameter = parameters.getRawParameterValue("TAPINTENSITY");
}

Analog_Delay_AP2AudioProcessor::~Analog_Delay_AP2AudioProcessor()
{
    delete analogDelayLine;
}

//==============================================================================
const juce::String Analog_Delay_AP2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Analog_Delay_AP2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Analog_Delay_AP2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Analog_Delay_AP2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Analog_Delay_AP2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Analog_Delay_AP2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Analog_Delay_AP2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Analog_Delay_AP2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Analog_Delay_AP2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Analog_Delay_AP2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Analog_Delay_AP2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    analogDelayLine->isMultiTapOn(*multiTapButtonState);
    analogDelayLine->setMultiTapIntensity(*multiTapIntensityParameter);
    analogDelayLine->clearBuffer();
    analogDelayLine->setSampleRate(sampleRate);
    
    analogDelayLine->setFeedback(*feedbackParameter);
    auto delaySamples = (*delayTimeParameter * sampleRate);
    auto multiTapDelaySamples = delaySamples + (delaySamples * (*multiTapParameter));
    analogDelayLine->setDrive(*driveParameter);
    analogDelayLine->setDistortionLevels(*breakUpParameter);
    analogDelayLine->setFilterCoeff(*cutOffParameter);
   
    
    delayPrevious = delaySamples;
    multiTapDelayPrevious = multiTapDelaySamples;
}

void Analog_Delay_AP2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Analog_Delay_AP2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Analog_Delay_AP2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (isAcousticPresetButtonPressed == 1)
    {
        getStateInformation(acousticPresetData);
        isAcousticPresetButtonPressed = 0;        // returns if condition to false so this getState function                                          is only called for onne instant
    }
    if (isStratPresetButtonPressed == 1)
    {
        getStateInformation(stratPresetData);
        isStratPresetButtonPressed = 0;
    }
        
    float currentMessageDataByte = midiProcessor.process(midiMessages);
     // pass midi messages to process function only at start of every buffer. Do not need to process messages every sample
    isMidiMessageRecieved = midiProcessor.getMessageRecieved();
    if (isMidiMessageRecieved == 1)           // this variable is set to one in process function if there is                                          a midi message being processed
    {
        if (currentMessageDataByte == 0)      // returned from midiprocess function
        {
            setStateInformation(acousticPresetData.getData(), acousticPresetData.getSize());
            //.getData() returns required void pointer to data
            std::cout << "Acoustic Guitar detected\n\n";
            
        }
        if (currentMessageDataByte == 1)
        {
            setStateInformation(stratPresetData.getData(), stratPresetData.getSize()); 
            std::cout << "Electric Guitar detected\n\n";
        }
        midiProcessor.setMessageComplete();     // returns first 'if' statment to false so that                                                     "setStateInformation" is only called when a new MIDI                                             Program change message is recieved
    }
   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //============================================================================================
    //Set new Parameters at start of each buffer
    
    analogDelayLine->setFeedback(*feedbackParameter);
    analogDelayLine->setDrive(*driveParameter);
    analogDelayLine->setDistortionLevels(*breakUpParameter);
    analogDelayLine->setFilterCoeff(*cutOffParameter);
    analogDelayLine->isMultiTapOn(*multiTapButtonState);
    analogDelayLine->setMultiTapIntensity(*multiTapIntensityParameter);
    
    auto delaySamples = (*delayTimeParameter * getSampleRate());
    auto multiTapDelaySamples = delaySamples + (delaySamples * (*multiTapParameter));
   
    float currentDelay = stepResponse(delaySamples, delayPrevious, 0.95);
    float currentMultiTapDelay = stepResponse(multiTapDelaySamples, multiTapDelayPrevious, 0.95);
    if (currentDelay != delaySamples)
        analogDelayLine->setDelaySamples(currentDelay, currentMultiTapDelay);

    //============================================================================================
   
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
        auto* channelData = buffer.getWritePointer (channel);
      
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
                {
                    channelData[sample] = analogDelayLine->delayProcess(channelData[sample], channel);
                }
            delayPrevious = currentDelay;
            multiTapDelayPrevious = currentMultiTapDelay;
        }
}

//==============================================================================
bool Analog_Delay_AP2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Analog_Delay_AP2AudioProcessor::createEditor()
{
    return new Analog_Delay_AP2AudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Analog_Delay_AP2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
        std::unique_ptr<juce::XmlElement> xml (state.createXml());
        copyXmlToBinary(*xml, destData);
}

void Analog_Delay_AP2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
        if (xmlState.get() != nullptr)
            if (xmlState->hasTagName(parameters.state.getType()))
                parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Analog_Delay_AP2AudioProcessor();
}
