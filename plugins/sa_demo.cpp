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
  //#include "sa_demo/sa_demo_page_base.h"
  //#include "sa_demo/sa_demo_page_plugin.h"
  //#include "sa_demo/sa_demo_page_gui.h"
#endif

//----------

#define MAX_VOICES    64
#define EDITOR_WIDTH  800
#define EDITOR_HEIGHT 600
#define EDITOR_SCALE  1.5

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

  sa_demo_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
    //SAT_GLOBAL.DEBUG.OBSERVER.observe(SAT_OBSERVE_FLOAT,&samplerate,"samplerate");
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

  double obs_test = 1.2;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_demo_plugin)

  // sa_demo_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  // : SAT_Plugin(ADescriptor,AHost) {
  // }

  //----------

  // virtual ~sa_demo_plugin() {
  // }

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultExtensions();
    registerExtension(CLAP_EXT_MINI_CURVE_DISPLAY);
    registerExtension(CLAP_EXT_GAIN_ADJUSTMENT_METERING);

    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");

    MProcessor = new sa_demo_processor(this);
    setProcessor(MProcessor);
    sa_demo_SetupParameters(this);

    #ifndef SAT_NO_GUI
      setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE,true);
    #endif

    //return SAT_Plugin::init();

    bool result = SAT_Plugin::init();
    printSupportedHostExtensions();

    if (result) {
      SAT_Host* host = getHost();
      if (host->ext.mini_curve_display) {
        host->ext.mini_curve_display->set_dynamic(getClapHost(),true);
        //host->ext.mini_curve_display->changed(getClapHost(),CLAP_MINI_CURVE_DISPLAY_CURVE_CHANGED);
      }
    }

    return result;
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->setSampleRate(sample_rate);
    bool result = SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);

    // if (result) {
    //   SAT_Host* host = getHost();
    //   if (host->ext.mini_curve_display) {
    //     host->ext.mini_curve_display->set_dynamic(getClapHost(),true);
    //     //host->ext.mini_curve_display->changed(getClapHost(),CLAP_MINI_CURVE_DISPLAY_CURVE_CHANGED);
    //   }
    // }

    return result;
  }

  //----------

  bool start_processing() override {
    bool result = SAT_Plugin::start_processing();

    // if (result) {
    //   SAT_Host* host = getHost();
    //   if (host->ext.mini_curve_display) {
    //     host->ext.mini_curve_display->set_dynamic(getClapHost(),true);
    //     //host->ext.mini_curve_display->changed(getClapHost(),CLAP_MINI_CURVE_DISPLAY_CURVE_CHANGED);
    //   }
    // }

    return result;
  }    

  //----------

  // setupEditor/Overlay

  #ifndef SAT_NO_GUI
    #include "sa_demo/sa_demo_gui.h"
  #endif

  //----------

  // void on_EditorListener_timer(double ADelta) override {
  //   SAT_Plugin::on_EditorListener_timer(ADelta);
  //   // for (uint32_t i=0; i<MAX_VOICES; i++) {
  //   //   uint32_t state = MProcessor->getVoiceState(i);
  //   //   voices->setVoiceState(i,state);
  //   // }
  //   // voices->do_Widget_redraw(voices);
  // }

//------------------------------
public: // gain-adjustment-metering
//------------------------------

  double gain_adjustment_metering_get() final {
    double value = SAT_RandomRange(-30.0,30.0);
    //SAT_PRINT("value: %.3f\n",value);
    return value;
  }

//------------------------------
public: // mini-curve-display
//------------------------------

  uint32_t mini_curve_display_get_curve_count() final {
    //SAT_PRINT("return 1\n");
    return 1;
  }

  //----------

  uint32_t mini_curve_display_render(clap_mini_curve_display_curve_data_t *curves, uint32_t curves_size) final {
    //SAT_PRINT("curves_size: %i\n",curves_size);
    //if (curves_size == 1) {
    for (uint32_t curve=0; curve<curves_size; curve++) {
      SAT_Host* host = getHost();
      clap_mini_curve_display_curve_hints_t hints = {0};
      bool result = host->ext.mini_curve_display->get_hints(getClapHost(),CLAP_MINI_CURVE_DISPLAY_CURVE_KIND_GAIN_RESPONSE,&hints);
      if (result) {
        // never printed, because bitwig/linux get_hints() returns false
        //SAT_PRINT("curve: %i : min %.2f,%.2f max %.2f,%.2f\n",curve,hints.x_min,hints.y_min,hints.x_max,hints.y_max);
      }
      uint32_t count = curves[curve].values_count;
      for (uint32_t i=0; i<count; i++) {
        curves[curve].curve_kind = CLAP_MINI_CURVE_DISPLAY_CURVE_KIND_TIME_SERIES;
        curves[curve].values[i] = SAT_RandomRangeInt(1,0xfffe);
      }
    }
    return curves_size;
  }

  //----------

  void mini_curve_display_set_observed(bool is_observed) final {
    SAT_PRINT("is_observed: %s\n",is_observed ? "true" : "false");
  }

  //----------

  bool mini_curve_display_get_axis_name(uint32_t curve_index, char *x_name, char *y_name, uint32_t name_capacity) final {
    //SAT_PRINT("curve_index: %i name_capacity: %i\n",curve_index,name_capacity);
    strcpy(x_name,"X");
    strcpy(y_name,"Y");
    return true;
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

