
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
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    
    SAT_Parameter* param1 = new SAT_Parameter("Param1", .025);
    appendParameter(param1);
    
    SAT_Parameter* param2 = new SAT_Parameter("Param2", 0.75);
    appendParameter(param2);
    
    setInitialEditorSize(600,400,2.0);
    return SAT_Plugin::init();
  }
  
  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(panel);
    
    SAT_MenuWidget* file_menu = new SAT_MenuWidget(SAT_Rect(100,100));
      file_menu->setInnerBorder(SAT_Rect(10,10,10,10));
      file_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Open"));
      file_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Close"));
      file_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Exit"));
    
    SAT_MenuWidget* edit_menu = new SAT_MenuWidget(SAT_Rect(100,100));
      edit_menu->setInnerBorder(SAT_Rect(10,10,10,10));
      edit_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Cut"));
      edit_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Copy"));
      edit_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Paste"));
    
    SAT_SelectorWidget* file_selector = new SAT_SelectorWidget(SAT_Rect(100,100,50,15),"File",file_menu);
    panel->appendChildWidget(file_selector);
    file_selector->setDrawSelectedText(false);
    file_selector->setDrawArrow(false);
    file_selector->setTextSize(10);
    file_selector->setAutoSizeMenu(true);
    
    SAT_SelectorWidget* edit_selector = new SAT_SelectorWidget(SAT_Rect(150,100,50,15),"Edit",edit_menu);
    panel->appendChildWidget(edit_selector);
    edit_selector->setDrawSelectedText(false);
    edit_selector->setDrawArrow(false);
    edit_selector->setTextSize(10);
    edit_selector->setAutoSizeMenu(true);
    
    panel->appendChildWidget(file_menu);
    panel->appendChildWidget(edit_menu);
    
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
