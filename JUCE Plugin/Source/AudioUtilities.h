/*
  ==============================================================================

    AudioUtilities.h
    Created: 29 Jan 2022 12:18:25pm
    Author:  James Maddigan

  ==============================================================================
*/

#pragma once

auto stepResponse (float input, float previous_input, float alpha)
    {
        return (1 - alpha) * input + alpha * previous_input;
    }
