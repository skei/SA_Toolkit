#ifndef sa_mael_included
#define sa_mael_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_plugin.h"
#include "plugin/sat_parameters.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_voice_manager.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "gui/sat_widgets.h"
#endif

//----------

  //#include "sa_mael/sa_mael_editor.h"
  #include "sa_mael/sa_mael_parameters.h"
  #include "sa_mael/sa_mael_voice.h"

  #define PLUGIN_NAME   "sa_mael"
  #define MAX_VOICES    32

  #define EDITOR_WIDTH  640
  #define EDITOR_HEIGHT 480
  #define EDITOR_SCALE  1.33

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_mael_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = SAT_URL,
  .support_url  = SAT_URL,
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// synth
//
//----------------------------------------------------------------------

class sa_mael_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<sa_mael_Voice,MAX_VOICES>  MVoiceManager = {};

//------------------------------
public:
//------------------------------

  //SAT_DEFAULT_PLUGIN_CONSTRUCTOR(mySynth);

  sa_mael_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    SAT_PRINT;
  };

  virtual ~sa_mael_plugin() {
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultSynthExtensions();
    registerExtension(CLAP_EXT_PARAM_INDICATION,&MExtParamIndication);
    registerExtension(CLAP_EXT_PRESET_LOAD,&MExtPresetLoad);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS,&MExtRemoteControls);
    registerExtension(CLAP_EXT_TRACK_INFO,&MExtTrackInfo);

    appendClapNoteInputPort("Input");
    appendStereoAudioOutputPort("Output");

    //appendParameters(sa_mael_Parameters,SA_MAEL_PARAM_COUNT);
    sa_mael_SetupParameters(this);

    //setEventMode(SAT_PLUGIN_EVENT_MODE_BLOCK);
    MVoiceManager.init(getClapPlugin(),getClapHost());
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_QUANTIZED);

    #if !defined (SAT_GUI_NOGUI)
      setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    #endif

    return SAT_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.threadPoolExec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = MAX_VOICES;
    info->voice_capacity  = MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------

  uint32_t remote_controls_count() final {
    return 1;
  }  
  
  //----------

  //TODO

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    switch (page_index) {
      case 0: {
        strcpy(page->section_name,"Section");
        page->page_id = 0;
        strcpy(page->page_name,"SA_Synth parameters");
        page->param_ids[0] = CLAP_INVALID_ID;
        page->param_ids[1] = CLAP_INVALID_ID;
        page->param_ids[2] = CLAP_INVALID_ID;
        page->param_ids[3] = CLAP_INVALID_ID;
        page->param_ids[4] = CLAP_INVALID_ID;
        page->param_ids[5] = CLAP_INVALID_ID;
        page->param_ids[6] = CLAP_INVALID_ID;
        page->param_ids[7] = CLAP_INVALID_ID;
        page->is_for_preset = false;
        return true;
      }
    }
    return false;
  }  
  
  //----------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
      loadPresetFromFile(location,load_key);
      SAT_Host* host = getHost();
      if (host && host->ext.preset_load) host->ext.preset_load->loaded(getClapHost(),location_kind,location,load_key);
      return true;
    }
    return false;
  }

//------------------------------
public: // events
//------------------------------

  bool on_plugin_noteOn(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOn(event);
    return true;
  }

  bool on_plugin_noteOff(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOff(event);
    return true;
  }

  bool on_plugin_noteChoke(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteChoke(event);
    return true;
  }

  bool on_plugin_noteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    MVoiceManager.handleParamValue(event);
    return true;
  }

  bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.handleParamMod(event);
    return true;
  }

  //bool on_plugin_transport(const clap_event_transport_t* event) final {
  //  return true;
  //}

  bool on_plugin_midi(const clap_event_midi_t* event) final {
    MVoiceManager.handleMidi(event);
    return true;
  }

  bool on_plugin_midiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.handleMidiSysex(event);
    return true;
  }

  bool on_plugin_midi2(const clap_event_midi2_t* event) final {
    MVoiceManager.handleMidi2(event);
    return true;
  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

//------------------------------
public: // process
//------------------------------

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);
    MVoiceManager.mixActiveVoices();
  }

//------------------------------
public: // gui
//------------------------------

  #if !defined (SAT_GUI_NOGUI)  
    #include "sa_mael/sa_mael_editor.h"
  #endif

};

//----------

#undef PLUGIN_NAME
#undef MAX_VOICES
#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT
#undef EDITOR_SCALE

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_mael_descriptor,sa_mael_plugin);
#endif

  // void SAT_Register(SAT_Registry* ARegistry) {
  //   uint32_t index = ARegistry->getNumDescriptors();
  //   /*SAT_Print("index %i = id %s\n",index,myDescriptor.id);*/
  //   ARegistry->registerDescriptor(&sa_mael_Descriptor);
  // }
  //
  // SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  //   /*SAT_Print("index %i\n",AIndex);*/
  //   if (AIndex == 0) {
  //     SAT_Plugin* plugin = new sa_mael_Plugin(ADescriptor,AHost); /* deleted in: ... */
  //     return plugin;
  //   }
  //   return nullptr;
  // }

//----------------------------------------------------------------------
#endif
