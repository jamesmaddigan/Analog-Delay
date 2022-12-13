/***** Preset.cpp *****/

#include "Preset.h"

void GuitarPreset::setParameters(int newIsMultiTap, float newMultiTapIntensity, float newMultiTapParameter, float newFeedback, float newDelayTime, float newDrive, float newDistortion, float newCutoffFreq)
	{
		gIsMultiTap = newIsMultiTap;
		gMultiTapIntensity = newMultiTapIntensity;
		gMultiTapParameter = newMultiTapParameter;
		gFeedbackParameter = newFeedback;
		gDelayTime = newDelayTime;
		gDriveParameter = newDrive;
		gDistortionLevel = newDistortion;
		gCutOffFreq = newCutoffFreq;
	}
	
int GuitarPreset::getIsMultiTap()
	{
		return gIsMultiTap;
	}
	
float GuitarPreset::getMultiTapIntensity()
	{
		return gMultiTapIntensity;
	}
	
float GuitarPreset::getMultiTapParameter()
	{
		return gMultiTapParameter;
	}
float GuitarPreset::getFeedback()
	{
		return gFeedbackParameter;
		rt_printf("STRAT");
	
	}
float GuitarPreset::getDelayTime()
	{
		return gDelayTime;
	}
float GuitarPreset::getDrive()
	{
		return gDriveParameter;
	}
float GuitarPreset::getDistortion()
	{
		return gDistortionLevel;
	}
float GuitarPreset::getCutoff()
	{
		return gCutOffFreq;
	}