
#include "base/sat.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0.0.1",
  .name         = "myPlugin",
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
//
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)
  
//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoOutputPort();
    
    SAT_Parameter* param1 = new SAT_Parameter("Param1", .025);
    appendParameter(param1);
    
    SAT_Parameter* param2 = new SAT_Parameter("Param2", 0.75);
    appendParameter(param2);
    
    setInitialEditorSize(400,300,3.0);
    return SAT_Plugin::init();
  }
  
  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(panel);
    
    SAT_DualValueWidget* dualvalue = new SAT_DualValueWidget(SAT_Rect(10,10,200,20),0.2,0.8);
    panel->appendChildWidget(dualvalue);
    
    SAT_DualSliderWidget* dualslider = new SAT_DualSliderWidget(SAT_Rect(10,35,200,20),"dualslider",0.2,0.8);
    panel->appendChildWidget(dualslider);
    
    SAT_RangeBarWidget* rangebar = new SAT_RangeBarWidget(SAT_Rect(10,60,200,20));
    panel->appendChildWidget(rangebar);
    
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

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)
