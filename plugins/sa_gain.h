#ifndef sa_gain_included
#define sa_gain_included
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_gain_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_gain",
  .name         = "sa_gain",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "a simple gain plugin",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_gain_plugin
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_gain_plugin)

  //----------
  
  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
//    setInitialEditorSize(300,160,3.0);
    appendParameter( new SAT_Parameter(0,"Gain","", 0.5, 0,2, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE) );
    return SAT_Plugin::init();
  }
  
  //----------

//  bool initEditorWindow(SAT_Editor* AEditor) final {
//    SAT_Window* window = AEditor->getWindow();
//    // background panel
//    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
//    window->appendRootWidget(panel);
//    // header
//    const char* plugin_format = getPluginFormat();
//    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,200,40),"gain",plugin_format);
//    panel->appendChildWidget(header);
//    // slider
//    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,50,200,20),"Gain",0.5);
//    panel->appendChildWidget(slider);
//    // connect
//    AEditor->connect( slider, getParameter(0) );
//    return true;
//  }

  //----------
  
//  void processAudio(SAT_ProcessContext* AContext) final {
//    float** inputs = AContext->process->audio_inputs[0].data32;
//    float** outputs = AContext->process->audio_outputs[0].data32;
//    uint32_t length = AContext->process->frames_count;
//    // process
//    SAT_CopyStereoBuffer(outputs,inputs,length);
//    // gain
//    double scale = getParameterValue(0) + getModulationValue(0);
//    scale = SAT_Clamp(scale,0,1);
//    SAT_ScaleStereoBuffer(outputs,scale,length);
//  }
  
  //----------
  
  void processSample(float* spl0, float* spl1) final {
    double gain = getParameterValue(0) + getModulationValue(0);
    gain = SAT_Clamp(gain,0,2);
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
  SAT_PLUGIN_ENTRY(sa_gain_descriptor,sa_gain_plugin)
  
#endif

//----------------------------------------------------------------------
#endif

