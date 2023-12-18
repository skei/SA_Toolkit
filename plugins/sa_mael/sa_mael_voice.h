#ifndef sa_mael_voice_included
#define sa_mael_voice_included
//----------------------------------------------------------------------

#define VOICE_SCALE   0.1

#include "audio/synthesis/sat_dsf_waveform.h"
#include "audio/synthesis/sat_morph_oscillator.h"
#include "audio/filters/sat_svf_filter.h"
//#include "audio/modulation/sat_curved_envelope.h"
#include "audio/modulation/sat_exp_envelope.h"

//----------------------------------------------------------------------

class sa_mael_Voice {

//------------------------------
private:
//------------------------------

  //SAT_Plugin*                       MPlugin     = nullptr;

  SAT_VoiceContext*                 MContext      = nullptr;
  uint32_t                          MIndex        = 0;
  sat_sample_t                      MSampleRate   = 0.0;

  // note on
  sat_param_t                       MKey          = 0.0;    // aka note
  sat_param_t                       MVelocity     = 1.0;

  // param
  sat_param_t                       p_gain        = 1.0;
  sat_param_t                       p_tuning      = 0.0;    // aka pitchbend

  uint32_t                          p_osc1_type   = 0;
  sat_param_t                       p_osc1_shape  = 0.0;
  sat_param_t                       p_osc1_width  = 0.0;

  uint32_t                          p_flt1_type   = 0;
  sat_param_t                       p_flt1_freq   = 0.0;
  sat_param_t                       p_flt1_q      = 0.0;

  sat_param_t                       p_env1_att    = 0.0;
  sat_param_t                       p_env1_dec    = 0.0;
  sat_param_t                       p_env1_sus    = 0.0;
  sat_param_t                       p_env1_rel    = 0.0;

  // mod
  sat_param_t                       m_gain        = 0.0;
  sat_param_t                       m_tuning      = 0.0;

  // note expr
  sat_param_t                       e_press       = 0.0;
  sat_param_t                       e_tuning      = 0.0;
  sat_param_t                       e_brightness  = 0.0;

  //

  sat_sample_t                      MPhase        = 0.0;
  sat_sample_t                      MPhaseAdd     = 0.0;

  SAT_MorphOscillator<sat_sample_t> MOscillator   = {};
  SAT_SVFFilter<sat_sample_t>       MFilter       = {};
  //SAT_CurvedEnvelope<sat_sample_t>  MEnvelope     = {};
  SAT_ExpEnvelope<sat_sample_t>     MEnvelope     = {};

