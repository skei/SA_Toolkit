#ifndef sa_tyr_osc_included
#define sa_tyr_osc_included
//----------------------------------------------------------------------

#include "base/utils/mip_fast_math.h"
#include "base/utils/mip_interpolation.h"
#include "audio/old/waveforms/mip_dsf_waveform.h"
#include "audio/old/filters/mip_allpass_filter.h"

template <class T>
class sa_tyr_osc {

//------------------------------
private:
//------------------------------

  T               tri_z1          = 0.0;
  //T               diff_z1         = 0.0;

//------------------------------
protected:
//------------------------------

  uint32_t        MType           = 0;
  T               MSampleRate     = 0.0;
  T               MInvSampleRate  = 0.0;

  T               MPhase          = 0.0;
  T               MPhaseAdd       = 0.0;

  T               MShape          = 0.0;
  T               MWidth          = 0.5;
  uint32_t        MWaveModType    = 0;
  T               MWaveModAmount  = 0;
  uint32_t        MPhaseModType   = 0;
  T               MPhaseModAmount = 0;

  //MIP_DsfWaveform MDsf            = {};
  //T               MPhase2         = 0.0;
  //T               MPhaseAdd2      = 0.0;

  MIP_AllpassFilter<T> MAllpassW  = {};
  MIP_AllpassFilter<T> MAllpassP  = {};

  T p_diff_prev = 0.0;
  T p_int_prev  = 0.0;

  T w_diff_prev = 0.0;
  T w_int_prev  = 0.0;

//------------------------------
public:
//------------------------------

  sa_tyr_osc() {
  }

  //----------

  ~sa_tyr_osc() {
  }

//------------------------------
public:
//------------------------------

  void setFrequency(T hz) {
    //MIP_Print("hz %.2f\n",hz);
    MIP_Assert( hz >= 1.0 );
    MIP_Assert( hz < (MSampleRate * 0.5) );
    MPhaseAdd = hz * MInvSampleRate;
  }

  void setSampleRate(T ARate) {
    MSampleRate = ARate;
    MIP_Assert(ARate > 0);
    MInvSampleRate = 1.0 / ARate;
  }

  void setType(uint32_t AType)          { MType = AType; }

  void setPh(T ph)                      { MPhase = ph; }
  void setPhAdd(T phadd)                { MPhaseAdd = phadd; }
  void setShape(T shape)                { MShape = shape; }
  void setWidth(T width)                { MWidth = width; }

  void setWaveModType(uint32_t AType)   { MWaveModType = AType; }
  void setWaveModAmount(T AAmount)      { MWaveModAmount = AAmount; }
  void setPhaseModType(uint32_t AType)  { MPhaseModType = AType; }
  void setPhaseModAmount(T AAmount)     { MPhaseModAmount = AAmount; }

  T getPh()     { return MPhase; }
  T getPhAdd()  { return MPhaseAdd; }

  //T getPh2()    { return MPhase2; }
  //T getPhAdd2() { return MPhaseAdd2; }

//------------------------------
private:
//------------------------------

T curve(T in, T wm) {
  T sign_in = MIP_Sign(in);
  T abs_in = abs(in);
  return MIP_Curve(abs_in,wm) * sign_in;
}

//------------------------------
public:
//------------------------------

  void restart(bool AResetPhase=true, bool ARandomPhase=true) {
    tri_z1 = 0.0;
    //diff_z1 = 0.0;
    if (AResetPhase) {
      if (ARandomPhase) MPhase = MIP_Random();
      else MPhase = 0.0;
      //MPhase2 = 0.0;
    }
    //MWrapped  = false;
  }

  //----------

