#ifndef sat_interpolation_included
#define sat_interpolation_included
//----------------------------------------------------------------------

// http://paulbourke.net/miscellaneous/interpolation/

//----------------------------------------------------------------------

float SAT_Interpolate_Linear(float t, float y1,float y2) {
  return ( y1*(1-t) + y2*t );
  //return y1 + t*(y2-y1);
}

//----------

/*
  A suitable orientated piece of a cosine function serves to provide a
  smooth transition between adjacent segments.
*/

float SAT_Interpolate_Cosine(float t, float y1,float y2) {
  float t2;
  t2 = (1-cosf(t*SAT_PI))/2;
  return(y1*(1-t2)+y2*t2);
}

//----------

/*
  Cubic interpolation is the simplest method that offers true continuity
  between the segments. As such it requires more than just the two
  endpoints of the segment but also the two points on either side of them.
  So the function requires 4 points in all.
  This does raise issues for how to interpolate between the first and last
  segments. A common solution is the dream up two extra points at the
  start and end of the sequence, the new points are created so that they
  have a slope equal to the slope of the start or end segment.



  Paul Breeuwsma proposes the following coefficients for a smoother
  interpolated curve, which uses the slope between the previous point
  and the next as the derivative at the current point. This results
  in what are generally referred to as Catmull-Rom splines.

    a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
    a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    a2 = -0.5*y0 + 0.5*y2;
    a3 = y1;

*/

float SAT_Interpolate_Cubic(float t, float y0,float y1, float y2,float y3) {
  float a0,a1,a2,a3,t2;
  t2 = t*t;
  a0 = y3 - y2 - y0 + y1;
  a1 = y0 - y1 - a0;
  a2 = y2 - y0;
  a3 = y1;
  return(a0*t*t2+a1*t2+a2*t+a3);
}

//----------

/*
  Hermite interpolation like cubic requires 4 points so that it can
  achieve a higher degree of continuity. In addition it has nice tension
  and biasing controls. Tension can be used to tighten up the curvature
  at the known points. The bias is used to twist the curve about the
  known points.

  Tension: 1 is high, 0 normal, -1 is low
  Bias: 0 is even, positive is towards first segment, negative towards the other
*/

float SAT_Interpolate_Hermite(float t, float y0, float y1, float y2, float y3, float tension=0, float bias=0) {
  float m0,m1,t2,t3;
  float a0,a1,a2,a3;

	t2 = t * t;
	t3 = t2 * t;
  m0  = (y1-y0)*(1+bias)*(1-tension)/2;
  m0 += (y2-y1)*(1-bias)*(1-tension)/2;
  m1  = (y2-y1)*(1+bias)*(1-tension)/2;
  m1 += (y3-y2)*(1-bias)*(1-tension)/2;
  a0 =  2*t3 - 3*t2 + 1;
  a1 =    t3 - 2*t2 + t;
  a2 =    t3 -   t2;
  a3 = -2*t3 + 3*t2;
  return(a0*y1+a1*m0+a2*m1+a3*y2);
}

//----------

// http://www.musicdsp.org/showone.php?id=93

// original

float SAT_Interpolate_Hermite1(float t, float y0, float y1, float y2, float y3) {
  // 4-point, 3rd-order Hermite (x-form)
  float c0 = y1;
  float c1 = 0.5f * (y2 - y0);
  float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
  float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
  return ((c3 * t + c2) * t + c1) * t + c0;
}

//----------

// james mccartney

float SAT_Interpolate_Hermite2(float t, float y0, float y1, float y2, float y3)
{
  // 4-point, 3rd-order Hermite (x-form)
  float c0 = y1;
  float c1 = 0.5f * (y2 - y0);
  float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
  float c2 = y0 - y1 + c1 - c3;
  return ((c3 * t + c2) * t + c1) * t + c0;
}

//----------

// james mccartney

float SAT_Interpolate_Hermite3(float t, float y0, float y1, float y2, float y3)
{
  // 4-point, 3rd-order Hermite (x-form)
  float c0 = y1;
  float c1 = 0.5f * (y2 - y0);
  float y0my1 = y0 - y1;
  float c3 = (y1 - y2) + 0.5f * (y3 - y0my1 - y2);
  float c2 = y0my1 + c1 - c3;
  return ((c3 * t + c2) * t + c1) * t + c0;
}

//----------

/*
  frac_pos: fractional value [0.0f - 1.0f] to interpolator
  pntr: pointer to float array where:
  pntr[0] = previous sample (idx = -1)
  pntr[1] = current sample (idx = 0)
  pntr[2] = next sample (idx = +1)
  pntr[3] = after next sample (idx = +2)
  The interpolation takes place between pntr[1] and pntr[2].
*/

// laurent de soras

float SAT_Interpolate_Hermite4(float t, float xm1, float x0, float x1, float x2)
{
  float c = (x1 - xm1) * 0.5f;
  float v = x0 - x1;
  float w = c + v;
  float a = w + v + (x2 - x0) * 0.5f;
  float b_neg = w + a;
  return ((((a * t) - b_neg) * t + c) * t + x0);
}

//----------

// http://www.pascalgamedevelopment.com/showthread.php?4621-Bezier-curves/page2&highlight=catmull

/*
  catmull-rom
  a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
  a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
  a2 = -0.5*y0 + 0.5*y2;
  a3 = y1;
*/

//----------

// https://code.google.com/p/nxpascal/source/browse/trunk/src/nxMath.pas#87

float SAT_Interpolate_CatmullRom(float t, float p0, float p1, float p2, float p3) {
  return 0.5 * ( 2*p1 + (p2-p0)*t +
               ( 2*p0 - 5*p1 + 4*p2 - p3) * t*t +
               ( 3*p1 -   p0 - 3*p2 + p3) * t*t*t );
}

//----------------------------------------------------------------------
//
// The Quest For The Perfect Resampler
// http://yehar.com/blog/wp-content/uploads/2009/08/deip.pdf
//
//----------------------------------------------------------------------

//------------------------------
// 6.1 Linear
//------------------------------

/*
  Linear interpolation gives a modified SNR of 19.1dB for 2x-, 33.8dB for 4x-,
  47.0dB for 8x-, 59.7dB for 16x- and 72.0dB for 32x-oversampled input.
*/

//----------

// Linear

