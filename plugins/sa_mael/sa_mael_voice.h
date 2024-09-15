#ifndef sa_mael_voice_included
#define sa_mael_voice_included
//----------------------------------------------------------------------

#include "base/util/sat_interpolation.h"
#include "audio/filters/sat_svf_filter.h"
#include "audio/modulation/sat_exp_envelope.h"
#include "audio/process/sat_delay.h"
#include "audio/synthesis/sat_morph_oscillator.h"

#define SA_MAEL_MAX_DELAY (1024 * 1024)
typedef SAT_InterpolatedDelay<sat_sample_t,SA_MAEL_MAX_DELAY> sa_mael_delay;

//----------------------------------------------------------------------

class sa_mael_voice {
 

//------------------------------
private:
//------------------------------

  SAT_VoiceContext*                   MContext      = nullptr;
  uint32_t                            MIndex        = 0;
  sat_sample_t                        MSampleRate   = 0.0;

  sat_param_t                         MKey          = 0.0;    // aka note
  sat_param_t                         MVelocity     = 1.0;
  sat_sample_t                        MPhase1       = 0.0;
  sat_sample_t                        MPhaseAdd1    = 0.0;
  sat_sample_t                        MPhase2       = 0.0;
  sat_sample_t                        MPhaseAdd2    = 0.0;
  SAT_MorphOscillator<sat_sample_t>   MOscillator1  = {};
  SAT_MorphOscillator<sat_sample_t>   MOscillator2  = {};
  SAT_SVFFilter<sat_sample_t>         MFilter       = {};
  SAT_ExpEnvelope<sat_sample_t>       MEnvelope     = {};

  sa_mael_delay   MDelay1 = {};
  sa_mael_delay   MDelay2 = {};
  
  // param

  sat_param_t                       p_gain        = 1.0;

  sat_param_t                       p_osc1_squ    = 0.0;
  sat_param_t                       p_osc1_tri    = 0.0;
  sat_param_t                       p_osc1_sin    = 0.0;
  sat_param_t                       p_osc1_width  = 0.0;
  sat_param_t                       p_osc1_oct    = 0.0;
  sat_param_t                       p_osc1_semi   = 0.0;
  sat_param_t                       p_osc1_cent   = 0.0;

  sat_param_t                       p_osc2_squ    = 0.0;
  sat_param_t                       p_osc2_tri    = 0.0;
  sat_param_t                       p_osc2_sin    = 0.0;
  sat_param_t                       p_osc2_width  = 0.0;
  sat_param_t                       p_osc2_oct    = 0.0;
  sat_param_t                       p_osc2_semi   = 0.0;
  sat_param_t                       p_osc2_cent   = 0.0;

  uint32_t                          p_mix_type    = 0;
  sat_param_t                       p_mix_amount  = 0.0;

  uint32_t                          p_flt1_type   = 0;
  sat_param_t                       p_flt1_freq   = 0.0;
  sat_param_t                       p_flt1_q      = 0.0;
  sat_param_t                       p_flt1_gain   = 1.0;

  sat_param_t                       p_env1_att    = 0.0;
  sat_param_t                       p_env1_dec    = 0.0;
  sat_param_t                       p_env1_sus    = 0.0;
  sat_param_t                       p_env1_rel    = 0.0;

  // mod

  sat_param_t                       m_osc1_squ    = 0.0;
  sat_param_t                       m_osc1_tri    = 0.0;
  sat_param_t                       m_osc1_sin    = 0.0;
  sat_param_t                       m_osc1_width  = 0.0;
  sat_param_t                       m_osc1_oct    = 0.0;
  sat_param_t                       m_osc1_semi   = 0.0;
  sat_param_t                       m_osc1_cent   = 0.0;

  sat_param_t                       m_osc2_squ    = 0.0;
  sat_param_t                       m_osc2_tri    = 0.0;
  sat_param_t                       m_osc2_sin    = 0.0;
  sat_param_t                       m_osc2_width  = 0.0;
  sat_param_t                       m_osc2_oct    = 0.0;
  sat_param_t                       m_osc2_semi   = 0.0;
  sat_param_t                       m_osc2_cent   = 0.0;

