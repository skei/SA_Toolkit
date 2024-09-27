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

#define SA_SYNTH_MAX_DELAY (1024 * 4)

struct sa_synth_delay_loop_fx {
  sat_sample_t process(sat_sample_t x) { return x; }
};

struct sa_synth_delay_clip_fx {
  sat_sample_t process(sat_sample_t x) { return SAT_Clamp(x,-1.0,1.0); }
};

//----------

typedef SAT_InterpolatedDelay<sat_sample_t,SA_SYNTH_MAX_DELAY,sa_synth_delay_loop_fx> sa_synth_delay;

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
  sa_synth_delay  MDelay        = {};

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

  void setFrequency(double hz) {
    SAT_Assert(hz >= 2.0);
    MDelayLength = (MSampleRate / hz);
    //SAT_Assert(MDelayLength < SA_SYNTH_MAX_DELAY);
    if (MDelayLength > SA_SYNTH_MAX_DELAY) MDelayLength = SA_SYNTH_MAX_DELAY;
  }

  //----------

  // bool res.hasWrapped()
  // T res.getPhase()
   

  void setImpulse(double AImpulse) {
    par_impulse = AImpulse;
  }

  void setShape(double AShape) {
    par_shape = AShape;
  }
  
  void setFeedback(double AFeedback) {
    par_feedback = pow(AFeedback,(1.0/16.0));
  }
  
  void setDamping(double ADamping) {
    par_damping = ADamping;
  }

  //----------

  void noteOn(uint32_t AIndex, double AValue) {
    double hz = SAT_NoteToHz(AIndex);
    setFrequency(hz);
    MDelay.clear();
    MDelay.start();
  }

  //----------

  void noteOff(uint32_t AIndex, double AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  sat_sample_t process(sat_sample_t AInput/*, double AOffset=0.0*/) {
    sat_sample_t in = AInput;
    if (MDelay.hasWrapped()) {
      sat_sample_t v = MDelay.process(0.0,MDelayLength,par_feedback/*,AOffset*/);
      return  v;
      // MDelay.process(0.0,MDelayLength,par_feedback);
      // sat_sample_t v1 = MDelay.tap(MDelayLength,par_impulse);
      // sat_sample_t v2 = MDelay.tap(MDelayLength,1.0 - par_impulse);
      // return v1 - v2;
    }
    else {
      MDelay.process(AInput,MDelayLength,par_feedback/*,AOffset*/);
      return AInput;
    }
  }

};

//----------------------------------------------------------------------
#endif
