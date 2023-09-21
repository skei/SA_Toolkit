


#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "me/myPlugin", "myPlugin", "skei.audio", "", "", "", "0.0.1", "a nice plugin", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    appendParameter(new SAT_Parameter("Gain",0.5))->setFlag(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  void processAudio(SAT_ProcessContext* AContext) final {
    SAT_CopyStereoBuffer(AContext->process->audio_outputs[0].data32,AContext->process->audio_inputs[0].data32,AContext->process->frames_count);
    SAT_ScaleStereoBuffer(AContext->process->audio_outputs[0].data32,SAT_Clamp(getParameterValue(0)+getModulationValue(0),0,1),AContext->process->frames_count);
  }
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)



//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*

#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"
#include "audio/sat_audio_utils.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "me/myPlugin/0.0.1", "myPlugin", "skei.audio", "", "", "", "0.0.1", "a nice plugin", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    setInitialEditorSize(300,120,3);
    appendParameter(new SAT_Parameter("Param1",0.3))->setFlag(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    AEditor->connect(AWindow->appendRootWidget(new SAT_PanelWidget(0))->appendChildWidget(new SAT_SliderWidget(SAT_Rect(50,50,200,20),"Gain",0.5)), getParameter(0));
    return true;
  }
  void processAudio(SAT_ProcessContext* AContext) final {
    SAT_CopyStereoBuffer(AContext->process->audio_outputs[0].data32,AContext->process->audio_inputs[0].data32,AContext->process->frames_count);
    SAT_ScaleStereoBuffer(AContext->process->audio_outputs[0].data32,SAT_Clamp(getParameterValue(0)+getModulationValue(0),0,1),AContext->process->frames_count);
  }
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)

*/

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*

#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

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
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

class myPlugin
: public SAT_Plugin {
  
public:

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)

  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoOutputPort();
    setInitialEditorSize(500,120,1.0);
    SAT_Parameter* param = new SAT_Parameter("Param1", 0.3);
    param->setFlag(CLAP_PARAM_IS_MODULATABLE);
    appendParameter(param);
    return SAT_Plugin::init();
  }
  
  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(panel);
    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,50,400,20),"Gain",0.5);
    panel->appendChildWidget(slider);
    AEditor->connect( slider, getParameter(0) );
    return true;
  }

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

SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)

*/









