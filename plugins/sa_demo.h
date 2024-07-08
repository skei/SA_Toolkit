#ifndef sa_demo_included
#define sa_demo_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_interleaved_processor.h"
#include "sa_demo/sa_demo_parameters.h"
//#include "sa_demo/sa_demo_voice.h"

#ifndef SAT_NO_GUI
  #include "plugin/sat_editor.h"
  #include "sa_demo/sa_demo_widgets.h"
  #include "sa_demo/sa_demo_page_blank.h"
  #include "sa_demo/sa_demo_page_widgets.h"
  #include "sa_demo/sa_demo_page_parameters.h"
  
  // #include "sa_demo/sa_demo_page_base.h"
  // #include "sa_demo/sa_demo_page_plugin.h"
  // #include "sa_demo/sa_demo_page_gui.h"

#endif

//----------


#define MAX_VOICES    64
#define EDITOR_WIDTH  800
#define EDITOR_HEIGHT 600
#define EDITOR_SCALE  1.0

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_demo_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_demo/v0",
  .name         = "sa_demo",
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
// processor
//
//----------------------------------------------------------------------

class sa_demo_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  bool  need_recalc = true;
  float samplerate = 0.0;
  
//------------------------------
public:
//------------------------------

  sa_demo_processor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~sa_demo_processor() {
  }

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    samplerate = ASampleRate;
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    need_recalc = true;
  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
    const clap_process_t* process = AContext->process;
    if (need_recalc) recalc(samplerate);
    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      *output0++ = *input0++;
      *output1++ = *input1++;
    }    
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_demo_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_demo_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_demo_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_demo_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");

    MProcessor = new sa_demo_processor(this);
    setProcessor(MProcessor);

    sa_demo_SetupParameters(this);

    #ifndef SAT_NO_GUI
      setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE,true);
    #endif

    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  #ifndef SAT_NO_GUI
    #include "sa_demo/sa_demo_gui.h"
  #endif

  //----------

  void on_editorListener_timer(SAT_Timer* ATimer, double ADelta) override {
    SAT_Plugin::on_editorListener_timer(ATimer,ADelta);
    // for (uint32_t i=0; i<MAX_VOICES; i++) {
    //   uint32_t state = MProcessor->getVoiceState(i);
    //   voices->setVoiceState(i,state);
    // }
    // voices->do_widget_redraw(voices);
  }  

};

#undef MAX_VOICES
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
  SAT_PLUGIN_ENTRY(sa_demo_descriptor,sa_demo_plugin)
#endif

//----------------------------------------------------------------------
#endif

