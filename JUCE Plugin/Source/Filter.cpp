/*
  ==============================================================================

    Filter.cpp
    Created: 28 Jan 2022 1:31:36pm
    Author:  James Maddigan

  ==============================================================================
*/

#include "Filter.h"
#include <math.h>

void LowPassFilter::setFilterCoeff(float newCutOffFreq, float sampleRate)
{
    w0 = (2*M_PI*newCutOffFreq)/sampleRate;
    
    alpha = sin(w0)/(2*Q);
    
    a0 = 1 + alpha;
    b0 = (1 - cos(w0))/2;
    a1 = (-2)*cos(w0);
    b1 = 1 - cos(w0);
    a2 = 1 - alpha;
    b2 = (1 - cos(w0))/2;
    
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
    a0 /= a0;
    
}

float LowPassFilter::process(float input)
{
    float output = (b0*input) + (b1*xz1) + (b2*xz2) - (a1*yz1) - (a2*yz2);
    
    xz2 = xz1;
    xz1 = input;
    
    yz2 = yz1;
    yz1 = output;
    
    return output;
}




