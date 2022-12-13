/*
  ==============================================================================

    DelayLine.cpp
    Created: 28 Jan 2022 1:30:37pm
    Author:  James Maddigan

  ==============================================================================
*/
#include "DelayLine.h"
#include <math.h>

Delay::Delay()  {}
Delay::~Delay() {}

void Delay::clearBuffer()
{
    for (int n = 0; n < numChannels; n++)
    {
        for (int m = 0; m < bufferLength; m++)
        {
            buffer[m][n] = 0;
        }
    }
    clearIndex();
}

void Delay::clearIndex()
{
    for (int n = 0; n < numChannels; n++)
        {
            readIndex[n] = 0;
        }
    
}

//======================================================================================================
// Parameter Setters

void Delay::setSampleRate (float newSampleRate)
{
    sampleRate = newSampleRate;
}

void Delay::setDistortionLevels(float breakUp)
{
    distortion.setdistortionAmount(breakUp);
}

void Delay::setFeedback (float newValue)
{
    feedback = newValue*0.8;
}

void Delay::setDelaySamples (float currentDelayParameter, float multiTapRate)
{
    delaySamples = currentDelayParameter;
    multiTapDelaySamples = multiTapRate;
    
}

void Delay::isMultiTapOn (int multiTapParameter)
{
    if (multiTapParameter == 1)
        multiTapState = true;
    
    else if (multiTapParameter == 0)
        multiTapState = false;
}

void Delay::setMultiTapIntensity (float newTapIntensity)
{
    multiTapInensity = newTapIntensity;
}

float Delay::interpolatedDelay (float readIndex, int channel)
{
    floorDelaySample = floor(readIndex);
    int floorDelaySampleMinus1 = floorDelaySample - 1;
    if (floorDelaySampleMinus1 < 0)
        floorDelaySampleMinus1 += bufferLength;
    ceilDelaySample = ceil(readIndex);
    int ceilDelaySamplePlus1 = ceilDelaySample + 1;
    if (ceilDelaySamplePlus1 > bufferLength)
        ceilDelaySamplePlus1 -= bufferLength;
    delaySampleFraction = readIndex - floorDelaySample;
    
   float delaySampleM2 = buffer[ceilDelaySamplePlus1][channel];
    float delaySampleM1 = buffer[ceilDelaySample][channel];
    float delaySampleM = buffer[floorDelaySample][channel];
    float delaySampleMminus1 = buffer[floorDelaySampleMinus1][channel];
    
    float a0 = delaySampleM2 - delaySampleM1 - delaySampleMminus1 + delaySampleM;
    float a1 = delaySampleMminus1 - delaySampleM - a0;
    float a2 = delaySampleM1 - delaySampleMminus1;
    float a3 = delaySampleM;
    
    float cubicInterpoation = (a0 * pow(delaySampleFraction, 3)) + (a1 * pow(delaySampleFraction, 2)) + (a2 * delaySampleFraction) + a3;

  // float interpolatedValue = ((1 - delaySampleFraction) * buffer[floorDelaySample][channel]) + (delaySampleFraction * buffer[ceilDelaySample][channel]);
    
    return cubicInterpoation;
}


void Delay::setDrive (float newValue)
{
    drive = newValue;
}

void Delay::setFilterCoeff (float cutOffFreq)
{
    delayLPF_L.setFilterCoeff(cutOffFreq, sampleRate);
    delayLPF_R.setFilterCoeff(cutOffFreq, sampleRate);
}

//======================================================================================================


void Delay::writeBuffer(float inputSample, int channel)
{
    
    inputSample *= drive;    // can set a drive value for the first repeat (written to write position)
    if (channel == 0)
    {
        buffer[writeIndex[channel]][1] = distortion.process(delayLPF_L.process(inputSample + (feedback * (readBuffer(channel)))));
    }
    if (channel == 1)
    {
        buffer[writeIndex[channel]][0] = distortion.process(delayLPF_R.process(inputSample + (feedback * (readBuffer(channel)))));
    }
    writeIndex [channel] ++;
    if (writeIndex [channel] == bufferLength)
        writeIndex [channel] = 0;
}

float Delay::readBuffer(int channel)
{
    rIndex = writeIndex[channel] - delaySamples;
        if (rIndex < 0)
            rIndex += bufferLength;
    
    return  interpolatedDelay(rIndex, channel);;
}
float Delay::multiTapRead(int channel)
{
    multTaprIndex = writeIndex[channel] - multiTapDelaySamples;
        if (multTaprIndex < 0)
            multTaprIndex += bufferLength;
    
    return interpolatedDelay(multTaprIndex, channel);
}

float Delay::delayProcess(float input, int channel)
{

    writeBuffer(input, channel);
    
    if (multiTapState == true)
        {
                return input + readBuffer(channel) + (multiTapInensity * multiTapRead(channel));
        }
    else if (multiTapState == false)
        {
                return input + readBuffer(channel);
        }
}


 
