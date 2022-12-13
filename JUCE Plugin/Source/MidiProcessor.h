
#pragma once

#include "JuceHeader.h"
#include <JuceHeader.h>
#include <iostream>
class MidiProcessor
{
public:
    float process(juce::MidiBuffer& midiMessages)
    {
        juce::MidiBuffer::Iterator it(midiMessages);
        juce::MidiMessage currentMessage;
        int samplePosition;
        
        while(it.getNextEvent(currentMessage, samplePosition))
        {
            if (currentMessage.isProgramChange())
            {
                const auto midiData = currentMessage.getRawData();
                dataByte = midiData [1];
                messageRecieved = 1;
            }
        }
        return dataByte;
    }
    
    int getMessageRecieved ()
    {
        return messageRecieved;
    }
    
    void setMessageComplete ()
    {
        messageRecieved = 0;
    }
    
private:
    
    float dataByte {};
    int messageRecieved {};
}; 
