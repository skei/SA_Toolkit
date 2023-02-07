#ifndef sat_integer_math_included
#define sat_integer_math_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "base/sat_const.h"
#include <math.h> // sqrtf

//----------------------------------------------------------------------

/*
  calculate the average value of a set of integers
  example:
    int ar[] = { -1, 2, 3, 4, 5 };
    int result = SAvrgInt(5, ar); // result is 2 (truncated)
  n - number of elements (n)
  ar - array of integers
  return int
*/

int SAT_AverageI(const unsigned int n, const int* ar) {
  int total = 0;
  unsigned int i=0;
  while (i<n) {
    total += ar[i];
    i++;
  }
  return total/n;
}

//----------

/*
  bit reverse algorithm
  origin: http://graphics.stanford.edu/~seander/bithacks.html
    unsigned int b = 0x0000000a;
    unsigned int result = axBitReverse(b); // result = 0xa0000000;
*/

uint32_t SAT_BitReverse(uint32_t v) {
  uint32_t r = v;
  int s = sizeof(v) * SAT_CHAR_BIT - 1;
  for (v >>= 1; v; v >>= 1) {
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s;
  return r;
}

//----------

// limits an integer number to [-limit, limit]

int SAT_ClampI(const int input, const int limit) {
  const int _t = (input > -limit) ? input : -limit;
  //return (_t > limit) ? _t : limit;
  return (_t < limit) ? _t : limit;
}

int SAT_ClampI(const int input, const int minval, const int maxval) {
  if (input < minval) return minval;
  if (input > maxval) return maxval;
  return input;
}


//----------

// http://bits.stephan-brumme.com/isPowerOfTwo.html

bool SAT_IsPowerOfTwo(uint32_t x) {
  return ((x&(x-1))==0);
}

//----------

// returns the larger of two integer numbers

int SAT_MaxI(const int a, const int b) {
  return (a > b) ? a : b;
}

//----------

// returns the smaller of two integer numbers

int SAT_MinI(const int a, const int b) {
  return (a < b) ? a : b;
}

//----------

// http://bits.stephan-brumme.com/roundUpToNextPowerOfTwo.html

uint32_t SAT_NextPowerOfTwo(uint32_t x) {
  x--;
  x |= x >> 1;  // handle  2 bit numbers
  x |= x >> 2;  // handle  4 bit numbers
  x |= x >> 4;  // handle  8 bit numbers
  x |= x >> 8;  // handle 16 bit numbers
  x |= x >> 16; // handle 32 bit numbers
  x++;
  return x;
}

//----------

// http://eli.thegreenplace.net/2004/07/30/a-cool-algorithm-for-counting-ones-in-a-bitstring/

uint32_t SAT_NumBits(uint32_t num) {
  uint32_t count = 0;
  while (num) {
    num &= (num-1);
    ++count;
  }
  return count;
}

//----------

/*
  calculate the RMS of a set (array) of int numbers
  example:
    int ar[] = { 1, -2, 3, 4, 5 };
    int result = SRMSI(5, ar);
  n - size of the array
  ar - array of integers
*/

int SAT_RmsI(const unsigned int n, const int* ar) {
  int numr = 0;
  unsigned int i=0;
  while (i<n) {
    numr += ar[i]*ar[i];
    i++;
  }
  return (int)(sqrtf((float)(numr/n)));
}

//----------------------------------------------------------------------
#endif
