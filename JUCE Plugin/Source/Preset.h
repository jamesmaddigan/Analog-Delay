/*
  ==============================================================================

    Preset.h
    Created: 29 May 2022 1:39:09pm
    Author:  James Maddigan

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class Preset
{
public:
    
    void setPresetParameters (int newIsMultiTap, float newMultiTapParameter, float newMultiTapIntensity, float newDelayTime, float newFeedBack, float newDrive, float newDistortion, float newCutOffFreq)
    {
        isMultiTap = newIsMultiTap;
        multiTapParameter = newMultiTapParameter;
        multiTapIntensity = newMultiTapIntensity;
        delayTime = newDelayTime;
        feedback = newFeedBack;
        drive = newDrive;
        distortion = newDistortion;
        cutOffFreq = newCutOffFreq;
    }
    
    
    int getMultiTap ()
    {
        return isMultiTap; 
    }
    float getMultiTapParameter ()
    {
        return multiTapParameter;
    }
    float getMultiTapIntensity ()
    {
        return multiTapIntensity;
    }
    float getDelayTime ()
    {
        return delayTime;
    }
    float getFeedBack ()
    {
        return feedback;
    }
    float getDrive ()
    {
        return drive;
    }
    float getDistortion ()
    {
        return distortion;
    }
    float getCutOffFreq ()
    {
        return cutOffFreq;
    }
    
    
private:
    int isMultiTap {};
    float multiTapParameter {};
    float multiTapIntensity {};
    float delayTime {};
    float feedback {};
    float drive {};
    float distortion {};
    float cutOffFreq {};
    
    
    
};
