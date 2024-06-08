
#include "plugin/sat_plugin.h"

#if !defined (SAT_GUI_NOGUI)
  #include "gui/sat_widgets.h"
#endif

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t audio_effect_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sat_audio_effect",
  .name         = "audio_effect",
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

class audio_effect
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(audio_effect);

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("Input");
    appendStereoAudioOutputPort("Output");
    appendParameter( new SAT_Parameter( "Gain", 0.5, 0, 1, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE ));
    #if !defined (SAT_GUI_NOGUI)
      setInitialEditorSize( 200, 200, 3.0 );
    #endif
    return SAT_Plugin::init();
  }

  //----------

  // bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //   return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  // }

  //----------

  #if !defined (SAT_GUI_NOGUI)
  
  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root);
    SAT_KnobWidget* knob = new SAT_KnobWidget( SAT_Rect(10,10,180,180), "qwe", 0.0 );
    root->appendChildWidget(knob);
    knob->setArcThickness(25);
    knob->setValueSize(45);
    knob->setTextSize(30);
    AEditor->connect( knob, getParameter(0) );
  }

  #endif

  //----------

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    sat_param_t gain = getModulatedParameterValue(0);
    *spl0 *= gain;
    *spl1 *= gain;
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(audio_effect_descriptor,audio_effect);
#endif
