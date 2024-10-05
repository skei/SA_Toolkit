#ifndef sa_synth_voice_included
#define sa_synth_voice_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "base/util/sat_interpolation.h"
#include "plugin/sat_plugin_base.h"

#include "sa_synth_osc.h"
#include "sa_synth_res.h"
#include "sa_synth_flt.h"
#include "sa_synth_env.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_synth_voice {

//------------------------------
private:
//------------------------------

  SAT_VoiceContext*   MContext      = nullptr;
  uint32_t            MIndex        = 0;
  sat_sample_t        MSampleRate   = 0.0;
  sa_synth_osc        MOsc1         = {};
  sa_synth_osc        MOsc2         = {};
  sa_synth_res        MRes1         = {};
  sa_synth_res        MRes2         = {};

  sa_synth_flt        MFlt1         = {};
  sa_synth_env        MEnv1         = {};

  uint32_t            MKey          = 0;
  double              MVelocity     = 0.0;

  sat_sample_t        MOsc1_out     = 0.0;
  sat_sample_t        MOsc2_out     = 0.0;
  sat_sample_t        MRes1_out     = 0.0;
  sat_sample_t        MRes2_out     = 0.0;

  // sat_param_t         MVoiceParam[SA_SYNTH_PARAM_COUNT] = {};
  
  sat_param_t         MVoiceMod[SA_SYNTH_PARAM_COUNT]   = {};
  sat_param_t         MVoiceExpr[16]                    = {}; // clap has 7 expressions defined..