float SAT_Interpolate_Linear(float x, float* y) {
  return y[0] + x*(y[1]-y[0]);
}

//------------------------------
// 6.2 B-Spline
//------------------------------

/*
  B-splines are a family of interpolators that can be constructed by convolving
  a drop-sample interpolator by a drop-sample interpolator repeated times.
  It is notable that higher-order B-splines don’t have zero crossings at
  integer x, therefore the interpolated curve will not necessarily go through
  the points.  That as such is not a bad quality.
  The higher the order of a B-spline, the more continuous derivatives it has.
  The number of continuous successive derivatives, also counting the impulse
  response function itself as the 0th derivative, has shown to define the slope
  of the overall spectral envelope of any piece-wise polynomial interpolator.
  The slope is that number plus 1, times -6dB/oct.  For B-splines, the number
  is same as the order.

  3rd-order B-spline gives a modified SNR of 38.2dB for 2x-, 67.6dB for 4x-,
  94.1dB for 8x-, 119.3dB for 16x- and 143.9dB for 32x-oversampled input.

  5th-order B-spline gives a modified SNR of 57.3dB for 2x-, 101.4dB for 4x-,
  141.1dB for 8x-, 179.0dB for 16x- and 215.9dB for 32x-oversampled input.
*/

//----------

// 4-point, 3rd-order B-spline (x-form)

float SAT_Interpolate_BSpline_4p3ox(float x, float* y) {
  float ym1py1 = y[-1]+y[1];
  float c0 = 1/6.0*ym1py1 + 2/3.0*y[0];
  float c1 = 1/2.0*(y[1]-y[-1]);
  float c2 = 1/2.0*ym1py1 - y[0];
  float c3 = 1/2.0*(y[0]-y[1]) + 1/6.0*(y[2]-y[-1]);
  return ((c3*x+c2)*x+c1)*x+c0;
}

//----------

// 4-point, 3rd-order B-spline (z-form)

float SAT_Interpolate_BSpline_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-1]+y[2], modd1 = y[2]-y[-1];
  float even2 = y[0]+y[1],  modd2 = y[1]-y[0];
  float c0 = 1/48.0*even1 + 23/48.0*even2;
  float c1 = 1/8.0*modd1 + 5/8.0*modd2;
  float c2 = 1/4.0*(even1-even2);
  float c3 = 1/6.0*modd1 - 1/2.0*modd2;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// 6-point, 5th-order B-spline (x-form)

float SAT_Interpolate_BSpline_6p5ox(float x, float* y) {
  float ym2py2 = y[-2]+y[2], ym1py1 = y[-1]+y[1];
  float y2mym2 = y[2]-y[-2], y1mym1 = y[1]-y[-1];
  float sixthym1py1 = 1/6.0*ym1py1;
  float c0 = 1/120.0*ym2py2 + 13/60.0*ym1py1 + 11/20.0*y[0];
  float c1 = 1/24.0*y2mym2 + 5/12.0*y1mym1;
  float c2 = 1/12.0*ym2py2 + sixthym1py1 - 1/2.0*y[0];
  float c3 = 1/12.0*y2mym2 - 1/6.0*y1mym1;
  float c4 = 1/24.0*ym2py2 - sixthym1py1 + 1/4.0*y[0];
  float c5 = 1/120.0*(y[3]-y[-2]) + 1/24.0*(y[-1]-y[2]) + 1/12.0*(y[1]-y[0]);
  return ((((c5*x+c4)*x+c3)*x+c2)*x+c1)*x+c0;
}

//------------------------------
// 6.3 Lagrange
//------------------------------

/*
  Lagrange polynomials are forced to go through a number of points. For
  example, the 4-point Lagrange interpolator polynomial is formed so that it
  goes through all of the four neighboring points, and the middle section is
  used. The 1st-order (2-point) Lagrange interpolator is the linear
  interpolator, which was already presented as part of the B-spline family.
  The order of the Lagrange polynomials is always one less than the number of
  points.

  3rd-order Lagrange gives a modified SNR of 27.7dB for 2x-, 52.8dB for 4x-,
  77.7dB for 8x-, 102.2dB for 16x- and 126.6dB for 32x-oversampled input.

  5th-order Lagrange gives a modified SNR of 35.2dB for 2x-, 70.9dB for 4x-,
  107.5dB for 8x-, 144.1dB for 16x- and 180.5dB for 32x-oversampled input.
*/

//----------

// 4-point, 3rd-order Lagrange (x-form)

float SAT_Interpolate_Lagrange_4p3ox(float x, float* y) {
  float c0 = y[0];
  float c1 = y[1] - 1/3.0*y[-1] - 1/2.0*y[0] - 1/6.0*y[2];
  float c2 = 1/2.0*(y[-1]+y[1]) - y[0];
  float c3 = 1/6.0*(y[2]-y[-1]) + 1/2.0*(y[0]-y[1]);
  return ((c3*x+c2)*x+c1)*x+c0;
}

//----------

// 4-point, 3rd-order Lagrange (z-form)

float SAT_Interpolate_Lagrange_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-1]+y[2], odd1 = y[-1]-y[2];
  float even2 = y[0]+y[1],  odd2 = y[0]-y[1];
  float c0 = 9/16.0*even2 - 1/16.0*even1;
  float c1 = 1/24.0*odd1 - 9/8.0*odd2;
  float c2 = 1/4.0*(even1-even2);
  float c3 = 1/2.0*odd2 - 1/6.0*odd1;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// 6-point, 5th-order Lagrange (x-form)

float SAT_Interpolate_Lagrange_4p5ox(float x, float* y) {
  float ym1py1 = y[-1]+y[1];
  float twentyfourthym2py2 = 1/24.0*(y[-2]+y[2]);
  float c0 = y[0];
  float c1 = 1/20.0*y[-2] - 1/2.0*y[-1] - 1/3.0*y[0] + y[1] - 1/4.0*y[2] + 1/30.0*y[3];
  float c2 = 2/3.0*ym1py1 - 5/4.0*y[0] - twentyfourthym2py2;
  float c3 = 5/12.0*y[0] - 7/12.0*y[1] + 7/24.0*y[2] - 1/24.0*(y[-2]+y[-1]+y[3]);
  float c4 = 1/4.0*y[0] - 1/6.0*ym1py1 + twentyfourthym2py2;
  float c5 = 1/120.0*(y[3]-y[-2]) + 1/24.0*(y[-1]-y[2]) + 1/12.0*(y[1]-y[0]);
  return ((((c5*x+c4)*x+c3)*x+c2)*x+c1)*x+c0;
}

