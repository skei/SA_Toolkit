#ifndef mip_eptr_waveform_included
#define mip_eptr_waveform_included
//----------------------------------------------------------------------

/*
  "Improved polynomial transition regions algorithm for alias-suppressed signal
  synthesis"
  (Dániel Ambrits an Balázs Bank)
  http://home.mit.bme.hu/~bank/publist/smc13.pdf

  based on the fact that the offset of DPW and PTR waveforms compared to the
  trivial (modulo-counter generated) waveform is due to a phase shift of the
  DPW and PTR signals. When this phase shift is removed, the linear regions of
  the waveform can be taken simply as the  trivial  waveform  values

*/

// paper also describes a dpw asymmetric triangle

//----------------------------------------------------------------------

//----------------------------------------------------------------------







//----------------------------------------------------------------------

//float SOsc_Saw_EPTR(float t, float dt) {
//  float result = t;
//  if (t > (1 - dt)) result = t - (t/dt) + (1.0f/dt) - 1.0f;
//  result -= 0.5;
//  result *= 2;
//  return result;
//}

class MIP_EptrSawWaveform {
  public:
    float process(float t, float dt) {
      float result = t;
      // t = sync ? -1 : t + dt + FMBuffer[i]*FMLevel.Value;
      if (t > (1 - dt)) result = t - (t/dt) + (1.0f/dt) - 1.0f;
      result -= 0.5;
      result *= 2;
      return result;
    }
};

//----------------------------------------------------------------------

/*

  http://home.mit.bme.hu/~bank/publist/smc13.pdf

  p = the current value of the trivial signal generator
  A = the gradient of the section. (A = 1, when the signal increases from −1
  to 1 during one period. If A < 0, the signal decreases.)
  T = f0/fs, where f0 is the fundamental frequency of the signal and
  fs is the sampling frequency

  // saw

  correct(p) = p - p/T + 1/T - 1

  p = p + 2*T;
  if p > 1 - T
    y = correct(p);
    p = p - 2;
  else
    y = p;

  // asymmetric triangle

  B = -A/(A-1), pmax=1, pmin=-1

  a2 = -1        / 4(A-1)T
  a1 =  2AT-4T+2 / 4(A-1)T
  a0 = -(AT-1)^2 / 4T(A-1)

  b2 = -1        / 4(B+1)T
  b1 =  2BT+4T-2 / 4/B+1)T
  b0 = -(BT+1)^2 / 4T(B+1)

  linear region = p
  correctMax(p) = a2*p^2 + a1*p + a0
  correctMin(p) = b2*p^2 + b1*p + b0


  if dir == 1 // counting up?
    p = p + 2*A*T;
    if p > 1 - A*T // transition region?
      y = correctMax(p);
      p = 1 + (p-1)*B/A;
      dir = -1;
    else // linear region
      y = p;
  else // counting down
    p = p + 2*B*T;
    if p < -1 - B*T // transition region?
      y = correctMin(p);
      p = -1 + (p+1) * A/B;
      dir = 1;
    else // linear region
      y = p;

  assumes that the values of |A| and |B| are not higher than fs/f0, so there is
  no region that fits between two samples. The allowed highest gradient case
  is close enough to the sawtooth waveform..
  the asymmetric triangle with a one sample-time transition can be safely used
  instead of the sawtooth signal.

  .. allowing continuous transition between symmetric triangle and sawtooth
  signals. Future research includes the extension of the algorithm to higher
  orders, and to arbitrary waveforms composed of line segments
  (e.g., trapezoidal waves)..

*/



//----------------------------------------------------------------------
#endif

/*
  https://cycling74.com/forums/topic/please-help-extend-my-polynomial-transition-region-sawtooth-to-an-eptr-sawtooth/
  https://github.com/tebjan/VVVV.Audio/blob/master/Source/VVVV.Audio.Signals/Sources/OscSignal.cs

  http://www.kvraudio.com/forum/viewtopic.php?p=5971739#p5971739 // tonfilm

  http://home.mit.bme.hu/~bank/publist/smc13.pdf

*/

//----------

/*
  pre-computing correction polynomials for the samples in the
  transition, while the linear regions of the signal are offset
  by a constant value
*/

//----------

/*


  http://www.kvraudio.com/forum/viewtopic.php?p=5971739#p5971739

  The EPTR in the screenshot uses this algorithm, which i found somewhere here in the forums:

  FPhase = sync ? -1 : FPhase + t2 + FMBuffer[i]*FMLevel.Value;
  if (FPhase > 1.0f - T) { //transition
    sample = FPhase - (FPhase / T) + (1.0f / T) - 1.0f;
    FPhase -= 2.0f;
  }
  else {
    sample = FPhase;
  }

*/

//----------

/*
  EPTR Saw (efficient polynomial transition region)
  http://www.kvraudio.com/forum/viewtopic.php?p=5619182#p5619182

  float SawOut;
  phaseAccumulator = phaseAccumulator + 2.f * phaseIncrement;
  if (phaseAccumulator > 1.f - phaseIncrement) {
    SawOut = phaseAccumulator - (phaseAccumulator/phaseIncrement) + (1.f / phaseIncrement) - 1.f;
    phaseAccumulator = phaseAccumulator - 2.f;
  }
  else {
    SawOut = phaseAccumulator;
  }

  .. against DPW, the CPU is in the same ballpark.
  .. the frequency response is also virtually the same.

  Yeah, I though as much, thanks for testing! On my machine it has
  *slightly* better CPU, this is definetly a winner in the
  'cheap & chearful class', since it behaves better when modulating.
*/

//----------

/*
  http://home.mit.bme.hu/~bank/publist/smc13.pdf
  improved polynomial transition regions algorithm for alias-suppressed
  signal synthesis
*/
