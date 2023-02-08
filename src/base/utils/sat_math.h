#ifndef sat_math_included
#define sat_math_included
//----------------------------------------------------------------------

#include <math.h>
#include <stdlib.h> // abs

#include "base/sat.h"

//----------------------------------------------------------------------

//bool SAT_AlmostEqual(float f1, float f2) {
//  float diff = fabs(f2 - f1);
//  if (diff < SAT_VERY_TINY) return true;
//  return false;
//}

bool SAT_AlmostEqual(float f1, float f2, float tol) {
  float diff = fabs(f2 - f1);
  if (diff < tol) return true;
  return false;
}

//----------
/*
  calculate the average value of a set of floats
  example:
    float ar[] = { -1.f, 2.f, 3.f, 4.f, 5.f };
    float result = SAverage(5,ar); // result is 2.6f
  n - number of elements (n)
  ar - array of floats
*/

float SAT_Average(const unsigned int n, const float* ar) {
  float total = 0;
  unsigned int i=0;
  while (i<n) {
    total += ar[i];   i++;
  }
  return total/n;
}

//----------

// limits a floating point number to [-limit, limit]

float SAT_Clamp(const float input, const float limit) {
  const float _t = (input > -limit) ? input : -limit;
  //return (_t > limit) ? _t : limit;
  return (_t < limit) ? _t : limit;
}

//----------

// limits a floating point number to [-limit, limit]

float SAT_Clamp(const float input, const float minval, const float maxval ) {
  const float _t = (input > minval) ? input : minval;
  return           (_t    < maxval) ? _t : maxval;
}

//----------

float SAT_Clip(float x, float l, float h) {
  return  (abs(x-l) + (l+h) - abs(x-h)) * 0.5;
}

//----------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=2049788#p2049788

  't' is a scaling variable - 0.5 gives you a straight line output (for
  straight line input) > 0.5 and the curve gets logarithmic-like, < 0.5
  and it gets exponential-like.
*/

/*
  see also:

  http://www.kvraudio.com/forum/viewtopic.php?p=3890342#p3890342
  For arbitrary shapes of x, between 0..1, try this:
    y = x / (x + (1 - (1/shape)) * (x - 1));
  where:
    shape = 0.5 -> lin
    shape > 0.5 -> log
    shape < 0.5 -> exp
  Just plot it in Excel or so to get the hang of it.
  http://www.kvraudio.com/forum/viewtopic.php?p=3890396#p3890396
  man, that is such a cool equation. Has it got a name? Thanks.
  Just need to invert it and add a scale multiplier.
    y = 1 + (-cx / (cx + (1 - (1/shape)) * (cx - 1)));
*/

//return x / (x+(1-1/t)*(x-1));

float SAT_Curve(float x, float t) {
  if (t < 0.001) t = 0.001;
  if (t > 0.999) t = 0.999;
  //t = (t*0.990f) + 0.005f;
  float a = ( 1.0f - 1.0f/t );
  return x / (x+a * (x-1) );
}


//----------

/*
  returns the fractional part of a floating point number
  (and stores the integer part in the second argument)

  example:
    value_pi = 3.141592;
    float integer;
    float fraction = SFract(value_pi, &integer);
    // fraction = 0.141592, integer = 3

  value - input variable
  intpart - pointer to integer part variable
  return fractional part
*/

/*
  SAT_Fract(0.1)   =  0.1
  SAT_Fract(0.9)   =  0.9
  SAT_Fract(1.1)   =  0.1
  SAT_Fract(1.9)   =  0.9
  SAT_Fract(-0.1)  = -0.1
  SAT_Fract(-0.9)  = -0.9
  SAT_Fract(-1.1)  = -0.1
  SAT_Fract(-1.9)  = -0.9
*/

float SAT_Fract(const float value) {
  float intpart = (float)(int)value;
  return (value - intpart);
}

//----------

float SAT_Fract(const float value, float* intpart) {
  *intpart = (float)(int)value;
  return (value - *intpart);
}

//----------

float SAT_Max(const float a, const float b) {
  return (a > b) ? a : b;
}

//----------

// returns the smaller of two floating point numbers

float SAT_Min(const float a, const float b) {
  return (a < b) ? a : b;
}

//----------

