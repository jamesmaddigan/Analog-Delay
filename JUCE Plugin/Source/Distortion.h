/*
  ==============================================================================

    Distortion.h
    Created: 28 Jan 2022 1:31:08pm
    Author:  James Maddigan

  ==============================================================================
*/

#pragma once

class Distortion
{
    
public:
    float process (float input);
    
    void setdistortionAmount (float distortionAmount);
    
private:
    float alpha;
};
