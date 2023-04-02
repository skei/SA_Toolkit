
#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define EDITOR_WIDTH  1000
#define EDITOR_HEIGHT 750
#define EDITOR_SCALE  1.0

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
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    
    SAT_Parameter* param = new SAT_Parameter("Param1", 0.3);
    param->setFlag(CLAP_PARAM_IS_MODULATABLE);
    appendParameter( param );
    
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

  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_PanelWidget* root = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(root);
    
    SAT_PanelWidget* left_panel = new SAT_PanelWidget(SAT_Rect(300));
    root->appendChildWidget(left_panel);
    left_panel->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);
    
      SAT_ScrollBoxWidget* scrollbox = new SAT_ScrollBoxWidget(SAT_Rect(0,0,300,200),true,true);
      left_panel->appendChildWidget(scrollbox);
      scrollbox->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_GroupBoxWidget* groupbox = new SAT_GroupBoxWidget(SAT_Rect(0,0,300,200),false);
      left_panel->appendChildWidget(groupbox);
      groupbox->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

      SAT_TextBoxWidget* textbox = new SAT_TextBoxWidget(SAT_Rect(0,0,300,200));
      left_panel->appendChildWidget(textbox);
      textbox->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
      
        for (uint32_t i=0; i<25; i++) { textbox->appendLine("Hello world!"); }

    SAT_SizerWidget* sizer1 = new SAT_SizerWidget(SAT_Rect(5),SAT_DIRECTION_LEFT,left_panel);
    root->appendChildWidget(sizer1);
    sizer1->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);

    SAT_PanelWidget* right_panel = new SAT_PanelWidget(SAT_Rect(300));
    root->appendChildWidget(right_panel);
    right_panel->setStretching(SAT_WIDGET_STRETCH_ALL);

    return true;
  }

  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)
