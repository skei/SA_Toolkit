#ifndef sa_mael_voice_included
#define sa_mael_voice_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "base/util/sat_interpolation.h"
#include "plugin/sat_plugin_base.h"

#include "sa_mael_osc.h"
#include "sa_mael_res.h"
#include "sa_mael_flt.h"
#include "sa_mael_env.h"

// clap has 7 expressions defined..
#define SA_MAEL_NUM_NOTE_EXPRESSIONS 16

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_mael_voice {

//------------------------------
private:
//------------------------------

  SAT_VoiceContext*   MContext      = nullptr;
  uint32_t            MIndex        = 0;
  sat_sample_t        MSampleRate   = 0.0;
  uint32_t            MOverSample   = 1;
  uint32_t            MBufferSize   = 1;
  uint32_t            MNoteKey      = 0;
  double              MNoteVelocity = 0.0;

  sa_mael_osc         MOsc1         = {};
  sa_mael_osc         MOsc2         = {};
  sa_mael_res         MRes1         = {};
  sa_mael_res         MRes2         = {};
  sa_mael_flt         MFlt1         = {};
  sa_mael_env         MEnv1         = {};

//sat_param_t         MVoiceParam[SA_MAEL_PARAM_COUNT] = {};
  sat_param_t         MVoiceMod[SA_MAEL_PARAM_COUNT] = {};
  sat_param_t         MVoiceExpr[SA_MAEL_NUM_NOTE_EXPRESSIONS] = {};

  sat_sample_t        osc1_out      = 0.0;
  sat_sample_t        osc2_out      = 0.0;
  sat_sample_t        res1_out      = 0.0;
  sat_sample_t        res2_out      = 0.0;

  sat_sample_t        o1_in_current = 0.0;
  sat_sample_t        o1_in_factor  = 0.0;
  sat_sample_t        o2_in_current = 0.0;
  sat_sample_t        o2_in_factor  = 0.0;
  sat_sample_t        r1_in_current = 0.0;
  sat_sample_t        r1_in_factor  = 0.0;
  sat_sample_t        r2_in_current = 0.0;
  sat_sample_t        r2_in_factor  = 0.0;


//------------------------------
private:
//------------------------------

  sat_param_t   o1_squ        = 0.0;
  sat_param_t   o1_tri        = 0.0;
  sat_param_t   o1_sin        = 0.0;
  sat_param_t   o1_width      = 0.0;
  sat_param_t   o1_amt        = 0.0;
  sat_param_t   o1_sm         = 0.0;
  sat_param_t   o1_in1        = 0.0;
  sat_param_t   o1_in2        = 0.0;
  sat_param_t   o1_in3        = 0.0;
  sat_param_t   o1_in4        = 0.0;
  sat_param_t   o1_ina        = 0.0;
  sat_param_t   o1_inn        = 0.0;
  sat_param_t   o1_oct        = 0.0;
  sat_param_t   o1_semi       = 0.0;
  sat_param_t   o1_cent       = 0.0;
  uint32_t      o1_type       = 0;

  sat_param_t   o2_squ        = 0.0;
  sat_param_t   o2_tri        = 0.0;
  sat_param_t   o2_sin        = 0.0;
  sat_param_t   o2_width      = 0.0;
  sat_param_t   o2_amt        = 0.0;
  sat_param_t   o2_sm         = 0.0;
  sat_param_t   o2_in1        = 0.0;
  sat_param_t   o2_in2        = 0.0;
  sat_param_t   o2_in3        = 0.0;
  sat_param_t   o2_in4        = 0.0;
  sat_param_t   o2_ina        = 0.0;
  sat_param_t   o2_inn        = 0.0;
  sat_param_t   o2_oct        = 0.0;
  sat_param_t   o2_semi       = 0.0;
  sat_param_t   o2_cent       = 0.0;
  uint32_t      o2_type       = 0;

  sat_param_t   r1_imp        = 0.0;
  sat_param_t   r1_rep        = 0.0;
  sat_param_t   r1_fb         = 0.0;
  sat_param_t   r1_damp       = 0.0;
  sat_param_t   r1_gain       = 0.0;
  sat_param_t   r1_drm        = 0.0;
  sat_param_t   r1_amt        = 0.0;
  sat_param_t   r1_sm         = 0.0;
  sat_param_t   r1_in1        = 0.0;
  sat_param_t   r1_in2        = 0.0;
  sat_param_t   r1_in3        = 0.0;
  sat_param_t   r1_in4        = 0.0;
  sat_param_t   r1_ina        = 0.0;
  sat_param_t   r1_inn        = 0.0;
  sat_param_t   r1_type       = 0.0;
  sat_param_t   r1_oct        = 0.0;
  sat_param_t   r1_semi       = 0.0;
  sat_param_t   r1_cent       = 0.0;

  sat_param_t   r2_imp        = 0.0;
  sat_param_t   r2_rep        = 0.0;
  sat_param_t   r2_fb         = 0.0;
  sat_param_t   r2_damp       = 0.0;
  sat_param_t   r2_gain       = 0.0;
  sat_param_t   r2_drm        = 0.0;
  sat_param_t   r2_amt        = 0.0;
  sat_param_t   r2_sm         = 0.0;
  sat_param_t   r2_in1        = 0.0;
  sat_param_t   r2_in2        = 0.0;
  sat_param_t   r2_in3        = 0.0;
  sat_param_t   r2_in4        = 0.0;
  sat_param_t   r2_ina        = 0.0;
  sat_param_t   r2_inn        = 0.0;
  sat_param_t   r2_type       = 0.0;
  sat_param_t   r2_oct        = 0.0;
  sat_param_t   r2_semi       = 0.0;
  sat_param_t   r2_cent       = 0.0;

  sat_param_t   flt_type      = 0.0;
  sat_param_t   flt_freq      = 0.0;
  sat_param_t   flt_q         = 0.0;
  sat_param_t   flt_gain      = 0.0;

  sat_param_t   env_a         = 0.0;
  sat_param_t   env_d         = 0.0;
  sat_param_t   env_s         = 0.0;
  sat_param_t   env_r         = 0.0;

  sat_param_t   mix_o1        = 0.0;
  sat_param_t   mix_o2        = 0.0;
  sat_param_t   mix_r1        = 0.0;
  sat_param_t   mix_r2        = 0.0;

//------------------------------
private:
//------------------------------

  // TODO: smoothing

  //----------

  sat_param_t getParameterValue(uint32_t AIndex) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    return params->getItem(AIndex)->getValue();
  }

  //----------

  sat_param_t getModulatedValue(uint32_t AIndex) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    sat_param_t value = params->getItem(AIndex)->getValue();
    sat_param_t minval = params->getItem(AIndex)->getMinValue();
    sat_param_t maxval = params->getItem(AIndex)->getMaxValue();
    value += MVoiceMod[AIndex];
    return SAT_Clamp(value,minval,maxval);
    //return getParameterValue(AIndex);
  }

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MOverSample = AContext->oversample;
    MBufferSize = AContext->buffersize;
    MSampleRate = AContext->sample_rate; // * MOverSample;
    MOsc1.setSampleRate(MSampleRate);
    MOsc2.setSampleRate(MSampleRate);
    MRes1.setSampleRate(MSampleRate);
    MRes2.setSampleRate(MSampleRate);
    MFlt1.setSampleRate(MSampleRate);
    MEnv1.setSampleRate(MSampleRate);
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return MEnv1.getValue();
  }

  //----------

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    MNoteKey = AIndex;
    MNoteVelocity = AValue;
    MOsc1.noteOn(AIndex,AValue);
    MOsc2.noteOn(AIndex,AValue);
    MRes1.noteOn(AIndex,AValue);
    MRes2.noteOn(AIndex,AValue);
    MFlt1.noteOn(AIndex,AValue);
    MEnv1.noteOn(AIndex,AValue);
    memset(MVoiceMod,0,SA_MAEL_PARAM_COUNT * sizeof(sat_param_t));
    memset(MVoiceExpr,0,SA_MAEL_NUM_NOTE_EXPRESSIONS * sizeof(sat_param_t));
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, sat_param_t AValue) {
    MOsc1.noteOff(AIndex,AValue);
    MOsc2.noteOff(AIndex,AValue);
    MRes1.noteOff(AIndex,AValue);
    MRes2.noteOff(AIndex,AValue);
    MFlt1.noteOff(AIndex,AValue);
    MEnv1.noteOff(AIndex,AValue);
    return SAT_VOICE_RELEASED;
  }

  //----------

  void noteChoke(uint32_t AIndex, sat_param_t AValue) {
  }

  //----------

  // CLAP_NOTE_EXPRESSION_VOLUME      // 0 < x <= 4, plain = 20 * log(x)
  // CLAP_NOTE_EXPRESSION_PAN         // 0 left, 0.5 center, 1 right
  // CLAP_NOTE_EXPRESSION_TUNING      // semitones, from -120 to +120
  // CLAP_NOTE_EXPRESSION_VIBRATO     // 0..1
  // CLAP_NOTE_EXPRESSION_EXPRESSION
  // CLAP_NOTE_EXPRESSION_BRIGHTNESS
  // CLAP_NOTE_EXPRESSION_PRESSURE

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
    MVoiceExpr[AIndex] = AValue;
    MOsc1.noteExpression(AIndex,AValue);
    MOsc2.noteExpression(AIndex,AValue);
    MRes1.noteExpression(AIndex,AValue);
    MRes2.noteExpression(AIndex,AValue);
    MFlt1.noteExpression(AIndex,AValue);
    MEnv1.noteExpression(AIndex,AValue);
  }

  //----------

  void parameter(uint32_t AIndex, sat_param_t AValue) {
    //MVoiceParam[AIndex] = AValue;
  }

  //----------

  void modulation(uint32_t AIndex, sat_param_t AValue) {
    MVoiceMod[AIndex] = AValue;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    sat_sample_t* buffer = MContext->voice_buffer;
    // buffer += (MIndex * MBufferSize);
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {

      sat_sample_t* audio_in = MContext->process_context->process->audio_inputs[0].data32[0];

      prepare_process();
      for (uint32_t i=0; i<ALength; i++) {
        sat_sample_t out = 0.0;
        osc1_out = process_osc1(*audio_in);
        osc2_out = process_osc2(*audio_in);
        res1_out = process_res1(*audio_in);
        res2_out = process_res2(*audio_in);
        out = (osc1_out * mix_o1) + (osc2_out * mix_o2) + (res1_out * mix_r1) + (res2_out * mix_r2);
//        out *= MNoteVelocity;
        out = process_flt1(out);
        out *= process_env1();
        *buffer++ = out;
        audio_in++;
      }
    }
    else {
      memset(buffer,0,ALength * sizeof(sat_sample_t));
    }
    if (MEnv1.getStage() == SAT_ENVELOPE_FINISHED) return SAT_VOICE_FINISHED;
    else return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

//------------------------------
private:
//------------------------------

  void prepare_process() {

    o1_squ        = getModulatedValue(SA_MAEL_PARAM_OSC1_SQU);
    o1_tri        = getModulatedValue(SA_MAEL_PARAM_OSC1_TRI);
    o1_sin        = getModulatedValue(SA_MAEL_PARAM_OSC1_SIN);
    o1_width      = getModulatedValue(SA_MAEL_PARAM_OSC1_WIDTH);
    o1_amt        = getModulatedValue(SA_MAEL_PARAM_OSC1_IN_AMOUNT);
    o1_sm         = getModulatedValue(SA_MAEL_PARAM_OSC1_IN_SMOOTH);
    o1_in1        = getParameterValue(SA_MAEL_PARAM_OSC1_IN_O1);
    o1_in2        = getParameterValue(SA_MAEL_PARAM_OSC1_IN_O2);
    o1_in3        = getParameterValue(SA_MAEL_PARAM_OSC1_IN_R1);
    o1_in4        = getParameterValue(SA_MAEL_PARAM_OSC1_IN_R2);
    o1_ina        = getParameterValue(SA_MAEL_PARAM_OSC1_IN_A);
    o1_inn        = getParameterValue(SA_MAEL_PARAM_OSC1_IN_N);
    o1_oct        = getModulatedValue(SA_MAEL_PARAM_OSC1_OCT);
    o1_semi       = getModulatedValue(SA_MAEL_PARAM_OSC1_SEMI);
    o1_cent       = getModulatedValue(SA_MAEL_PARAM_OSC1_CENT);
    o1_type       = getParameterValue(SA_MAEL_PARAM_OSC1_TYPE);

    o2_squ        = getModulatedValue(SA_MAEL_PARAM_OSC2_SQU);
    o2_tri        = getModulatedValue(SA_MAEL_PARAM_OSC2_TRI);
    o2_sin        = getModulatedValue(SA_MAEL_PARAM_OSC2_SIN);
    o2_width      = getModulatedValue(SA_MAEL_PARAM_OSC2_WIDTH);
    o2_amt        = getModulatedValue(SA_MAEL_PARAM_OSC2_IN_AMOUNT);
    o2_sm         = getModulatedValue(SA_MAEL_PARAM_OSC2_IN_SMOOTH);
    o2_in1        = getParameterValue(SA_MAEL_PARAM_OSC2_IN_O1);
    o2_in2        = getParameterValue(SA_MAEL_PARAM_OSC2_IN_O2);
    o2_in3        = getParameterValue(SA_MAEL_PARAM_OSC2_IN_R1);
    o2_in4        = getParameterValue(SA_MAEL_PARAM_OSC2_IN_R2);
    o2_ina        = getParameterValue(SA_MAEL_PARAM_OSC2_IN_A);
    o2_inn        = getParameterValue(SA_MAEL_PARAM_OSC2_IN_N);
    o2_oct        = getModulatedValue(SA_MAEL_PARAM_OSC2_OCT);
    o2_semi       = getModulatedValue(SA_MAEL_PARAM_OSC2_SEMI);
    o2_cent       = getModulatedValue(SA_MAEL_PARAM_OSC2_CENT);
    o2_type       = getParameterValue(SA_MAEL_PARAM_OSC2_TYPE);

    r1_imp        = getModulatedValue(SA_MAEL_PARAM_RES1_IMPULSE);
    r1_rep        = getModulatedValue(SA_MAEL_PARAM_RES1_REPEAT);
    r1_fb         = getModulatedValue(SA_MAEL_PARAM_RES1_FEEDBACK);
    r1_damp       = getModulatedValue(SA_MAEL_PARAM_RES1_DAMPING);
    r1_gain       = getModulatedValue(SA_MAEL_PARAM_RES1_GAIN);
    r1_drm        = getModulatedValue(SA_MAEL_PARAM_RES1_DRM);
    r1_amt        = getModulatedValue(SA_MAEL_PARAM_RES1_IN_AMOUNT);
    r1_sm         = getModulatedValue(SA_MAEL_PARAM_RES1_IN_SMOOTH);
    r1_in1        = getParameterValue(SA_MAEL_PARAM_RES1_IN_O1);
    r1_in2        = getParameterValue(SA_MAEL_PARAM_RES1_IN_O2);
    r1_in3        = getParameterValue(SA_MAEL_PARAM_RES1_IN_R1);
    r1_in4        = getParameterValue(SA_MAEL_PARAM_RES1_IN_R2);
    r1_ina        = getParameterValue(SA_MAEL_PARAM_RES1_IN_A);
    r1_inn        = getParameterValue(SA_MAEL_PARAM_RES1_IN_N);
    r1_type       = getModulatedValue(SA_MAEL_PARAM_RES1_TYPE);
    r1_oct        = getModulatedValue(SA_MAEL_PARAM_RES1_OCT);
    r1_semi       = getModulatedValue(SA_MAEL_PARAM_RES1_SEMI);
    r1_cent       = getModulatedValue(SA_MAEL_PARAM_RES1_CENT);

    r2_imp        = getModulatedValue(SA_MAEL_PARAM_RES2_IMPULSE);
    r2_rep        = getModulatedValue(SA_MAEL_PARAM_RES2_REPEAT);
    r2_fb         = getModulatedValue(SA_MAEL_PARAM_RES2_FEEDBACK);
    r2_damp       = getModulatedValue(SA_MAEL_PARAM_RES2_DAMPING);
    r2_gain       = getModulatedValue(SA_MAEL_PARAM_RES2_GAIN);
    r2_drm        = getModulatedValue(SA_MAEL_PARAM_RES2_DRM);
    r2_amt        = getModulatedValue(SA_MAEL_PARAM_RES2_IN_AMOUNT);
    r2_sm         = getModulatedValue(SA_MAEL_PARAM_RES2_IN_SMOOTH);
    r2_in1        = getParameterValue(SA_MAEL_PARAM_RES2_IN_O1);
    r2_in2        = getParameterValue(SA_MAEL_PARAM_RES2_IN_O2);
    r2_in3        = getParameterValue(SA_MAEL_PARAM_RES2_IN_R1);
    r2_in4        = getParameterValue(SA_MAEL_PARAM_RES2_IN_R2);
    r2_ina        = getParameterValue(SA_MAEL_PARAM_RES2_IN_A);
    r2_inn        = getParameterValue(SA_MAEL_PARAM_RES2_IN_N);
    r2_type       = getModulatedValue(SA_MAEL_PARAM_RES2_TYPE);
    r2_oct        = getModulatedValue(SA_MAEL_PARAM_RES2_OCT);
    r2_semi       = getModulatedValue(SA_MAEL_PARAM_RES2_SEMI);
    r2_cent       = getModulatedValue(SA_MAEL_PARAM_RES2_CENT);

    flt_type      = getParameterValue(SA_MAEL_PARAM_FLT1_TYPE);
    flt_freq      = getModulatedValue(SA_MAEL_PARAM_FLT1_FREQ);
    flt_q         = getModulatedValue(SA_MAEL_PARAM_FLT1_Q);
    flt_gain      = getModulatedValue(SA_MAEL_PARAM_FLT1_GAIN);

    env_a         = getParameterValue(SA_MAEL_PARAM_ENV1_ATT);
    env_d         = getParameterValue(SA_MAEL_PARAM_ENV1_DEC);
    env_s         = getParameterValue(SA_MAEL_PARAM_ENV1_SUS);
    env_r         = getParameterValue(SA_MAEL_PARAM_ENV1_REL);

    mix_o1        = getModulatedValue(SA_MAEL_PARAM_MIX_O1);
    mix_o2        = getModulatedValue(SA_MAEL_PARAM_MIX_O2);
    mix_r1        = getModulatedValue(SA_MAEL_PARAM_MIX_R1);
    mix_r2        = getModulatedValue(SA_MAEL_PARAM_MIX_R2);

    flt_freq      = flt_freq * flt_freq * flt_freq;
    flt_q         = flt_q * flt_q * flt_q;

    o1_sm         = 1.0 - o1_sm;
    o2_sm         = 1.0 - o2_sm;
    r1_sm         = 1.0 - r1_sm;
    r2_sm         = 1.0 - r2_sm;

    r1_imp        = 1.0 - r1_imp;
    r2_imp        = 1.0 - r2_imp;

    r1_drm = (r1_drm * 2.0) - 1.0;        // 0..1 -> -1..1
    //r1_drm = SAT_Curve(r1_drm,0.95);
    r1_drm = SAT_CurveSigned(r1_drm,0.95);
    r1_drm = (r1_drm * 0.5) + 0.5;        // -1..1 -> 0..1

    r2_drm = (r2_drm * 2.0) - 1.0;        // 0..1 -> -1..1
    //r2_drm = SAT_Curve(r2_drm,0.95);
    r2_drm = SAT_CurveSigned(r2_drm,0.95);
    r2_drm = (r2_drm * 0.5) + 0.5;        // -1..1 -> 0..1

    o1_in_factor  = o1_sm * o1_sm * o1_sm;
    o2_in_factor  = o2_sm * o2_sm * o2_sm;
    r1_in_factor  = r1_sm * r1_sm * r1_sm;
    r2_in_factor  = r2_sm * r2_sm * r2_sm;

    MOsc1.setPulseWidth(o1_width);
    MOsc1.setSawSqu(o1_squ);
    MOsc1.setSquTri(o1_tri);
    MOsc1.setTriSin(o1_sin);

    MOsc2.setPulseWidth(o2_width);
    MOsc2.setSawSqu(o2_squ);
    MOsc2.setSquTri(o2_tri);
    MOsc2.setTriSin(o2_sin);

    MRes1.setImpulse(r1_imp);
    MRes1.setRepeat(r1_rep);
    MRes1.setFeedback(r1_fb);
    MRes1.setDamping(r1_damp);
    MRes1.setGain(r1_gain);
    MRes1.setType(r1_type);
    MRes1.setDrm(r1_drm);

    MRes2.setImpulse(r2_imp);
    MRes2.setRepeat(r2_rep);
    MRes2.setFeedback(r2_fb);
    MRes2.setDamping(r2_damp);
    MRes2.setGain(r2_gain);
    MRes2.setType(r2_type);
    MRes2.setDrm(r2_drm);

    MFlt1.set(flt_type,flt_freq,flt_q,flt_gain);
    MFlt1.update_coeffs();

    MEnv1.setADSR(env_a,env_d,env_s,env_r);

  }

  //----------

  sat_sample_t process_osc1(sat_sample_t audio_in) {
    sat_sample_t rnd = SAT_RandomRange(-1.0,1.0);

    sat_sample_t o1_in = (o1_in1 * osc1_out) + (o1_in2 * osc2_out) + (o1_in3 * res1_out) + (o1_in4 * res2_out) + (o1_ina * audio_in) + (o1_inn * rnd);
    o1_in_current += ((o1_in - o1_in_current) * o1_in_factor);
    o1_in = o1_in_current;

    o1_in *= o1_amt;
    sat_param_t o1_tuning = (o1_oct * 12.0) + o1_semi + o1_cent;
    o1_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];
    sat_param_t hz1 = SAT_NoteToHz(MNoteKey + o1_tuning);
    if (hz1 < 20.0) hz1 = 20.0;
    MOsc1.setFrequency(hz1);

    sat_sample_t mod = 0.0;
    sat_sample_t ofs = 0.0;
    switch (o1_type) {
      case SA_MAEL_OSC_PHASE_MOD: {
        mod = ((o1_in * 8.0) * o1_amt); // 2.5
        o1_tuning += mod;
        break;
      }
      case SA_MAEL_OSC_FREQ_MOD: {
        ofs = ((o1_in * 48.0) * o1_amt);  // 20
        break;
      }
    }

    sat_sample_t osc1 = MOsc1.process_mod(/*audio_in,*/mod,ofs);

    switch (o1_type) {
      case SA_MAEL_OSC_RING_MOD: {
        osc1 = SAT_Interpolate_Linear(o1_amt,osc1,(osc1 * o1_in));
        break;
      }
      case SA_MAEL_OSC_AMPL_MOD: {
        osc1 = SAT_Interpolate_Linear(o1_amt,osc1,(osc1 * abs(o1_in)));
        break;
      }
      case SA_MAEL_OSC_MAX: {
        sat_sample_t n = osc1;
        if (abs(o1_in) > abs(osc1)) n = o1_in;
        osc1 = SAT_Interpolate_Linear(o1_amt,osc1,n);
        break;
      }
    }

    osc1 = SAT_Clamp(osc1,-1,1);
    return osc1;
  }

  //----------

  sat_sample_t process_osc2(sat_sample_t audio_in) {
    sat_sample_t rnd = SAT_RandomRange(-1.0,1.0);

    sat_sample_t o2_in = (o2_in1 * osc1_out) + (o2_in2 * osc2_out) + (o2_in3 * res1_out) + (o2_in4 * res2_out) + (o2_ina * audio_in) + (o2_inn * rnd);
    o2_in_current += ((o2_in - o2_in_current) * o2_in_factor);
    o2_in = o2_in_current;

    o2_in *= o2_amt;
    sat_param_t o2_tuning = (o2_oct * 12.0) + o2_semi + o2_cent;
    o2_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];
    sat_param_t hz2 = SAT_NoteToHz(MNoteKey + o2_tuning);
    if (hz2 < 20.0) hz2 = 20.0;
    MOsc2.setFrequency(hz2);

    sat_sample_t mod = 0.0;
    sat_sample_t ofs = 0.0;
    switch (o2_type) {
      case SA_MAEL_OSC_PHASE_MOD: {
        mod = ((o2_in * 8.0) * o2_amt);
        break;
      }
      case SA_MAEL_OSC_FREQ_MOD: {
        ofs = ((o2_in * 48.0) * o2_amt);
        break;
      }
    }

    sat_sample_t osc2 = MOsc2.process_mod(/*audio_in,*/mod,ofs);

    switch (o2_type) {
      case SA_MAEL_OSC_RING_MOD: {
        osc2 = SAT_Interpolate_Linear(o2_amt,osc2,(osc2 * o2_in));
        break;
      }
      case SA_MAEL_OSC_AMPL_MOD: {
        osc2 = SAT_Interpolate_Linear(o2_amt,osc2,(osc2 * abs(o2_in)));
        break;
      }
      case SA_MAEL_OSC_MAX: {
        sat_sample_t n = osc2;
        if (abs(o2_in) > abs(osc2)) n = o2_in;
        osc2 = SAT_Interpolate_Linear(o2_amt,osc2,n);
        break;
      }
    }

    osc2 = SAT_Clamp(osc2,-1,1);
    return osc2;
  }

  //----------

  sat_sample_t process_res1(sat_sample_t audio_in) {
    sat_sample_t rnd = SAT_RandomRange(-1.0,1.0);

    sat_sample_t r1_in = (r1_in1 * osc1_out) + (r1_in2 * osc2_out) + (r1_in3 * res1_out) + (r1_in4 * res2_out) + (r1_ina * audio_in) + (r1_inn * rnd);
    r1_in_current += ((r1_in - r1_in_current) * r1_in_factor);
    r1_in = r1_in_current;
    r1_in *= r1_amt;

    sat_param_t r1_tuning = (r1_oct * 12.0) + r1_semi + r1_cent;
    r1_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];;
    sat_param_t hz1 = SAT_NoteToHz(MNoteKey + r1_tuning);
    if (hz1 < 20.0) hz1 = 20.0;
    MRes1.setFrequency(hz1);

    sat_sample_t res1 = MRes1.process(r1_in);

    res1 = SAT_Clamp(res1,-1,1);
    return res1;
  }

  //----------

  sat_sample_t process_res2(sat_sample_t audio_in) {
    sat_sample_t rnd = SAT_RandomRange(-1.0,1.0);
    
    sat_sample_t r2_in = (r2_in1 * osc1_out) + (r2_in2 * osc2_out) + (r2_in3 * res1_out) + (r2_in4 * res2_out) + (r2_ina * audio_in) + (r2_inn * rnd);
    r2_in_current += ((r2_in - r2_in_current) * r2_in_factor);
    r2_in = r2_in_current;

    r2_in *= r2_amt;
    sat_param_t r2_tuning = (r2_oct * 12.0) + r2_semi + r2_cent;
    r2_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];;
    sat_param_t hz2 = SAT_NoteToHz(MNoteKey + r2_tuning);
    if (hz2 < 20.0) hz2 = 20.0;
    MRes2.setFrequency(hz2);

    sat_sample_t res2 = MRes2.process(r2_in);

    res2 = SAT_Clamp(res2,-1,1);
    return res2;
  }

  //----------

  sat_sample_t process_flt1(sat_sample_t spl) {
    if (flt_type == 0) return spl;
    else return MFlt1.process(spl);
  }

  //----------

  sat_sample_t process_env1() {
    return MEnv1.process();
  }

};

//----------------------------------------------------------------------
#endif