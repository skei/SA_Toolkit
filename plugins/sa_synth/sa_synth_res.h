#ifndef sa_synth_res_included
#define sa_synth_res_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/process/sat_delay.h"
// #include "audio/filters/sat_svf_filter.h"
// #include "base/util/sat_interpolation.h"
//#include "sa_synth_flt.h"

//----------------------------------------------------------------------
//
// delay
//
//----------------------------------------------------------------------

// double fastatan(double x) {
//   return (x / (1.0 + 0.28 * (x * x)));
// }

struct sa_synth_delay_loop_fx {

  sat_sample_t factor = 0.0;
  sat_sample_t current = 0.0;
  sat_sample_t gain = 0.0;

  //----------

  sat_sample_t process(sat_sample_t x) {

    // gain
    x *= 1.0 + gain;

    // clip
    x = SAT_Clamp(x,-1,1);
    // x = (x * 1.5) - ((x * x * x) * 0.5);
    //x = (x / (1.0 + 0.28 * (x * x)));

    // filter
    current += ((x - current) * factor);

    return current;

  }

};

//----------

#define SA_SYNTH_MAX_DELAY (1024 * 4)

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

  double                  MSampleRate       = 0.0;
  double                  MDelayLength      = 0.0;
  sa_synth_delay_loop_fx  MImpulseFilter    = {};
  sa_synth_delay          MDelay            = {};
  uint32_t                MType             = 0;

  sat_param_t             par_impulse_freq  = 0.0;
  sat_param_t             par_feedback      = 0.0;
  sat_param_t             par_damping_freq  = 0.0;
  sat_param_t             par_gain          = 0.0;

  sat_sample_t            imp_factor        = 0.0;
  sat_sample_t            imp_current       = 0.0;



//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    MSampleRate = ASampleRate;
    MDelay.setSampleRate(ASampleRate);
    MDelay.clear();
  }

  //----------

  void setFrequency(double hz) {
    //SAT_Assert(hz >= 2.0);
    MDelayLength = (MSampleRate / hz);
    //SAT_Assert(MDelayLength < SA_SYNTH_MAX_DELAY);
    if (MDelayLength > SA_SYNTH_MAX_DELAY) MDelayLength = SA_SYNTH_MAX_DELAY;
  }

  //----------

  // bool res.hasWrapped()
  // T res.getPhase()
   
  void setImpulse(double AFreq) {
    par_impulse_freq = AFreq;
  }

  void setFeedback(double AFeedback) {
    par_feedback = pow(AFeedback,(1.0/16.0));
  }
  
  void setDamping(double AFreq) {
    par_damping_freq = AFreq;
  }

  void setGain(double AGain) {
    par_gain = (AGain * AGain * AGain * AGain * AGain);
  }

  void setType(uint32_t AType) {
    MType = AType;
  }

  //----------

  void noteOn(uint32_t AIndex, double AValue) {
    double hz = SAT_NoteToHz(AIndex);
    setFrequency(hz);

    // MImpulseFilter.setType(par_impulse_type);
    // MImpulseFilter.setFreq(par_impulse_freq);
    // MImpulseFilter.setQ(par_impulse_q);
    // MImpulseFilter.setGain(par_impulse_gain);

    // MDelay.getLoopFX()->filter.setType(par_damping_type);
    // MDelay.getLoopFX()->filter.setFreq(par_damping_freq);
    // MDelay.getLoopFX()->filter.setQ(par_damping_q);
    // MDelay.getLoopFX()->filter.setGain(par_damping_gain);

    // imp_factor = par_impulse_freq;
    // MDelay.getLoopFX()->factor = par_damping_freq;

    imp_current = 0.0;
    imp_factor = par_damping_freq;

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

    sat_sample_t factor = par_impulse_freq * par_impulse_freq * par_impulse_freq;
    imp_current += ((AInput - imp_current) * factor);
    sat_sample_t in = imp_current;

    MImpulseFilter.factor = par_impulse_freq;
    MImpulseFilter.gain = 1.0;//par_gain;
    in = MImpulseFilter.process(in);

    MDelay.getLoopFX()->factor = par_damping_freq;
    MDelay.getLoopFX()->gain = par_gain;//(par_gain * 0.5) + 1.0;

    if (MDelay.hasWrapped()) {
      if (MType == 0) in = 0.0;
    }

    return MDelay.process(in,MDelayLength,par_feedback/*,AOffset*/);


  }

};

//----------------------------------------------------------------------
#endif