//----------

// 6-point, 5th-order Lagrange (z-form)

float SAT_Interpolate_Lagrange_4p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-2]+y[3], odd1 = y[-2]-y[3];
  float even2 = y[-1]+y[2], odd2 = y[-1]-y[2];
  float even3 = y[0]+y[1],  odd3 = y[0]-y[1];
  float c0 = 3/256.0*even1 - 25/256.0*even2 + 75/128.0*even3;
  float c1 = 25/384.0*odd2 - 75/64.0*odd3 - 3/640.0*odd1;
  float c2 = 13/32.0*even2 - 17/48.0*even3 - 5/96.0*even1;
  float c3 = 1/48.0*odd1 - 13/48.0*odd2 + 17/24.0*odd3;
  float c4 = 1/48.0*even1 - 1/16.0*even2 + 1/24.0*even3;
  float c5 = 1/24.0*odd2 - 1/12.0*odd3 - 1/120.0*odd1;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.4 Hermite
//------------------------------

/*
  Hermite interpolation is the first-order member of the family of osculating
  interpolators that in addition to matching the function value at the control
  points also match a number of derivatives.  Hermite interpolation matches the
  first derivative.  Since the actual derivative of the function (audio) is not
  known, Hermite interpolation matches to the derivatives of even-order
  Lagrangian polynomes. For example, the 4-point cubic Hermite will match its
  derivative at y[x] to the derivative of a (Lagrangian) parabolic going
  through y[x−1], y[x] and y[x+1]. By increasing the order of the Lagrangians,
  one can create cubic Hermites that take advantage of more points. Note that
  as the order of Lagrangians increases, they converge to sinc interpolators,
  but the Hermite, limited by its cubic formulation, can only converge up to a
  point. Another approach to create Hermite interpolators is to ramp between
  two successive Lagrangians with a linear weighting ramp.  This also ensures
  the first derivative to match those of the Lagrangians at the points, but it
  also gives higher order Hermite interpolators, increasing how close they
  (theoretically) can approach the perfect sinc interpolator.

  4-point, 3rd-order Hermite gives a modified SNR of 23.5dB for 2x-, 44.2dB for
  4x-, 64.0dB for 8x-, 83.1dB for 16x- and 101.8dB for 32x-oversampled input.

  6-point, 3rd-order Hermite gives a modified SNR of 30.5dB for 2x-, 60.2dB for
  4x-, 89.1dB for 8x-, 116.3dB for 16x- and 142.3dB for 32x-oversampled input.

  6-point, 5th-order Hermite gives a modified SNR of 31.0dB for 2x-, 62.3dB for
  4x-, 93.7dB for 8x-, 124.7dB for 16x- and 155.4dB for 32x-oversampled input.

*/

//----------

// 4-point, 3rd-order Hermite (x-form)

float SAT_Interpolate_Hermite_4p3ox(float x, float* y) {
  float c0 = y[0];
  float c1 = 1/2.0*(y[1]-y[-1]);
  float c2 = y[-1] - 5/2.0*y[0] + 2*y[1] - 1/2.0*y[2];
  float c3 = 1/2.0*(y[2]-y[-1]) + 3/2.0*(y[0]-y[1]);
  return ((c3*x+c2)*x+c1)*x+c0;
}

//----------

// 4-point, 3rd-order Hermite (z-form)

float SAT_Interpolate_Hermite_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-1]+y[2], odd1 = y[-1]-y[2];
  float even2 = y[0]+y[1],  odd2 = y[0]-y[1];
  float c0 = 9/16.0*even2 - 1/16.0*even1;
  float c1 = 1/8.0*odd1 - 11/8.0*odd2;
  float c2 = 1/4.0*(even1-even2);
  float c3 = 3/2.0*odd2 - 1/2.0*odd1;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// 6-point, 3rd-order Hermite (x-form)

float SAT_Interpolate_Hermite_6p3ox(float x, float* y) {
  float c0 = y[0];
  float c1 = 1/12.0*(y[-2]-y[2]) + 2/3.0*(y[1]-y[-1]);
  float c2 = 5/4.0*y[-1] - 7/3.0*y[0] + 5/3.0*y[1] - 1/2.0*y[2] + 1/12.0*y[3] - 1/6.0*y[-2];
  float c3 = 1/12.0*(y[-2]-y[3]) + 7/12.0*(y[2]-y[-1]) + 4/3.0*(y[0]-y[1]);
  return ((c3*x+c2)*x+c1)*x+c0;
}

//----------

// 6-point, 3rd-order Hermite (z-form)

float SAT_Interpolate_Hermite_6p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-2]+y[3], odd1 = y[-2]-y[3];
  float even2 = y[-1]+y[2], odd2 = y[-1]-y[2];
  float even3 = y[0]+y[1], fourthirdthodd3 = 4/3.0*(y[0]-y[1]);
  float c0 = 1/96.0*even1 - 3/32.0*even2 + 7/12.0*even3;
  float c1 = 7/48.0*odd2 - fourthirdthodd3 - 1/48.0*odd1;
  float c2 = 3/8.0*even2 - 1/3.0*even3 - 1/24.0*even1;
  float c3 = 1/12.0*odd1 - 7/12.0*odd2 + fourthirdthodd3;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// 6-point, 5th-order Hermite (x-form)

float SAT_Interpolate_Hermite_6p5ox(float x, float* y) {
  float eighthym2 = 1/8.0*y[-2];
  float eleventwentyfourthy2 = 11/24.0*y[2];
  float twelfthy3 = 1/12.0*y[3];
  float c0 = y[0];
  float c1 = 1/12.0*(y[-2]-y[2]) + 2/3.0*(y[1]-y[-1]);
  float c2 = 13/12.0*y[-1] - 25/12.0*y[0] + 3/2.0*y[1] - eleventwentyfourthy2 + twelfthy3 - eighthym2;
  float c3 = 5/12.0*y[0] - 7/12.0*y[1] + 7/24.0*y[2] - 1/24.0*(y[-2]+y[-1]+y[3]);
  float c4 = eighthym2 - 7/12.0*y[-1] + 13/12.0*y[0] - y[1] + eleventwentyfourthy2 - twelfthy3;
  float c5 = 1/24.0*(y[3]-y[-2]) + 5/24.0*(y[-1]-y[2]) + 5/12.0*(y[1]-y[0]);
  return ((((c5*x+c4)*x+c3)*x+c2)*x+c1)*x+c0;
}

