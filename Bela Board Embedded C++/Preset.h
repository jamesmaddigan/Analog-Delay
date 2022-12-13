/***** Preset.h *****/
#pragma once

#include <Bela.h>
#include <stdlib.h>
#include <cstdlib>

class GuitarPreset 
{
	public:
	
	void setParameters(int newIsMultiTap, float newMultiTapIntensity, float newMultiTapParameter, float newFeedback, float newDelayTime, float newDrive, float newDistortion, float newCutoffFreq);
	int getIsMultiTap ();
	float getMultiTapIntensity();
	float getMultiTapParameter();
	float getFeedback();
	float getDelayTime();
	float getDrive();
	float getDistortion();
	float getCutoff();
	
	private:
	
		int gIsMultiTap {};
		float gMultiTapIntensity {};   //set intensity between 0.1 and 3 for second tap amplidude
		float gMultiTapParameter {};  // set multi tap parameter between 0.1 and 0.5 to change distance from initial repeat
		float gFeedbackParameter {};  // set feedback between 0.01 and 1 (for infinite feedback delay)
		float gDelayTime {};          // set delay time between 0.05 and 0.6 seconds (for analog delay replication)
		float gDriveParameter {};     // set drive to between 0.5 and 1.5 for initial repeat Amplitude
		float gDistortionLevel {};   // set between 1 and 2 for distortion of repeats
		float gCutOffFreq {};      // set cutoff frequency for LPF between 200 hz and 2500hz
};