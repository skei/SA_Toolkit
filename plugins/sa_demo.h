#ifndef sa_demo_included
#define sa_demo_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------

#ifdef SAT_DEBUG
  #define PLUGIN_NAME   "sa_demo (debug)"
#else
  #define PLUGIN_NAME   "sa_demo"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_demo_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME "/" SAT_VERSION,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
  //.features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_DETECTOR, nullptr }
  //.features     = (const char*[]){ CLAP_PLUGIN_FEATURE_ANALYZER, nullptr }
};







//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_demo_plugin
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_demo_plugin)

  //----------

  bool init() final {
    registerDefaultExtensions();    
    //appendClapNoteInputPort();
    appendStereoInputPort();
    appendStereoOutputPort();
    SAT_Parameter* param = appendParameter( new SAT_Parameter("Param1", 0.3) );
    param->setFlag(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    float** inputs = AContext->process->audio_inputs[0].data32;
    float** outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;
    SAT_CopyStereoBuffer(outputs,inputs,length);
    double scale = getParameterValue(0) + getModulationValue(0);
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }
  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_demo_descriptor,sa_demo_plugin)
#endif







//----------------------------------------------------------------------
#endif


