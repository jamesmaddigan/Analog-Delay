/***** Distortion.h *****/
#pragma once

 class Distortion
 {

 public:
     float process (float input);

     void setdistortionAmount (float distortionAmount);

 private:
     float alpha;
 };