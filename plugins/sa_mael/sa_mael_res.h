#ifndef sa_mael_res_included
#define sa_mael_res_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/process/sat_delay.h"
// #include "audio/filters/sat_svf_filter.h"
// #include "base/util/sat_interpolation.h"
//#include "sa_mael_flt.h"

//----------------------------------------------------------------------
//
// delay
//
//----------------------------------------------------------------------

// double fastatan(double x) {
//   return (x / (1.0 + 0.28 * (x * x)));
// }

struct sa_mael_delay_loop_fx {

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

#define SA_MAEL_MAX_DELAY (1024 * 4)

typedef SAT_InterpolatedDelay<sat_sample_t,SA_MAEL_MAX_DELAY,sa_mael_delay_loop_fx> sa_mael_delay;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_mael_res {

//------------------------------
private:
//------------------------------

  double                  MSampleRate     = 0.0;
  double                  MDelayLength    = 0.0;
  sa_mael_delay_loop_fx  MImpulseFilter  = {};
  sa_mael_delay          MDelay          = {};
  uint32_t                MType           = 0;

  uint32_t  MIndex = 0;
  double    MValue = 0.0;

  sat_param_t             par_impulse     = 0.0;
  sat_param_t             par_repeat      = 0.0;
  sat_param_t             par_feedback    = 0.0;
  sat_param_t             par_damping     = 0.0;
  sat_param_t             par_gain        = 0.0;

  sat_sample_t            imp_current     = 0.0;
  sat_sample_t            imp_target      = 0.0;
//sat_sample_t            imp_factor      = 0.0;

  bool                    is_repeating    = false;
  int32_t                 repeat_count    = 0;

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
    //SAT_Assert(MDelayLength < SA_MAEL_MAX_DELAY);
    if (MDelayLength > SA_MAEL_MAX_DELAY) MDelayLength = SA_MAEL_MAX_DELAY;
  }

  //----------

  // bool res.hasWrapped()
  // T res.getPhase()
   
  void setImpulse(double AImpulse) {
    par_impulse = (AImpulse * AImpulse * AImpulse * AImpulse * AImpulse);
    par_impulse *= 0.1;
//    SAT_PRINT("AImpulse %f par_impulse %f\n",AImpulse,par_impulse); // looks ok..
  }

  void setRepeat(double ARepeat) {
    par_repeat = (ARepeat * ARepeat * ARepeat);
    par_repeat *= MSampleRate;
//    SAT_PRINT("AImpulse %f par_impulse %f\n",AImpulse,par_impulse); // looks ok..
  }

  void setFeedback(double AFeedback) {
    par_feedback = pow(AFeedback,(1.0/16.0));
  }
  
  void setDamping(double ADamping) {
    par_damping = ADamping;
  }

  void setGain(double AGain) {
    par_gain = (AGain * AGain * AGain * AGain * AGain);
  }

  void setType(uint32_t AType) {
    MType = AType;
  }

  //----------

  void noteOn(uint32_t AIndex, double AValue) {
    //MIndex = AIndex;
    //MValue = AValue;
    double hz = SAT_NoteToHz(AIndex);
    setFrequency(hz);
    imp_current = 1.0;
    imp_target = 0.0;
    //imp_factor = par_impulse;
    //SAT_PRINT("imp_current %.3f imp_factor %.3f\n",imp_current,imp_factor); // imp_factor = 0
    MDelay.clear();
    MDelay.start();
    // if ((par_repeat > 0.0) && (par_repeat < MSampleRate)) is_repeating = true;
    // else is_repeating = false;
    // repeat_count = par_repeat;
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

    imp_current += ((imp_target - imp_current) * par_impulse);
    in *= imp_current;

    MDelay.getLoopFX()->factor = par_damping;
    MDelay.getLoopFX()->gain = par_gain;

    // else {
    //   is_repeating = false;
    // }

    //repeat_count = par_repeat;
    // if (is_repeating) {
    //   repeat_count -= 1;
    //   if (repeat_count <= 0) {
    //     repeat_count = par_repeat;
    //     //MDelay.reset();
    //     imp_current = 1.0;
    //     imp_target = 0.0;
    //   }
    // }

    if (MDelay.hasWrapped()) {
      switch (MType) {
        case 0: 
          in = 0.0;
          break;
      }
    }

    sat_sample_t result = MDelay.process(in,MDelayLength,par_feedback/*,AOffset*/);

    if ((par_repeat > 0.0) && (par_repeat < MSampleRate)) {
      is_repeating = true;
      if (is_repeating) {
        repeat_count -= 1;
        if (repeat_count <= 0) {
          repeat_count = par_repeat;
          //MDelay.reset();
          imp_current = 1.0;
          imp_target = 0.0;
        }
      }
    }

    return result;

  }

};

//----------------------------------------------------------------------
#endif