//----------

// 6-point, 5th-order Hermite (z-form)

float SAT_Interpolate_Hermite_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-2]+y[3], odd1 = y[-2]-y[3];
  float even2 = y[-1]+y[2], odd2 = y[-1]-y[2];
  float even3 = y[0]+y[1],  odd3 = y[0]-y[1];
  float c0 = 3/256.0*even1 - 25/256.0*even2 + 75/128.0*even3;
  float c1 = -3/128.0*odd1 + 61/384.0*odd2 - 87/64.0*odd3;
  float c2 = -5/96.0*even1 + 13/32.0*even2 - 17/48.0*even3;
  float c3 = 5/48.0*odd1 - 11/16.0*odd2 + 37/24.0*odd3;
  float c4 = 1/48.0*even1 - 1/16.0*even2 + 1/24.0*even3;
  float c5 = -1/24.0*odd1 + 5/24.0*odd2 - 5/12.0*odd3;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.5 2nd-order-osculating
//------------------------------

/*
  The definition of 2nd-order osculating interpolators is the same as of the
  Hermites, but also the second derivative is matched with the Lagrangians.
  Necessarily, the order of the interpolation polynomial must be at least 5,
  since there are 6 parameters, the function and two derivatives, all three at
  both of the two points, to match. Up to the 6-point version, these parameters
  leave no degrees of freedom in the 5th-order polynomial.

  4-point, 5th-order 2nd-order-osculating gives a modified SNR of 22.1dB for
  2x-, 41.9dB for 4x-, 61.1dB for 8x-, 79.9dB for 16x- and 98.3dB for
  32x-oversampled input.

  6-point, 5th-order 2nd-order-osculating gives a modified SNR of 29.9dB for
  2x-, 60.4dB for 4x-, 91.4dB for 8x-, 122.1dB for 16x- and 152.6dB for
  32x-oversampled input.
*/

//----------

// 4-point, 5th-order 2nd-order-osculating (x-form)

float SAT_Interpolate_2ndOrderOsculating_4p5ox(float x, float* y) {
  float y1my0 = y[1]-y[0];
  float y2mym1 = y[2]-y[-1];
  float c0 = y[0];
  float c1 = 1/2.0*(y[1]-y[-1]);
  float c2 = 1/2.0*(y[-1]+y[1]) - y[0];
  float c3 = 9/2.0*y1my0 - 3/2.0*y2mym1;
  float c4 = 5/2.0*y2mym1 - 15/2.0*y1my0;
  float c5 = y[-1] + 3*y1my0 - y[2];
  return ((((c5*x+c4)*x+c3)*x+c2)*x+c1)*x+c0;
}

//----------

// 4-point, 5th-order 2nd-order-osculating (z-form)

float SAT_Interpolate_2ndOrderOsculating_4p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-1]+y[2], odd1 = y[-1]-y[2];
  float even2 = y[0]+y[1],  odd2 = y[0]-y[1];
  float c0 = 9/16.0*even2 - 1/16.0*even1;
  float c1 = 3/16.0*odd1 - 25/16.0*odd2;
  float c2 = 1/4.0*(even1-even2);
  float c5 = odd1 - 3*odd2;
  return (((c5*z*z-c5)*z+c2)*z+c1)*z+c0;
}

//----------

// 6-point, 5th-order 2nd-order-osculating (x-form)

float SAT_Interpolate_2ndOrderOsculating_6p5ox(float x, float* y) {
  float c0 = y[0];
  float c1 = 1/12.0*(y[-2]-y[2]) + 2/3.0*(y[1]-y[-1]);
  float c2 = 2/3.0*(y[-1]+y[1]) - 1/24.0*(y[-2]+y[2]) - 5/4.0*y[0];
  float c3 = 13/8.0*y[-1] - 35/12.0*y[0] + 11/4.0*y[1] - 11/8.0*y[2] + 7/24.0*y[3] - 3/8.0*y[-2];
  float c4 = 13/24.0*y[-2] - 8/3.0*y[-1] + 21/4.0*y[0] - 31/6.0*y[1] + 61/24.0*y[2] - 1/2.0*y[3];
  float c5 = 5/24.0*(y[3]-y[-2]) + 25/24.0*(y[-1]-y[2]) + 25/12.0*(y[1]-y[0]);
  return ((((c5*x+c4)*x+c3)*x+c2)*x+c1)*x+c0;
}

//----------

// 6-point, 5th-order 2nd-order-osculating (z-form)

float SAT_Interpolate_2ndOrderOsculating_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-2]+y[3], odd1 = y[-2]-y[3];
  float even2 = y[-1]+y[2], odd2 = y[-1]-y[2];
  float even3 = y[0]+y[1],  odd3 = y[0]-y[1];
  float c0 = 3/256.0*even1 - 25/256.0*even2 + 75/128.0*even3;
  float c1 = 27/128.0*odd2 - 281/192.0*odd3 - 13/384.0*odd1;
  float c2 = 13/32.0*even2 - 17/48.0*even3 - 5/96.0*even1;
  float c3 = 3/16.0*odd1 - 53/48.0*odd2 + 19/8.0*odd3;
  float c4 = 1/48.0*even1 - 1/16.0*even2 + 1/24.0*even3;
  float c5 = 25/24.0*odd2 - 25/12.0*odd3 - 5/24.0*odd1;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.6 Watte tri-linear
//------------------------------

/*
  These two interpolators do not fall in any of the previous categories.
  Watte tri-linear was presented by Jon Watte on the music-dsp mailing list.
  It is constructed by first creating two linear functions, first going through
  y[x+1] and y[x−1] and the second through y[x] and y[x+2], by shifting the
  first to penetrate y[x] and the second to y[x+1], and by weighting between
  them with a linear ramp. The impulse response, visually quite resembles a
  4-point Lagrangian...

  Watte tri-linear gives a modified SNR of 27.9dB for 2x-, 34.9dB for 4x-,
  46.8dB for 8x-, 59.3dB for 16x- and 71.8dB for 32x-oversampled input
*/