//------------------------------
private:
//------------------------------

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

  //----------

  void update_tuning() {
    // prepare for next sample
    // sat_param_t tuning1 = (osc1_oct * 12.0) + osc1_semi + osc1_cent;
    // tuning1 += e_tuning;
    // sat_param_t hz1 = SAT_NoteToHz(MKey + tuning1);
    // MOscillator1.setFrequency(hz1);
    // sat_param_t tuning2 = (osc2_oct * 12.0) + osc2_semi + osc2_cent;
    // tuning2 += (e_tuning + fm2);
    // sat_param_t hz2 = SAT_NoteToHz(MKey + tuning2);
    // MOscillator2.setFrequency(hz2);
  }

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
    MOsc1.setSampleRate(MSampleRate);
    MOsc2.setSampleRate(MSampleRate);
    MRes1.setSampleRate(MSampleRate);
    MRes2.setSampleRate(MSampleRate);
    MFlt1.setSampleRate(MSampleRate);
    MEnv1.setSampleRate(MSampleRate);
    // memset(MVoiceParam,0,SA_SYNTH_PARAM_COUNT * sizeof(sat_param_t));
    // memset(MVoiceMod,0,SA_SYNTH_PARAM_COUNT * sizeof(sat_param_t));
    // memset(MVoiceExpr,0,16 * sizeof(sat_param_t));
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return MEnv1.getValue();
  }

  //----------

  /*
    should we need to copy parameters over from global?
  */

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    MKey = AIndex;
    MVelocity = AValue;
    //SAT_ParameterArray* params = MContext->process_context->parameters;
    init_osc1(AIndex,AValue);
    init_osc2(AIndex,AValue);
    init_res1(AIndex,AValue);
    init_res2(AIndex,AValue);
    init_flt1(AIndex,AValue);
    init_env1(AIndex,AValue);

    memset(MVoiceMod,0,SA_SYNTH_PARAM_COUNT * sizeof(sat_param_t));
    memset(MVoiceExpr,0,16 * sizeof(sat_param_t));

    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, sat_param_t AValue) {
    //return SAT_VOICE_FINISHED;
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

  // CLAP_NOTE_EXPRESSION_VOLUME = 0,    // with 0 < x <= 4, plain = 20 * log(x)
  // CLAP_NOTE_EXPRESSION_PAN = 1,       // pan, 0 left, 0.5 center, 1 right
  // CLAP_NOTE_EXPRESSION_TUNING = 2,    // Relative tuning in semitones, from -120 to +120
  // CLAP_NOTE_EXPRESSION_VIBRATO = 3,   // 0..1
  // CLAP_NOTE_EXPRESSION_EXPRESSION = 4,
  // CLAP_NOTE_EXPRESSION_BRIGHTNESS = 5,
  // CLAP_NOTE_EXPRESSION_PRESSURE = 6,

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
    //SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
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
    //SAT_PRINT("%i = %.3f\n",AIndex,AValue);
    //MVoiceParam[AIndex] = AValue;
  }

  //----------

  void modulation(uint32_t AIndex, sat_param_t AValue) {
    MVoiceMod[AIndex] = AValue;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    sat_sample_t* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;

    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {

      SAT_ParameterArray* params = MContext->process_context->parameters;
      SAT_Assert(params);

      sat_param_t o1_mix = getModulatedValue(SA_SYNTH_PARAM_MIX_O1);
      sat_param_t o2_mix = getModulatedValue(SA_SYNTH_PARAM_MIX_O2);
      sat_param_t r1_mix = getModulatedValue(SA_SYNTH_PARAM_MIX_R1);
      sat_param_t r2_mix = getModulatedValue(SA_SYNTH_PARAM_MIX_R2);

      for (uint32_t i=0; i<ALength; i++) {
        sat_sample_t out = 0.0;

        // process
        MOsc1_out = process_osc1();
        MOsc2_out = process_osc2();
        MRes1_out = process_res1();
        MRes2_out = process_res2();

        // mix
        out = (MOsc1_out * o1_mix) + (MOsc2_out * o2_mix) + (MRes1_out * r1_mix) + (MRes2_out * r2_mix);
        out *= MVelocity;

        // flt.. env.. out
        out = process_flt1(out);
        out *= process_env1();
        *buffer++ = out;

        // prepare for next sample
        // sat_param_t tuning1 = (osc1_oct * 12.0) + osc1_semi + osc1_cent;
        // tuning1 += e_tuning;
        // sat_param_t hz1 = SAT_NoteToHz(MKey + tuning1);
        // MOscillator1.setFrequency(hz1);
        // sat_param_t tuning2 = (osc2_oct * 12.0) + osc2_semi + osc2_cent;
        // tuning2 += (e_tuning + fm2);
        // sat_param_t hz2 = SAT_NoteToHz(MKey + tuning2);
        // MOscillator2.setFrequency(hz2);

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

  void init_osc1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t o1_squ   = getParameterValue(SA_SYNTH_PARAM_OSC1_SQU);
    sat_param_t o1_tri   = getParameterValue(SA_SYNTH_PARAM_OSC1_TRI);
    sat_param_t o1_sin   = getParameterValue(SA_SYNTH_PARAM_OSC1_SIN);
    sat_param_t o1_width = getParameterValue(SA_SYNTH_PARAM_OSC1_WIDTH);
    MOsc1.setPulseWidth(o1_width);
    MOsc1.setSawSqu(o1_squ);
    MOsc1.setSquTri(o1_tri);
    MOsc1.setTriSin(o1_sin);
    sat_param_t o1_oct  = getParameterValue(SA_SYNTH_PARAM_OSC1_OCT);
    sat_param_t o1_semi = getParameterValue(SA_SYNTH_PARAM_OSC1_SEMI);
    sat_param_t o1_cent = getParameterValue(SA_SYNTH_PARAM_OSC1_CENT);
    sat_param_t o1_tuning = (o1_oct * 12.0) + o1_semi + o1_cent;
    //o1_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];
    sat_param_t hz1 = SAT_NoteToHz(MKey + o1_tuning);
    if (hz1 < 20.0) hz1 = 20.0;
    MOsc1.setFrequency(hz1);
    MOsc1.reset();
    MOsc1.noteOn(AIndex,AValue);
  }

  //----------

  void init_osc2(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t o2_squ   = getParameterValue(SA_SYNTH_PARAM_OSC2_SQU);
    sat_param_t o2_tri   = getParameterValue(SA_SYNTH_PARAM_OSC2_TRI);
    sat_param_t o2_sin   = getParameterValue(SA_SYNTH_PARAM_OSC2_SIN);
    sat_param_t o2_width = getParameterValue(SA_SYNTH_PARAM_OSC2_WIDTH);
    MOsc2.setPulseWidth(o2_width);
    MOsc2.setSawSqu(o2_squ);
    MOsc2.setSquTri(o2_tri);
    MOsc2.setTriSin(o2_sin);
    sat_param_t o2_oct  = getParameterValue(SA_SYNTH_PARAM_OSC2_OCT);
    sat_param_t o2_semi = getParameterValue(SA_SYNTH_PARAM_OSC2_SEMI);
    sat_param_t o2_cent = getParameterValue(SA_SYNTH_PARAM_OSC2_CENT);
    sat_param_t o2_tuning = (o2_oct * 12.0) + o2_semi + o2_cent;
    //o2_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];
    sat_param_t hz2 = SAT_NoteToHz(MKey + o2_tuning);
    if (hz2 < 20.0) hz2 = 20.0;
    MOsc2.setFrequency(hz2);
    MOsc2.reset();
    MOsc2.noteOn(AIndex,AValue);
  }

  //----------

  void init_res1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t r1_imp_freq   = getParameterValue(SA_SYNTH_PARAM_RES1_IMPULSE_FREQ);
    sat_param_t r1_fb         = getParameterValue(SA_SYNTH_PARAM_RES1_FEEDBACK);
    sat_param_t r1_damp_freq  = getParameterValue(SA_SYNTH_PARAM_RES1_DAMPING_FREQ);
    sat_param_t r1_gain       = getParameterValue(SA_SYNTH_PARAM_RES1_GAIN);
    MRes1.setFeedback(r1_fb);
    MRes1.setGain(r1_gain);
    sat_param_t r1_oct  = getParameterValue(SA_SYNTH_PARAM_RES1_OCT);
    sat_param_t r1_semi = getParameterValue(SA_SYNTH_PARAM_RES1_SEMI);
    sat_param_t r1_cent = getParameterValue(SA_SYNTH_PARAM_RES1_CENT);
    sat_param_t r1_tuning = (r1_oct * 12.0) + r1_semi + r1_cent;
    //r1_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];
    sat_param_t hz1 = SAT_NoteToHz(MKey + r1_tuning);
    if (hz1 < 20.0) hz1 = 20.0;
    MRes1.setFrequency(hz1);
    // MRes1.reset();
    MRes1.noteOn(AIndex,AValue);
  }

  //----------

  void init_res2(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t r2_imp_freq  = getParameterValue(SA_SYNTH_PARAM_RES2_IMPULSE_FREQ);
    sat_param_t r2_fb        = getParameterValue(SA_SYNTH_PARAM_RES2_FEEDBACK);
    sat_param_t r2_damp_freq = getParameterValue(SA_SYNTH_PARAM_RES2_DAMPING_FREQ);
    sat_param_t r2_gain      = getParameterValue(SA_SYNTH_PARAM_RES2_GAIN);
    MRes2.setFeedback(r2_fb);
    MRes2.setGain(r2_gain);
    sat_param_t r2_oct  = getParameterValue(SA_SYNTH_PARAM_RES2_OCT);
    sat_param_t r2_semi = getParameterValue(SA_SYNTH_PARAM_RES2_SEMI);
    sat_param_t r2_cent = getParameterValue(SA_SYNTH_PARAM_RES2_CENT);
    sat_param_t r2_tuning = (r2_oct * 12.0) + r2_semi + r2_cent;
    //r2_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];
    sat_param_t hz2 = SAT_NoteToHz(MKey + r2_tuning);
    if (hz2 < 20.0) hz2 = 20.0;
    MRes2.setFrequency(hz2);
    // MRes2.reset();
    MRes2.noteOn(AIndex,AValue);
  }

  //----------

  void init_flt1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t type = getParameterValue(SA_SYNTH_PARAM_FLT1_TYPE);
    sat_param_t freq = getParameterValue(SA_SYNTH_PARAM_FLT1_FREQ);
    sat_param_t q    = getParameterValue(SA_SYNTH_PARAM_FLT1_Q);
    sat_param_t gain = getParameterValue(SA_SYNTH_PARAM_FLT1_GAIN);
    MFlt1.set(type,freq,q,gain);
    MFlt1.update_coeffs();
    MFlt1.reset();
  }

  //----------

  void init_env1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t a = getParameterValue(SA_SYNTH_PARAM_ENV1_ATT);
    sat_param_t d = getParameterValue(SA_SYNTH_PARAM_ENV1_DEC);
    sat_param_t s = getParameterValue(SA_SYNTH_PARAM_ENV1_SUS);
    sat_param_t r = getParameterValue(SA_SYNTH_PARAM_ENV1_REL);
    MEnv1.setADSR(a,d,s,r);
    MEnv1.noteOn();
  }

