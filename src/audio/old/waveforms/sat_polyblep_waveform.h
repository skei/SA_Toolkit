#ifndef mip_polyblep_waveform_included
#define mip_polyblep_waveform_included
//----------------------------------------------------------------------

#include "base/utils/mip_math.h"

/*

// http://www.kvraudio.com/forum/viewtopic.php?p=5269941#p5269941
// subtract result from naive saw..

float MIP_PolyBlep(float t, float dt) {
  // 0 <= t < 1
  if (t < dt) {
    t /= dt;
    return t+t - t*t - 1.0f; // 2 * (t - t^2/2 - 0.5)
  }
  // -1 < t < 0
  else if (t > (1-dt)) {
    t = (t-1.0f) / dt;
    return t*t + t+t + 1.0f; // 2 * (t^2/2 + t + 0.5)
  }
  else return 0.0f;
}

*/

//----------------------------------------------------------------------

/*
  Polynomial Bandlimited Step

  http://martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/ // same as (kvr/tale)
  http://code.google.com/p/foo-plugins/source/browse/trunk/incubator/foo-yc20/polyblep.cpp?r=211 // pekonen,. higher order polyblep
  http://code.google.com/p/foo-plugins/source/browse/trunk/incubator/foo-yc20/polyblep.cpp?r=211  // pekonen #2
  http://research.spa.aalto.fi/publications/papers/smc2010-phaseshaping/phaseshapers.py

  http://www.kvraudio.com/forum/viewtopic.php?f=33&t=375517 // tale
  http://www.kvraudio.com/forum/viewtopic.php?t=398553 // mystran
  http://www.kvraudio.com/forum/viewtopic.php?p=5965741#p5965741
  http://www.kvraudio.com/forum/viewtopic.php?p=5939531#p5939531
  http://www.kvraudio.com/forum/viewtopic.php?f=33&t=423884  // 7th order 4 point

  http://www.willpirkle.com/forum/algorithm-design/trivial-square-wave-polyblep/

*/

//----------------------------------------------------------------------
// http://www.kvraudio.com/forum/viewtopic.php?p=5269941#p5269941
//----------------------------------------------------------------------

// tale

// subtract result from naive saw..

float MIP_PolyBlep(float t, float dt) {
  // 0 <= t < 1
  if (t < dt) {
    t /= dt;
    return t+t - t*t - 1.0f; // 2 * (t - t^2/2 - 0.5)
  }
  // -1 < t < 0
  else if (t > (1-dt)) {
    t = (t-1.0f) / dt;
    return t*t + t+t + 1.0f; // 2 * (t^2/2 + t + 0.5)
  }
  else return 0.0f;
}

//----------------------------------------------------------------------

// http://www.taletn.com/reaper/mono_synth/

// Adapted from "Phaseshaping Oscillator Algorithms for Musical Sound
// Synthesis" by Jari Kleimola, Victor Lazzarini, Joseph Timoney, and Vesa
// Valimaki.
// http://www.acoustics.hut.fi/publications/papers/smc2010-phaseshaping/

// subtract result from naive saw..

float MIP_PolyBlep2(float t, float dt) {
  if (t<dt) return - MIP_Sqr(t/dt - 1.0f);
  else if (t > 1.0f - dt) return MIP_Sqr((t - 1.0f) / dt + 1.0f);
  return 0;
}

//----------------------------------------------------------------------
// http://research.spa.aalto.fi/publications/papers/smc2010-phaseshaping/phaseshapers.py
//----------------------------------------------------------------------

// x = signal
// T = phase increment (f0/fs)
// h = transition height (negative for falling transitions)
// (after Valimaki + Huovilainen, IEEE SPM, Vol.24, No.2, p.121)

/*
  skei: assume t is phase-wrapped (%1)
*/

// add result to naive saw..