//----------

// 4-point, 2nd-order Watte tri-linear (x-form)

float SAT_Interpolate_WatteTriLinear_4p2ox(float x, float* y) {
  float ym1py2 = y[-1]+y[2];
  float c0 = y[0];
  float c1 = 3/2.0*y[1] - 1/2.0*(y[0]+ym1py2);
  float c2 = 1/2.0*(ym1py2-y[0]-y[1]);
  return (c2*x+c1)*x+c0;
}

//----------

// 4-point, 2nd-order Watte tri-linear (z-form)

float SAT_Interpolate_WatteTriLinear_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-1]+y[2], even2 = y[0]+y[1];
  float c0 = 5/8.0*even2 - 1/8.0*even1;
  float c1 = y[1]-y[0];
  float c2 = 1/2.0*(even1-even2);
  return (c2*z+c1)*z+c0;
}

//------------------------------
// 6.7 Parabolic 2x
//------------------------------

/*
  "Parabolic  2x" is my (Olli Niemitalo) own design, and was created to be the
  lowest order 4-point interpolator with continuous function and first
  differential. As the differential must be zero at x=0 and at the endpoints
  x=±2, and be continuous at the borders of the sections, the only parameter
  left to define was the height of the curve, which was set so that the
  integral x=−2..2 became unity to ensure magnitude 1 for DC in the frequency
  response. The interpolated curve does not necessarily go through the points.

  Parabolic 2x gives a modified SNR of 28.6dB for 2x-, 50.7dB for 4x-, 70.6dB
  for 8x-, 89.5dB for 16x- and 108.0dB for 32x-oversampled input.
*/

//----------

// 4-point, 2nd-order parabolic 2x (x-form)

float SAT_Interpolate_Parabolic2x_4p2ox(float x, float* y) {
  float y1mym1 = y[1]-y[-1];
  float c0 = 1/2.0*y[0] + 1/4.0*(y[-1]+y[1]);
  float c1 = 1/2.0*y1mym1;
  float c2 = 1/4.0*(y[2]-y[0]-y1mym1);
  return (c2*x+c1)*x+c0;
}

//----------

// 4-point, 2nd-order parabolic 2x (z-form)

float SAT_Interpolate_Parabolic2x_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[-1]+y[2], even2 = y[0]+y[1];
  float c0 = 1/16.0*even1 + 7/16.0*even2;
  float c1 = 1/4.0*(y[1]-y[0]+y[2]-y[-1]);
  float c2 = 1/4.0*(even1-even2);
  return (c2*z+c1)*z+c0;
}

//------------------------------
// 6.8 2-point, 3rd-order optimal
//------------------------------

/*
  2-point, 3rd-order optimal interpolators converge to linear interpolation as
  the oversampling ratio increases. This is an indication to use linear
  interpolation at very high oversampling ratios.

  2-point, 3rd-order optimal interpolators give modified SNRs of 28.0dB for
  2x-, 39.1dB for 4x-, 49.7dB for 8x-, 61.0dB for 16x- and 72.7dB for
  32x-oversampled input.
*/

//----------

// Optimal 2x (2-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal2x_2p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float c0 = even1*0.50037842517188658;
  float c1 = odd1*1.00621089801788210;
  float c2 = even1*-0.004541102062639801;
  float c3 = odd1*-1.57015627178718420;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 4x (2-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal4x_2p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float c0 = even1*0.50013034073688023;
  float c1 = odd1*1.09617817497678520;
  float c2 = even1*-0.001564088842561871;
  float c3 = odd1*-1.32598918957298410;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 8x (2-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal8x_2p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float c0 = even1*0.50004007194083089;
  float c1 = odd1*1.06397659072500650;
  float c2 = even1*-0.000480863289971321;
  float c3 = odd1*-0.73514591836770027;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 16x (2-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal16x_2p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float c0 = even1*0.50001096675880796;
  float c1 = odd1*1.03585606328743830;
  float c2 = even1*-0.000131601105693441;
  float c3 = odd1*-0.38606621963374965;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 32x (2-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal32x_2p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float c0 = even1*0.50000286037713559;
  float c1 = odd1*1.01889120864375270;
  float c2 = even1*-0.000034324525627571;
  float c3 = odd1*-0.19775766248673177;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.9 4-point, 2nd-order optimal
//------------------------------

/*
  The 4-point, 2nd-order optimal interpolators are a bit strange - the impulse
  responses, especially the higher oversampling ratio versions, do not even
  resemble anything that we have previously seen. The explanation is that there
  is a transfer function zero on the transition band. This causes a large
  sidelobe that at higher oversampling ratios exceeds unity in magnitude.

  4-point, 2nd-order optimal interpolators give modified SNRs of 45.1dB for
  2x-, 64.6dB for 4x-, 83.5dB for 8x-, 101.9dB for 16x- and 120.2dB for
  32x-oversampled input.
*/

// Optimal 2x (4-point, 2nd-order) (z-form)

float SAT_Interpolate_Optimal2x_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.42334633257225274 + even2*0.07668732202139628;
  float c1 = odd1*0.26126047291143606 + odd2*0.24778879018226652;
  float c2 = even1*-0.213439787561776841 + even2*0.21303593243799016;
  return (c2*z+c1)*z+c0;
}

//----------

// Optimal 4x (4-point, 2nd-order) (z-form)

float SAT_Interpolate_Optimal4x_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.38676264891201206 + even2*0.11324319172521946;
  float c1 = odd1*0.01720901456660906 + odd2*0.32839294317251788;
  float c2 = even1*-0.228653995318581881 + even2*0.22858390767180370;
  return (c2*z+c1)*z+c0;
}

//----------

// Optimal 8x (4-point, 2nd-order) (z-form)

float SAT_Interpolate_Optimal8x_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.32852206663814043 + even2*0.17147870380790242;
  float c1 = odd1*-0.35252373075274990 + odd2*0.45113687946292658;
  float c2 = even1*-0.240052062078895181 + even2*0.24004281672637814;
  return (c2*z+c1)*z+c0;
}

//----------

// Optimal 16x (4-point, 2nd-order) (z-form)

float SAT_Interpolate_Optimal16x_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.20204741371575463 + even2*0.29795268253813623;
  float c1 = odd1*-1.11855475338366150 + odd2*0.70626377291054832;
  float c2 = even1*-0.245061178654743641 + even2*0.24506002360805534;
  return (c2*z+c1)*z+c0;
}

