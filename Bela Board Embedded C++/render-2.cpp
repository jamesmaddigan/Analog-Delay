#include <Bela.h>
#include <stdlib.h>
#include <cmath>
#include <DelayLine.h>
#include "AudioUtilities.h"
#include <Preset.h>
#include <libraries/Gui/Gui.h>
#include <libraries/Scope/Scope.h>
#include <libraries/GuiController/GuiController.h>
#include <libraries/Midi/Midi.h>
#include <cstdlib>


Gui gui;
GuiController controller;
Scope scope;
Delay analogDelay; 
Midi gMidi;                      //Midi Object
GuitarPreset acousticGuitar;
GuitarPreset stratocaster;

const char* gMidiPort0 = "hw:0,0,0";    //create port with name gMidiPort0 assigned to port you want to use. List all ports by typing amidi -l into command line
float gSampleRate;
int isMultiTap {0};
float multiTapIntensity {1.f};   //set intensity between 0.1 and 3 for second tap amplidude
float multiTapParameter {0.25f};  // set multi tap parameter between 0.1 and 0.5 to change distance from initial repeat
float feedbackParameter {0.5f};  // set feedback between 0.01 and 1 (for infinite feedback delay)
float delayTime {0.3f};          // set delay time between 0.05 and 0.6 seconds (for analog delay replication)
float driveParameter {1.5f};     // set drive to between 0.5 and 1.5 for initial repeat Amplitude
float distortionLevel {1.5f};   // set between 1 and 2 for distortion of repeats
float cutOffFreq {1000.f};      // set cutoff frequency for LPF between 200 hz and 2500hz
float delaySamples;
float multiTapDelaySamples;
float delayPrevious;
float multiTapDelayPrevious;

int gMidiMessageForPresetRecieved {0};
int gChooseAnalogControls {0};
midi_byte_t program {};

float input0 {};
float input1 {};
float input2 {};
float input3 {};
float input4 {};
float input5 {};
float input6 {};



float previousInput0 {};
float previousInput1 {};
float previousInput2 {};

float midiFeedback {};
float midiDelayTime {};
float midiDrive {};

float previousFeedback {};
float previousDelayTime {};
float previousDrive {};

 
int gButton = 3; // digital pin 1 - check the pin diagram in the IDE
int gAcousticLED = 0; // digital pin 0 - check the pin diagram in the IDE
int gStratLED = 1;
int gMultiTapButtton = 2;
enum
{
	kAcousticProgram = 0,
	kElectricProgram = 1,
};




bool setup(BelaContext *context, void *userData)
{

	gMidi.readFrom(gMidiPort0);    //initialise settings in setup function
	gMidi.writeTo(gMidiPort0);
	gMidi.enableParser(true);
	
	// Set the mode of digital pins
	pinMode(context, 0, gButton, INPUT); //set input
	pinMode(context, 0, gAcousticLED, OUTPUT); // setoutput
	pinMode(context, 0, gStratLED, OUTPUT); // setoutput
	
	gChooseAnalogControls = 1;

	previousFeedback = 0;
	previousDelayTime = 0;
	previousDrive = 0;
	

	
	analogDelay.isMultiTapOn(isMultiTap);
	analogDelay.setSampleRate(context->audioSampleRate);
	analogDelay.setMultiTapIntensity(multiTapIntensity);
	analogDelay.clearBuffer();
	analogDelay.setFeedback(feedbackParameter);
	delaySamples = (delayTime * (context->audioSampleRate));
	multiTapDelaySamples = delaySamples + (delaySamples * multiTapParameter);
	analogDelay.setDrive(driveParameter); 
	analogDelay.setDistortionLevels(distortionLevel);
	analogDelay.setFilterCoeff(cutOffFreq);
	
	delayPrevious = delaySamples;                              // initialise previous delay sample for step function
    multiTapDelayPrevious = multiTapDelaySamples;
    gMidiMessageForPresetRecieved = 0;
    
    acousticGuitar.setParameters(0, 1.2, 0.5, 0.4, 0.3, 1, 1, 1000);
    stratocaster.setParameters(1, 1, 0.25, 0.75, 0.6, 1.25, 2, 1500);
	
    return true;
}

