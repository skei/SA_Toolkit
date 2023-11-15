#ifndef mip_farrow_filter_included
#define mip_farrow_filter_included
//----------------------------------------------------------------------

class MIP_FarrowFilter {
};

#if 0

// https://www.dsprelated.com/showcode/205.php

/* ****************************************************
 * Farrow resampler (with optional bank switching)
 * M. Nentwig, 2011
 * Input stream is taken from stdin
 * Output stream goes to stdout
 * Main target was readability, is not optimized for efficiency.
 * ****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#if 1

  /* **************************************************************
   * example coefficients.
   * Each column [c0; c1; c2; ...] describes a polynomial for one tap
     coefficent in fractional time ft [0, 1]:
   * tapCoeff = c0 + c1 * ft + c2 * ft ^ 2 + ...
   * Each column corresponds to one tap.
   * The example filters is based on a 6th order Chebyshev Laplace-domain
     prototype.
   * This version uses three sub-segments per tap (NBANKS = 3)
   * **************************************************************/

  const double cMatrix[] = {
    2.87810386e-4,    4.70096244e-3,    7.93412570e-2,    4.39824536e-1,
    1.31192924e+000,  2.67892232e+000,  4.16465421e+000,  5.16499621e+000,
    5.15592605e+000,  3.99000369e+000,  2.00785470e+000, -7.42377060e-2,
   -1.52569354e+000, -1.94402804e+000, -1.40915797e+000, -3.86484652e-1,
    5.44712939e-1,    9.77559688e-1,    8.32191447e-1,    3.22691788e-1,
   -2.13133045e-1,   -5.08501962e-1,   -4.82928807e-1,   -2.36313854e-1,
    4.76034568e-2,    2.16891966e-1,    2.20894063e-1,    1.08361553e-1,
   -2.63421832e-2,   -1.06276015e-1,   -1.07491548e-1,   -5.53793711e-2,
    4.86314061e-3,    3.94357182e-2,    4.06217506e-2,    2.17199064e-2,
    1.60318761e-3,   -8.40370106e-3,   -8.10525279e-3,   -3.62112499e-3,
   -4.13413072e-4,    2.33101911e-4,
   -3.26760325e-3,   -6.46028234e-3,    1.46793247e-1,    5.90235537e-1,
    1.18931309e+000,  1.57853546e+000,  1.40402774e+000,  5.76506323e-1,
   -6.33522788e-1,   -1.74564700e+000, -2.24153717e+000, -1.91309453e+000,
   -9.55568978e-1,    1.58239169e-1,    9.36193787e-1,    1.10969783e+000,
    7.33284446e-1,    1.06542194e-1,   -4.15412084e-1,   -6.06616434e-1,
   -4.54898908e-1,   -1.20841199e-1,    1.82941623e-1,    3.12543429e-1,
    2.49935829e-1,    8.05376898e-2,   -7.83213666e-2,   -1.47769751e-1,
   -1.18735248e-1,   -3.70656555e-2,    3.72608374e-2,    6.71425397e-2,
    5.17812605e-2,    1.55564930e-2,   -1.40896327e-2,   -2.35058137e-2,
   -1.59635057e-2,   -3.44701792e-3,    4.14108065e-3,    4.56234829e-3,
    1.59503132e-3,   -3.17301882e-4,
    5.64310141e-3,    7.74786707e-2,    2.11791763e-1,    2.84703201e-1,
    1.85158633e-1,   -8.41118142e-2,   -3.98497442e-1,   -5.86821615e-1,
   -5.40397941e-1,   -2.47558080e-1,    1.50864737e-1,    4.59312895e-1,
    5.41539400e-1,    3.84673917e-1,    9.39576331e-2,   -1.74932542e-1,
   -3.01635463e-1,   -2.56239225e-1,   -9.87146864e-2,    6.82216764e-2,
    1.59795852e-1,    1.48668245e-1,    6.62563431e-2,   -2.71234898e-2,
   -8.07045577e-2,   -7.76841351e-2,   -3.55333136e-2,    1.23206602e-2,
    3.88535040e-2,    3.64199073e-2,    1.54608563e-2,   -6.59814558e-3,
   -1.72735099e-2,   -1.46307777e-2,   -5.04363288e-3,    3.31049461e-3,
    6.01267607e-3,    3.83904192e-3,    3.92549958e-4,   -1.36315264e-3,
   -9.76017430e-4,    7.46699178e-5
  };

  #define NTAPS (14)
  #define NBANKS (3)
  #define ORDER (2)

