
#include "sat.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_block_processor.h"

//----------------------------------------------------------------------
// descriptor
//----------------------------------------------------------------------

clap_plugin_descriptor_t gain_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "gain v1 blablabla",
  .name         = "gain",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "this is a massively cool plugin!",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
// processor
//----------------------------------------------------------------------

class gain_processor
: public SAT_BlockProcessor {

public:

  //SAT_DEFAULT_PROCESSOR_CONSTRUCTOR(gain_processor)

  gain_processor(SAT_ProcessorOwner* AOwner)
  : SAT_BlockProcessor(AOwner) {
  }

  virtual ~gain_processor() {
  }

public:

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    double gain = getParameterValue(0);
    *spl0 *= gain;
    *spl1 *= gain;
  }

};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class gain
: public SAT_Plugin {

private:

  gain_processor* MProcessor = nullptr;

public:

  //SAT_DEFAULT_PLUGIN_CONSTRUCTOR(gain)

  gain(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  virtual ~gain() {
    if (MProcessor) deleteProcessor(MProcessor);
  }

public:

  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    MProcessor = new gain_processor(this);
    setProcessor(MProcessor);
    appendParameter( new SAT_Parameter( "Gain", "", 1.0, 0,1, CLAP_PARAM_IS_AUTOMATABLE ));
    return SAT_Plugin::init();
  }

  // bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //   MProcessor->setSampleRate(sample_rate);
  //   return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  // }

};

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(gain_descriptor,gain);
