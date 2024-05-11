#ifndef sa_botage_included
#define sa_botage_included
//----------------------------------------------------------------------

#include "plugin/sat_parameters.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

const char* txt_eight[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };

#define NUM_FX_TYPES 3
const char* fx_type_text[NUM_FX_TYPES] = {
  "Filter",
  "Reverb",
  "Distortion"
};

//----------

#define PLUGIN_NAME   "sa_botage"
#define PLUGIN_DESC   "sabotage your loops!"
#define PLUGIN_VER    "0.9.0"

#define EDITOR_WIDTH  840
#define EDITOR_HEIGHT 510
#define EDITOR_SCALE  1.5

//----------

#include "sa_botage/sa_botage_parameters.h"
#include "sa_botage/sa_botage_widgets.h"
#include "sa_botage/sa_botage_prob_page.h"
#include "sa_botage/sa_botage_seq_page.h"
#include "sa_botage/sa_botage_perf_page.h"

#include "sa_botage/sa_botage_processor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_botage_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = PLUGIN_VER,
  .description  = PLUGIN_DESC,
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_botage_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_botage_processor   MProcessor = {};

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_botage_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE,true);
    if (!sa_botage_SetupParameters(this)) return false;
    return SAT_Plugin::init();
  }

  //----------
  
  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor.activate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    MProcessor.process(AContext);
  }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    uint32_t index = event->param_id;
    double value = event->value;
    MProcessor.setParamValue(index,value);
    return true;
  }

  bool on_plugin_transport(const clap_event_transport_t* event) final {
    uint32_t flags = event->flags;
    MProcessor.transport(flags);
    return true;
  }

  //----------
  
  #include "sa_botage/sa_botage_gui.h"

  //----------

  void on_editorListener_timer(SAT_Timer* ATimer, double AElapsed) final {
    SAT_Plugin::on_editorListener_timer(ATimer,AElapsed);
    updateProbIndicators(&MProcessor);
    updateWaveformWidget(&MProcessor);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_botage_descriptor,sa_botage_plugin)
  
#endif

//----------------------------------------------------------------------

#undef PLUGIN_NAME
#undef PLUGIN_DESC
#undef PLUGIN_VER
#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT
#undef EDITOR_SCALE

//----------------------------------------------------------------------
#endif