// http://music.columbia.edu/pipermail/music-dsp/2002-July/049893.html
//
// there is an ANSI function (math.h):
// double fmod(double x,double y)
// returns the floating-point remainder of x / y. Or NaN if y==0.0.

// http://music.columbia.edu/pipermail/music-dsp/2002-July/049899.html
//
// Use fmod() rather than % for floating point values.
// Or use something like this:
//    fractional = phase / strive; // shouldn't this be "stride" ?
//    fractional -= floorf( fractional );
// Which is what you ended up with, but using an explicit floating
// point floorf() (which usually gets intrinsic-ed) rather than the
// casts.

//----------

float SAT_Modulo(float x, float y) {
  return x - (y * float(floor(x / y)));
}

//----------

/*
  AValue = value to quantize (0..1)
  ASteps = number of steps

*/

//float SAT_Trunc(const float v); // see below

float SAT_Quantize(float AValue, int32_t ASteps) {
  if (ASteps < 2) return AValue;
  if (ASteps == 2) {
    if (AValue < 0.5f) return 0.0f;
    else return 1.0f;
  }
  float v = AValue * (float)(ASteps/*-1*/);
  //int32_t i = SAT_Trunc(v);
  float i = floorf(v);

  //SAT_Print("i %f\n",i);

  //int i = (int)v;
  float res = i / (float)(ASteps);
  return SAT_Clamp(res,0,1);
}

//----------

/*
  calculate the RMS of a set (array) of float numbers

  example:
    float ar[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
    float result = SRMS(5, ar);   // result = 3.31662

  n - size of the array
  ar - array of floats
*/

float SAT_Rms(const unsigned int n, const float* ar) {
  float numr = 0;
  unsigned int i=0;
  while (i<n) {
    numr += ar[i]*ar[i];
    i++;
  }
  return sqrtf(numr/n);
}

// rounds a floating point number

/*
  SAT_Round(0.1)   =  0.0
  SAT_Round(0.9)   =  1.0
  SAT_Round(1.1)   =  1.0
  SAT_Round(1.9)   =  2.0
  SAT_Round(-0.1)  = -0.0
  SAT_Round(-0.9)  = -1.0
  SAT_Round(-1.1)  = -1.0
  SAT_Round(-1.9)  = -2.0
*/

float SAT_Round(const float v) {
  if (v < 0.f) return (float)(int)(v - 0.5f);
  else return (float)(int)(v + 0.5f);
}

//----------

/*
  SAT_Sigmoid(0.0)   //  0
  SAT_Sigmoid(0.1)   //  0.1495
  SAT_Sigmoid(0.3)   //  0.4365
  SAT_Sigmoid(0.5)   //  0.5875
  SAT_Sigmoid(0.7)   //  0.8785
  SAT_Sigmoid(0.9)   //  0.9855
  SAT_Sigmoid(1.0)   //  1
*/


float SAT_Sigmoid(float x) {
  if ( abs(x) < 1.0f ) return x * (1.5f - 0.5f * x * x);
  else {
    if (x < 0.0f) return -1.0f;
    else return 1.0f;
  }
}


//----------

/*
//https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
*/

/*
  SAT_Sign(0.5)  = 1
  SAT_Sign(1.5)  = 1
  SAT_Sign(-0.5) = -1
  SAT_Sign(-1.5) = -1
*/

float SAT_Sign(const float v, const float def=0.0f) {
  //union {
  //  int32_t i;
  //  float f;
  //} u;
  //u.f = v;
  //return (1 | (u.i >> 31));
  if (v < 0.0f) return -1.0f;
  else if (v > 0.0f) return 1.0f;
  else return def;
}

//----------

/*
  0.0 = 1
  0.1 = 0.9836
  0.3 = 0.8584
  0.5 = 0.6366
  0.7 = 0.3678
  0.9 = 0.1093
  1.0 = 0
*/

//double SAT_Sinc(double x) {
//  if (x==0) return 1;
//  else {
//    double pix = SAT_PI * x;
//    return sin(pix) / pix;
//  }
//}

float SAT_Sinc(float x) {
  if (x==0) return 1;
  else {
    float pix = SAT_PI * x;
    return sin(pix) / pix;
  }
}


//----------

float SAT_Sqr(float x) {
  return x*x;
}

//----------

