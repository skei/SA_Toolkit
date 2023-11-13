#ifndef sat_windowing_included
#define sat_windowing_included
//----------------------------------------------------------------------

#include <math.h>

/*
float SAT_Windowing_Rect(float pos);
float SAT_Windowing_Hann(float pos);
float SAT_Windowing_Hamming(float pos);
float SAT_Windowing_Blackman(float pos);
float SAT_Windowing_Gaussian(float pos);
float SAT_Windowing_Welch(float pos);
float SAT_Windowing_Bartlett(float pos);
*/

//----------------------------------------------------------------------

float SAT_Windowing_Rect(float pos) {
  return 1.0;
}

float SAT_Windowing_Hann(float pos) {
  float x = cosf(pos*SAT_PI/2.0);
  return x*x;
}

float SAT_Windowing_Hamming(float pos) {
  return 0.54 + 0.46 * cosf(SAT_PI*pos);
}

float SAT_Windowing_Blackman(float pos) {
  return 0.42 + 0.5 * cos(SAT_PI*pos) + 0.08 * cosf(2.0*SAT_PI*pos);
}

float SAT_Windowing_Gaussian(float pos) {
  float a = 4.0;
  float ax = a * pos;
  return exp(-0.5 * ax*ax);
}

float SAT_Windowing_Welch(float pos) {
  return 1.0 - pos*pos;
}

float SAT_Windowing_Bartlett(float pos) {
  if (pos == 0.0) return 1.0;
  else if (pos > -1.0 && pos < 0.0) return pos + 1.0;
  else if (pos > 0.0 && pos < 1.0) return 1.0 - pos;
  else return 0.0;
}

//----------------------------------------------------------------------
#endif
