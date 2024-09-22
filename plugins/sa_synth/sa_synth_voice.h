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

  sat_sample_t        MOsc1_z1      = 0.0;
  sat_sample_t        MOsc2_z1      = 0.0;
  sat_sample_t        MRes1_z1      = 0.0;
  sat_sample_t        MRes2_z1      = 0.0;



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
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return MEnv1.getValue();
  }

  //----------

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    MKey = AIndex;
    MVelocity = AValue;
    SAT_ParameterArray* params = MContext->process_context->parameters;
    init_osc1(AIndex,AValue);
    init_osc2(AIndex,AValue);
    init_res1(AIndex,AValue);
    init_res2(AIndex,AValue);
    init_flt1(AIndex,AValue);
    init_env1(AIndex,AValue);
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

  void noteExpression(uint32_t AIndex, sat_param_t AValue) {
    MOsc1.noteExpression(AIndex,AValue);
    MOsc2.noteExpression(AIndex,AValue);
    MRes1.noteExpression(AIndex,AValue);
    MRes2.noteExpression(AIndex,AValue);
    MFlt1.noteExpression(AIndex,AValue);
    MEnv1.noteExpression(AIndex,AValue);
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
        sat_sample_t out = 0.0;
        sat_sample_t osc1 = process_osc1();
        sat_sample_t osc2 = process_osc2();
        sat_sample_t res1 = process_res1();
        sat_sample_t res2 = process_res2();
        MOsc1_z1 = osc1;
        MOsc2_z1 = osc2;
        MRes1_z1 = res1;
        MRes2_z1 = res2;
        SAT_ParameterArray* params = MContext->process_context->parameters;
        SAT_Assert(params);
        sat_param_t o1_mix = params->getItem(SA_SYNTH_PARAM_MIX_O1)->getValue();
        sat_param_t o2_mix = params->getItem(SA_SYNTH_PARAM_MIX_O2)->getValue();
        sat_param_t r1_mix = params->getItem(SA_SYNTH_PARAM_MIX_R1)->getValue();
        sat_param_t r2_mix = params->getItem(SA_SYNTH_PARAM_MIX_R2)->getValue();
        out = (osc1 * o1_mix) + (osc2 * o2_mix) + (res1 * r1_mix) + (res2 * r2_mix);
        out = process_flt1(out);
        out *= process_env1();
        *buffer++ = out;
        update_tuning();
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
private:
//------------------------------

  void init_osc1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_param_t o1_squ = params->getItem(SA_SYNTH_PARAM_OSC1_SQU)->getValue();
    sat_param_t o1_tri = params->getItem(SA_SYNTH_PARAM_OSC1_TRI)->getValue();
    sat_param_t o1_sin = params->getItem(SA_SYNTH_PARAM_OSC1_SIN)->getValue();
    sat_param_t o1_width = params->getItem(SA_SYNTH_PARAM_OSC1_WIDTH)->getValue();
    MOsc1.setPulseWidth(o1_width);
    MOsc1.setSawSqu(o1_squ);
    MOsc1.setSquTri(o1_tri);
    MOsc1.setTriSin(o1_sin);

    sat_param_t o1_oct = params->getItem(SA_SYNTH_PARAM_OSC1_OCT)->getValue();
    sat_param_t o1_semi = params->getItem(SA_SYNTH_PARAM_OSC1_SEMI)->getValue();
    sat_param_t o1_cent = params->getItem(SA_SYNTH_PARAM_OSC1_CENT)->getValue();
    sat_param_t o1_tuning = (o1_oct * 12.0) + o1_semi + o1_cent;
    // o1_tuning += e_tuning;
    sat_param_t hz1 = SAT_NoteToHz(MKey + o1_tuning);
    MOsc1.setFrequency(hz1);

    //MOsc1.reset();
    MOsc1.noteOn(AIndex,AValue);

  }

  //----------

  void init_osc2(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_param_t o2_squ   = params->getItem(SA_SYNTH_PARAM_OSC2_SQU)->getValue();
    sat_param_t o2_tri   = params->getItem(SA_SYNTH_PARAM_OSC2_TRI)->getValue();
    sat_param_t o2_sin   = params->getItem(SA_SYNTH_PARAM_OSC2_SIN)->getValue();
    sat_param_t o2_width = params->getItem(SA_SYNTH_PARAM_OSC2_WIDTH)->getValue();
    MOsc2.setPulseWidth(o2_width);
    MOsc2.setSawSqu(o2_squ);
    MOsc2.setSquTri(o2_tri);
    MOsc2.setTriSin(o2_sin);

    sat_param_t o2_oct = params->getItem(SA_SYNTH_PARAM_OSC2_OCT)->getValue();
    sat_param_t o2_semi = params->getItem(SA_SYNTH_PARAM_OSC2_SEMI)->getValue();
    sat_param_t o2_cent = params->getItem(SA_SYNTH_PARAM_OSC2_CENT)->getValue();
    sat_param_t o2_tuning = (o2_oct * 12.0) + o2_semi + o2_cent;
    // o2_tuning += e_tuning;
    sat_param_t hz2 = SAT_NoteToHz(MKey + o2_tuning);
    MOsc2.setFrequency(hz2);

    //MOsc1.reset();
    MOsc2.noteOn(AIndex,AValue);
  }

  //----------

  void init_res1(uint32_t AIndex, double AValue) {
  }

  //----------

  void init_res2(uint32_t AIndex, double AValue) {
  }

  //----------

  void init_flt1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t type = params->getItem(SA_SYNTH_PARAM_FLT1_TYPE)->getValue();
    sat_param_t freq = params->getItem(SA_SYNTH_PARAM_FLT1_FREQ)->getValue();
    sat_param_t q    = params->getItem(SA_SYNTH_PARAM_FLT1_Q)->getValue();
    sat_param_t gain = params->getItem(SA_SYNTH_PARAM_FLT1_GAIN)->getValue();
    MFlt1.set(type,freq,q,gain);
    MFlt1.update_coeffs();
    MFlt1.reset();
  }

  //----------

  void init_env1(uint32_t AIndex, double AValue) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t a = params->getItem(SA_SYNTH_PARAM_ENV1_ATT)->getValue();
    sat_param_t d = params->getItem(SA_SYNTH_PARAM_ENV1_DEC)->getValue();
    sat_param_t s = params->getItem(SA_SYNTH_PARAM_ENV1_SUS)->getValue();
    sat_param_t r = params->getItem(SA_SYNTH_PARAM_ENV1_REL)->getValue();
    MEnv1.setADSR(a,d,s,r);
    MEnv1.noteOn();
  }

