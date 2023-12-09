#ifndef sa_botage_included
#define sa_botage_included
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

#include "sa_botage/sa_botage_parameters.h"
#include "sa_botage/sa_botage_widgets.h"
#include "sa_botage/sa_botage_prob_page.h"
#include "sa_botage/sa_botage_seq_page.h"
#include "sa_botage/sa_botage_perf_page.h"

#define PLUGIN_NAME   "sa_botage"
#define PLUGIN_DESC   "sabotage your loops!"
#define PLUGIN_VER    "0.5.0"

#define EDITOR_WIDTH  840
#define EDITOR_HEIGHT 510
#define EDITOR_SCALE  1.5

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
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_botage_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    
    //appendParameters(SA_BOTAGE_PARAM_COUNT,sa_botage_parameter_infos);
    
    appendParameter( new SAT_Parameter(     "Gain",               0.25        ));
    appendParameter( new SAT_IntParameter(  "Buffer Num Beats",   4,    1, 8  ));
    appendParameter( new SAT_IntParameter(  "Buffer Num Slices",  2,    1, 8  ));
    
    return SAT_Plugin::init();
  }
  
  //----------
  
  #include "sa_botage/sa_botage_gui.h"

  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    float** inputs = AContext->process->audio_inputs[0].data32;
    float** outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;
    
    // process
    SAT_CopyStereoBuffer(outputs,inputs,length);
    
//    // gain
//    double scale = getParameterValue(0) + getModulationValue(0);
//    scale = SAT_Clamp(scale,0,1);
//    SAT_ScaleStereoBuffer(outputs,scale,length);
    
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