  T process(T mod) {

    T t       = MPhase;
    T dt      = MPhaseAdd;

    T dt_mod  = 0.0;

    //--------------------
    // phase mod
    //--------------------

    switch (MPhaseModType) {

      case SA_TYR_PM_TYPE_OFF: {
        break;
      }

      case SA_TYR_PM_TYPE_CURVE: {
        t = curve(t,MPhaseModAmount);
        break;
      }

      case SA_TYR_PM_TYPE_SYNC: {
        t *= 1.0 + (MPhaseModAmount * 12);
        t = MIP_Fract(t);
        break;
      }

      case SA_TYR_PM_TYPE_SYNC_CLAMP: {
        t *= 1.0 + (MPhaseModAmount * 12);
        t = MIP_Clamp(t,0,1); // do we need this?
        t = MIP_Fract(t);
        break;
      }

      case SA_TYR_PM_TYPE_PHASE_MOD: {
        T f = mod * (MPhaseModAmount * 12);
        t += (t * f);
        t = MIP_Fract(t);
        break;
      }

      case SA_TYR_PM_TYPE_FREQ_MOD: {
        T f = mod * (MPhaseModAmount * 48);
        dt_mod = (dt * f);
        break;
      }

      case SA_TYR_PM_TYPE_DIFF: {
        break;
      }

      case SA_TYR_PM_TYPE_INT: {
        break;
      }

    }

    //--------------------
    // type
    //--------------------

    T out = 0.0;

    switch (MType) {

      case SA_TYR_OSC_TYPE_MORPH: {
        T shape = MShape * 3.0;
        T _squ = 0.0;
        T _tri = 0.0;
        T _sin = 0.0;

        if (shape < 1.0) {
          _squ = shape;
          _tri = 0.0;
          _sin = 0.0;
        }
        else if (shape < 2.0) {
          _squ = 1.0;
          _tri = shape - 1.0;
          _sin = 0.0;
        }
        else {
          _squ = 1.0;
          _tri = 1.0;
          _sin = shape - 2.0;
        }
        // saw 1
        T t1    = t;// + 0.5;
        t1      = MIP_Fract(t1);
        T saw1  = ((2.0f * t1) - 1.0);
        saw1   -= MIP_PolyBlep(t1,dt);
        // saw 2
        T t2    = t1 + MWidth;
        t2      = MIP_Fract(t2);
        T saw2  = ((2.0 * t2) - 1.0);
        saw2   -= MIP_PolyBlep(t2,dt);
        // squ
        T squ1  = saw1 - (saw2 * _squ);
        // tri
        tri_z1  = (dt * squ1) + ((1.0 - dt) * tri_z1);
        //T tri1  = squ1 * (1.0 - _tri) + (z1 * 4.0) * _tri;
        T tri1  = tri_z1 * 4.0;
        // sin

        T sin1  = sin(t * MIP_PI2);
//        T sin1  = MIP_SinF4( t * MIP_PI2);

        out     = MIP_Interpolate_Linear(_tri,squ1,tri1);
        out     = MIP_Interpolate_Linear(_sin,out,sin1);

        break;
      }

      //case SA_TYR_OSC_TYPE_DSF: {
      //  T t1  = t;
      //  T dt1 = dt;
      //  T t2  = MPhase2;
      //  T dt2 = MPhaseAdd2;
      //  float a = MShape * 0.9;
      //  float N = MDsf.findMaxPartials(dt1,dt2);
      //  out = MDsf.process(t1,t2,a,N);
      //  break;
      //}

      //case SA_TYR_OSC_TYPE_WAVETABLE: {
      //  break;
      //}

    }

    //--------------------
    // wave mod
    //--------------------

    switch (MWaveModType) {

      case SA_TYR_WM_TYPE_OFF: {
        break;
      }

      case SA_TYR_WM_TYPE_CURVE: {
        out = curve(out,MWaveModAmount);
        break;
      }

      case SA_TYR_WM_TYPE_FOLD: {
        T s = MIP_Sign(out);
        T a = abs(out);
        a *= 1.0 + (MWaveModAmount * 2.0);
        if (a > 1.0) {
          a = 1.0 + (1.0 - a);
        }
        out = a * s;
        break;
      }

      case SA_TYR_WM_TYPE_AMPL_MOD: {
        T am = out * abs(mod);
        out = MIP_Interpolate_Linear(MWaveModAmount,out,am);
        break;
      }

      case SA_TYR_WM_TYPE_RING_MOD: {
        T rm = out * mod;
        out = MIP_Interpolate_Linear(MWaveModAmount,out,rm);
        break;
      }

      case SA_TYR_WM_TYPE_RAMP_DOWN: {
        T f = 1.0 - (MPhase * MWaveModAmount);
        out *= f;
        break;
      }

      //

      case SA_TYR_WM_TYPE_DIFF: {
        break;
      }

      case SA_TYR_WM_TYPE_INT: {
        break;
      }

      //

    }

    //--------------------

    MPhase += (MPhaseAdd + dt_mod);

    //    if ((MPhase < 0.0) || (MPhase >= 1.0)) {
    //      //MWrapped = true;
    //      MPhase = MIP_Fract(MPhase);
    //    }

    while (MPhase < 0.0)  MPhase += 1.0;
    while (MPhase >= 1.0) MPhase -= 1.0;

    //MPhase2 += MPhaseAdd2 + additional_phase_add;
    //MPhase2 = MIP_Fract(MPhase2);

    return out;

  }

};

//----------------------------------------------------------------------
#endif
