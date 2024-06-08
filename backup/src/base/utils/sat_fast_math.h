#ifndef sat_fast_math_included
#define sat_fast_math_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

// optimizations
// approximations

//----------------------------------------------------------------------

#include <math.h> // expf, log2
#include "sat.h"

//#define SAT_MATH_EXTRA_PRECISION

float SAT_SqrtF(const float x); // KAsinF

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// fast approximation of the arc-cosine function for range [-1, 1]

float SAT_AcosF(const float x) {
  const float x2 = x*x;
  return x*(x2*(-0.55*x2 + 0.097) - 1.008) + 1.571;
}

//----------

// fast approximation of the arc-sine function for range [-1, 1]

float SAT_AsinF(const float x) {
  return SAT_PI05 - SAT_SqrtF(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
  x*(0.0187293 + 0.395*x))));
}

//----------

// fast approximation of the arc-tangens function for range [-2, 2]

float SAT_AtanF(const float x) {
  const float x2 = x*x;
  return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

//----------

// fast approximation of the cosine function for range [-pi, pi]

float SAT_CosF(const float x) {
  const float x2 = x*x;
  return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

//----------

//inline float fast_cos(float x) {
//  return fast_sin(x + HALF_PI);
//}

//float KCosF2(const float x) {
//  return KSinF2(x + SAT_PI2);
//}

//----------

// http://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b

// sin = cos

//template<typename T>
float SAT_CosF4(float x) {
  float tp = SAT_INVPI2;
    x *= tp;
    x -= 0.25 + floorf(x + 0.25);
    x *= 16.0 * (abs(x) - 0.5);
//    #if EXTRA_PRECISION
    x += 0.225 * x * (abs(x) - 1.0);
//    #endif
    return x;
}

//---

float SAT_SinF4(float x) {
  return SAT_CosF4(x-SAT_PI05);
}

//----------

// approximation of the hyperbolic-cosine function (fpu)

float SAT_CoshF(const float x) {
  const float _e = expf(abs(x));
  return (_e + 1.0f/_e)*0.5f;
}

// fast approximation of the hyperbolic-cosine function for range [-3.5, 3.5]

float SAT_CoshF2(const float x) {
  const float x2 = x*x;
  return x2*(0.065*x2 + 0.428) + 1.025;
}

//----------

/*
  fast approximation of [e] to the power of a number
  based on http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf
  note: original is for double precision (has a double to float cast)
*/

// is SAT_LITTLE_ENDIAN defined?

float SAT_ExpF(const float v) {
  union {
    double d;
    struct {
      // ???
      //#ifdef SAT_LITTLE_ENDIAN
      int i,j;
      //#else
      //int j,i;
      //#endif
    } s;
  } u;
  u.s.i = (int)(1512775*(double)v) + 1072632447;
  u.s.j = 0;
  return (float)u.d;
}

//----------

// http://music.columbia.edu/pipermail/music-dsp/2014-September/072873.html

/*
  I'm hoping to find a fast approximation for exp2(), which I can
  implement in 32 bit fixed point.  So far, the best I've turned up by
  searching is this from the archives.
  http://www.musicdsp.org/showone.php?id=106
    n = input + 1.0;
    n = n * n;
    n = n * 2.0 / 3.0;
    n = n + 4.0 / 3.0;
    n = n / 2.0;
  Some quick tests show the error gets to about 0.34% (not including small
  errors I'll add with 32 bit fixed point representation) when the input
  around 0.72.
*/

//----------

// invert of x: (1/x) - fast / inaccurate

float SAT_InverseF(float x) {
  union {
    uint32_t i;
    float j;
  } u;
  u.j = x;
  u.i = 0x7EEEEEEE - u.i;
  return u.j;
}

//----------

/*
  returns 1/x
  http://bits.stephan-brumme.com/inverse.html
  // 32-bit only
*/

float SAT_Inverse2_32(float x) {
  unsigned int *i = (unsigned int*)&x; // re-interpret as a 32 bit integer
  // warning: dereferencing pointer \91i\92 does break strict-aliasing rules|
  *i = 0x7F000000 - *i; // adjust exponent
  // *i = 0x7EEEEEEE - *i;
  return x;
}

//----------

/*
  fast approximation of the invert squre root function
  based on code found in 'quake 3 arena' by 'id software'
*/

float SAT_InvSqrtF(const float x) {
  // const float halfx = 0.5f*x;
  union {
    float j;
    int i;
  } u;
  u.j = x;
  u.i = 0x5f3759df - (u.i >> 1); // good initial guess
  // newton iteration <- enable for better result
  // return u.j*(1.5f - u.j*u.j*halfx) + 0.001f;
  return u.j;
}

//----------

// http://bits.stephan-brumme.com/invSquareRoot.html

float SAT_InvSqrtF2_32(float x) {
  float xHalf = 0.5f*x;                 // for Newton iteration
  unsigned int *i = (unsigned int*) &x; // same as above
  // warning: dereferencing pointer \91i\92 does break strict-aliasing rules|
  *i = 0x5F375A86 - (*i>>1);            // one Newton iteration, repeating further improves precision
  return x * (1.5f - xHalf*x*x);
}

//----------

float SAT_InvSqrtF3_32(float x) {
  unsigned int *i = (unsigned int*) &x; // access float with integer logic
  // warning: dereferencing pointer \91i\92 does break strict-aliasing rules|
  *i = 0x5F375A86 - (*i>>1); // approximation with empirically found "magic number"
  return x;
}

//----------

/*
  fast approximation of the logarithm base 2 function
  based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
*/

float SAT_Log2F(const float val) {
  if (val > 0.f) {
    union {
      int i;
      float j;
    } u;
    u.j = val;
    const int log_2 = ((u.i >> 23) & 255) - 128;
    u.i &= ~(255 << 23);
    u.i += 127 << 23;
    return (0.05f + u.j + (float)log_2);
  }
  else return 0.f;
}

//----------

// calculates the logarithm base 10 of a floating point number

float SAT_Log10F(const float x) {
  // log10(e) = 0.4342945239647
  // also: log10(x) = log2(x) - ln(x)
  const float y = (x - 1)/(x + 1);
  const float y2 = y*y;
  return (2.f*y*(1 + y2*0.3333333333f + y2*y2*0.2f))*0.4342945239647f;
}

//----------

/*
  fast approximation of the natural logarithm function
  based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
*/

float SAT_LogF(const float &val) {
  return (log2(val)*0.69314718f);
}

//----------

// performs fast pow(float, integer)

float SAT_PowF(float x, unsigned long n) {
  float res = 1;
  while (n > 0) {
    if (n & 1) res *= x;
    n >>= 1;
    x *= x;
  }
  return res;
}

//----------

/*
  http://martin.ankerl.com/2007/10/04/optimized-pow-approximation-for-java-and-c-c/
  7.8 times faster
*/

double SAT_PowF2(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

//----------

/*
  Paul Mineiro have some nice code which uses some dirty, but fully working,
  tricks:
  http://fastapprox.googlecode.com/svn/trunk/fastapprox/src/fastonebigheader.h

  inline float fastpow2 (float p) {
    float offset = (p < 0) ? 1.0f : 0.0f;
    float clipp = (p < -126) ? -126.0f : p;
    int w = clipp;
    float z = clipp - w + offset;
    union { uint32_t i; float f; } v = {
      cast_uint32_t ( (1 << 23) *  (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };
    return v.f;
  }

  The accuracy is "fastpow2 relative accuracy (positive p) = 1.58868e-05":
*/



//----------

// fast approximation of the sine function for range [-pi, pi]

float SAT_SinF(float x) {
  x *= (1.2732395447f - 0.4052847345f * abs(x));
  return 0.225f * (x * abs(x) - x) + x;
}

//----------

//#define SAT_MATH_EXTRA_PRECISION

float SAT_SinF2(float x) {
  const float B = 4/SAT_PI;
  const float C = -4/(SAT_PI*SAT_PI);
  float y = B * x + C * x * abs(x);
  #ifdef SAT_FAST_MATH_EXTRA_PRECISION
  //  const float Q = 0.775;
  const float P = 0.225;
  y = P * (y * abs(y) - y) + y;   // Q * y + P * y * abs(y)
  #endif
  return y;
}

//#undef SAT_MATH_EXTRA_PRECISION

//----------

// fast approximation of the hyperbolic-sine function for range [-3.5, 3.5]

float SAT_SinhF(const float x) {
  const float x2 = x*x;
  return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

//----------

/*
  fast approximation of the squre root function
  based on: http://www.azillionmonkeys.com/qed/sqroot.html
*/

float SAT_SqrtF(const float x) {
  const float halfx = x*0.5;
  union {
    int i;
    float j;
  } u;
  u.j = x;
  u.i = (0xbe6ec85f - u.i) >> 1;                // good initial guess
  return x*u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration
}

//----------

// fast approximation of the tangens function for range [-pi, pi]

float SAT_TanF(const float x) {
  const float x2 = x*x;
  return (x*(105 - 10*x2)) / (105 - x2*(45 - x2));
}

//----------

//inline float fast_tan(float x) {
//  return common::fast_sin(x) / common::fast_cos(x);
//}


//----------

// fast approximation of the hyperbolic-tangens function for range [-4.2, 4.2]

float SAT_TanhF(const float x) {
  const float x2 = x*x;
  return x*(27 + x2) / (27 + 9*x2);
}

//----------

// http://www.juce.com/comment/319543#comment-319543

/*
float rational_tanh(x) {
  if (x<-3) return -1;
  else if (x>3) return 1;
  else return x * (27+x*x) / (27+9*x*x);
}
*/

//----------

// cschueler's rational tanh approximation
// from http://www.musicdsp.org/showone.php?id=238

/*

inline float rational_tanh(float x) {
  if (x < -3.0f) return -1.0f;
  else if (x > 3.0f) return 1.0f;
  else return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

*/

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// http://music.columbia.edu/pipermail/music-dsp/2002-April/048411.html

//Fast logarithm (2-based) approximation:

//#include <assert.h>

/*
int floorOfLn2( float f ) {
  assert( f > 0. );
  assert( sizeof(f) == sizeof(int) );
  assert( sizeof(f) == 4 );
  return (((*(int *)&f)&0x7f800000)>>23)-0x7f;
}
*/

//----------

/*
  You could make it not truncate to integer, and be monotonic, but still only a
  rough approximation (lower than or equal to the actual value):
*/

/*
float approxLn2( float f ) {
  assert( f > 0. );
  assert( sizeof(f) == sizeof(int) );
  assert( sizeof(f) == 4 );
  int i = (*(int *)&f);
  return (((i&0x7f800000)>>23)-0x7f)+(i&0x007fffff)/(float)0x800000;
}
*/

//----------------------------------------------------------------------
#endif