//------------------------------
private:
//------------------------------

  sat_sample_t process_osc1() {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t o1_squ = params->getItem(SA_SYNTH_PARAM_OSC1_SQU)->getValue();
    sat_param_t o1_tri = params->getItem(SA_SYNTH_PARAM_OSC1_TRI)->getValue();
    sat_param_t o1_sin = params->getItem(SA_SYNTH_PARAM_OSC1_SIN)->getValue();
    sat_param_t o1_width = params->getItem(SA_SYNTH_PARAM_OSC1_WIDTH)->getValue();
    MOsc1.setPulseWidth(o1_width);
    MOsc1.setSawSqu(o1_squ);
    MOsc1.setSquTri(o1_tri);
    MOsc1.setTriSin(o1_sin);

    sat_param_t o1_amt  = params->getItem(SA_SYNTH_PARAM_OSC1_IN_AMOUNT)->getValue();
    sat_param_t o1_in1  = params->getItem(SA_SYNTH_PARAM_OSC1_IN_O1)->getValue();
    sat_param_t o1_in2  = params->getItem(SA_SYNTH_PARAM_OSC1_IN_O2)->getValue();
    sat_param_t o1_in3  = params->getItem(SA_SYNTH_PARAM_OSC1_IN_R1)->getValue();
    sat_param_t o1_in4  = params->getItem(SA_SYNTH_PARAM_OSC1_IN_R2)->getValue();
    sat_sample_t o1_in  = (o1_in1 * MOsc1_z1) + (o1_in2 * MOsc2_z1) + (o1_in3 * MRes1_z1) + (o1_in4 * MRes2_z1);

    sat_param_t o1_oct = params->getItem(SA_SYNTH_PARAM_OSC1_OCT)->getValue();
    sat_param_t o1_semi = params->getItem(SA_SYNTH_PARAM_OSC1_SEMI)->getValue();
    sat_param_t o1_cent = params->getItem(SA_SYNTH_PARAM_OSC1_CENT)->getValue();
    sat_param_t o1_tuning = (o1_oct * 12.0) + o1_semi + o1_cent;
    // o1_tuning += expr_tuning;

    sat_param_t mod = 0.0;
    uint32_t o1_type = params->getItem(SA_SYNTH_PARAM_OSC1_TYPE)->getValue();
    switch (o1_type) {
      case SA_SYNTH_OSC_PHASE_MOD: {
        mod = ((o1_in * 4.0) * o1_amt);
        o1_tuning += mod;
        break;
      }
    }

    sat_param_t hz1 = SAT_NoteToHz(MKey + o1_tuning);
    MOsc1.setFrequency(hz1);
    sat_sample_t osc1 = MOsc1.process_mod(0.0,mod);

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
    }

    osc1 = SAT_Clamp(osc1,-1,1);
    return osc1;
  }

  //----------

  sat_sample_t process_osc2() {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t o2_squ    = params->getItem(SA_SYNTH_PARAM_OSC2_SQU)->getValue();
    sat_param_t o2_tri    = params->getItem(SA_SYNTH_PARAM_OSC2_TRI)->getValue();
    sat_param_t o2_sin    = params->getItem(SA_SYNTH_PARAM_OSC2_SIN)->getValue();
    sat_param_t o2_width  = params->getItem(SA_SYNTH_PARAM_OSC2_WIDTH)->getValue();
    MOsc2.setPulseWidth(o2_width);
    MOsc2.setSawSqu(o2_squ);
    MOsc2.setSquTri(o2_tri);
    MOsc2.setTriSin(o2_sin);

    sat_param_t o2_amt    = params->getItem(SA_SYNTH_PARAM_OSC2_IN_AMOUNT)->getValue();
    sat_param_t o2_in1    = params->getItem(SA_SYNTH_PARAM_OSC2_IN_O1)->getValue();
    sat_param_t o2_in2    = params->getItem(SA_SYNTH_PARAM_OSC2_IN_O2)->getValue();
    sat_param_t o2_in3    = params->getItem(SA_SYNTH_PARAM_OSC2_IN_R1)->getValue();
    sat_param_t o2_in4    = params->getItem(SA_SYNTH_PARAM_OSC2_IN_R2)->getValue();
    sat_sample_t o2_in    = (o2_in1 * MOsc1_z1) + (o2_in2 * MOsc2_z1) + (o2_in3 * MRes1_z1) + (o2_in4 * MRes2_z1);

    sat_param_t o2_oct    = params->getItem(SA_SYNTH_PARAM_OSC2_OCT)->getValue();
    sat_param_t o2_semi   = params->getItem(SA_SYNTH_PARAM_OSC2_SEMI)->getValue();
    sat_param_t o2_cent   = params->getItem(SA_SYNTH_PARAM_OSC2_CENT)->getValue();
    sat_param_t o2_tuning = (o2_oct * 12.0) + o2_semi + o2_cent;
    // o2_tuning += e_tuning;

    sat_param_t mod = 0.0;
    uint32_t o2_type = params->getItem(SA_SYNTH_PARAM_OSC2_TYPE)->getValue();
    switch (o2_type) {
      case SA_SYNTH_OSC_PHASE_MOD: {
        mod = ((o2_in * 4.0) * o2_amt);
        o2_tuning += mod;
        break;
      }
    }

    sat_param_t hz2 = SAT_NoteToHz(MKey + o2_tuning);
    MOsc2.setFrequency(hz2);
    sat_sample_t osc2 = MOsc2.process_mod(0.0,mod);

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
    }

    osc2 = SAT_Clamp(osc2,-1,1);
    return osc2;
  }

  //----------

  sat_sample_t process_res1() {

    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_param_t r1_imp    = params->getItem(SA_SYNTH_PARAM_RES1_IMPULSE)->getValue();
    sat_param_t r1_shp    = params->getItem(SA_SYNTH_PARAM_RES1_SHAPE)->getValue();
    sat_param_t r1_fb     = params->getItem(SA_SYNTH_PARAM_RES1_FEEDBACK)->getValue();
    sat_param_t r1_damp   = params->getItem(SA_SYNTH_PARAM_RES1_DAMPING)->getValue();
    // MRes1.setImpulse(r1_imp);
    // MRes1.setShape(r1_shp);
    // MRes1.setFeedback(r1_fb);
    // MRes1.setDamping(r1_damp);

    sat_param_t r1_amt    = params->getItem(SA_SYNTH_PARAM_RES1_IN_AMOUNT)->getValue();
    sat_param_t r1_in1    = params->getItem(SA_SYNTH_PARAM_RES1_IN_O1)->getValue();
    sat_param_t r1_in2    = params->getItem(SA_SYNTH_PARAM_RES1_IN_O2)->getValue();
    sat_param_t r1_in3    = params->getItem(SA_SYNTH_PARAM_RES1_IN_R1)->getValue();
    sat_param_t r1_in4    = params->getItem(SA_SYNTH_PARAM_RES1_IN_R2)->getValue();
    sat_sample_t r1_in    = (r1_in1 * MOsc1_z1) + (r1_in2 * MOsc2_z1) + (r1_in3 * MRes1_z1) + (r1_in4 * MRes2_z1);

    sat_param_t r1_oct    = params->getItem(SA_SYNTH_PARAM_RES1_OCT)->getValue();
    sat_param_t r1_semi   = params->getItem(SA_SYNTH_PARAM_RES1_SEMI)->getValue();
    sat_param_t r1_cent   = params->getItem(SA_SYNTH_PARAM_RES1_CENT)->getValue();
    sat_param_t r1_tuning = (r1_oct * 12.0) + r1_semi + r1_cent;
    // r1_tuning += e_tuning;

    double offset = 0.0;

    // sat_param_t mod = 0.0;
    // uint32_t r1_type = params->getItem(SA_SYNTH_PARAM_RES1_TYPE)->getValue();
    // switch (r1_type) {
    // }

    sat_param_t hz1 = SAT_NoteToHz(MKey + r1_tuning);
    MRes1.setFrequency(hz1);
    sat_sample_t res1 = MRes1.process(r1_in,r1_fb,offset);

    // switch (r1_type) {
    // }

    res1 = SAT_Clamp(res1,-1,1);
    return res1;

  }

  //----------

  sat_sample_t process_res2() {

    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);

    sat_param_t r2_imp    = params->getItem(SA_SYNTH_PARAM_RES2_IMPULSE)->getValue();
    sat_param_t r2_shp    = params->getItem(SA_SYNTH_PARAM_RES2_SHAPE)->getValue();
    sat_param_t r2_fb     = params->getItem(SA_SYNTH_PARAM_RES2_FEEDBACK)->getValue();
    sat_param_t r2_damp   = params->getItem(SA_SYNTH_PARAM_RES2_DAMPING)->getValue();
    // MRes2.setImpulse(r2_imp);
    // MRes2.setShape(r2_shp);
    // MRes2.setFeedback(r2_fb);
    // MRes2.setDamping(r2_damp);

    sat_param_t  r2_amt   = params->getItem(SA_SYNTH_PARAM_RES2_IN_AMOUNT)->getValue();
    sat_param_t  r2_in1   = params->getItem(SA_SYNTH_PARAM_RES2_IN_O1)->getValue();
    sat_param_t  r2_in2   = params->getItem(SA_SYNTH_PARAM_RES2_IN_O2)->getValue();
    sat_param_t  r2_in3   = params->getItem(SA_SYNTH_PARAM_RES2_IN_R1)->getValue();
    sat_param_t  r2_in4   = params->getItem(SA_SYNTH_PARAM_RES2_IN_R2)->getValue();
    sat_sample_t r2_in    = (r2_in1 * MOsc1_z1) + (r2_in2 * MOsc2_z1) + (r2_in3 * MRes1_z1) + (r2_in4 * MRes2_z1);

    sat_param_t r2_oct    = params->getItem(SA_SYNTH_PARAM_RES2_OCT)->getValue();
    sat_param_t r2_semi   = params->getItem(SA_SYNTH_PARAM_RES2_SEMI)->getValue();
    sat_param_t r2_cent   = params->getItem(SA_SYNTH_PARAM_RES2_CENT)->getValue();
    sat_param_t r2_tuning = (r2_oct * 12.0) + r2_semi + r2_cent;
    // r2_tuning += e_tuning;

    double offset = 0.0;

    // sat_param_t mod = 0.0;
    // uint32_t r2_type = params->getItem(SA_SYNTH_PARAM_RES2_TYPE)->getValue();
    // switch (r2_type) {
    // }

    sat_param_t hz2 = SAT_NoteToHz(MKey + r2_tuning);
    MRes2.setFrequency(hz2);
    sat_sample_t res2 = MRes2.process(r2_in,r2_fb,offset);

    // switch (r2_type) {
    // }

    res2 = SAT_Clamp(res2,-1,1);
    return res2;

  }

  //----------

  sat_sample_t process_flt1(sat_sample_t spl) {
    SAT_ParameterArray* params = MContext->process_context->parameters;
    SAT_Assert(params);
    sat_param_t type = params->getItem(SA_SYNTH_PARAM_FLT1_TYPE)->getValue();
    sat_param_t freq = params->getItem(SA_SYNTH_PARAM_FLT1_FREQ)->getValue();
    sat_param_t q    = params->getItem(SA_SYNTH_PARAM_FLT1_Q)->getValue();
    sat_param_t gain = params->getItem(SA_SYNTH_PARAM_FLT1_GAIN)->getValue();
    if (type != 0) {
      freq = (freq * freq * freq);
      q = (q * q * q);
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
    sat_param_t a = params->getItem(SA_SYNTH_PARAM_ENV1_ATT)->getValue();
    sat_param_t d = params->getItem(SA_SYNTH_PARAM_ENV1_DEC)->getValue();
    sat_param_t s = params->getItem(SA_SYNTH_PARAM_ENV1_SUS)->getValue();
    sat_param_t r = params->getItem(SA_SYNTH_PARAM_ENV1_REL)->getValue();
    MEnv1.setADSR(a,d,s,r);
    sat_sample_t env = MEnv1.process();
    return env;
  }

};

//----------------------------------------------------------------------
#endif