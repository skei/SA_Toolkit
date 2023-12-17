#ifndef sa_mael_voice_included
#define sa_mael_voice_included
//----------------------------------------------------------------------

#define VOICE_SCALE   0.1

#include "audio/synthesis/sat_dsf_waveform.h"
#include "audio/synthesis/sat_morph_oscillator.h"
#include "audio/filters/sat_svf_filter.h"
#include "audio/modulation/sat_curved_envelope.h"

//----------------------------------------------------------------------

class sa_mael_Voice {

//------------------------------
private:
//------------------------------

  SAT_VoiceContext*                 MContext    = nullptr;
  uint32_t                          MIndex      = 0;
  sat_sample_t                      MSampleRate = 0.0;

  // note on
  sat_param_t                       MKey        = 0.0;    // aka note
  sat_param_t                       MVelocity   = 1.0;

  // param
  sat_param_t                       MPGain      = 1.0;
  sat_param_t                       MPTuning    = 0.0;    // aka pitchbend
  sat_param_t                       MPFilter    = 1.0;

  // mod
  sat_param_t                       MMGain      = 0.0;
  sat_param_t                       MMTuning    = 0.0;
  sat_param_t                       MMFilter    = 0.0;

  // note expr
  sat_param_t                       MEPress     = 0.0;
  sat_param_t                       METuning    = 0.0;
  sat_param_t                       MEBright    = 0.0;

  //

  sat_sample_t                      MPhase      = 0.0;
  sat_sample_t                      MPhaseAdd   = 0.0;

  //SAT_DsfWaveform<sat_sample_t>     MDsf        = {};
  SAT_MorphOscillator<sat_sample_t> MOscillator = {};
  SAT_SVFFilter<sat_sample_t>       MFilter     = {};
  SAT_CurvedEnvelope<sat_sample_t>  MEnvelope   = {};

  //sat_param_t p_shape = 0.0;
  //sat_param_t p_width = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return 1.0;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {

    sat_sample_t* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;

    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {

        MPhase = SAT_Fract(MPhase);

        sat_sample_t v = (MPhase * 2.0) - 1.0;  // 0..1 -> -1..1

        // sat_sample_t phase2 = SAT_Trunc(MPhase + 0.25);
        // sat_sample_t a      = p_shape * 10;
        // uint32_t     N      = p_width * 16;
        // sat_sample_t v = MDsf.process(MPhase,phase2,a,N);

        sat_sample_t env = 1.0;
        *buffer++ = v * env * VOICE_SCALE;

        sat_param_t tuning = MPTuning + MMTuning;
        tuning = SAT_Clamp(tuning,-1,1);
        tuning +=  METuning;
        sat_sample_t hz = SAT_NoteToHz(MKey + tuning);

        MPhaseAdd = 1.0 / SAT_HzToSamples(hz,MSampleRate);
        MPhase += MPhaseAdd;

      }
    }
    else {
      memset(buffer,0,ALength * sizeof(sat_sample_t));
    }
    return AState;
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
    MPTuning  = par_tuning->getValue();
    MMTuning  = 0.0;
    METuning  = 0.0;
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, sat_param_t AValue) {
    return SAT_VOICE_FINISHED;
    //return SAT_VOICE_RELEASED;
  }

  //----------

  void noteChoke(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
    switch (AIndex) {
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MEPress  = AValue; break;
      case CLAP_NOTE_EXPRESSION_TUNING:     METuning = AValue; break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MEBright = AValue; break;
    }
  }

  //----------

  void parameter(uint32_t AIndex, sat_param_t AValue) {
    SAT_Print("%i = %f\n",AIndex,AValue);
    switch (AIndex) {
      case SA_MAEL_PARAM_GAIN:        MPGain  = AValue; break;
      //case SA_MAEL_PARAM_OSC1_SHAPE:  p_shape = AValue; break;
      //case SA_MAEL_PARAM_OSC1_WIDTH:  p_width = AValue; break;
    }
  }

  //----------

  void modulation(uint32_t AIndex, sat_param_t AValue) {
    switch (AIndex) {
      case 0: MMGain    = AValue; break;
      case 1: MMTuning  = AValue; break;
      case 2: MMFilter  = AValue; break;
    }
  }

};

//----------------------------------------------------------------------

#undef VOICE_SCALE

//----------------------------------------------------------------------
#endif
