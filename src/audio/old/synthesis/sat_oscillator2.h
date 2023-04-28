#ifndef mip_oscillator2_included
#define mip_oscillator2_included
//----------------------------------------------------------------------

#include "base/utils/mip_math.h"
#include "audio/old/waveforms/mip_polyblep_waveform.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class T>
class MIP_Oscillator2 {

//------------------------------
private:
//------------------------------

  T MPulseWidth = 0.5;
  T MSawSqu     = 0.0;
  T MSquTri     = 0.0;
  T MTriSin     = 0.0;
  T ph          = 0.0;
  T phadd       = 0.0;
  T z1          = 0.0;
  T MSampleRate = 0.0;
  T MIrate      = 0.0;

//------------------------------
public:
//------------------------------

  //MIP_Oscillator2() {
  //}

  //----------

  //~MIP_Oscillator2() {
  //}

//------------------------------
public:
//------------------------------

  T getPhase(void) { return ph; }
  T getPhaseAdd(void) { return phadd; }

//------------------------------
public:
//------------------------------

  void setSampleRate(T rate) {
    MIP_Assert( rate > 0 );
    MSampleRate = rate;
    MIrate = 1.0 / MSampleRate;
  }

  //----------

  void setFrequency(T hz) {
    //KTrace("hz %.2f\n",hz);
    MIP_Assert( hz >= 1.0 );
    MIP_Assert( hz < (MSampleRate*0.5) );
    //phadd = hz / srate;
    phadd = hz * MIrate;
  }

  //----------

  void setPhase(T a) {
    //MIP_Assert( a >= 0 );
    //MIP_Assert( a <  1 );
    ph = MIP_Fract(a);
  }

  //----------

  void setPhaseAdd(T a) {
    MIP_Assert( a >= 0.0 );
    MIP_Assert( a <= 1.0 );
    phadd = a;
  }

  //----------

  void reset(void) {
    ph = 0.0;
    phadd = 0.0;
    z1 = 0.0;
  }

  //----------

  void setPulseWidth(T pw) {
    MIP_Assert( pw >= 0.0 );
    MIP_Assert( pw <= 1.0 );
    MPulseWidth = pw;
  }

  //----------

  void setSawSqu(T sawsqu) {
    MIP_Assert( sawsqu >= 0.0 );
    MIP_Assert( sawsqu <= 1.0 );
    MSawSqu = sawsqu;
  }

  //----------

  void setSquTri(T squtri) {
    MIP_Assert( squtri >= 0.0 );
    MIP_Assert( squtri <= 1.0 );
    MSquTri = squtri;
  }

  //----------

  void setTriSin(T trisin) {
    MIP_Assert( trisin >= 0.0 );
    MIP_Assert( trisin <= 1.0 );
    MTriSin = trisin;
  }

//------------------------------
public:
//------------------------------

  T process() {
    MIP_Assert( MSampleRate > 0.0 );
    ph += phadd;
    ph = MIP_Fract(ph);
    T t1 = ph + 0.5;
    t1 = MIP_Fract(t1);
    T saw1 = 2.0f * t1 - 1.0;
    saw1 -= MIP_PolyBlep(t1,phadd);
    T t2 = t1 + MPulseWidth;
    t2 = MIP_Fract(t2);
    T saw2 = 2.0 * t2 - 1.0;
    saw2 -= MIP_PolyBlep(t2,phadd);
    T squ = saw1 - (saw2*MSawSqu);
    z1 = (phadd * squ) + ((1.0 - phadd) * z1);
    //TODO: KInterpolate_Linear
    T tri  = squ * (1.0 - MSquTri) + (z1 * 4.0)        * MSquTri;
    T sine = tri * (1.0 - MTriSin) + sin(ph * MIP_PI2) * MTriSin;
    T out  = tri * (1.0 - MTriSin) + sine              * MTriSin;
    return out;
  }

  //----------

  T process_mod(T mod, T ofs=0.0) {
    MIP_Assert( MSampleRate > 0.0 );
    //ph += phadd;
    ph += (phadd + (phadd*ofs));
    ph = MIP_Fract(ph);

    //ph *= scale;

    T phm = MIP_Fract(ph+mod);
    T t1 = phm + 0.5;
    t1 = MIP_Fract(t1);
    T saw1 = 2.0f * t1 - 1.0;
    saw1 -= MIP_PolyBlep(t1,phadd);
    T t2 = t1 + MPulseWidth;
    t2 = MIP_Fract(t2);
    T saw2 = 2.0 * t2 - 1.0;
    saw2 -= MIP_PolyBlep(t2,phadd);
    T squ = saw1 - (saw2*MSawSqu);
    z1 = (phadd * squ) + ((1.0 - phadd) * z1);
    T tri  = squ * (1.0 - MSquTri) + (z1 * 4.0)       * MSquTri;
    T sine = tri * (1.0 - MTriSin) + sin(phm*MIP_PI2) * MTriSin;
    T out  = tri * (1.0 - MTriSin) + sine             * MTriSin;
    return out;
  }

};

//----------------------------------------------------------------------
#endif
