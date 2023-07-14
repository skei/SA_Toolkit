#ifndef test_synth_voice_included
#define test_synth_voice_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_voice_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class test_synth_voice {

//------------------------------
private:
//------------------------------

  uint32_t          MIndex    = 0;
  SAT_VoiceContext* MContext  = nullptr;

  float             srate     = 0.0;
  float             ph        = 0.0;
  float             phadd     = 0.0;
  
//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    //SAT_Print("AIndex %i\n",AIndex);
    MIndex = AIndex;
    MContext = AContext;
    srate = AContext->sample_rate;
  }

  //----------

  sat_sample_t getEnvLevel() {
    return 0.0;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
      
    float* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        ph = SAT_Fract(ph);
        //float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1
        float v = sin(ph * SAT_PI2);
        *buffer++ = v * 0.1;
        ph += phadd;
      }
    } // playing
    else {
      // if the voice are off, waiting, or finished
      memset(buffer,0,ALength * sizeof(float));
    }
    return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    ph = 0.0;
    float hz = SAT_NoteToHz(AKey);
    phadd = 1.0 / SAT_HzToSamples(hz,srate);
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    return SAT_VOICE_FINISHED;
  }

  //----------

  void noteChoke(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
  }

  //----------

  void noteExpression(uint32_t AExpression, double AValue) {
    //SAT_Print("\n");
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

};

//----------------------------------------------------------------------
#endif

