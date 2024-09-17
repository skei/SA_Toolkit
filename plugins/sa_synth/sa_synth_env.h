#ifndef sa_synth_env_included
#define sa_synth_env_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "audio/modulation/sat_exp_envelope.h"


typedef SAT_ExpEnvelope<sat_sample_t> sa_synth_exp_env;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_synth_env {

//------------------------------
private:
//------------------------------

  sa_synth_exp_env  MEnvelope   = {};
  double            MSampleRate = 0.0;

  sat_param_t       par_a       = 0.0;
  sat_param_t       par_d       = 0.0;
  sat_param_t       par_s       = 0.0;
  sat_param_t       par_r       = 0.0;

//------------------------------
public:
//------------------------------

  int32_t       getStage() { return MEnvelope.getStage();  }
  sat_sample_t  getValue() { return MEnvelope.getValue();  }

  //----------

  void setSampleRate(double ASampleRate) {
    MSampleRate = ASampleRate;
    MEnvelope.setSampleRate(ASampleRate);
  }

  //----------

  void setA(sat_param_t AA) {
    par_a = AA * 5.0;
    MEnvelope.setAttack(par_a);
  }

  void setD(sat_param_t AD) {
    par_d = AD * 5.0;
    MEnvelope.setRelease(par_d);
  }

  void setS(sat_param_t AS) {
    par_s = AS * AS;
    MEnvelope.setSustain(par_s);
  }

  void setR(sat_param_t AR) {
    par_r = AR * 5.0;
    MEnvelope.setRelease(par_r);
  }

  void setADSR(sat_param_t AA, sat_param_t AD, sat_param_t AS, sat_param_t AR) {
    par_a = AA * 5.0;
    par_d = AD * 5.0;
    par_s = AS * AS;
    par_r = AR * 5.0;
    MEnvelope.setADSR(par_a,par_d,par_s,par_r);
  }

  //----------

  void reset(void) {
    MEnvelope.reset();
  }

//------------------------------
public:
//------------------------------

  void noteOn(uint32_t AIndex=0, double AValue=0) {
    reset();
    MEnvelope.noteOn();
  }

  //----------

  void noteOff(uint32_t AIndex=0, double AValue=0) {
    MEnvelope.noteOff();
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  sat_sample_t process() {
    sat_sample_t env = MEnvelope.process();
    return env;
  }

};

//----------------------------------------------------------------------
#endif
