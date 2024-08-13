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

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
//#include "audio/sat_audio_utils.h"

// #include "sat.h"
// #include "audio/sat_audio_math.h"
// #include "audio/sat_audio_utils.h"
#include "plugin/sat_parameters.h"
// #include "plugin/sat_plugin.h"

#define phaserlfoshape  4.0
#define lfoskipsamples  20

//----------------------------------------------------------------------
//
// phaser
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
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_phaser_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_phaser/v0",
  .name         = "sa_phaser",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_PHASER, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_phaser_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;

  SAT_Phaser    MPhaserL = {};
  SAT_Phaser    MPhaserR = {};
  
//------------------------------
public:
//------------------------------

  sa_phaser_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
  }

  //----------

  virtual ~sa_phaser_processor() {
  }

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    samplerate = ASampleRate;
    MPhaserL.init(ASampleRate);
    MPhaserR.init(ASampleRate);
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    //need_recalc = true;
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
  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    const clap_process_t* process = AContext->process;
    if (need_recalc) recalc(samplerate);
    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;

      spl0 = MPhaserL.process(spl0);
      spl1 = MPhaserR.process(spl1);

      *output0++ = spl0;
      *output1++ = spl1;
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
// plugin
//
//----------------------------------------------------------------------

class sa_phaser_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_phaser_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_phaser_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_phaser_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter( new SAT_Parameter(   "LFO Freq",       "", 0.4,  0.05, 5.0, flags ));
    appendParameter( new SAT_Parameter(   "LFO StartPhase", "", 0,    0,    256, flags ));
    appendParameter( new SAT_Parameter(   "Feedback",       "", 0,    0,    1,   flags ));
    appendParameter( new SAT_Parameter(   "Depth",          "", 100,  0,    200, flags ));
    appendParameter( new SAT_IntParameter("Stages",         "", 2,    0,    10,  flags ));
    appendParameter( new SAT_Parameter(   "Dry / Wet",      "", 128,  0,    256, flags ));
    MProcessor = new sa_phaser_processor(this);
    setProcessor(MProcessor);
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

};

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_phaser_descriptor,sa_phaser_plugin)
#endif

#undef phaserlfoshape
#undef lfoskipsamples

//----------------------------------------------------------------------
#endif