//------------------------------
private:
//------------------------------

  sat_sample_t process_osc1() {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_sample_t audio  = 0.0;
    sat_sample_t rnd    = SAT_RandomRange(-1.0,1.0);

    // setup osc
    sat_param_t o1_squ   = getModulatedValue(SA_SYNTH_PARAM_OSC1_SQU);
    sat_param_t o1_tri   = getModulatedValue(SA_SYNTH_PARAM_OSC1_TRI);
    sat_param_t o1_sin   = getModulatedValue(SA_SYNTH_PARAM_OSC1_SIN);
    sat_param_t o1_width = getModulatedValue(SA_SYNTH_PARAM_OSC1_WIDTH);

    MOsc1.setPulseWidth(o1_width);
    MOsc1.setSawSqu(o1_squ);
    MOsc1.setSquTri(o1_tri);
    MOsc1.setTriSin(o1_sin);

    // inputs
    sat_param_t o1_amt = getModulatedValue(SA_SYNTH_PARAM_OSC1_IN_AMOUNT);
    sat_param_t o1_in1 = getParameterValue(SA_SYNTH_PARAM_OSC1_IN_O1);
    sat_param_t o1_in2 = getParameterValue(SA_SYNTH_PARAM_OSC1_IN_O2);
    sat_param_t o1_in3 = getParameterValue(SA_SYNTH_PARAM_OSC1_IN_R1);
    sat_param_t o1_in4 = getParameterValue(SA_SYNTH_PARAM_OSC1_IN_R2);
    sat_param_t o1_ina = getParameterValue(SA_SYNTH_PARAM_OSC1_IN_A);
    sat_param_t o1_inn = getParameterValue(SA_SYNTH_PARAM_OSC1_IN_N);
    sat_sample_t o1_in = (o1_in1 * MOsc1_out) + (o1_in2 * MOsc2_out) + (o1_in3 * MRes1_out) + (o1_in4 * MRes2_out) + (o1_ina * audio) + (o1_inn * rnd);
    o1_in *= o1_amt;

    // tuning
    sat_param_t o1_oct  = getModulatedValue(SA_SYNTH_PARAM_OSC1_OCT);
    sat_param_t o1_semi = getModulatedValue(SA_SYNTH_PARAM_OSC1_SEMI);
    sat_param_t o1_cent = getModulatedValue(SA_SYNTH_PARAM_OSC1_CENT);
    sat_param_t o1_tuning = (o1_oct * 12.0) + o1_semi + o1_cent;
    o1_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];;

    sat_sample_t mod = 0.0;
    sat_sample_t ofs = 0.0;

    // pre modulation (phase)
    uint32_t o1_type = getParameterValue(SA_SYNTH_PARAM_OSC1_TYPE);
    switch (o1_type) {
      case SA_SYNTH_OSC_PHASE_MOD: {
        mod = ((o1_in * 2.5) * o1_amt);
        o1_tuning += mod;
        break;
      }
      case SA_SYNTH_OSC_FREQ_MOD: {
        ofs = ((o1_in * 20.0) * o1_amt);
        break;
      }
    }

    // process
    sat_param_t hz1 = SAT_NoteToHz(MKey + o1_tuning);
    if (hz1 < 20.0) hz1 = 20.0;
    MOsc1.setFrequency(hz1);
    sat_sample_t osc1 = MOsc1.process_mod(audio,mod,ofs);

    // post modulation (wave)
    switch (o1_type) {
      case SA_SYNTH_OSC_RING_MOD: {
        //osc1 *= (o1_in * o1_amt);
        osc1 = SAT_Interpolate_Linear(o1_amt,osc1,(osc1 * o1_in));
        break;
      }
      case SA_SYNTH_OSC_AMPL_MOD: {
        //osc1 *= (abs(o1_in) * o1_amt);
        osc1 = SAT_Interpolate_Linear(o1_amt,osc1,(osc1 * abs(o1_in)));
        break;
      }
      case SA_SYNTH_OSC_MAX: {
        sat_sample_t n = osc1;
        if (abs(o1_in) > abs(osc1)) n = o1_in;
        osc1 = SAT_Interpolate_Linear(o1_amt,osc1,n);
        break;
      }
    }

    // out
    osc1 = SAT_Clamp(osc1,-1,1);
    return osc1;
  }

  //----------

  sat_sample_t process_osc2() {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_sample_t audio  = 0.0;
    sat_sample_t rnd    = SAT_RandomRange(-1.0,1.0);

    // setup osc
    sat_param_t o2_squ    = getModulatedValue(SA_SYNTH_PARAM_OSC2_SQU);
    sat_param_t o2_tri    = getModulatedValue(SA_SYNTH_PARAM_OSC2_TRI);
    sat_param_t o2_sin    = getModulatedValue(SA_SYNTH_PARAM_OSC2_SIN);
    sat_param_t o2_width  = getModulatedValue(SA_SYNTH_PARAM_OSC2_WIDTH);
    MOsc2.setPulseWidth(o2_width);
    MOsc2.setSawSqu(o2_squ);
    MOsc2.setSquTri(o2_tri);
    MOsc2.setTriSin(o2_sin);

    // inputs
    sat_param_t o2_amt    = getModulatedValue(SA_SYNTH_PARAM_OSC2_IN_AMOUNT);
    sat_param_t o2_in1    = getParameterValue(SA_SYNTH_PARAM_OSC2_IN_O1);
    sat_param_t o2_in2    = getParameterValue(SA_SYNTH_PARAM_OSC2_IN_O2);
    sat_param_t o2_in3    = getParameterValue(SA_SYNTH_PARAM_OSC2_IN_R1);
    sat_param_t o2_in4    = getParameterValue(SA_SYNTH_PARAM_OSC2_IN_R2);
    sat_param_t o2_ina    = getParameterValue(SA_SYNTH_PARAM_OSC2_IN_A);
    sat_param_t o2_inn    = getParameterValue(SA_SYNTH_PARAM_OSC2_IN_N);
    sat_sample_t o2_in    = (o2_in1 * MOsc1_out) + (o2_in2 * MOsc2_out) + (o2_in3 * MRes1_out) + (o2_in4 * MRes2_out) + (o2_ina * audio) + (o2_inn * rnd);
    o2_in *= o2_amt;

    // tuning
    sat_param_t o2_oct    = getModulatedValue(SA_SYNTH_PARAM_OSC2_OCT);
    sat_param_t o2_semi   = getModulatedValue(SA_SYNTH_PARAM_OSC2_SEMI);
    sat_param_t o2_cent   = getModulatedValue(SA_SYNTH_PARAM_OSC2_CENT);
    sat_param_t o2_tuning = (o2_oct * 12.0) + o2_semi + o2_cent;
    o2_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];;

    sat_sample_t mod = 0.0;
    sat_sample_t ofs = 0.0;

    // pre modulation (phase)
    uint32_t o2_type = getParameterValue(SA_SYNTH_PARAM_OSC2_TYPE);
    switch (o2_type) {
      case SA_SYNTH_OSC_PHASE_MOD: {
        mod = ((o2_in * 2.5) * o2_amt);
        break;
      }
      case SA_SYNTH_OSC_FREQ_MOD: {
        ofs = ((o2_in * 20.0) * o2_amt);
        break;
      }
    }

    // process
    sat_param_t hz2 = SAT_NoteToHz(MKey + o2_tuning);
    if (hz2 < 20.0) hz2 = 20.0;
    MOsc2.setFrequency(hz2);
    sat_sample_t osc2 = MOsc2.process_mod(audio,mod,ofs);

    // post modulation (wave)
    switch (o2_type) {
      case SA_SYNTH_OSC_RING_MOD: {
        //osc2 *= (o2_in * o2_amt);
        osc2 = SAT_Interpolate_Linear(o2_amt,osc2,(osc2 * o2_in));
        break;
      }
      case SA_SYNTH_OSC_AMPL_MOD: {
        //osc2 *= (abs(o2_in) * o2_amt);
        osc2 = SAT_Interpolate_Linear(o2_amt,osc2,(osc2 * abs(o2_in)));
        break;
      }
      case SA_SYNTH_OSC_MAX: {
        sat_sample_t n = osc2;
        if (abs(o2_in) > abs(osc2)) n = o2_in;
        osc2 = SAT_Interpolate_Linear(o2_amt,osc2,n);
        break;
      }
    }

    // out
    osc2 = SAT_Clamp(osc2,-1,1);
    return osc2;
  }

  //----------

  sat_sample_t process_res1() {

    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_sample_t audio  = 0.0;
    sat_sample_t rnd    = SAT_RandomRange(-1.0,1.0);

    sat_param_t r1_imp_freq   = getModulatedValue(SA_SYNTH_PARAM_RES1_IMPULSE_FREQ);
    sat_param_t r1_fb         = getModulatedValue(SA_SYNTH_PARAM_RES1_FEEDBACK);
    sat_param_t r1_damp_freq  = getModulatedValue(SA_SYNTH_PARAM_RES1_DAMPING_FREQ);
    sat_param_t r1_gain       = getModulatedValue(SA_SYNTH_PARAM_RES1_GAIN);
    MRes1.setImpulse(r1_imp_freq);
    MRes1.setFeedback(r1_fb);
    MRes1.setDamping(r1_damp_freq);
    MRes1.setGain(r1_gain);

    // inputs
    sat_param_t r1_amt    = getModulatedValue(SA_SYNTH_PARAM_RES1_IN_AMOUNT);
    sat_param_t r1_in1    = getParameterValue(SA_SYNTH_PARAM_RES1_IN_O1);
    sat_param_t r1_in2    = getParameterValue(SA_SYNTH_PARAM_RES1_IN_O2);
    sat_param_t r1_in3    = getParameterValue(SA_SYNTH_PARAM_RES1_IN_R1);
    sat_param_t r1_in4    = getParameterValue(SA_SYNTH_PARAM_RES1_IN_R2);
    sat_param_t r1_ina    = getParameterValue(SA_SYNTH_PARAM_RES1_IN_A);
    sat_param_t r1_inn    = getParameterValue(SA_SYNTH_PARAM_RES1_IN_N);
    sat_sample_t r1_in    = (r1_in1 * MOsc1_out) + (r1_in2 * MOsc2_out) + (r1_in3 * MRes1_out) + (r1_in4 * MRes2_out) + (r1_ina * audio) + (r1_inn * rnd);
    r1_in *= r1_amt;

    sat_param_t r1_type   = getModulatedValue(SA_SYNTH_PARAM_RES1_TYPE);
    MRes1.setType(r1_type);

    // tuning
    sat_param_t r1_oct    = getModulatedValue(SA_SYNTH_PARAM_RES1_OCT);
    sat_param_t r1_semi   = getModulatedValue(SA_SYNTH_PARAM_RES1_SEMI);
    sat_param_t r1_cent   = getModulatedValue(SA_SYNTH_PARAM_RES1_CENT);
    sat_param_t r1_tuning = (r1_oct * 12.0) + r1_semi + r1_cent;
    r1_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];;


    //double offset = 0.0;

    // pre modulation
    // sat_param_t mod = 0.0;
    // uint32_t r1_type = getParameterValue(SA_SYNTH_PARAM_RES1_TYPE);
    // switch (r1_type) {
    // }



    // process
    sat_param_t hz1 = SAT_NoteToHz(MKey + r1_tuning);
    if (hz1 < 20.0) hz1 = 20.0;
    MRes1.setFrequency(hz1);

    sat_sample_t res1 = MRes1.process(r1_in/*,offset*/);

    // post modulation
    // switch (r1_type) {
    // }

    // out
    res1 = SAT_Clamp(res1,-1,1);
    return res1;

  }

  //----------

  sat_sample_t process_res2() {

    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_sample_t audio  = 0.0;
    sat_sample_t rnd    = SAT_RandomRange(-1.0,1.0);

    sat_param_t r2_imp_freq   = getModulatedValue(SA_SYNTH_PARAM_RES2_IMPULSE_FREQ);
    sat_param_t r2_fb         = getModulatedValue(SA_SYNTH_PARAM_RES2_FEEDBACK);
    sat_param_t r2_damp_freq  = getModulatedValue(SA_SYNTH_PARAM_RES2_DAMPING_FREQ);
    sat_param_t r2_gain       = getModulatedValue(SA_SYNTH_PARAM_RES2_GAIN);
    MRes2.setImpulse(r2_imp_freq);
    MRes2.setFeedback(r2_fb);
    MRes2.setDamping(r2_damp_freq);
    MRes2.setGain(r2_gain);

    // inputs
    sat_param_t  r2_amt   = getModulatedValue(SA_SYNTH_PARAM_RES2_IN_AMOUNT);
    sat_param_t  r2_in1   = getParameterValue(SA_SYNTH_PARAM_RES2_IN_O1);
    sat_param_t  r2_in2   = getParameterValue(SA_SYNTH_PARAM_RES2_IN_O2);
    sat_param_t  r2_in3   = getParameterValue(SA_SYNTH_PARAM_RES2_IN_R1);
    sat_param_t  r2_in4   = getParameterValue(SA_SYNTH_PARAM_RES2_IN_R2);
    sat_param_t  r2_ina   = getParameterValue(SA_SYNTH_PARAM_RES2_IN_A);
    sat_param_t  r2_inn   = getParameterValue(SA_SYNTH_PARAM_RES2_IN_N);
    sat_sample_t r2_in    = (r2_in1 * MOsc1_out) + (r2_in2 * MOsc2_out) + (r2_in3 * MRes1_out) + (r2_in4 * MRes2_out) + (r2_ina * audio) + (r2_inn * rnd);
    r2_in *= r2_amt;

    sat_param_t r2_type   = getModulatedValue(SA_SYNTH_PARAM_RES2_TYPE);
    MRes2.setType(r2_type);

    // tuning
    sat_param_t r2_oct    = getModulatedValue(SA_SYNTH_PARAM_RES2_OCT);
    sat_param_t r2_semi   = getModulatedValue(SA_SYNTH_PARAM_RES2_SEMI);
    sat_param_t r2_cent   = getModulatedValue(SA_SYNTH_PARAM_RES2_CENT);
    sat_param_t r2_tuning = (r2_oct * 12.0) + r2_semi + r2_cent;
    r2_tuning += MVoiceExpr[CLAP_NOTE_EXPRESSION_TUNING];;


    //double offset = 0.0;

    // pre modulation
    // sat_param_t mod = 0.0;
    // uint32_t r2_type = getParameterValue(SA_SYNTH_PARAM_RES2_TYPE);
    // switch (r2_type) {
    // }

    // process
    sat_param_t hz2 = SAT_NoteToHz(MKey + r2_tuning);
    if (hz2 < 20.0) hz2 = 20.0;
    MRes2.setFrequency(hz2);
    sat_sample_t res2 = MRes2.process(r2_in/*,offset*/);

    // post modulation
    // switch (r2_type) {
    // }

    // out
    res2 = SAT_Clamp(res2,-1,1);
    return res2;

  }

  //----------

  sat_sample_t process_flt1(sat_sample_t spl) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t type = getParameterValue(SA_SYNTH_PARAM_FLT1_TYPE);
    sat_param_t freq = getModulatedValue(SA_SYNTH_PARAM_FLT1_FREQ);
    sat_param_t q    = getModulatedValue(SA_SYNTH_PARAM_FLT1_Q);
    sat_param_t gain = getModulatedValue(SA_SYNTH_PARAM_FLT1_GAIN);


    if (type != 0) {
      freq = (freq * freq * freq);
      q = (q * q * q);

//      freq += MVoiceMod[CLAP_NOTE_EXPRESSION_PRESSURE];

      MFlt1.set(type,freq,q,gain);
      MFlt1.update_coeffs();
      return MFlt1.process(spl);
    }
    else {
      return spl;
    }
  }

  //----------

  sat_sample_t process_env1() {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t a = getParameterValue(SA_SYNTH_PARAM_ENV1_ATT);
    sat_param_t d = getParameterValue(SA_SYNTH_PARAM_ENV1_DEC);
    sat_param_t s = getParameterValue(SA_SYNTH_PARAM_ENV1_SUS);
    sat_param_t r = getParameterValue(SA_SYNTH_PARAM_ENV1_REL);
    MEnv1.setADSR(a,d,s,r);
    sat_sample_t env = MEnv1.process();
    return env;
  }

};

//----------------------------------------------------------------------
#endif