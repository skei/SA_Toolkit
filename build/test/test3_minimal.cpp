
#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0",
  .name         = "myPlugin",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0",
  .description  = "a nice plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
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

  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoInputPort();
    appendStereoOutputPort();
    setInitialEditorSize(300,300,1.0);
    SAT_Parameter* param = new SAT_Parameter("Param1", 0.3);
    param->setFlag(CLAP_PARAM_IS_MODULATABLE);
    appendParameter( param );
    return SAT_Plugin::init();
  }
  
  //----------
  
  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(panel);
    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(50,50,200,200),"Gain",0.5);
    panel->appendChildWidget(knob);
    knob->setTextSize(25);
    knob->setValueSize(35);
    knob->setArcThickness(20);
    AEditor->connect( knob, getParameter(0) );
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
//
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)
