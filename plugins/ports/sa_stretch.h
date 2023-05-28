#ifndef sa_stretch_included
#define sa_stretch_included
//----------------------------------------------------------------------

// signalsmith stretch (see /extern/sigmalsmith)
// https://github.com/Signalsmith-Audio/signalsmith-stretch

#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

// gcc complained, so i had to add this
#undef Complex

#include "extern/signalsmith/signalsmith-stretch.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define PLUGIN_NAME "sa_stretch"

//----------

const clap_plugin_descriptor_t sa_stretch_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "signalsmith stretch",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_PITCH_SHIFTER,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_stretch_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  signalsmith::stretch::SignalsmithStretch<float> MStretch = {};

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_stretch_plugin)

  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoAudioOutputPort();
    appendParameter( new SAT_Parameter(   "Factor", 1,   0,  8 ));
    appendParameter( new SAT_Parameter("Semitones", 0, -24, 24 ));
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  
  //----------
  
  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MStretch.presetDefault(2,sample_rate);
    //MStretch.presetCheaper(2,sample_rate);
    //MStretch.configure(channels,blockSamples,intervalSamples);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }
  
  //----------
  
  bool start_processing() override {
    MStretch.reset();
    return SAT_Plugin::start_processing();
  }
  
  //----------
  
  bool handleParamValue(const clap_event_param_value_t* event) {
    double  v  = event->value;
    int32_t iv = SAT_Trunc(event->value);
    switch (event->param_id) {
      case 0: MStretch.setTransposeFactor(v);    return true;  // 2 = octave up
      case 1: MStretch.setTransposeSemitones(iv); return true;  // 12 semitones
    }
    return false;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    float** inputs = AContext->process->audio_inputs[0].data32;
    float** outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;
    //SAT_CopyStereoBuffer(outputs,inputs,length);
    MStretch.process(inputs,length,outputs,length);
  }
  
  //----------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_stretch_descriptor,sa_stretch_plugin)
  
#endif

//----------

#undef PLUGIN_NAME

//----------------------------------------------------------------------
#endif

