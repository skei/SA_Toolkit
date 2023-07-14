
#include "base/sat.h"
#include "base/utils/sat_jit.h"
//#include "base/utils/sat_vm.h"
//#include "base/utils/sat_vm_compiler.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t test_jit_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/test_jit",
  .name         = "test_jit",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "a nice plugin",
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

class test_jit_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(test_jit_plugin)
  
//------------------------------
public:
//------------------------------

  bool init() final {
    
    test_jit();
    
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    
    SAT_Parameter* param1 = new SAT_Parameter("Param1", .025);
    appendParameter(param1);
    
    SAT_Parameter* param2 = new SAT_Parameter("Param2", 0.75);
    appendParameter(param2);
    
    setInitialEditorSize(600,400,1.333);
    
    return SAT_Plugin::init();
  }
  
  //----------

  bool initEditorWindow(SAT_Editor* AEditor/*, SAT_Window* AWindow*/) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    window->appendRootWidget(panel);
    
    panel->setFillGradient(true);
    panel->setGradientColors(SAT_Green,0.2,0.2);
    
    return true;
  }

  //----------
  
  bool handleParamValue(const clap_event_param_value_t* event) override {
    SAT_Print("param_id %i value %f\n",event->param_id,event->value);
    return true;
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
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(test_jit_descriptor,test_jit_plugin)
#endif
