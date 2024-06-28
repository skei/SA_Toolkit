#ifndef sa_tyr_voice_included
#define sa_tyr_voice_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "audio/filters/sat_svf_filter.h"
#include "audio/modulation/sat_exp_envelope.h"
#include "audio/process/sat_delay.h"

//----------

template<typename T>
struct sa_tyr_delay_fx {
  T process(T x) { return x; }
};

template<typename T>
struct sa_tyr_clip_fx {
  T process(T x) { return SAT_Clamp(x,-1,1); }
};

#define SA_TYR_DELAY_SIZE (1024 * 16)

typedef SAT_SVFFilter<sat_sample_t> sa_tyr_filter;
typedef SAT_InterpolatedDelay<sat_sample_t,SA_TYR_DELAY_SIZE,sa_tyr_delay_fx<sat_sample_t>,sa_tyr_clip_fx<sat_sample_t>> sa_tyr_delay;
typedef SAT_ExpEnvelope<sat_sample_t> sa_tyr_envelope;

//----------------------------------------------------------------------

class sa_tyr_voice {

//------------------------------
private:
//------------------------------

  uint32_t                        MIndex      = 0;
  SAT_VoiceContext*               MContext    = nullptr;
  float                           MSampleRate = 0.0;

  sa_tyr_delay                    MDelay      = {};
  sa_tyr_envelope                 MEnvelope   = {};
  sa_tyr_filter                   MFilter     = {};

  // note on
  double                          MNoteOnKey  = 0.0;
  double                          MNoteOnVel  = 1.0;

  // synth
  float                           MPhase      = 0.0;
  float                           MPhaseAdd   = 0.0;
  
  // parameters
  double                          MPGain      = 1.0;
  double                          MPTuning    = 0.0;
  double                          MPFilter    = 1.0;

  // modulation
  double                          MMGain      = 0.0;
  double                          MMTuning    = 0.0;
  double                          MMFilter    = 0.0;

  // note expressions
  double                          MEPress     = 0.0;
  double                          METuning    = 0.0;
  double                          MEBright    = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
    MEnvelope.setSampleRate(MSampleRate);
    MEnvelope.setADSR( 0.0, 0.0, 1.0, 2.5 );
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return MEnvelope.getValue();
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {

    float* input = MContext->process_context->process->audio_inputs[0].data32[0];
    input += AOffset;

    float* output = MContext->voice_buffer;
    output += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    output += AOffset;

    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {

        double tu = MPTuning + MMTuning;
        tu = SAT_Clamp(tu,-1,1);
        tu +=  METuning;
        float hz = SAT_NoteToHz(MNoteOnKey + tu);
        float delay = SAT_HzToSamples(hz,MSampleRate);
        MPhaseAdd = 1.0 / delay;
        MPhase = SAT_Fract(MPhase);

        sat_sample_t env = MEnvelope.process();

        float in = *input++;
        float out = 0.0;
        //float feedback = 0.0;

        if (MDelay.hasWrapped()) {
          out = MDelay.process( 0.0, delay, 1.0 );
        }

        else {
          //out = in;
          out = MDelay.process( in, delay, 1.0 );
        }

        *output++ = out * env;
         MPhase += MPhaseAdd;

      } // for length
    } // playing

    else {
      memset(output,0,ALength * sizeof(float));
    }

    if (MEnvelope.getStage() == SAT_ENVELOPE_FINISHED) return SAT_VOICE_FINISHED;
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

    // reset voice

    MNoteOnKey  = AIndex;
    MNoteOnVel  = AValue;
    MPhase      = 0.0;
    MPTuning    = par_tuning->getValue();
    MMTuning    = 0.0;
    METuning    = 0.0;

    //MDelay.reset();
    MDelay.start();

    MEnvelope.reset();
    MEnvelope.setADSR(0,0,1,2.5);
    MEnvelope.noteOn();    

    MFilter.resetState();

    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, double AValue) {
    MEnvelope.noteOff();
    //MNoteOffVel = AValue;
    return SAT_VOICE_RELEASED;
  }

  //----------

  void noteChoke(uint32_t AIndex, double AValue) {
    // todo
  }

  //----------

  void noteExpression(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MEPress  = AValue; break;
      case CLAP_NOTE_EXPRESSION_TUNING:     METuning = AValue; break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MEBright = AValue; break;
    }
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    //sat_param_t a5 = (AValue * 5.0);
    switch (AIndex) {
      case 0: MPGain    = AValue; break;
      case 1: MPTuning  = AValue; break;
      case 2: MPFilter  = AValue; break;
      //case SA_MAEL_PARAM_ENV1_ATT:    MEnvelope.setAttack(a5);  break;
      //case SA_MAEL_PARAM_ENV1_DEC:    MEnvelope.setDecay(a5);   break;
      //case SA_MAEL_PARAM_ENV1_SUS:    MEnvelope.setSustain(a5); break;
      //case SA_MAEL_PARAM_ENV1_REL:    MEnvelope.setRelease(a5); break;    
    }
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case 0: MMGain    = AValue; break;
      case 1: MMTuning  = AValue; break;
      case 2: MMFilter  = AValue; break;
    }
  }

};

//----------

#undef SA_TYR_DELAY_SIZE



//----------------------------------------------------------------------
#endif
