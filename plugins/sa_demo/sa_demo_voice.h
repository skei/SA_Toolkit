#ifndef sa_demo_voice_included
#define sa_demo_voice_included
//----------------------------------------------------------------------

//#define VOICE_SCALE   0.1

#include "base/util/sat_interpolation.h"
#include "audio/sat_audio_utils.h"
#include "audio/filters/sat_svf_filter.h"
#include "audio/synthesis/sat_morph_oscillator.h"
#include "audio/modulation/sat_exp_envelope.h"
#include "plugin/processor/sat_voice_context.h"

//----------------------------------------------------------------------

class sa_demo_voice {

//------------------------------
private:
//------------------------------

  SAT_VoiceContext*                 MContext      = nullptr;
  uint32_t                          MIndex        = 0;
  sat_sample_t                      MSampleRate   = 0.0;

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
  
  uint32_t noteOn(uint32_t AIndex, double AValue) {
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, sat_param_t AValue) {
    return SAT_VOICE_FINISHED;
  }

  //----------

  void noteChoke(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  void parameter(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  void modulation(uint32_t AIndex, sat_param_t AValue) {
  }

//----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    sat_sample_t* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        *buffer++ = 1.0;
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


};

//----------------------------------------------------------------------

#undef VOICE_SCALE

//----------------------------------------------------------------------
#endif
