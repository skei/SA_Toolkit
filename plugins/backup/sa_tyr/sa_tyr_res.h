#ifndef sa_tyr_res_included
#define sa_tyr_res_included
//----------------------------------------------------------------------

//#include "base/utils/sat_interpolation.h"
#include "audio/backup/filters/sat_rc_filter.h"
#include "audio/backup/filters/sat_svf_filter.h"
#include "audio/backup/processing/sat_delay.h"

#define DELAY_LENGTH (4096 * 4)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class T>
class sa_tyr_res {

//------------------------------
public:
//------------------------------

  struct sa_tyr_DelayFx {

    SAT_RcFilter<T> filter  = {};
    T               rough   = 0.0;
    T               shape   = 0.0;

    T process(T x) {
      T r = SAT_Random();
      //SAT_Print("r %.2f rough %.2f\n",r,rough);
      if (r >= rough) x = -x;

//      T s = (shape*shape*shape*shape);
//      x *= (1.0 + s);
//      x = atan(x);
//      x = SAT_Clamp(x,-1,1);

      return filter.process(x);
    }

  };

//------------------------------
public:
//------------------------------

  typedef SAT_InterpolatedDelay<DELAY_LENGTH,sa_tyr_DelayFx> sa_tyr_Delay;

//------------------------------
private:
//------------------------------

  T             MSampleRate   = 0.0;
  sa_tyr_Delay  MDelay        = {};
  //SAT_SvfFilter MInputShaper  = {};

  T             MHz           = 0.0;
  T             MShape        = 0.0;
  T             MFeedback     = 0.0;
  T             MDamp         = 0.0;
  T             MRough        = 0.0;
  T             MImpulse      = 0.0;

  uint32_t      MMode         = 0;
  uint32_t      MSpeed        = 0;
  uint32_t      MSpeedCounter = 0;

//------------------------------
public:
//------------------------------

  sa_tyr_res() {
  }

  //----------

  ~sa_tyr_res() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  void setSampleRate(T ARate) {
    MSampleRate = ARate;
  }

  void setHz(T AHz) {
    MHz = AHz;
  }

  void setShape(T s)        { MShape = s; }
  void setFeedback(T f)     { MFeedback = f; }
  void setDamp(T d)         { MDamp = d; }
  void setRough(T r)        { MRough = r; }
  void setImpulse(T i)      { MImpulse = i; }

  void setMode(uint32_t m)  { MMode = m; }
  void setSpeed(uint32_t s) { MSpeed = s; }

  //void setHz(T hz) {
  //}

  //----------

  void restart() {
    MDelay.clear();
    MDelay.start();
    MSpeedCounter = 0;
  }

  //----------

  T process(T in) {

    T out = in;
    T delay = (MSampleRate / MHz);
    //delay *= 0.5;

    // feedback

    T fb = MFeedback;
    fb = SAT_Curve(fb,0.98);

    // damp

    T damp = MDamp;
    damp = damp * damp;
    damp = 1.0 - damp;

    MDelay.getFeedbackFX()->filter.setWeight(damp);

    // rough

    float ro = 1.0 - MRough;

    if (ro < 0.5) {
      ro = ro * 2.0; // 0..0,5 -> 0..1
      ro = SAT_Curve(ro,0.02);
    }
    else {
      ro = (ro - 0.5) * 2.0; // 0.5..1 -> 0..1
      ro = SAT_Curve(ro,0.98);
      ro += 1.0;
    }
    // 0..2 -> 0..1
    ro *= 0.5;

    MDelay.getFeedbackFX()->rough = ro;

    //

    T _in = in;

    //_in *= (MShape * 2.0);

//    switch (MMode) {
//      case SA_TYR_RES_TYPE_PLUCK: {
//        if (MDelay.hasWrapped()) {
//          _in = 0.0;
//        }
//        break;
//      }
//      case SA_TYR_RES_TYPE_REP: {
//        break;
//      }
//    }

    if ((MMode == SA_TYR_RES_TYPE_PLUCK) && MDelay.hasWrapped()) {
      _in = 0.0;
    }

    MDelay.getFeedbackFX()->shape = MShape;
    out = MDelay.process(_in,fb,delay);

//    if (MDelay.hasWrapped() && (MMode == SA_TYR_RES_TYPE_REP)) {
//      out += (MImpulse * _in);
//    }
//    else {
//      out += _in;
//    }

    if ((MMode == SA_TYR_RES_TYPE_REP) && MDelay.hasWrapped()) {
      out += (MImpulse * _in);
    }
    else {
      out += _in;
    }

    //return (MImpulse * _in) + out;
    return out;

  }

};

#undef DELAY_LENGTH


//----------------------------------------------------------------------
#endif

