#ifndef mip_ptr_waveform_included
#define mip_ptr_waveform_included
//----------------------------------------------------------------------

/*
  "Reducing aliasing from synthetic audio signals using polynomial transition regions"
  (Jari Kleimola and Vesa Välimäki)
  https://aaltodoc.aalto.fi/bitstream/handle/123456789/7747/publication6.pdf?sequence=9

  "replaces the samples on a finite region around each discontinuity with values
  taken from a smooth polynomial, based on a novel interpretation of the
  differentiated polynomial waveform (DPW) method"
*/

//----------------------------------------------------------------------

/*
  - free of the transient problems in the DPW algorithm
  - can be directly applied to any discontinuous waveform,
    not just the classical waveforms.

  The third advantage of the PTR algorithm is that it scales to arbitrary
  transition heights, and that it can be applied to other waveforms besides the
  trivial sawtooth — even to those that are nondifferentiable.

  sync:

  The DPW method fails to reduce the aliasing of hard sync implementations,
  because the derivatives of the waveshapers are discontinuous at the instant
  of phase reset. This is not of concern in PTR, which applies the transition
  polynomials separately for each discontinuity. However, the correction term
  of the transition polynomial has to be scaled according to the height of the
  discontinuity at the instant of the reset, which is determined by the
  frequency ratio  r = fslave/fmaster as
  h = 1 when r is an integer, r-floor(r) otherwise
  To preserve the phase continuity at the fundamental frequency, the phase of
  the slave oscillator is reset to Øs = rØm, where Øm is the phase of the
  master oscillator after the modulo operation

*/

/*
  "based on pre-computing correction polynomials for the samples in the
  transition, while the linear regions of the signal are offset by a constant
  value"
*/

//----------------------------------------------------------------------

/*
  http://research.spa.aalto.fi/publications/papers/spl-ptr/
  (python src)
*/

//----------------------------------------------------------------------

/*
  ptr1
        a b : worst best
  + 1     1   2     1
  - 2   1     3     3
  * 2   1 1   3     3
  / 1         1     1
*/

class MIP_Ptr1SawWaveform {
  public:
    float process(float t, float dt) {
      float T0 = dt;
      float P0 = 1/dt;
      float h = 1;
      float cdc = T0;
      float DC = 1 + cdc;
      float a1 = 2 - 2*h*P0;
      float a0 = 2*h - DC;
      float p = t;
      float y = 0;
      if (p >= T0) y = 2*p - DC;
      else y = a1*p + a0;
      return y;
    }
};

//----------------------------------------------------------------------

/*
  ptr2
        a b c : worst best
  + 2     2 2   4     2
  - 4   1       4     5
  * 3   1 3 3   6     4
  / 1           1     1
*/

class MIP_Ptr2SawWaveform {
  public:
    float process(float t, float dt) {
      float T0 = dt;
      float P0 = 1/dt;
      float h = 1;
      float T2 = T0+T0;
      float cdc = T2;
      float DC = 1 + cdc;
      float a21 = -h;
      float a11 = T2;
      float a01 = 2*h - DC;
      float a22 = h;
      float a12 = T2 - 4*h;
      float a02 = 4*h - DC;
      float p = t;
      float y = 0;
      if (p >= T2) {
        y = 2*p - DC;
      }
      else if (p >= T0) {
        float D = p * P0;
        y = (a22*D + a12)*D + a02;
      }
      else {
        float D = p * P0;
        y = (a21*D + a11)*D + a01;
      }
      return y;
    }
};

//----------------------------------------------------------------------

/*
  ptr3
        a b c : worst best
  + 3     3 2   6     3
  - 8   1       8     9
  * 4   1 4 4   8     4
  / 2           2     2
*/

class MIP_Ptr3SawWaveform {
  public:
    float process(float t, float dt) {
      float T0 = dt;
      float P0 = 1/dt;
      float h = 1;
      float T2 = T0+T0;
      float T3 = T2+T0;
      float cdc = T3;
      float DC = 1 + cdc;

      float h_3 = (h/3);
      float a31 = -h_3; // -h/3;
      float a11 = T2;
      float a01 = 2*h - DC;

      float a32 = 2*h_3; // 2*h/3;
      float a22 = -3*h;
      float a12 = T2 - a22;
      float a02 = h - DC;

      float a33 = a31;  // -h_3;
      float a23 = -a22; // -3*h
      float h9 = 9*h;
      float a13 = T2 - h9; // - 9*h;
      float a03 = h9 - DC; // 9*h - DC;

      float p = t;
      float y = 0;
      if (p >= T3) {
        y = 2*p - DC;
      }
      else if (p >= T2) {
        float D = p * P0;
        y = ((a33*D + a23)*D + a13)*D + a03;
      }
      else if (p >= T0) {
        float D = p * P0;
        y = ((a32*D + a22)*D + a12)*D + a02;
      }
      else {
        float D = p * P0;
        y = (a31*D*D + a11)*D + a01;
      }
      return y;
    }
};

//----------------------------------------------------------------------
// xor
//----------------------------------------------------------------------

/*

class MIP_Ptr1XorWaveform {
  public:
    float process(float t, float dt) {
      float T0 = dt;
      //float P0 = 1/dt;
      float T1 = 2*T0;
      float P1 = 2*T1;
      float cdc = 1 + T1;
      float p0 = t; // 0
      float p1 = t; // 0

      float s = 0; // trivial_xor

// p0 += T0 -> t += dt
// p1 += T1 -> t += dt
// h = 1;
// y = t * (2 - 2*h*(1/dt)) + 2*h - (1+dt)

      if (p0 < 0.5) {
        float h = 0.5;
        if (p1 < T1) {
          s = p1*(2 - 2*h*P1) + 2*h - cdc;
        }
      }
      else if (p0 < 0.75) {
        float h = 0.5;
        if (p1 < T1) {
          s = p1*(2 - 2*h*P1) + 2*h - cdc + 1;
        }
      }
      else {
        float h = 1;
        float pp = p1 - 0.5;
        if (pp < T1) {
          s = pp*(2 - 2*h*P1) + 2*h - cdc;
        }
      }

      return s;

      //p0 += T0
      //p1 += T1
      //if p0 > 1:	p0 -= 1
      //if p1 > 1:	p1 -= 1

    }
};

*/

//----------------------------------------------------------------------
#endif