void render(BelaContext *context, void *userData)
{
	
	while(gMidi.getParser()->numAvailableMessages() > 0)         // if statement to query if there are any new midi messages
	{
		MidiChannelMessage message;                              // if there is at least 1 new midi messsage available, create new object of type MidiChannelMessage
		message = gMidi.getParser()->getNextChannelMessage();    // call this function to make object 'message' equal to the next available message
		message.prettyPrint();                                   // call method to print message to the terminal (for debugging purposes)
	
	
		if(message.getType() == kmmProgramChange)
		{
			program = message.getDataByte(0);                // take data byte of message (which program to change to)
			gChooseAnalogControls = 0;                           // Analog controls statement bypassed
			gMidiMessageForPresetRecieved = 1;
			//message.prettyPrint();
			//rt_printf("%f\nNew Program = ", program);
		}
	
	}
	
	
	
   	// This for() loop goes through all the samples in the block
	for (unsigned int n = 0; n < context->audioFrames; n++)
		{
			float out_l = 0;
    		float out_r = 0;
			
			
			int status = digitalRead(context, n, gButton);  										 // status of the button, if status == Low then button pressed
			int isMultiTapStatus = digitalRead(context, n, gMultiTapButtton);
		
    		input0 = analogRead(context, n/2, 0);    
    		input1 = analogRead(context, n/2, 1);
    		input2 = analogRead(context, n/2, 2);
    		input3 = analogRead(context, n/2, 3);
    		input4 = analogRead(context, n/2, 4);
    		input5 = analogRead(context, n/2, 5);
    		input6 = analogRead(context, n/2, 6);
    		
    		// use button function to turn off preset mode and return midiMessageForPresetPerformed and midiMessageForPresetRecieved variable back to 0 for next time CNN is run
    		// another button to save current state as preset
    		
    		
    		if (gMidiMessageForPresetRecieved == 1) 
    		{
    			if (program == 0.f)
    			{
    				feedbackParameter = acousticGuitar.getFeedback();
    				delayTime = acousticGuitar.getDelayTime();
    				driveParameter = acousticGuitar.getDrive();
    				distortionLevel = acousticGuitar.getDistortion();
    				cutOffFreq = acousticGuitar.getCutoff();
    				multiTapParameter = acousticGuitar.getMultiTapParameter();
    				multiTapIntensity = acousticGuitar.getMultiTapIntensity();
    				
    				digitalWrite(context, n, gAcousticLED, HIGH);
    				digitalWrite(context, n, gStratLED, LOW);
    			}
    			else if(program == 1.f)
    			{
    				feedbackParameter = stratocaster.getFeedback();
    				delayTime = stratocaster.getDelayTime();
    				driveParameter = stratocaster.getDrive();
    				distortionLevel = stratocaster.getDistortion();
    				cutOffFreq = stratocaster.getCutoff();
    				multiTapParameter = stratocaster.getMultiTapParameter();
    				multiTapIntensity = stratocaster.getMultiTapIntensity();
    				
    				
    				digitalWrite(context, n, gStratLED, HIGH);
    				digitalWrite(context, n, gAcousticLED, LOW);
    			}
    			
    			gMidiMessageForPresetRecieved = 0;                      									// will only perform if statement once to change parameters, which means less processing needed per audio frame
    		}
    		
    		if (gChooseAnalogControls == 1)
    		{
    																										// if midi message recieved, skip this step of taking parameters from analog inputs and use presets instead
    		
    				feedbackParameter = map(input0, 0, 3.3/4.096, 0.1, 1);
    				delayTime = map(input1, 0, 3.3/4.096, 0.1, 0.6);
    				if (delayTime < 0.1)
    				delayTime = 0.1;
    				else if (delayTime >0.6)
    				delayTime = 0.6;
    				delayTime = ceil(delayTime * 100)/100;
    				driveParameter = map(input2, 0, 3.3/4.096, 0.5, 2);
    				distortionLevel = map(input3, 0, 3.3/4.096, 1, 2);
    				cutOffFreq = map(input4, 0, 3.3/4.096, 200, 2000);
    				multiTapParameter = map(input5, 0, 3.3/4.096, 0.05, 0.4);
    				multiTapParameter = ceil(multiTapParameter * 50)/50;
    				multiTapIntensity = map(input6, 0, 3.3/4.096, 0.25, 3);
    				if (isMultiTapStatus == HIGH)
    				{
    					isMultiTap = 1;
    				}
    				
    				else if (isMultiTapStatus == LOW)
    				{
    					isMultiTap = 0;
    				}
    		}
    		
    		
    		analogDelay.isMultiTapOn(isMultiTap);
    		analogDelay.setMultiTapIntensity(multiTapIntensity);
    		analogDelay.setFeedback(feedbackParameter);                   // parameter sent to delay line either from calling preset value if midi message recieved, or from analog control
    		analogDelay.setDrive(driveParameter); 
			analogDelay.setDistortionLevels(distortionLevel);
			analogDelay.setFilterCoeff(cutOffFreq);
			delaySamples = (delayTime * (context->audioSampleRate));
			multiTapDelaySamples = delaySamples + (delaySamples * multiTapParameter);
			
			float currentDelay = delaySamples;
    		float currentMultiTapDelay = multiTapDelaySamples;
    		
        	analogDelay.setDelaySamples(currentDelay, currentMultiTapDelay);
     	
        	if (status == LOW)
    		{
    			gChooseAnalogControls = 1;      //resets midi message recieved variable to 0 so will take analog inputs
    			digitalWrite(context, n, gStratLED, LOW);
    			digitalWrite(context, n, gAcousticLED, LOW);
    		}
        
        	// Read audio inputs
    		 out_l = audioRead(context,n,0);
    		 out_r = audioRead(context,n,0);
    		 
    		 out_l = analogDelay.delayProcess(out_l, 0);
    		 out_r = analogDelay.delayProcess(out_r, 0);    // mono input, take left channel
    		 
    		 audioWrite(context, n, 0, out_l);
    		 audioWrite(context, n, 1, out_r);
    		 
    		
    		
    }
    		// rt_printf("feedbakc: %f\n", feedbackParameter);
			 previousFeedback = feedbackParameter;
    		 previousDelayTime = delayTime;
    		 previousDrive = driveParameter;
    		 previousInput0 = input0;
    		 previousInput1 = input1;
    		 previousInput2 = input2;
    		 
}

// cleanup() runs once at the end of the program
void cleanup(BelaContext *context, void *userData)
{
	// Nothing to do here
}