  //

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
    //MPlugin = AContext->process_context->plugin;
    MOscillator.reset();
    MOscillator.setSampleRate(MSampleRate);
    //MOscillator.setMorph(p_osc1_shape);
    //MOscillator.setPulseWidth(p_osc1_width);
    MEnvelope.setSampleRate(MSampleRate);
    MEnvelope.setADSR( 1.0, 0.0, 1.0, 1.0 );
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return MEnvelope.getValue();
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    sat_sample_t* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {

        MPhase = SAT_Fract(MPhase);

        MOscillator.setPhase(MPhase);
        MOscillator.setPhaseAdd(MPhaseAdd);
        MOscillator.setMorph(p_osc1_shape);
        MOscillator.setPulseWidth(p_osc1_width);
        sat_sample_t v = MOscillator.process();

        
        //MFilter.resetState();
        MFilter.updateCoefficients(p_flt1_freq * (MSampleRate * 0.5),p_flt1_q,p_flt1_type,MSampleRate);
        v = MFilter.tick(v);

        sat_sample_t env = MEnvelope.process();
        *buffer++ = v * env * VOICE_SCALE;

        sat_param_t tuning = p_tuning + m_tuning;
        tuning = SAT_Clamp(tuning,-1,1);
        tuning +=  e_tuning;
        sat_sample_t hz = SAT_NoteToHz(MKey + tuning);
        MPhaseAdd = 1.0 / SAT_HzToSamples(hz,MSampleRate);
        MPhase += MPhaseAdd;

      }
    }
    else {
      memset(buffer,0,ALength * sizeof(sat_sample_t));
    }
    if (MEnvelope.getStage() == SAT_ENVELOPE_FINISHED) {
      return SAT_VOICE_FINISHED;
    }
    else return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    SAT_Plugin*     plugin      = MContext->process_context->plugin;
    SAT_Parameter*  par_tuning  = plugin->getParameter(1);
    MKey      = AIndex;
    MVelocity = AValue;
    MPhase    = 0.0;

    p_tuning  = par_tuning->getValue();
    m_tuning  = 0.0;
    e_tuning  = 0.0;

    p_osc1_type  = plugin->getParameterValue(SA_MAEL_PARAM_OSC1_TYPE);
    p_osc1_shape = plugin->getParameterValue(SA_MAEL_PARAM_OSC1_SHAPE);
    p_osc1_width = plugin->getParameterValue(SA_MAEL_PARAM_OSC1_WIDTH);
    MOscillator.reset();

    p_flt1_type = plugin->getParameterValue(SA_MAEL_PARAM_FLT1_TYPE);
    p_flt1_freq = plugin->getParameterValue(SA_MAEL_PARAM_FLT1_FREQ);
    p_flt1_freq = (p_flt1_freq * p_flt1_freq * p_flt1_freq);
    p_flt1_q   = plugin->getParameterValue(SA_MAEL_PARAM_FLT1_Q);
    p_flt1_q   = (p_flt1_q * p_flt1_q * p_flt1_q);
    p_flt1_q   = SAT_Clamp( (p_flt1_q * 40), 0.025, 40.0 );

    //MFilter.updateCoefficients(p_flt1_freq,p_flt1_bw,p_flt1_type,MSampleRate);

    p_env1_att = plugin->getParameterValue(SA_MAEL_PARAM_ENV1_ATT);
    p_env1_dec = plugin->getParameterValue(SA_MAEL_PARAM_ENV1_DEC);
    p_env1_sus = plugin->getParameterValue(SA_MAEL_PARAM_ENV1_SUS);
    p_env1_rel = plugin->getParameterValue(SA_MAEL_PARAM_ENV1_REL);
    MEnvelope.reset();
    MEnvelope.setADSR(p_env1_att*5,p_env1_dec*5,p_env1_sus,p_env1_rel*5);
    MEnvelope.noteOn();

    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, sat_param_t AValue) {
    //MOscillator.noteOff(AValue);
    MEnvelope.noteOff();
    //return SAT_VOICE_FINISHED;
    return SAT_VOICE_RELEASED;
  }

  //----------

  void noteChoke(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
    switch (AIndex) {
      case CLAP_NOTE_EXPRESSION_PRESSURE:   e_press       = AValue; break;
      case CLAP_NOTE_EXPRESSION_TUNING:     e_tuning      = AValue; break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: e_brightness  = AValue; break;
    }
  }

  //----------

  void parameter(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    sat_param_t a3 = (AValue*AValue*AValue);
    switch (AIndex) {
      case SA_MAEL_PARAM_GAIN:        p_gain        = AValue;   break;
      case SA_MAEL_PARAM_OSC1_TYPE:   p_osc1_type   = AValue;   break;
      case SA_MAEL_PARAM_OSC1_SHAPE:  p_osc1_shape  = AValue;   break;
      case SA_MAEL_PARAM_OSC1_WIDTH:  p_osc1_width  = AValue;   break;
      case SA_MAEL_PARAM_FLT1_TYPE:   p_flt1_type   = AValue;   SAT_Print("%i\n",p_flt1_type); break;
      case SA_MAEL_PARAM_FLT1_FREQ:   p_flt1_freq   = a3;       break;
      case SA_MAEL_PARAM_FLT1_Q:      p_flt1_q      = SAT_Clamp( (a3*40), 0.025, 40.0 );        break;
      case SA_MAEL_PARAM_ENV1_ATT:    p_env1_att    = AValue*5; MEnvelope.setAttack(AValue*5);  break;
      case SA_MAEL_PARAM_ENV1_DEC:    p_env1_dec    = AValue*5; MEnvelope.setDecay(AValue*5);   break;
      case SA_MAEL_PARAM_ENV1_SUS:    p_env1_sus    = AValue;   MEnvelope.setSustain(AValue);   break;
      case SA_MAEL_PARAM_ENV1_REL:    p_env1_rel    = AValue*5; MEnvelope.setRelease(AValue*5); break;
    }
  }

  //----------

  void modulation(uint32_t AIndex, sat_param_t AValue) {
    switch (AIndex) {
      case SA_MAEL_PARAM_GAIN: m_gain    = AValue; break;
      //case 1: MMTuning  = AValue; break;
      //case 2: MMFilter  = AValue; break;
    }
  }

};

//----------------------------------------------------------------------

#undef VOICE_SCALE

//----------------------------------------------------------------------
#endif
