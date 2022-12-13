/*
  ==============================================================================

    FIlter.h
    Created: 28 Jan 2022 1:31:29pm
    Author:  James Maddigan

  ==============================================================================
*/

#pragma once
#include <math.h>
class LowPassFilter
{
public:
    void setFilterCoeff(float newCutOffFreq, float sampleRate);
    float process (float input);
    
private:
    float xz1 = 0;
    float xz2 = 0;
    float yz1 = 0;
    float yz2 = 0;
    
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;
    
    float sampleRate;
    float w0;
    float Q = 1/sqrt(2);
    float alpha;

};
