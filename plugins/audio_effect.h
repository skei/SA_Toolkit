

#if 0


#include "plugin/sat_plugin.h"
#include "plugin/sat_editor.h"
#include "gui/sat_widgets.h"

//----------

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

//----------

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
    setInitialEditorSize(200,200,1.0);
    return SAT_Plugin::init();
  }

  void processStereoSample(float* spl0, float* spl1) final {
    sat_param_t gain = getParameterValue(0);
    *spl0 *= 0.1;
    *spl1  = 0.0;
  }

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(0,AWindow);
    AWindow->setRootWidget(root);
    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(50,50,100,100),"Gain",0.1);
    root->appendChildWidget(knob);
    knob->setTextSize(15);
    knob->setValueSize(25);
    AEditor->connect(knob,getParameter(0));
  }

};

//----------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);



#endif // 0

//----------------------------------------------------------------------
//
// minimal
//
//----------------------------------------------------------------------



//#if 0

#include "plugin/sat_plugin.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "me/myPlugin", "my plugin", "me", "", "", "", "0.0.0", "", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);
public:
  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("input");
    appendStereoAudioOutputPort("output");
    appendParameter( new SAT_Parameter("gain",0.5) );
    return SAT_Plugin::init();
  }
  void processStereoSample(float* spl0, float* spl1) final {
    *spl0 *= getParameterValue(0);
    *spl1 *= getParameterValue(0);
  }
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

//#endif // 0

