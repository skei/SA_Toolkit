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

typedef SAT_InterpolatedDelay<sat_sample_t,SA_SYNTH_MAX_DELAY,sa_synth_delay_loop_fx/*,sa_synth_delay_clip_fx*/> sa_synth_delay;

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
  sa_synth_delay  MDelay1       = {};
  sa_synth_delay  MDelay2       = {};
//double          MFeedBack     = 0.0;

  // parameters

  sat_param_t   par_impulse     = 0.0;
  sat_param_t   par_shape       = 0.0;
  sat_param_t   par_feedback    = 0.0;
  sat_param_t   par_damping     = 0.0;

  sat_sample_t  d1_z1           = 0.0;
  sat_sample_t  d2_z1           = 0.0;

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    MSampleRate = ASampleRate;
    MDelay1.clear();
    MDelay2.clear();
  }

  //----------

  void setFrequency(double hz) {
    SAT_Assert(hz >= 2.0);
    MDelayLength = (MSampleRate / hz) * 0.5;
    SAT_Assert(MDelayLength < SA_SYNTH_MAX_DELAY);
  }

  //----------

  void noteOn(uint32_t AIndex, double AValue) {
    double hz = SAT_NoteToHz(AIndex);
    setFrequency(hz);
    MDelay1.clear();
    MDelay2.clear();
    MDelay1.start();
    MDelay2.start();
  }

  //----------

  void noteOff(uint32_t AIndex, double AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  sat_sample_t process(sat_sample_t AInput, double AFeedBack, double AOffset=0.0) {
    sat_sample_t d1 = MDelay1.process(AInput + d1_z1,MDelayLength,AFeedBack,AOffset);
    sat_sample_t d2 = MDelay2.process(AInput + d2_z1,MDelayLength,AFeedBack,(1.0 - AOffset));
    sat_sample_t out = (d1 + d2);
    d1_z1 = d1;
    d2_z1 = d2;
    return out;
  }

};

//----------------------------------------------------------------------
#endif
