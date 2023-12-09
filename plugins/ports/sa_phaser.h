#ifndef sa_phaser_included
#define sa_phaser_included
//----------------------------------------------------------------------

// based on:
// https://github.com/Themaister/RetroArch-DSP-plugins/blob/master/phaser.cpp

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"

#define phaserlfoshape  4.0
#define lfoskipsamples  20

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Phaser {
private:
  float         freq;
  float         startphase;
  float         fb;
  int           depth;
  int           stages;
  int           drywet;
  unsigned long skipcount;
  float         old[24];
  float         gain;
  float         fbout;
  float         lfoskip;
  float         phase;

public:

  SAT_Phaser() {
  }

  ~SAT_Phaser() {
    for (int j=0; j<stages; j++) old[j] = 0;
  }

public:

  void SetLFOFreq(float val) {
    freq = val;
  }

  void SetLFOStartPhase(float val) {
    startphase = val;
  }

  void SetFeedback(float val) {
    fb = val;
  }

  void SetDepth(float val) {
    depth = val;
  }

  void SetStages(float val) {
    stages = val;
  }

  void SetDryWet(float val) {
    drywet = val;
  }

  void init(float samplerate) {
    skipcount = 0;
    gain      = 0.0;
    fbout     = 0.0;
    lfoskip   = freq * 2.0 * SAT_PI / samplerate;
    phase     = startphase * SAT_PI / 180.0;
    for (int j=0; j<stages; j++) old[j] = 0;
  }

  float process(float in) {
    float m, tmp, out;
    int j;
    m = in + fbout * fb / 100.0;
    if (((skipcount++) % lfoskipsamples) == 0) {
      gain = (1.0 + cosf((float)skipcount * lfoskip + phase)) / 2.0;
      gain = (expf(gain * phaserlfoshape) - 1.0) / (expf(phaserlfoshape) - 1.0);
      gain = 1.0 - gain / 255.0 * (float)depth;
    }
    for (j=0; j<stages; j++) {
      tmp = old[j];
      old[j] = gain * tmp + m;
      m = tmp - gain * old[j];
    }
    fbout = m;
    out = ((m * (float)drywet) + (in * (255.0 - (float)drywet))) / 255.0;
    if (out < -1.0) out = -1.0;
    if (out > 1.0) out = 1.0;
    return out;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_phaser_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_phaser",
  .name         = "sa_phaser",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_PHASER,
                    nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_phaser_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float MSampleRate = 0.0;

  SAT_Phaser    MPhaserL = {};
  SAT_Phaser    MPhaserR = {};

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_phaser_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    
    appendParameter( new SAT_Parameter(   "LFO Freq",       0.4,  0.05, 5.0 ));
    appendParameter( new SAT_Parameter(   "LFO StartPhase", 0,    0,    256 ));
    appendParameter( new SAT_Parameter(   "Feedback",       0,    0,    1 ));
    appendParameter( new SAT_Parameter(   "Depth",          100,  0,    200 ));
    appendParameter( new SAT_IntParameter("Stages",         2,    0,    10  ));
    appendParameter( new SAT_Parameter(   "Dry / Wet",      128,  0,    256 ));
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    //need_recalc = true;
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    MPhaserL.init(sample_rate);
    MPhaserR.init(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    uint32_t index = event->param_id;
    double   value = event->value;
    switch (index) {
      case 0: MPhaserL.SetLFOFreq(value);       MPhaserR.SetLFOFreq(value);       break;
      case 1: MPhaserL.SetLFOStartPhase(value); MPhaserR.SetLFOStartPhase(value); break;
      case 2: MPhaserL.SetFeedback(value);      MPhaserR.SetFeedback(value);      break;
      case 3: MPhaserL.SetDepth(value);         MPhaserR.SetDepth(value);         break;
      case 4: MPhaserL.SetStages(value);        MPhaserR.SetStages(value);        break;
      case 5: MPhaserL.SetDryWet(value);        MPhaserR.SetDryWet(value);        break;
    }
    //MNeedRecalc = true;
    return true;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      
      spl0 = MPhaserL.process(spl0);
      spl1 = MPhaserR.process(spl1);

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_phaser_descriptor,sa_phaser_plugin)
#endif

//----------

#undef phaserlfoshape
#undef lfoskipsamples

//----------------------------------------------------------------------
#endif

