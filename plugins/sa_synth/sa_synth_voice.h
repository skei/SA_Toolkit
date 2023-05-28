#ifndef sa_synth_voice_included
#define sa_synth_voice_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_voice_context.h"

#include "audio/old/filters/sat_svf_filter.h"
#include "audio/old/waveforms/sat_polyblep2_waveform.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class sa_synth_voice {

//------------------------------
private:
//------------------------------

  uint32_t            MIndex      = 0;
  SAT_VoiceContext*   MContext    = nullptr;
  //SAT_ParameterArray* MParameters = nullptr;
  
  SAT_SvfFilter     MFilter       = {};

  float             srate         = 0.0;
  float             ph            = 0.0;
  float             phadd         = 0.0;
  
  double p_tun    = 0.0;
  double p_flt    = 0.0;
  double p_fr     = 1.0;
  double p_bw     = 0.5;
  
  double m_tun    = 0.0;
  double m_flt    = 0.0;
  double m_fr     = 0.0;
  double m_bw     = 0.0;

  double x_tuning = 0.0;
  
  double MKey = 0;
  double MVelocity = 0;
  
//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    //SAT_Print("AIndex %i\n",AIndex);
    MIndex = AIndex;
    MContext = AContext;
    srate = AContext->sample_rate;
    
      // process_context not set yet.. (only set/valid in process()
    //MParameters = MContext->process_context->parameters;
    //SAT_Print("MParameters %p\n",MParameters);
    //SAT_Assert(MParameters);
    
  }

  //----------

  sat_sample_t getEnvLevel() {
    return 0.0;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    //SAT_ParameterArray* parameters = MContext->process_context->parameters;
    float* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      
      for (uint32_t i=0; i<ALength; i++) {

        ph = SAT_Fract(ph);
        
        //float v = sin(ph * SAT_PI2);
        
        //float v = MOsc1.process(ph,phadd);
        
        float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1
        v -= SAT_PolyBlepSawWaveform(ph,phadd);
        
        uint32_t  flt = SAT_ClampI( p_flt + m_flt, 0,4);
        double    fr  = SAT_Clamp(  p_fr  + m_fr,  0,1);
        double    bw  = SAT_Clamp(  p_bw  + m_bw,  0,1);
        
        //SAT_Print("flt %i fr %.2f bw %.2f\n",flt,fr,bw);
        //SAT_Print("flt %i p_fr %.2f m_fr %.2f fr %.2f\n",flt,p_fr,m_fr,fr);
        
        MFilter.setMode(flt);
        MFilter.setFreq(fr);
        MFilter.setBW(bw);
        
        v = MFilter.process(v);
        
        *buffer++ = v * 0.1;
        
        //SAT_Parameter* tuning = parameters->getItem(1);
        //double tun = tuning->getValue() + tuning->getModulation();
        double tun = p_tun + m_tun;//tuning->getValue() + tuning->getModulation();
        tun = SAT_Clamp(tun,-2,2);
        tun +=  x_tuning;
        
        float hz = SAT_NoteToHz(MKey + tun);
        phadd = 1.0 / SAT_HzToSamples(hz,srate);
        
        ph += phadd;
        
      }
    } // playing
    else {
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
    //float hz = SAT_NoteToHz(AKey);
    //phadd = 1.0 / SAT_HzToSamples(hz,srate);
    MKey = AKey;
    MVelocity = AVelocity;
    m_flt     = 0;
    m_fr      = 0;
    m_bw      = 0;
    x_tuning  = 0.0;
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
    switch (AExpression) {
      //case CLAP_NOTE_EXPRESSION_VOLUME:     SAT_Print("NOTE VOLUME EXPRESSION %.3f\n",value);     break; // with 0 < x <= 4, plain = 20 * log(x)
      //case CLAP_NOTE_EXPRESSION_PAN:        SAT_Print("NOTE PAN EXPRESSION %.3f\n",value);        break; // pan, 0 left, 0.5 center, 1 right
      //case CLAP_NOTE_EXPRESSION_TUNING:     SAT_Print("NOTE TUNING EXPRESSION %.3f\n",value);     break; // relative tuning in semitone, from -120 to +120
      case CLAP_NOTE_EXPRESSION_TUNING:
        x_tuning = AValue;
        //SAT_Print("CLAP_NOTE_EXPRESSION_TUNING AValue %f\n",AExpression,AValue);
        break;
      //case CLAP_NOTE_EXPRESSION_VIBRATO:    SAT_Print("NOTE VIBRATO EXPRESSION %.3f\n",value);    break; // 0..1
      //case CLAP_NOTE_EXPRESSION_EXPRESSION: SAT_Print("NOTE EXPRESSION EXPRESSION %.3f\n",value); break; // 0..1
      //case CLAP_NOTE_EXPRESSION_BRIGHTNESS: SAT_Print("NOTE BRIGHTNESS EXPRESSION %.3f\n",value); break; // 0..1
      //case CLAP_NOTE_EXPRESSION_PRESSURE:   SAT_Print("NOTE PRESSURE EXPRESSION %.3f\n",value);   break; // 0..1
    }
    //SAT_Print("\n");
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
    switch (AIndex) {
      case 1: p_tun = AValue; break;
      case 6: p_flt = AValue; break;
      case 7: p_fr  = AValue; break;
      case 8: p_bw  = AValue; break;
    }
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
    switch (AIndex) {
      case 1: m_tun = AValue; break;
      case 6: m_flt = AValue; break;
      case 7: m_fr  = AValue; break;
      case 8: m_bw  = AValue; break;
    }
  }

};

//----------------------------------------------------------------------
#endif

