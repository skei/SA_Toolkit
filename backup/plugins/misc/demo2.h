
#include "plugin/sat_plugin.h"

#if !defined (SAT_GUI_NOGUI)
  #include "gui/sat_widgets.h"
#endif

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t demo2_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sat_demo2",
  .name         = "demo2",
  .vendor       = "skei.audio",
  .url          = "https://github.com/skei/SA_Toolkit",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]) { CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class demo2
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(demo2);

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("Input");
    appendStereoAudioOutputPort("Output");
    // appendParameter( new SAT_Parameter( "Gain", 0.5, 0, 1, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE ));
    #if !defined (SAT_GUI_NOGUI)
      setInitialEditorSize( 800, 600, 1.0 );
    #endif
    return SAT_Plugin::init();
  }

  //----------

  #if !defined (SAT_GUI_NOGUI)
    #include "demo2/demo2_gui.h"
  #endif

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(demo2_descriptor,demo2);
#endif
