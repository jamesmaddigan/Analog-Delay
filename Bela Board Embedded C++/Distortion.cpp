/***** Distortion.cpp *****/

#include "Distortion.h"
 #include <math.h>

 float Distortion::process(float input)
 {
     float output = 0.4 * (atan(alpha * input)/atan(alpha));
    return output;
 }

 void Distortion::setdistortionAmount(float DistortionAmount)
 {
     alpha = DistortionAmount * 2;
 }