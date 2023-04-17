#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/test_params/0.0.1",
  .name         = "test_params",
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
    setInitialEditorSize(400,200,1.0);
    
    SAT_Parameter* p1 = new SAT_Parameter("Param1", 0.0, -2, 4);
    SAT_Parameter* p2 = new SAT_IntParameter("Param2", 0.0, -5.0, 5.0);

    //p1->setFlag(CLAP_PARAM_IS_MODULATABLE);
    //p2->setFlag(CLAP_PARAM_IS_MODULATABLE);
    //p2->setFlag(CLAP_PARAM_IS_STEPPED);

    appendParameter(p1);
    appendParameter(p2);
    
    return SAT_Plugin::init();
  }
  
  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(panel);
    
    SAT_SliderWidget* s1 = new SAT_SliderWidget(SAT_Rect(10,10,380,20),"p1",0);
    SAT_SliderWidget* s2 = new SAT_SliderWidget(SAT_Rect(10,40,380,20),"p2",0);

    panel->appendChildWidget(s1);
    panel->appendChildWidget(s2);

    AEditor->connect( s1, getParameter(0) );
    AEditor->connect( s2, getParameter(1) );
    
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