//----------

// Optimal 32x (4-point, 2nd-order) (z-form)

float SAT_Interpolate_Optimal32x_4p2oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*-0.04817865217726123 + even2*0.54817866412548932;
  float c1 = odd1*-2.62328241292796620 + odd2*1.20778105913587620;
  float c2 = even1*-0.247552438397138281 + even2*0.24755229501840223;
  return (c2*z+c1)*z+c0;
}

//------------------------------
// 6.10 4-point, 3rd-order optimal
//------------------------------

/*
  Visually, the impulse responses of the 4-point, 3rd-order optimal
  interpolators resemble that of the B-spline. The frequency response shows
  groups of zeros bombarding the stopbands for greater modified frequency
  response flatness.

  4-point, 3rd-order optimal interpolators give modified SNRs of 65.9dB for
  2x-, 89.0dB for 4x-, 112.9dB for 8x-, 136.9dB for 16x- and 161.0dB for
  32x-oversampled input
*/

// Optimal 2x (4-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal2x_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.45868970870461956 + even2*0.04131401926395584;
  float c1 = odd1*0.48068024766578432 + odd2*0.17577925564495955;
  float c2 = even1*-0.246185007019907091 + even2*0.24614027139700284;
  float c3 = odd1*-0.36030925263849456 + odd2*0.10174985775982505;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 4x (4-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal4x_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46209345013918979 + even2*0.03790693583186333;
  float c1 = odd1*0.51344507801315964 + odd2*0.16261507145522014;
  float c2 = even1*-0.248540332990294211 + even2*0.24853570133765701;
  float c3 = odd1*-0.42912649274763925 + odd2*0.13963062613760227;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 8x (4-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal8x_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46360002085841184 + even2*0.03640000638072349;
  float c1 = odd1*0.52776949859997280 + odd2*0.15746108253367153;
  float c2 = even1*-0.249658121535793251 + even2*0.24965779466617388;
  float c3 = odd1*-0.46789242171187317 + odd2*0.15551896027602030;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 16x (4-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal16x_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46436507349411416 + even2*0.03563492826010761;
  float c1 = odd1*0.53463126553787166 + odd2*0.15512856361039451;
  float c2 = even1*-0.249923540967159741 + even2*0.24992351991649797;
  float c3 = odd1*-0.48601256046234864 + odd2*0.16195131297091253;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 32x (4-point, 3rd-order) (z-form)

float SAT_Interpolate_Optimal32x_4p3oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46465589031535864 + even2*0.03534410979496938;
  float c1 = odd1*0.53726845877054186 + odd2*0.15424449410914165;
  float c2 = even1*-0.249981930954029101 + even2*0.24998192963009191;
  float c3 = odd1*-0.49369595780454456 + odd2*0.16455902278580614;
  return ((c3*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.11 4-point, 4th-order optimal
//------------------------------

/*
  The 4-point, 4th-order optimal interpolators don’t differ much from the
  3rd-order ones. They are better, though.

  4-point, 4th-order optimal interpolators give modified SNRs of 69.8dB for
  2x-, 101.1dB for 4x-, 126.4dB for 8x-, 150.7dB for 16x- and 174.9dB for
  32x-oversampled input.
*/

//----------

// Optimal 2x (4-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal2x_4p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.45645918406487612 + even2*0.04354173901996461;
  float c1 = odd1*0.47236675362442071 + odd2*0.17686613581136501;
  float c2 = even1*-0.253674794204558521 + even2*0.25371918651882464;
  float c3 = odd1*-0.37917091811631082 + odd2*0.11952965967158000;
  float c4 = even1*0.04252164479749607 + even2*-0.04289144034653719;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 4x (4-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal4x_4p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46567255120778489 + even2*0.03432729708429672;
  float c1 = odd1*0.53743830753560162 + odd2*0.15429462557307461;
  float c2 = even1*-0.251942101340217441 + even2*0.25194744935939062;
  float c3 = odd1*-0.46896069955075126 + odd2*0.15578800670302476;
  float c4 = even1*0.00986988334359864 + even2*-0.00989340017126506;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 8x (4-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal8x_4p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46771532012068961 + even2*0.03228466824404497;
  float c1 = odd1*0.55448654344364423 + odd2*0.14851181120641987;
  float c2 = even1*-0.250587283698110121 + even2*0.25058765188457821;
  float c3 = odd1*-0.49209020939096676 + odd2*0.16399414834151946;
  float c4 = even1*0.00255074537015887 + even2*-0.00255226912537286;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 16x (4-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal16x_4p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46822774170144532 + even2*0.03177225758005808;
  float c1 = odd1*0.55890365706150436 + odd2*0.14703258836343669;
  float c2 = even1*-0.250153411893796031 + even2*0.25015343462990891;
  float c3 = odd1*-0.49800710906733769 + odd2*0.16600005174304033;
  float c4 = even1*0.00064264050033187 + even2*-0.00064273459469381;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 32x (4-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal32x_4p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float c0 = even1*0.46835497211269561 + even2*0.03164502784253309;
  float c1 = odd1*0.56001293337091440 + odd2*0.14666238593949288;
  float c2 = even1*-0.250038759826233691 + even2*0.25003876124297131;
  float c3 = odd1*-0.49949850957839148 + odd2*0.16649935475113800;
  float c4 = even1*0.00016095224137360 + even2*-0.00016095810460478;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.12 6-point, 4th-order optimal
//------------------------------

/*
  The same phenomenon can be seen with the 6-point, 4th-order optimal
  interpolators as with the 4-point, 2nd-order ones. The transition band zero
  makes the impulse response weird-looking and causes a large sidelobe in the
  transition band. The sidelobe height greatly exceeds unity with higher
  oversampling ratios. This may cause design problems in the oversampling
  stage in form of increased stopband attenuation requirements.

  6-point, 4th-order optimal interpolators give modified SNRs of 89.8dB for
  2x-, 120.6dB for 4x-, 151.2dB for 8x-, 181.6dB for 16x- and 212.0dB for
  32x-oversampled input.
*/

//----------

// Optimal 2x (6-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal2x_6p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.37484203669443822 + even2*0.11970939637439368 + even3*0.00544862268096358;
  float c1 = odd1*0.19253897284651597 + odd2*0.22555179040018719 + odd3*0.02621377625620669;
  float c2 = even1*-0.154026006475653071 + even2*0.10546111301131367 + even3*0.04856757454258609;
  float c3 = odd1*-0.06523685579716083 + odd2*-0.04867197815057284 + odd3*0.04200764942718964;
  float c4 = even1*0.03134095684084392 + even2*-0.04385804833432710 + even3*0.01249475765486819;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 4x (6-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal4x_6p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.26148143200222657 + even2*0.22484494681472966   + even3*0.01367360612950508;
  float c1 = odd1*-0.20245593827436142 + odd2*0.29354348112881601   + odd3*0.06436924057941607;
  float c2 = even1*-0.022982104451679701 + even2*-0.09068617668887535   + even3*0.11366875749521399;
  float c3 = odd1*0.36296419678970931 + odd2*-0.26421064520663945   + odd3*0.08591542869416055;
  float c4 = even1*0.02881527997393852 + even2*-0.04250898918476453   + even3*0.01369173779618459;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 8x (6-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal8x_6p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.07571827673995030 + even2*0.39809419102537769 + even3*0.02618753167558019;
  float c1 = odd1*-0.87079480370960549 + odd2*0.41706012247048818 + odd3*0.12392296259397995;
  float c2 = even1*0.186883718356452901 + even2*-0.40535151498252686 + even3*0.21846781431808182;
  float c3 = odd1*1.09174419992174300 + odd2*-0.62917625718809478 + odd3*0.15915674384870970;
  float c4 = even1*0.03401038103941584 + even2*-0.05090907029392906 + even3*0.01689861603514873;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 16x (6-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal16x_6p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*-0.30943127416213301 + even2*0.75611844407537543 + even3*0.05331283006820442;
  float c1 = odd1*-2.23586327978235700 + odd2*0.66020840412562265 + odd3*0.25104761112921636;
  float c2 = even1*0.625420761014402691 + even2*-1.06313460380183860 + even3*0.43771384337431529;
  float c3 = odd1*2.57088518304678090 + odd2*-1.36878543609177150 + odd3*0.30709424868485174;
  float c4 = even1*0.03755086455339280 + even2*-0.05631219122315393 + even3*0.01876132424143207;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 32x (6-point, 4th-order) (z-form)

float SAT_Interpolate_Optimal32x_6p4oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*-1.05730227922290790 + even2*1.45069541587021430 + even3*0.10660686335233649;
  float c1 = odd1*-4.87455554035028720 + odd2*1.12509567592532630 + odd3*0.49985370215839708;
  float c2 = even1*1.479370435823112101 + even2*-2.34405608915933780 + even3*0.86468565335070746;
  float c3 = odd1*5.42677291742286180 + odd2*-2.79672428287565160 + odd3*0.59267998874843331;
  float c4 = even1*0.03957507923965987 + even2*-0.05936083498715066 + even3*0.01978575568000696;
  return (((c4*z+c3)*z+c2)*z+c1)*z+c0;
}

//------------------------------
// 6.13 6-point, 5th-order optimal
//------------------------------

/*
  The 6-point, 5th-order optimal interpolator impulse responses resemble that
  of the B-spline, and the frequency responses look nice - lots of zeros at
  where they are mostly needed.

  6-point, 5th-order optimal interpolators give modified SNRs of 111.4dB for
  2x-, 149.3dB for 4x-, 185.4dB for 8x-, 221.5dB for 16x- and 257.8dB for
  32x-oversampled input.
*/

// Optimal 2x (6-point, 5th-order) (z-form)

float SAT_Interpolate_Optimal2x_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.40513396007145713 + even2*0.09251794438424393 + even3*0.00234806603570670;
  float c1 = odd1*0.28342806338906690 + odd2*0.21703277024054901 + odd3*0.01309294748731515;
  float c2 = even1*-0.191337682540351941 + even2*0.16187844487943592 + even3*0.02946017143111912;
  float c3 = odd1*-0.16471626190554542 + odd2*-0.00154547203542499 + odd3*0.03399271444851909;
  float c4 = even1*0.03845798729588149 + even2*-0.05712936104242644 + even3*0.01866750929921070;
  float c5 = odd1*0.04317950185225609 + odd2*-0.01802814255926417 + odd3*0.00152170021558204;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 4x (6-point, 5th-order) (z-form)

float SAT_Interpolate_Optimal4x_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.41496902959240894 + even2*0.08343081932889224 + even3*0.00160015038681571;
  float c1 = odd1*0.31625515004859783 + odd2*0.21197848565176958 + odd3*0.00956166668408054;
  float c2 = even1*-0.203271896548875371 + even2*0.17989908432249280 + even3*0.02337283412161328;
  float c3 = odd1*-0.20209241069835732 + odd2*0.01760734419526000 + odd3*0.02985927012435252;
  float c4 = even1*0.04100948858761910 + even2*-0.06147760875085254 + even3*0.02046802954581191;
  float c5 = odd1*0.06607747864416924 + odd2*-0.03255079211953620 + odd3*0.00628989632244913;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 8x (6-point, 5th-order) (z-form)

float SAT_Interpolate_Optimal8x_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.41660797292569773 + even2*0.08188468587188069 + even3*0.00150734119050266;
  float c1 = odd1*0.32232780822726981 + odd2*0.21076321997422021 + odd3*0.00907649978070957;
  float c2 = even1*-0.205219993961471501 + even2*0.18282942057327367 + even3*0.02239057377093268;
  float c3 = odd1*-0.21022298520246224 + odd2*0.02176417471349534 + odd3*0.02898626924395209;
  float c4 = even1*0.04149963966704384 + even2*-0.06224707096203808 + even3*0.02074742969707599;
  float c5 = odd1*0.07517133281176167 + odd2*-0.03751837438141215 + odd3*0.00747588873055296;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 16x (6-point, 5th-order) (z-form)

float SAT_Interpolate_Optimal16x_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.41809989254549901 + even2*0.08049339946273310 + even3*0.00140670799165932;
  float c1 = odd1*0.32767596257424964 + odd2*0.20978189376640677 + odd3*0.00859567104974701;
  float c2 = even1*-0.206944618112960001 + even2*0.18541689550861262 + even3*0.02152772260740132;
  float c3 = odd1*-0.21686095413034051 + odd2*0.02509557922091643 + odd3*0.02831484751363800;
  float c4 = even1*0.04163046817137675 + even2*-0.06244556931623735 + even3*0.02081510113314315;
  float c5 = odd1*0.07990500783668089 + odd2*-0.03994519162531633 + odd3*0.00798609327859495;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------

// Optimal 32x (6-point, 5th-order) (z-form)

float SAT_Interpolate_Optimal32x_6p5oz(float x, float* y) {
  float z = x - 1/2.0;
  float even1 = y[1]+y[0], odd1 = y[1]-y[0];
  float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
  float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
  float c0 = even1*0.42685983409379380 + even2*0.07238123511170030 + even3*0.00075893079450573;
  float c1 = odd1*0.35831772348893259 + odd2*0.20451644554758297 + odd3*0.00562658797241955;
  float c2 = even1*-0.217009177221292431 + even2*0.20051376594086157 + even3*0.01649541128040211;
  float c3 = odd1*-0.25112715343740988 + odd2*0.04223025992200458 + odd3*0.02488727472995134;
  float c4 = even1*0.04166946673533273 + even2*-0.06250420114356986 + even3*0.02083473440841799;
  float c5 = odd1*0.08349799235675044 + odd2*-0.04174912841630993 + odd3*0.00834987866042734;
  return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// http://music.columbia.edu/pipermail/music-dsp/2000-August/038440.html

/*
  (Dave/Muon Software Ltd has some 4-point spline thing?)
  Thanks Bram (and Dave), got that one too now, works ok!
  --------------------
  const double L1 = floor[-1];
  const double L0 = floor[ 0];
  const double H0 = floor[+1];
  const double H1 = floor[+2];
  const double x = fractpos;
  return (float)(L0 + .5f* x*(H0-L1 + x*(H0 + L0*(-2) + L1 + x*( (H0
  - L0)*9 + (L1 - H1)*3 + x*((L0 - H0)*15 + (H1 -  L1)*5 + x*((H0 -
  L0)*6 + (L1 - H1)*2))))));
  --------------------------
  -olli
*/

//----------

// http://music.columbia.edu/pipermail/music-dsp/2000-August/038442.html

/*
  Here's the three point version of the 5-point interpolation I posted the
  other week. Credit for these goes to Josuha Scholar of comp.dsp
  (neonfish99 at aol.com).
  I haven't had any time to turn this into a niftier function yet so just the
  raw code straight from Josh:

  // interpolates between L0 and H0 taking the previous (L1) and next (H1) points into account

  inline float ThirdInterp(const float x,const float L1,const float L0,const float H0,const float H1) {
    return
    L0 +
    .5f*
    x*(H0-L1 +
      x*(H0 + L0*(-2) + L1 +
          x*( (H0 - L0)*9 + (L1 - H1)*3 +
            x*((L0 - H0)*15 + (H1 -  L1)*5 +
                x*((H0 - L0)*6 + (L1 - H1)*2 )))));
  }

*/

//----------

/*
  http://music.columbia.edu/pipermail/music-dsp/2000-August/038455.html

  Now my interpolator collection has grown, and i need to ask if you know
  some of the missing/uncertain names, if there are agreed ones. Is any of
  these possibly "b-spline", which i've heard mentioned occasionally?

  // 1-point flat (didn't bother to copy all routines)
  // 2-point linear
  // 2-point raised cosine

  // 4-point spline (Joshua Scholar) (name???)
  const double L1 = floor[-1];
  const double L0 = floor[ 0];
  const double H0 = floor[+1];
  const double H1 = floor[+2];
  const double x = fractpos;
  return (float)(L0 + .5f* x*(H0-L1 + x*(H0 + L0*(-2) + L1 + x*( (H0
  - L0)*9 + (L1 - H1)*3 +
  x*((L0 - H0)*15 + (H1 -  L1)*5 + x*((H0 - L0)*6 + (L1 - H1)*2
  ))))));

  // 4-point spline (name???)
  const double ym1 = floor[-1];
  const double  y0 = floor[ 0];
  const double  y1 = floor[+1];
  const double  y2 = floor[+2];
  const double   x = fractpos;
  return(float)(x*x*x*(y0-y1+y2-ym1)+x*x*(-2.0*y0+y1-y2+2.0*ym1)+x*(y1-ym1)+y0);

  // 4-point geometric average spline (correct???) (name???)
  const double y0 = floor[-1];
  const double y1 = floor[ 0];
  const double y2 = floor[+1];
  const double y3 = floor[+2];
  const double yd = fractpos;
  return (float)((y1 + (y1-y0)*yd)*(1-yd)/2 + (y1 + (y2-y1)*yd)/2 +
  (y2 + (y3-y2)*(yd-1))*yd/2);

  // 6-point expanded Hermite (linear ramp between two 4th degree
  // polynomials both going through 5 points) (Is there a name for this
  // already???) UNOPTIMIZED
  const double ym2 = floor[-2];
  const double ym1 = floor[-1];
  const double  y0 = floor[ 0];
  const double  y1 = floor[+1];
  const double  y2 = floor[+2];
  const double  y3 = floor[+3];
  const double   x = fractpos;
  return (float) (
  x*x*x*x*x*(-5.0/12.0*y0+5.0/12.0*y1-5.0/24.0*y2+1.0/24.0*y3+5.0/24.0*ym1-
  1.0/24.0*ym2)+
  x*x*x*x*(13.0/12.0*y0-y1+11.0/24.0*y2-1.0/12.0*y3-7.0/12.0*ym1+0.125*ym2)+
  x*x*x*(5.0/12.0*y0-7.0/12.0*y1+7.0/24.0*y2-1.0/24.0*y3-1.0/24.0*ym1-
  1.0/24.0*ym2)+
  x*x*(-25.0/12.0*y0+1.5*y1-11.0/24.0*y2+1.0/12.0*y3+13.0/12.0*ym1-0.125*ym2)+
  x*(2.0/3.0*y1-1.0/12.0*y2-2.0/3.0*ym1+1.0/12.0*ym2)+
  y0);
*/

//----------------------------------------------------------------------
#endif
