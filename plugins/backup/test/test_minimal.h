
#if 0

#include "plugin/sat_plugin.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "minimal", "minimal", SAT_VENDOR, "", "", "", SAT_VERSION, "", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    appendParameter(new SAT_Parameter("Param1",0.3))->setFlag(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  void processAudio(SAT_ProcessContext* AContext) final {
    SAT_CopyStereoBuffer(AContext->process->audio_outputs[0].data32,AContext->process->audio_inputs[0].data32,AContext->process->frames_count);
    SAT_ScaleStereoBuffer(AContext->process->audio_outputs[0].data32,SAT_Clamp(getParameterValue(0)+getModulationValue(0),0,1),AContext->process->frames_count);
  }
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)

#endif // 0

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#include "base/sat.h"
#include "plugin/sat_plugin.h"
//#include "audio/sat_audio_utils.h"
//#include "gui/sat_widgets.h"

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin",
  .name         = "myPlugin",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "a nice plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

class myPlugin
: public SAT_Plugin {
  
public:

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    SAT_Parameter* param = new SAT_Parameter("Param1", 0.3);
    param->setFlag(CLAP_PARAM_IS_MODULATABLE);
    appendParameter(param);
    //setInitialEditorSize(500,120,1.0);
    return SAT_Plugin::init();
  }
  
  //bool initEditorWindow(SAT_Editor* AEditor) final {
  //  SAT_PanelWidget* panel = new SAT_PanelWidget(0);
  //  SAT_Window* window = AEditor->getWindow();
  //  window->appendRootWidget(panel);
  //  SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,50,400,20),"Gain",0.5);
  //  panel->appendChildWidget(slider);
  //  AEditor->connect( slider, getParameter(0) );
  //  return true;
  //}

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

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)

