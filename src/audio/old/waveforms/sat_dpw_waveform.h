#ifndef mip_dpw_waveform_included
#define mip_dpw_waveform_included
//----------------------------------------------------------------------

/*
  differentiated parabolic waveform
  differentiated polynomial waveform?
  "spectral tilt modification of the continuous-time signal before sampling"
*/

//----------------------------------------------------------------------

/*
  based on polynomial waveshaping, differentiating, and scaling a trivial
  sawtooth waveform, i.e., a bipolar modulo counter output. Squaring the
  bipolar modulo counter output leads to a piecewise parabolic signal, which is
  equivalent to the integral of the sawtooth waveform, and the differentiator
  can be implemented using the first-order difference operator. This algorithm
  has been recently extended to an arbitrary number of integrations and
  differentiations, and is called the differentiated polynomial waveform (DPW)
  method. The DPW method has a perceptual advantage, as aliasing is suppressed
  most in the lower half of the spectrum, where the hearing is most sensitive.
  The drawbacks of this approach are the cumbersome scaling of the output
  signal, and the requirement for differentiable polynomial waveforms
*/

//----------------------------------------------------------------------

// "based on the spectral tilt modification of the continuous-time signal
// before sampling."

/*
  - generate a ramp
  - square the result
  - use a differential filter
    (IE, find the difference between the last value and the new value)
  - then scale the results by F/SR
    (where F = frequency, and SR = sample rate).
*/

// needs state (history) variables

//----------------------------------------------------------------------

/*

  papers:

  "New approaches to digital subtractive synthesis"
  (Antti Huovilainen and Vesa Välimäki)
  http://www.music.mcgill.ca/~ich/research/misc/papers/cr1071.pdf

  forums:

  http://www.kvraudio.com/forum/viewtopic.php?f=33&t=375517
  http://www.kvraudio.com/forum/viewtopic.php?p=5171622#p5171622
  http://www.kvraudio.com/forum/viewtopic.php?p=5538080#p5538080
  http://www.kvraudio.com/forum/viewtopic.php?p=5586476#p5586476 // shabby
  http://www.kvraudio.com/forum/viewtopic.php?p=1710116#1710116  // paul sernine (order 5)
  http://www.kvraudio.com/forum/viewtopic.php?p=5271962#p5271962 // tale, order 2/3
  http://www.kvraudio.com/forum/viewtopic.php?p=1710116#1710116 // order 5

  others:

  http://users.spa.aalto.fi/vpv/DAFX13-keynote-slides.pdf
  https://www.elec.york.ac.uk/events/seminars/Vesa-York-VA-seminar-web-140609.pdf
  https://books.google.no/books?id=oaYgBQAAQBAJ&pg=PA208&lpg=PA208&dq=oscillator+dpw+differentiated+parabolic+wave&source=bl&ots=610D1-S378&sig=LO-Sx7cnkjY_YMGpJvpYSzybZH4&hl=no&sa=X&ved=0ahUKEwiqu47ThYbKAhUi_XIKHcvdDJIQ6AEIKjAB#v=onepage&q=oscillator%20dpw%20differentiated%20parabolic%20wave&f=false

  source code:

*/

//----------------------------------------------------------------------

// http://www.kvraudio.com/forum/viewtopic.php?p=5586476#p5586476
// shabby

// assuming t is frac wrapped

/*
  dpw
        a b c : worst best
  +
  - 2
  * 1
  / 1
*/

class MIP_DpwSawWaveform {
  private:
    float x1;
  public:
    MIP_DpwSawWaveform() {
      reset();/*x1 = 0;*/
    }
    void reset(void) { x1 = 0; }
    float process(float t, float dt) {
      // Correct Signed naive integral
      float sum = t*t - t;
      float res = (sum - x1) / dt;
      x1 = sum;
      return res;
    }
};

//----------------------------------------------------------------------

// tale - order 2

/*
  dpw2
        a b c : worst best
  + 2
  - 2
  * 4
  / 1
*/

class MIP_Dpw2SawWaveform {
  private:
    float x1;
  public:
    MIP_Dpw2SawWaveform() {
      x1 = 0;
    }
    void reset(void) { x1 = 0; }
    float process(float t, float dt) {

      // Correct phase, so it would be in line with sin(2.*MIP_PI * t)
      t += (0.5f + (0.5f*dt));
      if (t >= 1) t -= 1;

      float x = 2.0f * t - 1; // Naive saw

      float x0 = x*x;
      float y = (x0 - x1) / (4.0f * dt);
      x1 = x0;
      return y;
    }
};

