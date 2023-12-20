#ifndef sat_oscillator_included
#define sat_oscillator_included
//----------------------------------------------------------------------

#include "base/utils/sat_math.h"
#include "audio/synthesis/sat_polyblep.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class T>
class SAT_MorphOscillator {

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

  //SAT_MorphOscillator() {
  //}

  //----------

  //~SAT_MorphOscillator() {
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
    SAT_Assert( rate > 0 );
    MSampleRate = rate;
    MIrate = 1.0 / MSampleRate;
  }

  //----------

  void setFrequency(T hz) {
    //KTrace("hz %.2f\n",hz);
    SAT_Assert( hz >= 1.0 );
    SAT_Assert( hz < (MSampleRate * 0.5) );
    //phadd = hz / srate;
    phadd = hz * MIrate;
  }

  //----------

  void setPhase(T a) {
    //SAT_Assert( a >= 0 );
    //SAT_Assert( a <  1 );
    ph = SAT_Fract(a);
  }

  //----------

  void setPhaseAdd(T a) {
    SAT_Assert( a >= 0.0 );
    SAT_Assert( a <= 1.0 );
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
    SAT_Assert( pw >= 0.0 );
    SAT_Assert( pw <= 1.0 );
    MPulseWidth = pw;
  }

  //----------

  void setSawSqu(T sawsqu) {
    SAT_Assert( sawsqu >= 0.0 );
    SAT_Assert( sawsqu <= 1.0 );
    MSawSqu = sawsqu;
  }

  //----------

  void setSquTri(T squtri) {
    SAT_Assert( squtri >= 0.0 );
    SAT_Assert( squtri <= 1.0 );
    MSquTri = squtri;
  }

  //----------

  void setTriSin(T trisin) {
    SAT_Assert( trisin >= 0.0 );
    SAT_Assert( trisin <= 1.0 );
    MTriSin = trisin;
  }

  //----------

  void setMorph(T morph) {
    MSawSqu = 0.0;
    MSquTri = 0.0;
    MTriSin = 0.0;
    T i3 = (1.0 / 3.0);
    T i32 = (1.0 / 3.0) * 2.0;
    T v;
    if (morph <= i3) {
      // 0..0.33 -> 0..1
      v = (morph - 0.0) * 3.0; 
      setSawSqu(v);
      setSquTri(0);
      setTriSin(0);
    }
    else if (morph <= i32) {
      v = (morph - i3) * 3.0; // 0.33..0.66 -> 0..1
      setSawSqu(1);
      setSquTri(v);
      setTriSin(0);
    }
    else {
      v = (morph - i32) * 3.0; // 0.66..1 -> 0..1
      setSawSqu(1);
      setSquTri(1);
      setTriSin(v);
    }

  }

//------------------------------
public:
//------------------------------

  T process() {
    SAT_Assert( MSampleRate > 0.0 );
    ph += phadd;
    ph = SAT_Fract(ph);
    T t1 = ph + 0.5;
    t1 = SAT_Fract(t1);
    T saw1 = 2.0f * t1 - 1.0;
//    saw1 -= SAT_PolyBlep<T>(t1,phadd);
    T t2 = t1 + MPulseWidth;
    t2 = SAT_Fract(t2);
    T saw2 = 2.0 * t2 - 1.0;
//    saw2 -= SAT_PolyBlep<T>(t2,phadd);
    T squ = saw1 - (saw2*MSawSqu);
    z1 = (phadd * squ) + ((1.0 - phadd) * z1);
    //TODO: SAT_InterpolateLinear
    T tri  = squ * (1.0 - MSquTri) + (z1 * 4.0)        * MSquTri;
    T sine = tri * (1.0 - MTriSin) + sin(ph * SAT_PI2) * MTriSin;
    T out  = tri * (1.0 - MTriSin) + sine              * MTriSin;
    return out;
  }

  //----------

  T process_mod(T mod, T ofs=0.0) {
    SAT_Assert( MSampleRate > 0.0 );
    //ph += phadd;
    ph += (phadd + (phadd*ofs));
    ph = SAT_Fract(ph);
    //ph *= scale;
    T phm = SAT_Fract(ph+mod);
    T t1 = phm + 0.5;
    t1 = SAT_Fract(t1);
    T saw1 = 2.0f * t1 - 1.0;
    saw1 -= SAT_PolyBlep<T>(t1,phadd);
    T t2 = t1 + MPulseWidth;
    t2 = SAT_Fract(t2);
    T saw2 = 2.0 * t2 - 1.0;
    saw2 -= SAT_PolyBlep<T>(t2,phadd);
    T squ = saw1 - (saw2*MSawSqu);
    z1 = (phadd * squ) + ((1.0 - phadd) * z1);
    T tri  = squ * (1.0 - MSquTri) + (z1 * 4.0)       * MSquTri;
    T sine = tri * (1.0 - MTriSin) + sin(phm*SAT_PI2) * MTriSin;
    T out  = tri * (1.0 - MTriSin) + sine             * MTriSin;
    return out;
  }

};

//----------------------------------------------------------------------
#endif