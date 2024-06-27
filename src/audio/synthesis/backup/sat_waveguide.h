#ifndef mip_waveguide_included
#define mip_waveguide_included
//----------------------------------------------------------------------

// something fishy with this..

/*
  based on: http://musicdsp.org/archive.php?classid=4#98
*/

/*
  Class for waveguide/delay effects
  Type : IIR filter
  References : Posted by arguru[AT]smartelectronix.com
  Notes :
  Flexible-time, non-sample quantized delay , can be used for stuff like waveguide synthesis or time-based (chorus/flanger) fx.
  MAX_WG_DELAY is a constant determining MAX buffer size (in samples)
*/

//template<int MAX_WG_DELAY>

//----------------------------------------------------------------------

// srate 48khz, 20hz = 2400 samples,, 192k = 9600

//#define MAX_WG_DELAY 65536
#define MAX_WG_DELAY 10240

#include "audio/filters/mip_rc_filter.h"

#define MIP_WAVEGUIDE_MODE_OFF     0
#define MIP_WAVEGUIDE_MODE_PLUCKED 1
#define MIP_WAVEGUIDE_MODE_BOWED   2
#define MIP_WAVEGUIDE_MODE_BLOWN   3

//----------------------------------------------------------------------

class MIP_WaveGuide {

  private:

    MIP_RcFilter MFilter_LP;
    float         MBuffer[MAX_WG_DELAY] = {0};
    uint32_t      MMode                 = MIP_WAVEGUIDE_MODE_PLUCKED;
    int           MCounter              = 0;
    bool          MWrapped              = false;
    float         MPhase                = 0.0f;

  //MIP_RcFilter MFilter_HP;
  //float         MBuffer2[MAX_WG_DELAY];
  //bool          MImpulse  = false;

  public:

    void setSampleRate(float rate)  { MFilter_LP.setSampleRate(rate); /*MFilter_HP.sampleRate(rate);*/ }
    void setDamping(float damp)     { MFilter_LP.setWeight(damp); }
    void setMode(uint32_t AMode)    { MMode = AMode; }

    bool hasWrapped(void)           { return MWrapped; }


    //bool isImpulse(void)            { return MImpulse; }
    //void setImpulse(bool imp=true)  { MImpulse = imp; }
    //void damp_hi(float damp)    { MFilter_HP.weight(damp); }

  public:

    MIP_WaveGuide() {
      clear();
      //MImpulse = false;
    }

    //----------

    virtual ~MIP_WaveGuide() {
    }

    //----------

    void clear(void) {
      MCounter  = 0;
      MWrapped  = false;
      MPhase    = 0.0f;
      // this created a spike in cu each time a note was started..
      // do we need to clear the buffer?
      MIP_Memset(MBuffer,0,MAX_WG_DELAY*sizeof(float));
    }

    void reset() {
      MWrapped  = false;
      MPhase    = 0.0f;
    }

    //----------

    /*
    float softclip(float x) {
      if (x <= -1) return (-2 / 3);
      else if (x >= 1) return (2 / 3);
      else return x- ((x*x*x) / 3);
    }
    */

    //----------

    /*
      - separate feed & tap..
      - dual delay lines, feed the output from one into the other
        with either filtering/damping, and/or inversion
    */

    //----------

    float feed(float AInput, float AFeedback, float ADelay) {

      if ((MMode == MIP_WAVEGUIDE_MODE_PLUCKED) && MWrapped) AInput = 0.0f;

      // calculate delay offset
      float back = (float)MCounter - ADelay;
      if (back < 0.0) back = MAX_WG_DELAY + back;
      int index0 = (int)back;
      int index_1 = index0-1;
      int index1 = index0+1;
      int index2 = index0+2;
      if (index_1 < 0) index_1 = MAX_WG_DELAY - 1;
      //if (index1 >= MAX_WG_DELAY) index1 = 0;
      //if (index2 >= MAX_WG_DELAY) index2 = 0;
      if (index1 >= MAX_WG_DELAY) index1 -= MAX_WG_DELAY;
      if (index2 >= MAX_WG_DELAY) index2 -= MAX_WG_DELAY;
      // interpolate
      float y_1= MBuffer [index_1];
      float y0 = MBuffer [index0];
      float y1 = MBuffer [index1];
      float y2 = MBuffer [index2];
      float x = (float)back - (float)index0;
      float c0 = y0;
      float c1 = 0.5f*(y1-y_1);
      float c2 = y_1 - 2.5f*y0 + 2.0f*y1 - 0.5f*y2;
      float c3 = 0.5f*(y2-y_1) + 1.5f*(y0-y1);
      float output = ((c3*x+c2)*x+c1)*x+c0;
      // feedback/damping

      float fb = (AInput + output * AFeedback);
      fb = MFilter_LP.process(fb);
      //fb = MIP_Clamp(fb,-1,1);
      //fb = softclip(fb);
      fb = atan(fb);

      MBuffer[MCounter] = fb;
      MCounter++;
      if (MCounter >= MAX_WG_DELAY) { MCounter = 0; /*MWrapped = true;*/ }

      MPhase += 1.0f;
      if (MPhase >= ADelay) { MWrapped = true; }

      return output;
    }
    //----------

    // tap = 0..1 (delay multiplier)
    // delay length

    /*

    float tap(float ADelay, float ATap) {
      // calculate delay offset
      float back = (float)MCounter - (ADelay*ATap);
      if (back < 0.0) back = MAX_WG_DELAY + back;
      int index0 = (int)back;
      int index_1 = index0-1;
      int index1 = index0+1;
      int index2 = index0+2;
      if (index_1 < 0) index_1 = MAX_WG_DELAY - 1;
      if (index1 >= MAX_WG_DELAY) index1 = 0;
      if (index2 >= MAX_WG_DELAY) index2 = 0;
      // interpolate
      float y_1= MBuffer [index_1];
      float y0 = MBuffer [index0];
      float y1 = MBuffer [index1];
      float y2 = MBuffer [index2];
      float x = (float)back - (float)index0;
      float c0 = y0;
      float c1 = 0.5f*(y1-y_1);
      float c2 = y_1 - 2.5f*y0 + 2.0f*y1 - 0.5f*y2;
      float c3 = 0.5f*(y2-y_1) + 1.5f*(y0-y1);
      float output = ((c3*x+c2)*x+c1)*x+c0;
      // feedback/damping
//      float fb = MFilter_LP.process(AInput + output * AFeedback); // was +
//      fb = MIP_Clamp(fb,-1,1);
//      MBuffer[MCounter] = fb;
//      MCounter++;
//      if (MCounter >= ADelay) { MWrapped = true; }
//      if (MCounter >= MAX_WG_DELAY) { MCounter = 0; } //MWrapped = true; }
      return output;
    }

    */

};

//----------------------------------------------------------------------



//----------------------------------------------------------------------

#undef MAX_WG_DELAY

//----------------------------------------------------------------------
#endif