#else

  /* Alternative example
   * Similar impulse response as above
   * A conventional Farrow design (no subdivisions => NBANKS = 1), order 3
   */

  const double cMatrix[] = {
   -8.57738278e-3,    7.82989032e-1,    7.19303539e+000,  6.90955718e+000,
   -2.62377450e+000, -6.85327127e-1,    1.44681608e+000, -8.79147907e-1,
    7.82633997e-2,    1.91318985e-1,   -1.88573400e-1,    6.91790782e-2,
    3.07723786e-3,   -6.74800912e-3,
    2.32448021e-1,    2.52624309e+000,  7.67543936e+000, -8.83951796e+000,
   -5.49838636e+000,  6.07298348e+000, -2.16053205e+000, -7.59142947e-1,
    1.41269409e+000, -8.17735712e-1,    1.98119464e-1,    9.15904145e-2,
   -9.18092030e-2,    2.74136108e-2,
   -1.14183319e+000,  6.86126458e+000, -6.86015957e+000, -6.35135894e+000,
    1.10745051e+001, -3.34847578e+000, -2.22405694e+000,  3.14374725e+000,
   -1.68249886e+000,  2.54083065e-1,    3.22275037e-1,   -3.04794927e-1,
    1.29393976e-1,   -3.32026332e-2,
    1.67363115e+000, -2.93090391e+000, -1.13549165e+000,  5.65274939e+000,
   -3.60291782e+000, -6.20715544e-1,    2.06619782e+000, -1.42159644e+000,
    3.75075865e-1,    1.88433333e-1,   -2.64135123e-1,    1.47117661e-1,
   -4.71871047e-2,    1.24921920e-2
  };

  #define NTAPS (14)
  #define NBANKS (1)
  #define ORDER (3)

#endif

//----------

/* Set here the ratio between output and input sample rate.
 * It could be changed even during runtime! */

#define INCR (1.0 / 6.28 / 3)

//----------

/* delay line storage */
double delayLine[NTAPS];

//----------

/* Coefficient lookup "table" */
static double c(int ixTap, int ixBank, int ixPower) {
  return cMatrix[ixPower * (NTAPS * NBANKS) + ixTap * NBANKS + ixBank];
}

//----------

int main (void) {
  /* clear delay line */
  int ix;
  for (ix = 0; ix < NTAPS; ++ix) delayLine[ix] = 0;
  /* Index of last input sample that was read
   * First valid sample starts at 0 */
  int sampleIndexInput = -1;
  /* Position of next output sample in input stream */
  double sampleIndexOutput = 0.0;
  /* loop forever. Terminate, once out of input data. */
  while (1) {
    /* Split output sample location into integer and fractional part:
     * Integer part corresponds to sample index in input stream
     * fractional part [0, 1[ spans one tap (covering NBANKS segments) */
    int sio_int = floor(sampleIndexOutput);
    double sio_fracInTap = sampleIndexOutput - (double)sio_int;
    assert((sio_fracInTap >= 0) && (sio_fracInTap < 1));
    /* Further split the fractional part into
     * - bank index
     * - fractional part within the bank */
    int sio_intBank = floor(sio_fracInTap * (double) NBANKS);
    double sio_fracInBank = sio_fracInTap * (double) NBANKS - (double)sio_intBank;
    assert((sio_fracInBank >= 0) && (sio_fracInBank < 1));
    /* ****************************************************
     * load new samples into the delay line, until the last
     * processed input sample (sampleIndexInput) catches
     * up with the integer part of the output stream position (sio_int)
     * ***************************************************/
    while (sampleIndexInput < sio_int) {
      /* Advance the delay line one step */
      ++sampleIndexInput;
      for (ix = NTAPS-1; ix > 0; --ix) delayLine[ix] = delayLine[ix-1];
      /* Read one input sample */
//----------
      int flag = scanf("%lf", &delayLine[0]);
//----------
      if (flag != 1) goto done; /* there's nothing wrong with "goto" as "break" for multiple loops ... */
    }
    /* ****************************************************
     * Calculate one output sample:
     * "out" sums up the contribution of each tap
     * ***************************************************/
    double out = 0;
    int ixTap;
    for (ixTap = 0; ixTap < NTAPS; ++ixTap) {
      /* ****************************************************
       * Contribution of tap "ixTap" to output:
       * ***************************************************/
      /* Evaluate polynomial in sio_fracInBank:
       * c(ixTap, sio_intBank, 0) is the constant coefficient
       * c(ixTap, sio_intBank, 1) is the linear coefficient etc
       */
      double hornerSum = c(ixTap, sio_intBank, ORDER);
      int ixPower;
      for (ixPower = ORDER-1; ixPower >= 0; --ixPower) {
        hornerSum *= sio_fracInBank;
        hornerSum += c(ixTap, sio_intBank, ixPower);
      } /* for ixPower */
      /* ****************************************************
       * Weigh the delay line sample of this tap with the
       * polynomial result and add to output
       * ***************************************************/
      out += hornerSum * delayLine[ixTap];
    } /* for ixTap */
    /* ****************************************************
     * Generate output sample and advance the position of
     * the next output sample in the input data stream
     * ***************************************************/
//----------
    printf("%1.12le\n", out);
//----------
    sampleIndexOutput += INCR;
  } /* loop until out of input data */
  done: /* out of input data => break loops and continue here */
  return 0;
} /* main */

#endif // 0

//----------------------------------------------------------------------
#endif
