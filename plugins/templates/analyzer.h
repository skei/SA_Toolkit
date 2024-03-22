#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t analyzer_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sat_analyzer",
  .name         = "analyzer",
  .vendor       = "skei.audio",
  .url          = "https://github.com/skei/SA_Toolkit",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_ANALYZER, nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class analyzer
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(analyzer)
 
  //----------

  bool send_event(void* event) {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    out_events->try_push(out_events,(const clap_event_header_t*)event);
    return true;
  }

  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(analyzer_descriptor,analyzer);
#endif
