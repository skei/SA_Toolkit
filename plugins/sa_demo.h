#ifndef sa_demo_included
#define sa_demo_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_VST3
//#define SAT_PLUGIN_USE_PRESET_DISCOVERY
//#define SAT_DEBUG_WINDOW
//#define SAT_DEBUG_OBSERVER

#define PLUGIN_NAME   "sa_demo"
#define EDITOR_WIDTH  800
#define EDITOR_HEIGHT 600
#define EDITOR_SCALE  1.6
#define NUM_VOICES    32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
#include "audio/sat_voice_manager.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"

#include "sa_demo/sa_demo_editor.h"
#include "sa_demo/sa_demo_voice.h"
#include "sa_demo/sa_demo_widgets.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_demo_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "...",
  .features     = (const char* []) {
    CLAP_PLUGIN_FEATURE_INSTRUMENT,
    nullptr
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

  SAT_VoiceManager<sa_demo_voice,NUM_VOICES>  MVoiceManager = {};
  SAT_VoicesWidget*   MVoicesWidget   = nullptr;
  SAT_WaveformWidget* MWaveformWidget = nullptr;

  //double obs1 = 0.0; // being 'observed'
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_demo_plugin);

//------------------------------
public:
//------------------------------

  bool init() final {
    //SAT_Print("id: '%s'\n",sa_demo_descriptor.id);
    registerAllExtensions();
    appendClapNoteInputPort();
    appendStereoOutputPort();
    
    appendParameter(new SAT_Parameter(    "Param1", 0.1        ));
    appendParameter(new SAT_Parameter(    "Param2", 0.4, -2, 4 ));
    appendParameter(new SAT_Parameter(    "Param3", 0.7,  0, 2 ));
    appendParameter(new SAT_IntParameter( "Param4", 0,   -5, 5 ));
    appendParameter(new SAT_Parameter(    "P4",     0.2        ));
    appendParameter(new SAT_Parameter(    "P5",     0.8        ));
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    
    SAT_Host* host = getHost();
    const clap_plugin_t*  clapplugin = getPlugin();
    const clap_host_t* claphost = host->getHost();
    MVoiceManager.init(clapplugin,claphost);
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_INTERLEAVED);
    
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    
    return SAT_Plugin::init();
  }

  //----------
  
  //void destroy() final {
  //  if (debugwindow) {
  //    debugwindow->stopEventThread();
  //    delete debugwindow;
  //  }
  //  SAT_Plugin::destroy();
  //}
  
  //----------
  
  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    //SAT_Print("sample_rate %.2f min_frames_count %i max_frames_count %i\n",sample_rate,min_frames_count,max_frames_count);
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    //SAT_Print("-> %i (CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES)\n",NUM_VOICES);
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public:
//------------------------------

  SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
    sa_demo_editor* editor = new sa_demo_editor(AListener,AWidth,AHeight,getHost(),this);
    return editor;
  }

  //----------
  
  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    sa_demo_editor* editor = (sa_demo_editor*)AEditor;
    editor->initialize(AWindow,getPluginFormat());
    return true;
  }
  
//------------------------------
public:
//------------------------------

  void do_editorListener_timer() final {
    SAT_Plugin::do_editorListener_timer();
    //update voices widget.. (move to widget itself (+ register timer)
    // before or after sat_plugin:: ?
    /*
    for (uint32_t voice=0; voice<NUM_VOICES; voice++) {
      uint32_t state = MVoiceManager.getVoiceState(voice);
      MVoicesWidget->setVoiceState(voice,state);
    }
    MVoicesWidget->parentRedraw();
    */
  }

//------------------------------
public:
//------------------------------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

  //----------

  bool handleNoteOn(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOn(event);
    return true;
  }

  //----------

  bool handleNoteOff(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOff(event);
    return true;
  }

  //----------

  bool handleNoteChoke(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteChoke(event);
    return true;
  }

  //----------

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  //----------

  bool handleParamValue(const clap_event_param_value_t* event) final {
    MVoiceManager.handleParamValue(event);
    return true;
  }

  //----------

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.handleParamMod(event);
    return true;
  }

  //----------

  bool handleTransport(const clap_event_transport_t* event) final {
    return false;
  }

  //----------

  bool handleMidi(const clap_event_midi_t* event) final {
    MVoiceManager.handleMidi(event);
    return true;
  }

  //----------

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.handleMidiSysex(event);
    return true;
  }

  //----------

  bool handleMidi2(const clap_event_midi2_t* event) final {
    MVoiceManager.handleMidi2(event);
    return true;
  }

//------------------------------
public:
//------------------------------

  // block

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);
    //sat_param_t scale = getParameterValue(2) + getModulationValue(2);
    //scale = SAT_Clamp(scale,0,1);
    //SAT_ScaleStereoBuffer(outputs,scale,length);
    //obs1 = MVoiceManager.getNumPlayingVoices();
  }

  //----------

  // interleaved

  //void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) final {
  //  const clap_process_t* process = AContext->process;
  //  //uint32_t length = process->frames_count;
  //  float* inputs[2];
  //  float* outputs[2];
  //  inputs[0]  = process->audio_inputs[0].data32[0] + offset;
  //  inputs[1]  = process->audio_inputs[0].data32[1] + offset;
  //  outputs[0] = process->audio_outputs[0].data32[0] + offset;
  //  outputs[1] = process->audio_outputs[0].data32[1] + offset;
  //  SAT_CopyStereoBuffer(outputs,inputs,length);
  //  sat_param_t scale = getParameterValue(2) + getModulationValue(2);
  //  scale = SAT_Clamp(scale,0,1);
  //  SAT_ScaleStereoBuffer(outputs,scale,length);
  //}

  //----------

  // quantized

  //void processAudio(SAT_ProcessContext* AContext, uint32_t offset) final {
  //  processAudio(AContext,offset,SAT_AUDIO_QUANTIZED_SIZE);
  //}

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_demo_descriptor,sa_demo_plugin);

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#undef PLUGIN_NAME
#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT
#undef EDITOR_SCALE
#undef NUM_VOICES

//----------------------------------------------------------------------
#endif

