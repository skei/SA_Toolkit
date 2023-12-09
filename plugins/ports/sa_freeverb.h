#ifndef sa_freeverb_included
#define sa_freeverb_included
//----------------------------------------------------------------------

// based on:
// https://github.com/Themaister/RetroArch-DSP-plugins/blob/master/freeverb.cpp

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
//#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"

const char* mode_txt[] = { "Normal", "Freeze" };

const int	  sa_freeverb_numcombs        = 8;
const int	  sa_freeverb_numallpasses    = 4;

const float	sa_freeverb_muted           = 0.0;
const float	sa_freeverb_fixedgain       = 0.015;

const float	sa_freeverb_scalewet        = 3.0;
const float	sa_freeverb_scaledry        = 2.0;
const float	sa_freeverb_scaledamp       = 0.4;
const float	sa_freeverb_scaleroom       = 0.28;

const float	sa_freeverb_offsetroom      = 0.7;
const float	sa_freeverb_initialroom     = 0.5;
const float	sa_freeverb_initialdamp     = 0.5;
const float	sa_freeverb_initialwet      = (1.0 / sa_freeverb_scalewet);
const float	sa_freeverb_initialdry      = 0.0;
const float	sa_freeverb_initialwidth    = 1.0;
const float	sa_freeverb_initialmode     = 0.0;
const float	sa_freeverb_freezemode      = 0.5;

const int   sa_freeverb_combtuningL1    = 1116;
const int   sa_freeverb_combtuningL2    = 1188;
const int   sa_freeverb_combtuningL3    = 1277;
const int   sa_freeverb_combtuningL4    = 1356;
const int   sa_freeverb_combtuningL5    = 1422;
const int   sa_freeverb_combtuningL6    = 1491;
const int   sa_freeverb_combtuningL7    = 1557;
const int   sa_freeverb_combtuningL8    = 1617;

const int   sa_freeverb_allpasstuningL1 = 556;
const int   sa_freeverb_allpasstuningL2 = 441;
const int   sa_freeverb_allpasstuningL3 = 341;
const int   sa_freeverb_allpasstuningL4 = 225;

