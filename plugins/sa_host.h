#ifndef sa_host_included
#define sa_host_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"

//----------

#include "sa_host/sa_host_widgets.h"
#include "sa_host/sa_host_editor.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_host_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_host",
  .name         = "sa_host",
  .vendor       = SAT_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
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

class sa_host_plugin
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_host_plugin)
  
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
    
    setInitialEditorSize(256,256,2.0);
    return SAT_Plugin::init();
  }
  
  //----------

//  bool initEditorWindow(SAT_Editor* AEditor) final {
//    SAT_Window* window = AEditor->getWindow();
//    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
//    window->appendRootWidget(panel);
//    
//    //----------
//    
//    SAT_MenuWidget* file_menu   = new SAT_MenuWidget(SAT_Rect(60, (3*15)+10 ));
//    SAT_MenuWidget* host_menu   = new SAT_MenuWidget(SAT_Rect(60, (3*15)+10 ));
//    SAT_MenuWidget* plugin_menu = new SAT_MenuWidget(SAT_Rect(60, (3*15)+10 ));
//    SAT_MenuWidget* setup_menu  = new SAT_MenuWidget(SAT_Rect(60, (4*15)+10 ));
//    
//    file_menu->setInnerBorder(SAT_Rect(5,5,5,5));
//    file_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Load"));
//    file_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Save"));
//    file_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Exit"));
//    
//    host_menu->setInnerBorder(SAT_Rect(5,5,5,5));
//    host_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"..."));
//    host_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"..."));
//    host_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),".."));
//    
//    plugin_menu->setInnerBorder(SAT_Rect(5,5,5,5));
//    plugin_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Load plugin"));
//    plugin_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Open editor"));
//    plugin_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Load preset"));
//
//    setup_menu->setInnerBorder(SAT_Rect(5,5,5,5));
//    setup_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Audio"));
//    setup_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Midi"));
//    setup_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Paths"));
//    setup_menu->appendChildWidget( new SAT_MenuItemWidget(SAT_Rect(15),"Options"));
//
//    //----------
//    
//    SAT_SelectorWidget* file_selector   = new SAT_SelectorWidget(SAT_Rect(  0,0, 40,15), "File",   file_menu  );
//    SAT_SelectorWidget* host_selector   = new SAT_SelectorWidget(SAT_Rect( 40,0, 40,15), "Host",   host_menu  );
//    SAT_SelectorWidget* plugin_selector = new SAT_SelectorWidget(SAT_Rect( 80,0, 40,15), "Plugin", plugin_menu);
//    SAT_SelectorWidget* setup_selector  = new SAT_SelectorWidget(SAT_Rect(120,0, 40,15), "Setup",  setup_menu );
//
//    panel->appendChildWidget(file_selector);
//    panel->appendChildWidget(host_selector);
//    panel->appendChildWidget(plugin_selector);
//    panel->appendChildWidget(setup_selector);
//
//    file_selector->setName("File");
//    file_selector->setDrawSelectedText(false);
//    file_selector->setDrawArrow(false);
//    file_selector->setTextSize(8);
//    //file_selector->setAutoSizeMenu(true);
//    
//    host_selector->setName("Host");
//    host_selector->setDrawSelectedText(false);
//    host_selector->setDrawArrow(false);
//    host_selector->setTextSize(8);
//    //host_selector->setAutoSizeMenu(true);
//    
//    plugin_selector->setName("Plugin");
//    plugin_selector->setDrawSelectedText(false);
//    plugin_selector->setDrawArrow(false);
//    plugin_selector->setTextSize(8);
//    //plugin_selector->setAutoSizeMenu(true);
//    
//    setup_selector->setName("Setup");
//    setup_selector->setDrawSelectedText(false);
//    setup_selector->setDrawArrow(false);
//    setup_selector->setTextSize(8);
//    //setup_selector->setAutoSizeMenu(true);
//    
//    //----------
//    
//    panel->appendChildWidget(file_menu);
//    panel->appendChildWidget(host_menu);
//    panel->appendChildWidget(plugin_menu);
//    panel->appendChildWidget(setup_menu);
//    
//    return true;
//  }

//------------------------------
public:
//------------------------------

  SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
    sa_host_editor* editor = new sa_host_editor(AListener,AWidth,AHeight,getHost(),this);
    return editor;
  }

  //----------
  
  bool initEditorWindow(SAT_Editor* AEditor/*, SAT_Window* AWindow*/) final {
    SAT_Window* window = AEditor->getWindow();
    sa_host_editor* editor = (sa_host_editor*)AEditor;
    editor->initialize(window,getPluginFormat());
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
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_host_descriptor,sa_host_plugin)
#endif

//----------------------------------------------------------------------
#endif
