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
  .features     = (const char*[]) { CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
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
    appendParameter( new SAT_Parameter( "Gain", 0.5, 0, 1, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE ));
    setInitialEditorSize( 200, 200, 3.0 );
    return SAT_Plugin::init();
  }

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    sat_param_t gain = getModulatedParameterValue(0);
    *spl0 *= gain;
    *spl1 *= gain;
  }

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root);
    SAT_KnobWidget* knob = new SAT_KnobWidget( SAT_Rect(10,10,180,180), "Gain", 0.1 );
    root->appendChildWidget(knob);
    knob->setArcThickness(25);
    knob->setTextSize(30);
    knob->setValueSize(45);
    AEditor->connect( knob, getParameter(0) );
  }

};

//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);