float MIP_PolyBlep3(float t, float dt, float h) {
  float t0,c0;
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

/*
  h = -1 for aws
  h = to-phase - from.phase

  if sync polyblep(t,dt,h)
  else polyblep(t,dt,-2)


*/

//----------------------------------------------------------------------
// https://code.google.com/p/foo-plugins/source/browse/trunk/incubator/foo-yc20/polyblep.cpp?r=211
//----------------------------------------------------------------------

// valimaki huovilainen

/*
float MIP_PolyBlep4(float t, float dt) {
  if (t>0) return (t - (t*t)/2.0 - 0.5);
  return ((t*t)/2.0 + t + 0.5);
}
*/

// These have slightly less high frequency aliasing bands but have much more sub-harmonic aliasing
// than the one by Välimäki & Huovilainen.

// pekonen

/*
float MIP_PolyBlep5(float t, float dt) {
  if (t>0) return t*t*t*t*3/14 - t*t*t*4/7 + t*6/7 - 0.5;
  else return -t*t*t*t*3/14 - t*t*t*4/7 + t*6/7 + 0.5;
}
*/

// pekonen2

/*float MIP_PolyBlep6(float t, float dt) {
  if (t>0) return t*t*t*t*3/26 - t*t*t*4/13 - t*t*3/13 + t*24/26 - 0.5;
  else return -t*t*t*t*3/26 - t*t*t*4/13 + t*t*3/13 + t*24/26 + 0.5;
}
*/

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class MIP_PolyblepSawWaveform {
  public:
    float process(float t, float dt) {
      // Correct phase, so it would be in line with sin(2.*M_PI * t)
      t += 0.5f;
      if (t >= 1) t -= 1;
      float saw = 2.0f * t - 1;
      saw -= MIP_PolyBlep(t,dt);
      return saw;
    }
};

class MIP_Polyblep2SawWaveform {
  public:
    float process(float t, float dt) {
      // Correct phase, so it would be in line with sin(2.*M_PI * t)
      t += 0.5f;
      if (t >= 1) t -= 1;
      float saw = 2.0f * t - 1;
      saw -= MIP_PolyBlep2(t,dt);
      return saw;
    }
};

class MIP_Polyblep3SawWaveform {
  public:
    float process(float t, float dt) {
      // Correct phase, so it would be in line with sin(2.*M_PI * t)
      t += 0.5f;
      if (t >= 1) t -= 1;
      float saw = 2.0f * t - 1;
      saw += MIP_PolyBlep3(t,dt,-2);
      return saw;
    }
};


//----------------------------------------------------------------------

// pwm: replace the 0.5 to adjust pulsewidth

class MIP_PolyblepSquWaveform {
  public:
    float process(float t, float dt, float pw=0.5f) {
      // Correct phase, so it would be in line with sin(2.*M_PI * t)
      /*t += 0.5;
      if t >= 1 then t -= 1;*/
      float saw = 2.0f * t - 1;
      saw -= MIP_PolyBlep(t,dt);
      float result = saw;

      //t += 0.5f; // pulsewidth
      t += pw;
      if (t >= 1.0f) t -= 1.0f;
      saw = 2.0f * t - 1;
      saw -= MIP_PolyBlep(t,dt);
      result -= saw;
      return result;
    }
};


//----------------------------------------------------------------------

class MIP_PolyblepTriWaveform {
  private:
    MIP_PolyblepSquWaveform squ;
    float x1;
  public:
    MIP_PolyblepTriWaveform() {
      x1 = 0;
    }
    float process(float t, float dt) {
      float a = squ.process(t,dt);
      float result = (dt * a) + ((1.0f - dt) * x1);
      x1 = result;
      result *= 4;
      return result;
    }
};

//----------------------------------------------------------------------
// https://github.com/martinfinke/PolyBLEP/blob/master/PolyBLEP.cpp
//----------------------------------------------------------------------

class MIP_PolyblepRampWaveform {
  public:
    float process(float t, float dt) {
      //t += 0.5f;
      if (t >= 1) t -= 1;
      float ramp = 1 - 2*t;
      ramp += MIP_PolyBlep(t,dt);
      return ramp;
    }
};

//----------------------------------------------------------------------
#endif

/*

http://www.kvraudio.com/forum/viewtopic.php?f=33&t=425792&start=30

  double poly_blep(double t, double dt) {
    if (t < dt) {
      t = t/dt - 1;
      return -t*t;
    }
    else if (t > 1 - dt) {
      t = (t - 1)/dt + 1;
      return t*t;
    }
    else {
      return 0;
    }
  }

  // Setup oscillator.
  double freq = 220; // Hz
  double pw = 0.5; // [0.0..1.0]
  double phase = 0;
  double phase_inc = freq / sample_rate;

  // Generate samples.
  for (int i = 0; i < num_samples; ++i) {
    // Start with naive PW square.
    double sample;
    if (phase < pw) {
      sample = 1;
    }
    else {
      sample = -1;
    }

    // Correct rising discontinuity.
    sample = sample + poly_blep(phase, phase_inc);

    // Correct falling discontinuity.
    double phase2 = phase + 1 - pw;
    phase2 = phase2 - floor(phase2);
    sample = sample - poly_blep(phase2, phase_inc);

    // Increment phase for next sample.
    phase = phase + phase_inc;
    phase = phase - floor(phase);

    // Output current sample.
    output_buffer[i] = sample;
}
*/




#if 0

  http://www.kvraudio.com/forum/viewtopic.php?p=5965741#p5965741


function poly_blep(t, dt)
{
  if (t < dt)
  {
    t = t/dt - 1;
    return -t*t;
  }
  else if (t > 1 - dt)
  {
    t = (t - 1)/dt + 1;
    return t*t;
  }
  else
  {
    return 0;
  }
}

// Setup oscillator.
double freq = 220; // Hz
double pw = 0.5; // [0.0..1.0]
double phase = 0;
double phase_inc = freq / sample_rate;

// Generate samples.
for (int i = 0; i < num_samples; ++i)
{
  // Start with naive PW square.
  double sample;
  if (phase < pw) { sample = 1; }
  else { sample = -1; }
  // Correct rising discontinuity.
  sample = sample + poly_blep(phase, phase_inc);
  // Correct falling discontinuity.
  double phase2 = phase + 1 - pw;
  phase2 = phase2 - floor(phase2);
  sample = sample - poly_blep(phase2, phase_inc);
  // Increment phase for next sample.
  phase = phase + phase_inc;
  phase = phase - floor(phase);
  // Output current sample.
  output_buffer[i] = sample;
}

*)

