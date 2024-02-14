#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin",
  .name         = "my plugin",
  .vendor       = "me",
  .url          = "https://website.com/myPlugin",
  .manual_url   = "https://website.com/myPlugin/manual.pdf",
  .support_url  = "https://website.com/support",
  .version      = "0.0.0",
  .description  = "my precious plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

public:

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);

public:

  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("Input");
    appendStereoAudioOutputPort("Output");
    appendParameter( new SAT_Parameter("Gain",0.5) );
    setInitialEditorSize(200,200,2.0);
    return SAT_Plugin::init();
  }

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root);
    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(50,50,100,100),"Gain",0.1);
    root->appendChildWidget(knob);
    knob->setTextSize(15);
    knob->setValueSize(25);
    AEditor->connect(knob,getParameter(0));
  }

  void processStereoSample(float* spl0, float* spl1) final {
    sat_param_t gain = getParameterValue(0);
    *spl0 *= gain;
    *spl1 *= gain;
  }

};

//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);
