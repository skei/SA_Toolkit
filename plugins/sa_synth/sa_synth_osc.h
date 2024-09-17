#ifndef sa_synth_osc_included
#define sa_synth_osc_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "audio/synthesis/sat_morph_oscillator.h"

typedef SAT_MorphOscillator<sat_sample_t> sa_synth_morphosc;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_synth_osc {

//------------------------------
private:
//------------------------------

  sa_synth_morphosc MOscillator = {};
  double            MSampleRate = 0.0;
  double            strike      = 0.0;
  double            lift        = 0.0;
  double            slide       = 0.0;
  double            press       = 0.0;
  double            bright      = 0.0;

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    SAT_Assert(ASampleRate > 0.0);
    MSampleRate = ASampleRate;
    MOscillator.setSampleRate(ASampleRate);
  }

  void setFrequency(double AFrequency)  { MOscillator.setFrequency(AFrequency); }
  void setPhase(double APhase)          { MOscillator.setPhase(APhase); }
  void setPhaseAdd(double APhaseAdd)    { MOscillator.setPhaseAdd(APhaseAdd); }
  void setPulseWidth(double pw)         { MOscillator.setPulseWidth(pw); }
  void setSawSqu(double sawsqu)         { MOscillator.setSawSqu(sawsqu); }
  void setSquTri(double squtri)         { MOscillator.setSquTri(squtri); }
  void setTriSin(double trisin)         { MOscillator.setTriSin(trisin); }
  void reset(void)                      { MOscillator.reset(); }

//------------------------------
public:
//------------------------------

  void noteOn(uint32_t AIndex, double AValue) {
    //SAT_Assert(MSampleRate > 0.0);
    //double hz = SAT_NoteToHz(AIndex);
    //MOscillator.setFrequency(hz);
    strike = AValue;
  }

  //----------

  void noteOff(uint32_t AIndex, double AValue) {
    lift = AValue;
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
    switch (AIndex) {
      case CLAP_NOTE_EXPRESSION_TUNING:
        slide = AValue;
        break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:
        press = AValue;
        break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS:
        bright = AValue;
        break;
    }
  }

  //----------

  sat_sample_t process(sat_sample_t AInput) {
    return MOscillator.process();
  }

  //----------

  sat_sample_t process_mod(sat_sample_t AInput, sat_sample_t AMod, sat_sample_t AOfs=0.0) {
    return MOscillator.process_mod(AMod,AOfs);
  }

};

//----------------------------------------------------------------------
#endif
