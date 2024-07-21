#ifndef sa_botage_included
#define sa_botage_included
//----------------------------------------------------------------------

#include "sat.h"

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
//#include "audio/sat_audio_utils.h"

#include "plugin/sat_parameters.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

const char* txt_eight[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };

#define NUM_FX_TYPES 3
const char* fx_type_text[NUM_FX_TYPES] = {
  "Filter",
  "Reverb",
  "Distortion"
};

//----------

#include "sa_botage/sa_botage_parameters.h"
#ifndef SAT_NO_GUI
#include "sa_botage/sa_botage_widgets.h"
#include "sa_botage/sa_botage_prob_page.h"
#include "sa_botage/sa_botage_seq_page.h"
#include "sa_botage/sa_botage_perf_page.h"
#endif

#include "sa_botage/sa_botage_processor.h"

//----------------------------------------------------------------------

#define PLUGIN_NAME   "sa_botage"
#define PLUGIN_DESC   "sabotage your loops!"
#define PLUGIN_VER    "0.9.0"

#define EDITOR_WIDTH  840
#define EDITOR_HEIGHT 510
#define EDITOR_SCALE  1.5

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_botage_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_botage/v0",
  .name         = "sa_botage",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_botage_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_botage_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_botage_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE;

    // appendParameter(new SAT_Parameter( "Freq",  "",  2000,  100, 18000, flags ));
    // appendParameter(new SAT_Parameter( "Boost", "",  0,     0,   6,     flags ));
    // appendParameter(new SAT_Parameter( "Harm",  "",  0,     0,   100,   flags ));
    // appendParameter(new SAT_Parameter( "Mix",   "", -6,    -120, 0,     flags ));

    //if (!sa_botage_SetupParameters(this)) return false;
    sa_botage_SetupParameters(this);

    MProcessor = new sa_botage_processor(this);
    setProcessor(MProcessor);

    #ifndef SAT_NO_GUI
      setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE,true);
    #endif

    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    //MProcessor.activate(sample_rate);
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  #ifndef SAT_NO_GUI

  #ifndef SAT_NO_GUI
    #include "sa_botage/sa_botage_gui.h"
  #endif

    // bool setupEditor(SAT_Editor* AEditor) final {
    //   SAT_Window* window = AEditor->getWindow();
    //   SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
    //   window->setRootWidget(root);
    //   return true;
    // }

    // bool setupOverlay(SAT_Editor* AEditor) final {
    //   //SAT_Window* window = AEditor->getWindow();
    //   //SAT_Widget* overlay = window->getOverlayWidget();
    //   return true;
    // }

  #endif

  //----------

  void on_EditorListener_timer(SAT_Timer* ATimer, double ADelta) override {
    SAT_Plugin::on_EditorListener_timer(ATimer,ADelta);
//    updateProbIndicators(&MProcessor);
  #ifndef SAT_NO_GUI
    updateWaveformWidget(MProcessor);
  #endif
  }  

};

#undef PLUGIN_NAME
#undef PLUGIN_DESC
#undef PLUGIN_VER
#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT
#undef EDITOR_SCALE

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_botage_descriptor,sa_botage_plugin)
#endif

//----------------------------------------------------------------------
#endif
