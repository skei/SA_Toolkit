#ifndef sat_polyblep_included
#define sat_polyblep_included
//----------------------------------------------------------------------

#include "base/util/sat_math.h"

// tale
// http://www.kvraudio.com/forum/viewtopic.php?p=5269941#p5269941
// subtract result from naive saw..

template <typename T>
T SAT_PolyBlep(T t, T dt) {
  // 0 <= t < 1
  if (t < dt) {
    t /= dt;
    return t+t - t*t - 1.0; // 2 * (t - t^2/2 - 0.5)
  }
  // -1 < t < 0
  else if (t > (1.0 - dt)) {
    t = (t - 1.0) / dt;
    return t*t + t+t + 1.0; // 2 * (t^2/2 + t + 0.5)
  }
  else return 0.0;
}

//----------------------------------------------------------------------

// http://www.taletn.com/reaper/mono_synth/
// Adapted from "Phaseshaping Oscillator Algorithms for Musical Sound
// Synthesis" by Jari Kleimola, Victor Lazzarini, Joseph Timoney, and Vesa
// Valimaki.
// http://www.acoustics.hut.fi/publications/papers/smc2010-phaseshaping/
// subtract result from naive saw..

// template <typename T>
// T SAT_PolyBlep(T t, T dt) {
//   if (t<dt) return - SAT_Sqr(t/dt - 1.0f);
//   else if (t > 1.0f - dt) return SAT_Sqr((t - 1.0f) / dt + 1.0f);
//   return 0;
// }

//----------------------------------------------------------------------

// http://research.spa.aalto.fi/publications/papers/smc2010-phaseshaping/phaseshapers.py
// x = signal
// T = phase increment (f0/fs)
// h = transition height (negative for falling transitions)
// (after Valimaki + Huovilainen, IEEE SPM, Vol.24, No.2, p.121)
// add result to naive saw..
// skei: assume t is phase-wrapped (%1)

template <typename T>
T SAT_PolyBlep(T t, T dt, T h) {
  T t0,c0;
  if (t > (1-dt)) {             // -- before transition -----------------------
    t0 = t - 1;                 // fractional phase (negative, in samples)
    t0 /= dt;                   // fractional phase (-1,0)
    c0 = 0.5*t0*t0 + t0 + 0.5;  // correction polynomial
    c0 = c0 * h;                // scale with transition height
    return c0;
  }
  else if (t < dt) {            // -- after transition ------------------------
    t0 = t;                     // fractional phase (positive, in samples)
    t0 /= dt;                   // fractional phase (0,1)
    c0 = -0.5*t0*t0 + t0 - 0.5; // correction polynomial
    c0 = c0 * h;                // scale with transition height
    return c0;
  }
  else return 0;                // -- not inside transition area: nop ---------
};

//----------------------------------------------------------------------
#endif
