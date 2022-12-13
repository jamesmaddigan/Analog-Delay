/***** AudioUtilities.h *****/
#pragma once

 float stepResponse (float input, float previous_input, float alpha)
     {
         return (1 - alpha) * input + alpha * previous_input;
     }