#ifndef sa_synth_res_included
#define sa_synth_res_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/process/sat_delay.h"
// #include "audio/filters/sat_svf_filter.h"

// #include "base/util/sat_interpolation.h"

//----------------------------------------------------------------------
//
// delay
//
//----------------------------------------------------------------------

//#define SA_SYNTH_MAX_DELAY (1024 * 1024)
#define SA_SYNTH_MAX_DELAY (1024 * 4)

struct sa_synth_delay_loop_fx {
  sat_sample_t process(sat_sample_t x) { return x; }
};

struct sa_synth_delay_clip_fx {
  sat_sample_t process(sat_sample_t x) { return x; }
};

//----------

typedef SAT_InterpolatedDelay<sat_sample_t,SA_SYNTH_MAX_DELAY,sa_synth_delay_loop_fx,sa_synth_delay_clip_fx> sa_synth_delay;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_synth_res {

//------------------------------
private:
//------------------------------

  double          MSampleRate   = 0.0;
  double          MDelayLength  = 0.0;
  double          MFeedBack     = 0.0;
  sa_synth_delay  MDelay        = {};

  // parameters

  sat_param_t   par_impulse     = 0.0;
  sat_param_t   par_shape       = 0.0;
  sat_param_t   par_feedback    = 0.0;
  sat_param_t   par_damping     = 0.0;

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    MSampleRate = ASampleRate;
    MDelay.clear();
  }

  //----------

  void noteOn(uint32_t AIndex, double AValue) {
    double hz = SAT_NoteToHz(AIndex);
    MDelayLength = MSampleRate / hz;
    MFeedBack = AValue; // test
  }

  //----------

  void noteOff(uint32_t AIndex, double AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  sat_sample_t process(sat_sample_t AInput) {
    sat_sample_t out = 0.0;
    out = MDelay.process(AInput,MDelayLength,MFeedBack);
    return out;
  }

};

//----------------------------------------------------------------------
#endif