//----------------------------------------------------------------------

{
  http://www.kvraudio.com/forum/viewtopic.php?p=5939531#p5939531

  desc:Naive saw
  slider1:440<20,20000,1>Frequency (Hz)
  @slider
    dt = slider1 / srate;
  @sample
    y = 2*t - 1;
    t += dt;
    t -= floor(t);
    spl0 = spl1 = 0.25 * y;

  desc:PolyBLEP saw
  slider1:440<20,20000,1>Frequency (Hz)
  @slider
    dt = slider1 / srate;
  @sample
    y1 = y0;
    y0 = 0;
    t += dt;
    t -= floor(t);
    t < dt ? (
      x = t / dt;
      y1 -= 0.5*x*x;
      x = 1 - x;
      y0 += 0.5*x*x;
    );
    y0 += t;
    y1 = 2*y1 - 1;
    spl0 = spl1 = 0.25 * y1;

  desc:Naive hard-sync saw
  slider1:440<20,20000,1>Master (Hz)
  slider2:1000<20,20000,1>Slave (Hz)
  @slider
    dt0 = slider1 / srate;
    dt1 = slider2 / srate;
  @sample
    y = 2*t1 - 1;
    t0 += dt0;
    t0 -= floor(t0);
    t1 += dt1;
    t1 -= floor(t1);
    t0 < dt0 ? t1 = t0 / dt0 * dt1;
    spl0 = spl1 = 0.25 * y;

  desc:PolyBLEP hard-sync saw
  slider1:440<20,20000,1>Master (Hz)
  slider2:1000<20,20000,1>Slave (Hz)
  @slider
    dt0 = slider1 / srate;
    dt1 = slider2 / srate;
  @sample
    y1 = y0;
    y0 = 0;
    t0 += dt0;
    t0 -= floor(t0);
    t1 += dt1;
    t1 -= floor(t1);
    t0 < dt0 ? t1 = t0 / dt0 * dt1;
    t = t0;
    n = ceil(dt1 / dt0);
    a = dt1 / dt0 - (n - 1);
    loop(n,
      t < dt0 ? (
        x = t / dt0;
        y1 -= a * 0.5*x*x;
        x = 1 - x;
        y0 += a * 0.5*x*x;
      );
      t += 1 - dt0 / dt1;
      t -= floor(t);
      a = 1;
    );
    y0 += t1;
    y1 = 2*y1 - 1;
    spl0 = spl1 = 0.25 * y1;

}

#endif // 0
