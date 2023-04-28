#ifndef mip_druttis_waveform_included
#define mip_druttis_waveform_included
//----------------------------------------------------------------------

#include <math.h>

/*
  http://musicdsp.org/showArchiveComment.php?ArchiveID=123
  by joakim[DOT]dahlstrom[AT]ongame[DOT]com

  How ever, the other day when finally understanding what bandlimited wave
  creation is (i am a noobie, been doing DSP stuf on and off for a half/year)
  it hit me i can implement one little part in my synths. It's all about the
  freq (that i knew), very simple you can reduce alias (the alias that you can
  hear that is) extremely by keeping track of your frequence, the way i solved
  it is using a factor, afact = 1 - sin(f*2PI). This means you can do audiable
  alias free synthesis without very complex algorithms or very huge tables,
  even though the sound becomes kind of low-filtered.
  Propably something like this is mentioned b4, but incase it hasn't this is
  worth looking up

  // Druttis

  Code :

  f := freq factor, 0 - 0.5 (0 to half samplingrate)

  afact(f) = 1 - sin(f*2PI)

  t := time (0 to ...)
  ph := phase shift (0 to 1)
  fm := freq mod (0 to 1)

  sine(t,f,ph,fm) = sin((t*f+ph)*2PI + 0.5PI*fm*afact(f))

  fb := feedback (0 to 1) (1 max saw)

  saw(t,f,ph,fm,fb) = sine(t,f,ph,fb*sine(t-1,f,ph,fm))

  pm := pulse mod (0 to 1) (1 max pulse)
  pw := pulse width (0 to 1) (1 square)

  pulse(t,f,ph,fm,fb,pm,pw) = saw(t,f,ph,fm,fb) - (t,f,ph+0.5*pw,fm,fb) * pm

  I am not completely sure about fm for saw & pulse since i cant test that atm.
  but it should work :) otherwise just make sure fm are 0 for saw & pulse.
  As you can see the saw & pulse wave are very variable.
*/

//----------------------------------------------------------------------

//class KWaveform_Sin_Druttis {
class MIP_DruttisSawWaveform {

  private:
    float _sr;
    float _prev_t;
    float _f;
    float _ps;
    float _fm;

  public:

    MIP_DruttisSawWaveform() {
      _sr     = 0.0f;
      _prev_t = 0.0f;
      //_f      = 0.0f;
      _ps     = 0.0f;
      _fm     = 0.5f;
    }

    // sample rate

    void setSR(float sr) {
      _sr = sr;
    }

    // phase shift (0 to 1)

    void setPS(float ps) {
      _ps = ps;
    }

    // freq mod (0 to 1)

    void setFM(float fm) {
      _fm = fm;
    }

    // freq factor, 0 - 0.5 (0 to half samplingrate)
    // = dt ? inv?

    //void setF(float f) {
    //  _f = f;
    //}

    float afact(float f) {
      //result := 1 - sin(f*MIP_PI2); // flaw
      return 1 - sqrt(f * 2 / _sr);
    }

    // t = time (0 to ...)
    // freq factor, 0 - 0.5 (0 to half samplingrate)

    float process(float t, float dt) {
      float f = 0.5f;//dt;
      return sin( (((t*f)+_ps)*MIP_PI2) + (MIP_PI05*_fm*afact(f)) );
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#if 0

// oops.. hack..

static float _sr      = 44100.0;
static float _prev_t  = 0.0;

//----------------------------------------------------------------------

void MIP_Waveform_Druttis_setSampleRate(float sr) {
  _sr = sr;
}

//----------

// f := freq factor, 0 - 0.5 (0 to half samplingrate)
// t  := time (0 to ...)
// ps := phase shift (0 to 1)
// fm := freq mod (0 to 1)
// fb := feedback (0 to 1) (1 max saw)
// pm := pulse mod (0 to 1) (1 max pulse)
// pw := pulse width (0 to 1) (1 square)

float _afact(float f) {
  //result := 1 - sin(f*MIP_PI2); // flaw
  return 1 - sqrt(f * 2 / _sr);
}

//----------

float MIP_Waveform_Sine_Druttis(float t, float f, float ps, float fm) {
  return sin( (((t*f)+ps)*MIP_PI2) + (MIP_PI05*fm*_afact(f)) );
}

//----------

float MIP_Waveform_Saw_Druttis(float t, float f, float ps, float fm, float fb) {
  float result = KWaveform_Sine_Druttis(t,f,ps,fb*KWaveform_Sine_Druttis(_prev_t/*t-1*/,f,ps,fm));
  _prev_t = t;
  return result;
}

//----------

float MIP_Waveform_Pulse_Druttis(float t, float f, float ps, float fm, float fb, float pm, float pw) {
  return KWaveform_Saw_Druttis(t,f,ps,fm,fb) - KWaveform_Saw_Druttis(t,f,ps+(0.5*pw),fm,fb) * pm;
}

#endif // 0

//----------------------------------------------------------------------
#endif
