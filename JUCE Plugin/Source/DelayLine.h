/*
  ==============================================================================

    DelayLine.h
    Created: 28 Jan 2022 1:30:30pm
    Author:  James Maddigan

  ==============================================================================
*/

#pragma once


#include "Distortion.h"
#include "Filter.h"

class Delay
{
public:
    Delay();
    ~Delay();
    
    void clearBuffer();
    void clearIndex();
    
    void setSampleRate (float sampleRate);
    void setDelaySamples (float currentDelayParameter, float multiTapRate);
    void setFeedback (float newValue);
    void setDrive (float newValue);
    void isMultiTapOn (int multiTapParameter);
    void setMultiTapIntensity (float newTapIntensity);
    
    void setDistortionLevels (float breakUp);
    void setFilterCoeff (float cutOffFreq);

    float delayProcess(float audioSample, int channel);
    
private:
    const int bufferLength = 96000;
    float buffer[96000][2];
    float delaySamples;
    float multiTapDelaySamples;
    int floorDelaySample;
    int ceilDelaySample;
    float delaySampleFraction;
    float feedback;
    const int numChannels = 2;
    
    float interpolatedDelay (float readIndex, int channel);
 
    int writeIndex[2] = { 0, 0 };
    int rIndex;
    int multTaprIndex;
    float readIndex[2] = {0,0};
    float multiTapReadIndex[2] = {0,0};
    float drive = 1;
    float multiTapInensity;
    void writeBuffer (float audioSample, int channel);
    float readBuffer (int channel);
    float multiTapRead (int channel);
    float sampleRate;
    bool multiTapState = false;
    
    

    Distortion distortion;
    LowPassFilter delayLPF_L;
    LowPassFilter delayLPF_R;
};





