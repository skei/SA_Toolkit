#ifndef mip_sinc_waveform_included
#define mip_sinc_waveform_included
//----------------------------------------------------------------------

// can't get this to work..
// spectrum doesn't look bad
// but just a pulse output?

/*
  Bandlimited waveforms...
  paul kellet
  http://musicdsp.org/showArchiveComment.php?ArchiveID=12

  Below is another waveform generation method based on a train of sinc
  functions (actually an alternating loop along a sinc between t=0 and
  t=period/2).

  The code integrates the pulse train with a dc offset to get a
  sawtooth, but other shapes can be made in the usual ways...
  Note that 'dc' and 'leak' may need to be adjusted for very high or low
  frequencies.

  I don't know how original it is (I ought to read more) but it is of
  usable quality, particularly at low frequencies. There's some scope
  for optimisation by using a table for sinc, or maybe a a truncated/
  windowed sinc?

  I think it should be possible to minimise the aliasing by fine tuning
  'dp' to slightly less than 1 so the sincs join together neatly, but I
  haven't found the best way to do it. Any comments gratefully received.

  float p=0.0f;       // current position
  float dp=1.0f;      // change in postion per sample
  float pmax;         // maximum position
  float x;            // position in sinc function
  float leak=0.995f;  // leaky integrator
  float dc;           // dc offset
  float saw;          // output

  //set frequency...
    pmax = 0.5f * getSampleRate() / freqHz;
    dc = -0.498f/pmax;
  //for each sample...
    p += dp;
    if (p < 0.0f) {
      p = -p;
      dp = -dp;
    }
    else if (p > pmax) {
      p = pmax + pmax - p;
      dp = -dp;
    }
    x = pi * p;
    if (x < 0.00001f) x=0.00001f; //don't divide by 0
    saw = leak*saw + dc + (float)sin(x)/(x);

*/

class MIP_SincSawWaveform {

  private:
    //float p;      // current position
    //float dp;     // change in postion per sample

    float _t;
    float _dt;

    float tmax;   // maximum position
    float x;      // position in sinc function
    float leak;   // leaky integrator
    float dc;     // dc offset
    float saw;    // output

    float srate;


  public:

    MIP_SincSawWaveform() {
      //p     = 0.0f;
      //dp    = 1.0f;

      _t = 0;
      _dt = 0;

      tmax  = 0;
      x     = 0;
      leak  = 0.995f;
      dc    = 0;
      saw   = 0;
      srate = 0;
    }

    //----------

    void setSampleRate(float arate) {
      //tmax = 0.5f * srate / hz;
      //dc = -0.498f / tmax;
      srate = arate;

    }

    //----------

    void setFrequency(float hz/*, float srate*/) {
      tmax = 0.5f * srate / hz;
      dc = -0.498f / tmax;
    }

    //----------

    float process(float t, float dt) {

      //t += dt;
      //if (t < 0.0f) {
      //  t = -t;
      //  dt = -dt;
      //}
      //else if (t > tmax) {
      //  t = tmax + tmax - t;
      //  dt = -dt;
      //}
      //x = MIP_PI * t;

      //dt = 1.0f;

      _t += _dt;
      if (_t < 0.0f) {
        _t = -_t;
        _dt = -_dt;
      }
      else if (_t > tmax) {
        _t = tmax + tmax - _t;
        //_t = -_t;
        _dt = -_dt;
      }
      x = MIP_PI * _t;

      if (x < 0.00001f) x=0.00001f; //don't divide by 0
      saw = leak*saw + dc + (float)sin(x)/(x);
      return saw;
    }

};

//----------------------------------------------------------------------
#endif