  sat_param_t                       m_mix_amount  = 0.0;

  sat_param_t                       m_flt1_freq   = 0.0;
  sat_param_t                       m_flt1_q      = 0.0;

  // note expr
  
  sat_param_t                       e_press       = 0.0;
  sat_param_t                       e_tuning      = 0.0;
  sat_param_t                       e_brightness  = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
    //MPlugin = AContext->process_context->plugin;
    MOscillator1.reset();
    MOscillator1.setSampleRate(MSampleRate);
    MOscillator2.reset();
    MOscillator2.setSampleRate(MSampleRate);
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
  
  uint32_t noteOn(uint32_t AIndex, double AValue) {

    //SAT_Plugin*     plugin      = MContext->process_context->plugin;
    SAT_ParameterArray* params = MContext->process_context->parameters;

    MKey      = AIndex;
    MVelocity = AValue;
    MPhase1    = 0.0;
    MPhase2    = 0.0;

//    p_tuning  = par_tuning->getValue();
//    m_tuning  = 0.0;

    //SAT_Parameter*  par_tuning  = plugin->getParameter(1);
    SAT_Parameter*  par_tuning  = params->getItem(1);

    p_osc1_squ    = params->getItem(SA_MAEL_PARAM_OSC1_SQU)->getValue();
    p_osc1_tri    = params->getItem(SA_MAEL_PARAM_OSC1_TRI)->getValue();
    p_osc1_sin    = params->getItem(SA_MAEL_PARAM_OSC1_SIN)->getValue();
    p_osc1_width  = params->getItem(SA_MAEL_PARAM_OSC1_WIDTH)->getValue();
    p_osc1_oct    = params->getItem(SA_MAEL_PARAM_OSC1_OCT)->getValue();
    p_osc1_semi   = params->getItem(SA_MAEL_PARAM_OSC1_SEMI)->getValue();
    p_osc1_cent   = params->getItem(SA_MAEL_PARAM_OSC1_CENT)->getValue();

    p_osc2_squ    = params->getItem(SA_MAEL_PARAM_OSC2_SQU)->getValue();
    p_osc2_tri    = params->getItem(SA_MAEL_PARAM_OSC2_TRI)->getValue();
    p_osc2_sin    = params->getItem(SA_MAEL_PARAM_OSC2_SIN)->getValue();
    p_osc2_width  = params->getItem(SA_MAEL_PARAM_OSC2_WIDTH)->getValue();
    p_osc2_oct    = params->getItem(SA_MAEL_PARAM_OSC2_OCT)->getValue();
    p_osc2_semi   = params->getItem(SA_MAEL_PARAM_OSC2_SEMI)->getValue();
    p_osc2_cent   = params->getItem(SA_MAEL_PARAM_OSC2_CENT)->getValue();

    p_mix_type    = params->getItem(SA_MAEL_PARAM_MIX_TYPE)->getValue();
    p_mix_amount  = params->getItem(SA_MAEL_PARAM_MIX_AMOUNT)->getValue();

    p_flt1_type   = params->getItem(SA_MAEL_PARAM_FLT1_TYPE)->getValue();
    p_flt1_freq   = params->getItem(SA_MAEL_PARAM_FLT1_FREQ)->getValue();
    p_flt1_freq   = (p_flt1_freq * p_flt1_freq * p_flt1_freq);

    p_flt1_q      = params->getItem(SA_MAEL_PARAM_FLT1_Q)->getValue();
    p_flt1_q      = (p_flt1_q * p_flt1_q * p_flt1_q);

    p_flt1_gain   = params->getItem(SA_MAEL_PARAM_FLT1_GAIN)->getValue();

    p_env1_att = params->getItem(SA_MAEL_PARAM_ENV1_ATT)->getValue();
    p_env1_dec = params->getItem(SA_MAEL_PARAM_ENV1_DEC)->getValue();
    p_env1_sus = params->getItem(SA_MAEL_PARAM_ENV1_SUS)->getValue();
    p_env1_rel = params->getItem(SA_MAEL_PARAM_ENV1_REL)->getValue();

    m_osc1_oct    = 0.0;
    m_osc1_semi   = 0.0;
    m_osc1_cent   = 0.0;

    m_osc2_oct    = 0.0;
    m_osc2_semi   = 0.0;
    m_osc2_cent   = 0.0;

    e_tuning      = 0.0;

    MOscillator1.reset();
    MOscillator2.reset();

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
    sat_param_t a3 = (AValue * AValue * AValue);
    sat_param_t a5 = (AValue * 5.0);
    int32_t ivalue = (int32_t)AValue;
    switch (AIndex) {
      case SA_MAEL_PARAM_GAIN:        p_gain        = AValue;   break;
      case SA_MAEL_PARAM_OSC1_SQU:    p_osc1_squ    = AValue;   break;
      case SA_MAEL_PARAM_OSC1_TRI:    p_osc1_tri    = AValue;   break;
      case SA_MAEL_PARAM_OSC1_SIN:    p_osc1_sin    = AValue;   break;
      case SA_MAEL_PARAM_OSC1_WIDTH:  p_osc1_width  = AValue;   break;
      case SA_MAEL_PARAM_OSC1_OCT:    p_osc1_oct    = ivalue;   break;
      case SA_MAEL_PARAM_OSC1_SEMI:   p_osc1_semi   = ivalue;   break;
      case SA_MAEL_PARAM_OSC1_CENT:   p_osc1_cent   = AValue;   break;
      case SA_MAEL_PARAM_OSC2_SQU:    p_osc2_squ    = AValue;   break;
      case SA_MAEL_PARAM_OSC2_TRI:    p_osc2_tri    = AValue;   break;
      case SA_MAEL_PARAM_OSC2_SIN:    p_osc2_sin    = AValue;   break;
      case SA_MAEL_PARAM_OSC2_WIDTH:  p_osc2_width  = AValue;   break;
      case SA_MAEL_PARAM_OSC2_OCT:    p_osc2_oct    = ivalue;   break;
      case SA_MAEL_PARAM_OSC2_SEMI:   p_osc2_semi   = ivalue;   break;
      case SA_MAEL_PARAM_OSC2_CENT:   p_osc2_cent   = AValue;   break;
      case SA_MAEL_PARAM_MIX_TYPE:    p_mix_type    = ivalue;   break;
      case SA_MAEL_PARAM_MIX_AMOUNT:  p_mix_amount  = AValue;   break;
      case SA_MAEL_PARAM_FLT1_TYPE:   p_flt1_type   = ivalue;   break;
      case SA_MAEL_PARAM_FLT1_FREQ:   p_flt1_freq   = a3;       break;
      case SA_MAEL_PARAM_FLT1_Q:      p_flt1_q      = a3;       break;
      case SA_MAEL_PARAM_FLT1_GAIN:   p_flt1_gain   = AValue;   break;
      case SA_MAEL_PARAM_ENV1_ATT:    p_env1_att    = a5;       MEnvelope.setAttack(a5);  break;
      case SA_MAEL_PARAM_ENV1_DEC:    p_env1_dec    = a5;       MEnvelope.setDecay(a5);   break;
      case SA_MAEL_PARAM_ENV1_SUS:    p_env1_sus    = a5;       MEnvelope.setSustain(a5); break;
      case SA_MAEL_PARAM_ENV1_REL:    p_env1_rel    = a5;       MEnvelope.setRelease(a5); break;
    }
  }

