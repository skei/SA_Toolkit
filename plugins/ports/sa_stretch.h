#ifndef sa_stretch_included
#define sa_stretch_included
//----------------------------------------------------------------------

// signalsmith stretch (see /extern/sigmalsmith)
// https://github.com/Signalsmith-Audio/signalsmith-stretch

// conflicts with our memtracer (disable it)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"

// gcc complained about Complex
#define prev_complex Complex
#undef Complex
#include "extern/signalsmith/signalsmith-stretch.h"
#define Complex prev_complex
#undef prev_complex

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_stretch_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_stretch/v0",
  .name         = "sa_stretch",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_PITCH_SHIFTER, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_stretch_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  //float samplerate = 0.0;
  signalsmith::stretch::SignalsmithStretch<float> MStretch = {};
  
//------------------------------
public:
//------------------------------

  sa_stretch_processor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~sa_stretch_processor() {
  }

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    MStretch.presetDefault(2,ASampleRate);
    MStretch.reset();
  }

  //----------

  void reset() {
    MStretch.reset();
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    double  v  = event->value;
    int32_t iv = SAT_Trunc(event->value);
    switch (event->param_id) {
      case 0: MStretch.setTransposeFactor(v);     break;    // 2 = octave up
      case 1: MStretch.setTransposeSemitones(iv); break;    // 12 semitones
    }
  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    const clap_process_t* process = AContext->process;
    float** inputs  = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MStretch.process(inputs,length,outputs,length);
  }

};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_stretch_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_stretch_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_stretch_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_stretch_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;
    appendParameter( new SAT_Parameter(   "Factor", "", 1,   0,  8, flags ));
    appendParameter( new SAT_Parameter("Semitones", "", 0, -24, 24, flags ));
    MProcessor = new sa_stretch_processor(this);
    setProcessor(MProcessor);
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  bool start_processing() final {
    MProcessor->reset();
    return SAT_Plugin::start_processing();
  }

};

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_stretch_descriptor,sa_stretch_plugin)
#endif

//----------------------------------------------------------------------
#endif