//----------------------------------------------------------------------

// tale - order 3

/*
  dpw
        a b c : worst best
  + 1
  - 6
  * 5
  / 1
*/

class MIP_Dpw3SawWaveform {
  private:
    float x1,x2;
  public:
    MIP_Dpw3SawWaveform() {
      x1 = 0;
      x2 = 0;
    }
    void reset(void) { x1=0; x2=0; }
    float process(float t, float dt) {

      t += 0.5f + dt;
      if (t >= 1) t -= 1;

      //t += (0.5 + (0.5*dt));
      //if t >= 1 then t -= 1;

      float x   = (2.0f * t) - 1; // naive saw

      float x0  = (x*x*x) - x; // hmmm.. 3*3*3 - 3 = 24
      float y   = ((x0 - x1) - (x1 - x2)) / (24.0f*dt*dt);
      x2  = x1;
      x1  = x0;
      return y;
    }
};

//----------------------------------------------------------------------

//http://mac.kaist.ac.kr/pubs/ValimakiNamSmithAbel-taslp2010.pdf
/*
  N   scaling
  1   1
  2   p / 4
  3   p^2 / 24
  4   p^3 / 192
  5   p^4 / 1920
  6   p^5 / 23040

*/

//----------

// http://users.spa.aalto.fi/vpv/DAFX13-keynote-slides.pdf

/*
  hmmm..
  something is wrong with the scale (the /224... stuff)
  around c7, this looks very promising in a spectrum analyzer
  but lower frequencies have waaayy to high amplitude
*/

/*
class KWaveform_Saw_DPW4 {
  private:
    float x1,x2,x3;
  public:
    KWaveform_Saw_DPW4() {
      x1 = 0;
      x2 = 0;
      x2 = 0;
    }
    float process(float t, float dt) {
      t += 0.5f + dt;
      if (t >= 1) t -= 1;
      //t += (0.5 + (0.5*dt));
      //if t >= 1 then t -= 1;
      float x   = (2.0f * t) - 1; // naive saw
      float x0  = (x*x*x*x) - 2*(x*x); // 4*4*4*4 - 2(4*4) =  256 - 32 = 224
      //float y   = ((x0 - x1) - (x1 - x2)) / (24.0f*dt*dt);
      float y = 0;//  = ((x0 - x1) - (x1 - x2) - (x2 - x3)) / (224.0f*dt*dt*dt); //4*8 + 4*4 + 4*2 = 32 + 16 + 8
      x3  = x2;
      x2  = x1;
      x1  = x0;
      return y;
    }
};
*/

//----------------------------------------------------------------------
#endif

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=5171622#p5171622
  A DPW triangle:
  a= abs(saw) - 0.5;  // centre it around zero
  b = a * abs(a);     // parabole
  c = a - b;
  d = differentiate(c);
  Now a DPW "Vintage saw":
  a1 = abs(saw) - 0.5;
  a2 = a1 * a1;       // parabole
  b = a2 * a2;        // another parabole
  c = b - a2;
  d = differentiate(c);
  output will be one octave higher though, so freq/2.
  //
  seems the amplitude varies a lot based on frequency. Do you know if
  there is a precise way to compensate for this?
  //
  Yes, you simply divide the output by frequency (more precisely, the
  phase increment). If you use higher order DPWs, be sure to use
  doubles, otherwise you'll quickly run into numerical issues.
*/

// http://www.kvraudio.com/forum/viewtopic.php?p=5538080#p5538080

/*
dpw: differentiated parabolic wave
https://www.elec.york.ac.uk/events/seminars/Vesa-York-VA-seminar-web-140609.pdf
  1. naive square: x[n] (-1..1)
  2. square signal: x[n]^2 (0..1)
  3. differentiate: x[n]^2 - x[n-1]^2
polynomial order:
  n=
  1   x
  2   x^2
  3   x^3 - x
  4   x^4 - 2x^2
  5   x^5 - 10x^3/3 + 7x/3
  6   x^6 - 5x^4 + 7x^2
the polynomial signal must be differened n-1 times and scaled to get the sawtooth wave
the ptr algorithm implements dpw efficiently and extends it
*/