/*
  SAT_Trunc(0.1)   =  0.0
  SAT_Trunc(0.9)   =  0.0
  SAT_Trunc(1.1)   =  1.0
  SAT_Trunc(1.9)   =  1.0
  SAT_Trunc(-0.1)  =  0.0
  SAT_Trunc(-0.9)  =  0.0
  SAT_Trunc(-1.1)  = -1.0
  SAT_Trunc(-1.9)  = -1.0
*/

//----------

float SAT_Trunc(const float v) {
  return (float)(int)v;
}

//----------

float SAT_Wrap(float v, float lo, float hi) {
  if (lo > hi) {
    //SAT_Swap(lo,hi);
    float temp = lo;
    lo = hi;
    hi = temp;
  }
  v -= lo; // adjust to 0
  float range = hi - lo;
  if (range == 0) { return hi; } // avoid dividing by 0
  return v - (range * floor(v/range)) + lo;
}

//----------------------------------------------------------------------


// https://github.com/Ixox/preenFM2/blob/master/src/third/wirish_math.h

/*
  Remap a number from one range to another.
  That is, a value equal to fromStart gets mapped to toStart, a value
  of fromEnd to toEnd, and other values are mapped proportionately.
  Does not constrain value to lie within [fromStart, fromEnd].
  If a "start" value is larger than its corresponding "end", the
  ranges are reversed, so map(n, 1, 10, 10, 1) would reverse the
  range [1,10].
  Negative numbers may appear as any argument.
  value     the value to map.
  fromStart the beginning of the value's current range.
  fromEnd   the end of the value's current range.
  toStart   the beginning of the value's mapped range.
  toEnd     the end of the value's mapped range.
  return    the mapped value.
*/

//int32_t SAT_Remap(int32_t value, int32_t fromStart, int32_t fromEnd, int32_t toStart, int32_t toEnd) {
//  return (value - fromStart) * (toEnd - toStart) / (fromEnd - fromStart) + toStart;
//}

//#define PI          3.1415926535897932384626433832795
//#define HALF_PI     1.5707963267948966192313216916398
//#define TWO_PI      6.283185307179586476925286766559
//#define DEG_TO_RAD  0.017453292519943295769236907684886
//#define RAD_TO_DEG 57.295779513082320876798154814105

//#define min(a,b)                ((a)<(b)?(a):(b))
//#define max(a,b)                ((a)>(b)?(a):(b))
//#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
//#define radians(deg)            ((deg)*DEG_TO_RAD)
//#define degrees(rad)            ((rad)*RAD_TO_DEG)
//#define sq(x)                   ((x)*(x))

// undefine stdlib's abs
//#ifdef abs
//#undef abs
//#endif
//#define abs(x) (((x) > 0) ? (x) : -(x))

//----------------------------------------------------------------------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=2788669#p2788669
  const EXP2LUT = [1/32,1/16,1/8,1/4,1/2,1,2,4,8,16,32];
  const 1_OVER_LN2 = 1/0.693147180559;
  inline float PowerOfTwo(float exponent) {
    int whole = Floor(exponent);
    float f = exponent - whole;
    float x = 3.4437662E-01*f*f + 6.5561579E-01*f + 1.0000038E+00;
    result = x * EXP2LUT[5+whole];
  }
  inline float Exp(float exponent) {
    exponent = exponent * 1_OVER_LN2;
    int whole = Floor(exponent);
    float f = exponent - whole;
    float x = 3.4437662E-01*f*f + 6.5561579E-01*f + 1.0000038E+00;
    result = x * EXP2LUT[5+whole];
  }
*/

//----------------------------------------------------------------------

// denormalization killing with dc as presented by Laurent de Soras in
// http://ldesoras.free.fr/doc/articles/denormal-en.pdf

//float SAT_AddDb(float val) {
//  // static const float antidenormal_dc = 1e-20;
//  static const float antidenormal_dc = 0.000000000000000001f;
//  return val + antidenormal_dc;
//}

//----------

//float KNoteToHz(float ANote) {
//  return 440 * pow(2.0,(ANote-69)*SAT_INV12F);
//  //return 440 * KPowF2(2.0,(ANote-69)*SAT_INV12F);  // too much error for notetohz
//}

//----------------------------------------------------------------------
#endif