/*
// FIXME: Fix this really ugly hack
inline float undenormalise(void *sample) {
  if (((*(unsigned int*)sample) &  0x7f800000) == 0)
    return 0.0f;
  return *(float*)sample;
}
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_CombFilter {

//------------------------------
private:
//------------------------------

  float   feedback    = 0.0;
  float   filterstore = 0.0;
  float   damp1       = 0.0;
  float   damp2       = 0.0;
  float*  buffer      = nullptr;
  int     bufsize     = 0;
  int     bufidx      = 0;

//------------------------------
public:
//------------------------------

  SAT_CombFilter() {
    //filterstore = 0;
    //bufidx = 0;
  }

  //----------

  void setbuffer(float *buf, int size) {
    buffer = buf;
    bufsize = size;
  }

  //----------

  inline float process(float input) {
    float output;
    output = buffer[bufidx];
    //undenormalise(&output);
    output = SAT_KillDenormal2(output);
    filterstore = (output * damp2) + (filterstore * damp1);
    //undenormalise(&filterstore);
    filterstore = SAT_KillDenormal2(filterstore);
    buffer[bufidx] = input + (filterstore * feedback);
    if (++bufidx >= bufsize) bufidx = 0;
    return output;
  }

  //----------

  void mute() {
    for (int i = 0; i < bufsize; i++) buffer[i] = 0;
  }

  //----------

  void setdamp(float val) {
    damp1 = val;
    damp2 = 1 - val;
  }

  //----------

  float getdamp() {
    return damp1;
  }

  //----------

  void setfeedback(float val) {
    feedback = val;
  }

  //----------

  float getfeedback() {
    return feedback;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_AllpassFilter {

//------------------------------
private:
//------------------------------

  float   feedback  = 0.0;
  float*  buffer    = nullptr;
  int     bufsize   = 0;
  int     bufidx    = 0;

//------------------------------
public:
//------------------------------

  SAT_AllpassFilter() {
    //bufidx = 0;
  }

  //----------

  void setbuffer(float *buf, int size) {
    buffer = buf;
    bufsize = size;
  }

  inline float process(float input) {
    float output;
    float bufout;
    bufout = buffer[bufidx];
    //undenormalise(&bufout);
    bufout = SAT_KillDenormal2(bufout);
    output = -input + bufout;
    buffer[bufidx] = input + (bufout * feedback);
    if (++bufidx >= bufsize) bufidx = 0;
    return output;
  }

  //----------

  void mute() {
    for (int i = 0; i < bufsize; i++)
    buffer[i] = 0;
  }

  //----------

  void setfeedback(float val) {
    feedback = val;
  }

  //----------

  float getfeedback() {
    return feedback;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Freeverb {

//------------------------------
private:
//------------------------------

  float             gain;
  float             roomsize, roomsize1;
  float             damp, damp1;
  float             wet, wet1, wet2;
  float             dry;
  float             width;
  float             mode;

  SAT_CombFilter    combL[sa_freeverb_numcombs];
  SAT_AllpassFilter	allpassL[sa_freeverb_numallpasses];

  float             bufcombL1[sa_freeverb_combtuningL1];
  float             bufcombL2[sa_freeverb_combtuningL2];
  float             bufcombL3[sa_freeverb_combtuningL3];
  float             bufcombL4[sa_freeverb_combtuningL4];
  float             bufcombL5[sa_freeverb_combtuningL5];
  float             bufcombL6[sa_freeverb_combtuningL6];
  float             bufcombL7[sa_freeverb_combtuningL7];
  float             bufcombL8[sa_freeverb_combtuningL8];

  float             bufallpassL1[sa_freeverb_allpasstuningL1];
  float             bufallpassL2[sa_freeverb_allpasstuningL2];
  float             bufallpassL3[sa_freeverb_allpasstuningL3];
  float             bufallpassL4[sa_freeverb_allpasstuningL4];

//------------------------------
public:
//------------------------------

  SAT_Freeverb() {
    combL[0].setbuffer(bufcombL1,sa_freeverb_combtuningL1);
    combL[1].setbuffer(bufcombL2,sa_freeverb_combtuningL2);
    combL[2].setbuffer(bufcombL3,sa_freeverb_combtuningL3);
    combL[3].setbuffer(bufcombL4,sa_freeverb_combtuningL4);
    combL[4].setbuffer(bufcombL5,sa_freeverb_combtuningL5);
    combL[5].setbuffer(bufcombL6,sa_freeverb_combtuningL6);
    combL[6].setbuffer(bufcombL7,sa_freeverb_combtuningL7);
    combL[7].setbuffer(bufcombL8,sa_freeverb_combtuningL8);
    allpassL[0].setbuffer(bufallpassL1,sa_freeverb_allpasstuningL1);
    allpassL[1].setbuffer(bufallpassL2,sa_freeverb_allpasstuningL2);
    allpassL[2].setbuffer(bufallpassL3,sa_freeverb_allpasstuningL3);
    allpassL[3].setbuffer(bufallpassL4,sa_freeverb_allpasstuningL4);
    allpassL[0].setfeedback(0.5f);
    allpassL[1].setfeedback(0.5f);
    allpassL[2].setfeedback(0.5f);
    allpassL[3].setfeedback(0.5f);
    setwet(sa_freeverb_initialwet);
    setroomsize(sa_freeverb_initialroom);
    setdry(sa_freeverb_initialdry);
    setdamp(sa_freeverb_initialdamp);
    setwidth(sa_freeverb_initialwidth);
    setmode(sa_freeverb_initialmode);
    mute();
  }

//------------------------------
private:
//------------------------------

  void update() {
    int i;
    wet1 = wet * (width / 2 + 0.5f);
    if (mode >= sa_freeverb_freezemode) {
      roomsize1 = 1;
      damp1 = 0;
      gain = sa_freeverb_muted;
    } else {
      roomsize1 = roomsize;
      damp1 = damp;
      gain = sa_freeverb_fixedgain;
    }
    for (i = 0; i < sa_freeverb_numcombs; i++) {
      combL[i].setfeedback(roomsize1);
    }
    for (i = 0; i < sa_freeverb_numcombs; i++) {
      combL[i].setdamp(damp1);
    }
  }

//------------------------------
public:
//------------------------------

  void mute() {
    int i;
    if (getmode() >= sa_freeverb_freezemode) return;
    for (i = 0; i < sa_freeverb_numcombs; i++) {
      combL[i].mute();
    }
    for (i = 0; i < sa_freeverb_numallpasses; i++) {
      allpassL[i].mute();
    }
  }

  //----------

  float processsample(float in) {
    float samp = in;
    float mono_out = 0.0f;
    float mono_in = samp;
    float input = (mono_in) * gain;
    for(int i=0; i<sa_freeverb_numcombs; i++) {
      mono_out += combL[i].process(input);
    }
    for(int i=0; i<sa_freeverb_numallpasses; i++) {
      mono_out = allpassL[i].process(mono_out);
    }
    samp = mono_in * dry + mono_out * wet1;
    return samp;
  }

  //----------

  void setroomsize(float value) {
    roomsize = (value * sa_freeverb_scaleroom) + sa_freeverb_offsetroom;
    update();
  }

  //----------

  float getroomsize() {
    return (roomsize - sa_freeverb_offsetroom) / sa_freeverb_scaleroom;
  }

  //----------

  void setdamp(float value) {
    damp = value * sa_freeverb_scaledamp;
    update();
  }

  //----------

  float getdamp() {
    return damp / sa_freeverb_scaledamp;
  }

  //----------

  void setwet(float value) {
    wet = value * sa_freeverb_scalewet;
    update();
  }

  //----------

  float getwet() {
    return wet / sa_freeverb_scalewet;
  }

  //----------

  void setdry(float value) {
    dry = value * sa_freeverb_scaledry;
    //update();
  }

  //----------

  float getdry() {
    return dry / sa_freeverb_scaledry;
  }

  //----------

  void setwidth(float value) {
    width = value;
    update();
  }

  //----------

  float getwidth() {
    return width;
  }

  //----------

  void setmode(float value) {
    mode = value;
    update();
  }

  //----------

  float getmode() {
    if (mode >= sa_freeverb_freezemode) return 1;
    else return 0;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_freeverb_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_freeverb",
  .name         = "sa_freeverb",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_REVERB,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_freeverb_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;
  
  SAT_Freeverb  MReverbL    = {};
  SAT_Freeverb  MReverbR    = {};

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_freeverb_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    
    appendParameter( new SAT_Parameter(     "Roomsize", 0.56, 0.01, 2.0 ));
    appendParameter( new SAT_Parameter(     "Damp",     0.45, 0.01, 2.0 ));
    appendParameter( new SAT_Parameter(     "Wet",      0.57, 0.01, 2.0 ));
    appendParameter( new SAT_Parameter(     "Dry",      0.43, 0.01, 2.0 ));
    appendParameter( new SAT_Parameter(     "Width",    0.56, 0.01, 2.0 ));
    appendParameter( new SAT_TextParameter( "Mode",     0,    0,    1, mode_txt ));
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    //need_recalc = true;
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------
  
  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    uint32_t index = event->param_id;
    double   value = event->value;
    switch (index) {
      case 0: MReverbL.setroomsize(value);  MReverbR.setroomsize(value);  break;
      case 1: MReverbL.setdamp(value);      MReverbR.setdamp(value);      break;
      case 2: MReverbL.setwet(value);       MReverbR.setwet(value);       break;
      case 3: MReverbL.setdry(value);       MReverbR.setdry(value);       break;
      case 4: MReverbL.setwidth(value);     MReverbR.setwidth(value);     break;
      case 5: MReverbL.setmode(value);      MReverbR.setmode(value);      break;
    }
    //need_recalc = true;
    return true;
  }
  
  //----------
  
  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    //if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
    
      spl0 = MReverbL.processsample(spl0);
      spl1 = MReverbR.processsample(spl1);

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  //void recalc(float srate) {
  //  need_recalc = false;
  //}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_freeverb_descriptor,sa_freeverb_plugin)
#endif

//----------------------------------------------------------------------
#endif