  //----------

  void modulation(uint32_t AIndex, sat_param_t AValue) {
    switch (AIndex) {
    //case SA_MAEL_PARAM_TUNING:      m_tuning      = AValue; break;
      case SA_MAEL_PARAM_OSC1_SQU:    m_osc1_squ    = AValue; break;
      case SA_MAEL_PARAM_OSC1_TRI:    m_osc1_tri    = AValue; break;
      case SA_MAEL_PARAM_OSC1_SIN:    m_osc1_sin    = AValue; break;
      case SA_MAEL_PARAM_OSC1_WIDTH:  m_osc1_width  = AValue; break;
      case SA_MAEL_PARAM_OSC1_OCT:    m_osc1_oct    = AValue; break;
      case SA_MAEL_PARAM_OSC1_SEMI:   m_osc1_semi   = AValue; break;
      case SA_MAEL_PARAM_OSC1_CENT:   m_osc1_cent   = AValue; break;
      case SA_MAEL_PARAM_OSC2_SQU:    m_osc2_squ    = AValue; break;
      case SA_MAEL_PARAM_OSC2_TRI:    m_osc2_tri    = AValue; break;
      case SA_MAEL_PARAM_OSC2_SIN:    m_osc2_sin    = AValue; break;
      case SA_MAEL_PARAM_OSC2_WIDTH:  m_osc2_width  = AValue; break;
      case SA_MAEL_PARAM_OSC2_OCT:    m_osc2_oct    = AValue; break;
      case SA_MAEL_PARAM_OSC2_SEMI:   m_osc2_semi   = AValue; break;
      case SA_MAEL_PARAM_OSC2_CENT:   m_osc2_cent   = AValue; break;
      case SA_MAEL_PARAM_MIX_AMOUNT:  m_mix_amount  = AValue; break;
      case SA_MAEL_PARAM_FLT1_FREQ:   m_flt1_freq   = AValue; break;
      case SA_MAEL_PARAM_FLT1_Q:      m_flt1_q      = AValue; break;
//    case SA_MAEL_PARAM_FLT1_GAIN:   m_flt1_gain   = AValue; break;
    }
  }

//----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    sat_sample_t* buffer = MContext->voice_buffer;

    // caluclate offset of our voice
    // (voices calculate into their own separate parts of a bigger buffer)

    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;

    // process only playing and released voices
    // other states doesn't make sound..
    // (so there's a memset(0) at the else part of this if)

    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {

        // move these outside of for loop?
        
        sat_param_t osc1_squ    = SAT_Clamp( (p_osc1_squ   + m_osc1_squ),    0,  1  );
        sat_param_t osc1_tri    = SAT_Clamp( (p_osc1_tri   + m_osc1_tri),    0,  1  );
        sat_param_t osc1_sin    = SAT_Clamp( (p_osc1_sin   + m_osc1_sin),    0,  1  );
        sat_param_t osc1_width  = SAT_Clamp( (p_osc1_width + m_osc1_width), -1,  1  );
        sat_param_t osc1_oct    = SAT_Clamp( (p_osc1_oct   + m_osc1_oct),   -4,  4  );
        sat_param_t osc1_semi   = SAT_Clamp( (p_osc1_semi  + m_osc1_semi),  -12, 12 );
        sat_param_t osc1_cent   = SAT_Clamp( (p_osc1_cent  + m_osc1_cent),  -1,  1  );

        sat_param_t osc2_squ    = SAT_Clamp( (p_osc2_squ   + m_osc1_squ),    0,  1  );
        sat_param_t osc2_tri    = SAT_Clamp( (p_osc2_tri   + m_osc1_tri),    0,  1  );
        sat_param_t osc2_sin    = SAT_Clamp( (p_osc2_sin   + m_osc1_sin),    0,  1  );
        sat_param_t osc2_width  = SAT_Clamp( (p_osc2_width + m_osc1_width), -1,  1  );
        sat_param_t osc2_oct    = SAT_Clamp( (p_osc2_oct   + m_osc2_oct),   -4,  4  );
        sat_param_t osc2_semi   = SAT_Clamp( (p_osc2_semi  + m_osc2_semi),  -12, 12 );
        sat_param_t osc2_cent   = SAT_Clamp( (p_osc2_cent  + m_osc2_cent),  -1,  1  );

        sat_param_t mix_amount  = SAT_Clamp( (p_mix_amount + m_mix_amount),  0,  1  );
        sat_param_t flt1_freq   = SAT_Clamp( (p_flt1_freq  + m_flt1_freq),   0,  1  );
        sat_param_t flt1_q      = SAT_Clamp( (p_flt1_q     + m_flt1_q),      0,  1  );
//      sat_param_t flt1_gain   = SAT_Clamp( (p_flt1_gain  + m_flt1_gain),   0,  1  );

        //MOscillator1.setMorph(osc1_shape);
        //MOscillator2.setMorph(osc2_shape);

        MOscillator1.setSawSqu(osc1_squ);
        MOscillator1.setSquTri(osc1_tri);
        MOscillator1.setTriSin(osc1_sin);
        MOscillator1.setPulseWidth(osc1_width);

        MOscillator2.setSawSqu(osc2_squ);
        MOscillator2.setSquTri(osc2_tri);
        MOscillator2.setTriSin(osc2_sin);
        MOscillator2.setPulseWidth(osc2_width);

        sat_sample_t out = 0.0;

        // osc1

        sat_sample_t osc1 = MOscillator1.process();

        // osc2

        sat_sample_t fm2 = 0.0;
        sat_sample_t pm2 = 0.0;

        switch (p_mix_type) {
          case 3: // fm
            fm2 = osc1 * mix_amount;
            break;
          case 4: // pm
            pm2 = osc1 * mix_amount;
            break;
        };

        sat_sample_t osc2 = MOscillator2.process_mod(pm2);

        // mix

        switch (p_mix_type) {
          case 0: // mix
            out = SAT_Interpolate_Linear(mix_amount,osc1,osc2);
            break;
          case 1: // rm
            out = SAT_Interpolate_Linear(mix_amount,osc1,(osc1*osc2));
            break;
          case 2: // am
            out = SAT_Interpolate_Linear(mix_amount,osc1,(abs(osc1)*osc2));
            break;
          case 3: // fm
            out = osc2;
            //fm2 = osc1 * p_mix_amount;
            break;
          case 4: // pm
            out = osc2;
            //pm2 = osc1 * p_mix_amount;
            break;
        }

        // flt
                
        flt1_freq *= (MSampleRate * 0.5);
        flt1_q = SAT_Clamp( (flt1_q * 40), 0.025, 40.0 );
        MFilter.updateCoefficients(flt1_freq,flt1_q,p_flt1_type,MSampleRate);
        MFilter.setGain(p_flt1_gain);
        out = MFilter.tick(out);

        // env

        sat_sample_t env = MEnvelope.process();
        out *= env;
        *buffer++ = out;

        // tuning
        // shouldn't these be _before_ osc1/2 calculation?
        // (or maybe we want 'ground zero' tuning for the first sample?)

        // osc1_oct = SAT_Trunc(osc1_oct);
        // osc1_semi = SAT_Trunc(osc1_semi);
        sat_param_t tuning1 = (osc1_oct * 12.0) + osc1_semi + osc1_cent;
        tuning1 += e_tuning;
        sat_param_t hz1 = SAT_NoteToHz(MKey + tuning1);
        MOscillator1.setFrequency(hz1);
        //MPhaseAdd1 = 1.0 / SAT_HzToSamples(hz1,MSampleRate);
        //MPhase1 += MPhaseAdd1;

        // osc2_oct = SAT_Trunc(osc2_oct);
        // osc2_semi = SAT_Trunc(osc2_semi);
        sat_param_t tuning2 = (osc2_oct * 12.0) + osc2_semi + osc2_cent;
        tuning2 += e_tuning;

        tuning2 += fm2;
        
        sat_param_t hz2 = SAT_NoteToHz(MKey + tuning2);
        MOscillator2.setFrequency(hz2);
        //MPhaseAdd2 = 1.0 / SAT_HzToSamples(hz2,MSampleRate);
        //MPhase2 += MPhaseAdd2;

      } // for samples
    } // playing/released
    else {
      memset(buffer,0,ALength * sizeof(sat_sample_t));
    }
    if (MEnvelope.getStage() == SAT_ENVELOPE_FINISHED) return SAT_VOICE_FINISHED;
    else return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

};

//----------------------------------------------------------------------
#endif
