#ifndef sat_lv2_plugin_included
#define sat_lv2_plugin_included
//----------------------------------------------------------------------

// clap-as-lv2

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/lv2/sat_lv2.h"
#include "plugin/lv2/sat_lv2_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2Plugin {
  
//------------------------------
private:
//------------------------------

//  LV2_Handle                      MHandle         = nullptr;
  const LV2_Descriptor*           MDescriptor     = nullptr;
  const char*                     MBundlePath     = nullptr;
  const LV2_Feature* const*       MFeatures       = nullptr;
  double                          MSampleRate     = 0.0;
  
  const clap_host_t*              MClapHost       = nullptr;
  const clap_plugin_t*            MClapPlugin     = nullptr;
  clap_process_t                  MProcess        = {0};

  SAT_HostImplementation*         MHost           = nullptr;
  SAT_Plugin*                     MPlugin         = nullptr;
  
  uint32_t                        MNumEvents      = 0;
  char                            MEvents[SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK * SAT_PLUGIN_MAX_EVENT_SIZE]  = {0};

  uint32_t                        MNumInputs      = 0;
  uint32_t                        MNumOutputs     = 0;
  uint32_t                        MNumParameters  = 0;

  float**                         MInputPtrs      = nullptr;
  float**                         MOutputPtrs     = nullptr;
  float**                         MParameterPtrs  = nullptr;
  
  LV2_URID                        MMidiInputUrid  = 0;
  const LV2_Atom_Sequence*        MAtomSequence   = nullptr;
  
  // editor
  
//------------------------------
public:
//------------------------------

  SAT_Lv2Plugin() {
    SAT_PRINT;
    
  }
  
  //----------
  
  ~SAT_Lv2Plugin() {
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  //LV2_Handle getHandle() {
  //  return MHandle;
  //}

  const LV2_Descriptor* getDescriptor() {
    return MDescriptor;
  }
  
  const clap_plugin_t* getClapPlugin() {
    return MClapPlugin;
  }
  
//------------------------------
public:
//------------------------------

  LV2_Handle lv2_plugin_instantiate(const LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
    
    SAT_Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    SAT_Lv2PrintFeatures(features);
    
    //---

    MDescriptor = descriptor;
    MSampleRate = sample_rate;
    MBundlePath = bundle_path;
    MFeatures = features;

    MHost = new SAT_HostImplementation();
    const clap_host_t* clap_host = MHost->getClapHost();
    
    char plugin_id_buffer[SAT_MAX_NAME_LENGTH];
    strcpy(plugin_id_buffer,descriptor->URI);
    char* id = plugin_id_buffer + 4; // skip 'urn:'
    uint32_t index = SAT_GLOBAL.REGISTRY.findDescriptor(id);
    
    const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    MClapPlugin = SAT_CreatePlugin(index,clap_descriptor,clap_host);
    MClapPlugin->init(MClapPlugin);
    MPlugin = (SAT_Plugin*)MClapPlugin->plugin_data;
    //SAT_Print("MPlugin %p\n",MPlugin);
    MPlugin->setPluginFormat("LV2");
    
    // ports
    
    clap_audio_port_info_t info;
    
    if (MPlugin->audio_ports_count(true) > 0) {
      if (MPlugin->audio_ports_get(0,true,&info))  {
        MNumInputs = info.channel_count;
      }
    }
    
    if (MPlugin->audio_ports_count(false) > 0) {
      if (MPlugin->audio_ports_get(0,false,&info))  {
        MNumOutputs = info.channel_count;
      }
    }

    MNumParameters = MPlugin->getNumParameters();
    
    MInputPtrs      = (float**)malloc(MNumInputs     * sizeof(float*));
    MOutputPtrs     = (float**)malloc(MNumOutputs    * sizeof(float*));
    MParameterPtrs  = (float**)malloc(MNumParameters * sizeof(float*));
    
    // midi

    LV2_URID_Map* urid_map = (LV2_URID_Map*)SAT_Lv2FindFeature(LV2_URID__map,features);
    if (urid_map) {
      MMidiInputUrid = SAT_Lv2MapUrid(LV2_MIDI__MidiEvent,urid_map);
    }
    
    return this;
  }

  //----------
  
  void lv2_plugin_cleanup() {
    SAT_PRINT;
    if (MClapPlugin) MClapPlugin->destroy(MClapPlugin);
    if (MHost) delete MHost;
    if (MInputPtrs) free(MInputPtrs);
    if (MOutputPtrs) free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
  }
  
  //----------

  void lv2_plugin_activate() {
    SAT_PRINT;
    MPlugin->activate(MSampleRate,0,1024);
    MPlugin->start_processing();
  }
  
  //----------

  void lv2_plugin_deactivate() {
    SAT_PRINT;
    MPlugin->stop_processing();
    MPlugin->deactivate();
  }
  
  //----------

  // we assume the ports are in this order:
  // inputs, outputs, parameters, midi_in

  void lv2_plugin_connect_port(uint32_t port, void* data_location) {
    //SAT_Print("port %i data_location %p\n",port,data_location);
    
    if (port < MNumInputs) {
      MInputPtrs[port] = (float*)data_location;
      return;
    }
    port -= MNumInputs;
    if (port < MNumOutputs) {
      MOutputPtrs[port] = (float*)data_location;
      return;
    }
    port -= MNumOutputs;
    if (port < MNumParameters) {
      MParameterPtrs[port] = (float*)data_location;
      return;
    }
    port -= MNumParameters;

    //if (MDescriptor->canReceiveMidi()) {
      MAtomSequence = (const LV2_Atom_Sequence*)data_location;
      port -= 1;
    //}

  }
  
  //----------

  void lv2_plugin_run(uint32_t sample_count) {
    //SAT_Print("lv2_run: %i\n",sample_count);
    
    // todo: convert to events + sort (params/midi)..
    // (see vst3)

    // parameters

    for (uint32_t i=0; i<MNumParameters; i++) {
      float v = *MParameterPtrs[i];
      //MHostValues[i] = v;
      //if (v != MProcessValues[i]) { // todo: slmilar_to?
      //  MProcessValues[i] = v;
      if (v != MPlugin->getParameterValue(i)) { // todo: almost_equal?
        MPlugin->setParameterValue(i,v); 
        clap_event_param_value_t event;
        event.header.size     = sizeof(clap_event_param_value_t);
        event.header.time     = 0;
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_VALUE;
        event.header.flags    = 0;
        event.param_id        = i;
        event.cookie          = nullptr;
        event.note_id         = -1;
        event.port_index      = -1;
        event.channel         = 1;
        event.key             = -1;
        event.value           = v;
        MPlugin->handleParamValueEvent(&event);
      }
    }
    
    // midi
    
    //if (MDescriptor->canReceiveMidi()) {
    uint32_t offset = 0;
    LV2_ATOM_SEQUENCE_FOREACH(MAtomSequence, ev) {
      if (ev->body.type == MMidiInputUrid) {
        const uint8_t* const msg = (const uint8_t*)(ev + 1);
        SAT_Print("midi %02x.%02x.%02x\n",msg[0],msg[1],msg[2]);
        offset = (uint32_t)ev->time.frames;
        clap_event_midi_t event;
        event.header.size     = sizeof(clap_event_midi_t);
        event.header.time     = offset;
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_MIDI;
        event.header.flags    = 0;
        event.port_index      = -1;
        event.data[0]         = msg[0];
        event.data[1]         = msg[1];
        event.data[2]         = msg[2];
        MPlugin->handleMidiEvent(&event);
      }
    }
    //}
    
    if (sample_count > 0) {
    
      clap_audio_buffer_t audio_inputs = {
        .data32         = MInputPtrs,
        .data64         = nullptr,
        .channel_count  = MNumInputs,
        .latency        = 0,
        .constant_mask  = 0
      };
      
      clap_audio_buffer_t audio_outputs = {
        .data32         = MOutputPtrs,
        .data64         = nullptr,
        .channel_count  = MNumOutputs,
        .latency        = 0,
        .constant_mask  = 0
      };

      MProcess.frames_count         = sample_count;
      MProcess.transport            = nullptr;
      MProcess.audio_inputs         = &audio_inputs;
      MProcess.audio_outputs        = &audio_outputs;
      MProcess.audio_inputs_count   = MNumInputs;
      MProcess.audio_outputs_count  = MNumOutputs;
      MProcess.in_events            = &MLadspaInputEvents;
      MProcess.out_events           = &MLadspaOutputEvents;
      MPlugin->process(&MProcess);
      MProcess.steady_time         += sample_count;
    
    }
    
    //todo: flush midi (midiout)
    
  }
  
  //----------

//------------------------------
private: // in_events
//------------------------------

  uint32_t input_events_size() {
    SAT_Print("-> %i\n",MNumEvents);
    return MNumEvents;
  }

  //----------

  const clap_event_header_t* input_events_get(uint32_t index) {
    SAT_Print("index %i\n",index);
    uint32_t pos = SAT_PLUGIN_MAX_EVENT_SIZE * index;
    return (const clap_event_header_t*)&MEvents[pos];
  }

  //----------

  clap_input_events_t MLadspaInputEvents = {
    this,
    input_events_size_callback,
    input_events_get_callback
  };

  //----------

  static uint32_t input_events_size_callback(const struct clap_input_events *list) {
    SAT_Lv2Plugin* plugin = (SAT_Lv2Plugin*)list->ctx;
    return plugin->input_events_size();
  }

  //----------

  static const clap_event_header_t* input_events_get_callback(const struct clap_input_events *list, uint32_t index) {
    SAT_Lv2Plugin* plugin = (SAT_Lv2Plugin*)list->ctx;
    return plugin->input_events_get(index);
  }

//------------------------------
private: // out_events
//------------------------------

  bool output_events_try_push(const clap_event_header_t *event) {
    SAT_PRINT;
    if (event->space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (event->type) {
        
        case CLAP_EVENT_NOTE_ON:
          //SAT_Print("TODO: send NOTE_ON to host\n");
          return true;
          
        case CLAP_EVENT_NOTE_OFF:
          //SAT_Print("TODO: send NOTE_OFF to host\n");
          return true;
          
        case CLAP_EVENT_NOTE_CHOKE:
          //SAT_Print("TODO: send NOTE_CHOKE to host\n");
          return true;
          
        case CLAP_EVENT_NOTE_END: {
          //SAT_Print("TODO: send NOTE_END to host\n");
          return true;
        }
          
        case CLAP_EVENT_NOTE_EXPRESSION:
          //SAT_Print("TODO: send NOTE_EXPRESSION to host\n");
          return true;
          
        case CLAP_EVENT_PARAM_GESTURE_BEGIN: {
          //SAT_Print("TODO: send PARAM_GESTURE_BEGIN to host\n");
          return true;
        }
          
        case CLAP_EVENT_PARAM_GESTURE_END: {
          //SAT_Print("TODO: send PARAM_GESTURE_END to host\n");
          return true;
        }

        case CLAP_EVENT_PARAM_VALUE: {
          //SAT_Print("queueing PARAM_VALUE (to host)\n");
          //clap_event_param_value_t* param_value = (clap_event_param_value_t*)event;
          //uint32_t index = param_value->param_id;
          //double value = param_value->value;
          //clap_param_info_t info;
          //MPlugin->params_get_info(index,&info); // crash? (win32)
          //double range = info.max_value - info.min_value;
          //if (range > 0) {
          //  value -= info.min_value;
          //  value /= range;
          //}
          //queueHostParam(index,value);
          return true;
        }

        case CLAP_EVENT_PARAM_MOD:
          //SAT_Print("TODO: send PARAM_MOD to host\n");
          return true;
          
        case CLAP_EVENT_TRANSPORT:
          //SAT_Print("TODO: send TRANSPORT to host\n");
          return true;
          
        case CLAP_EVENT_MIDI:
          //SAT_Print("TODO: send MIDI to host\n");
          return true;
          
        case CLAP_EVENT_MIDI_SYSEX:
          //SAT_Print("TODO: send MIDI_SYSEX to host\n");
          return true;
          
        case CLAP_EVENT_MIDI2:
          //SAT_Print("TODO: send MIDI2 to host\n");
          return true;
          
      } // switch
    } // clap_space
    return false;
  }

  //----------

  clap_output_events_t MLadspaOutputEvents = {
    this,
    output_events_try_push_callback
  };

  //----------

  static bool output_events_try_push_callback(const struct clap_output_events *list, const clap_event_header_t *event) {
    SAT_Lv2Plugin* plugin = (SAT_Lv2Plugin*)list->ctx;
    return plugin->output_events_try_push(event);
  }

//------------------------------
private:
//------------------------------

  //void queueHostParam(uint32_t AIndex, double AValue) {
  //  SAT_Print("AIndex %i AValue %f\n",AIndex,AValue);
  //  MQueuedHostParamValues[AIndex] = AValue;
  //  MHostParamQueue.write(AIndex);
  //}

  //----------

  //void flushHostParams() {
  //  SAT_PRINT;
  //  uint32_t index;
  //  while (MHostParamQueue.read(&index)) {
  //    double value = MQueuedHostParamValues[index];
  //    SAT_Print("flush! %i = %.3f\n",index,value);
  //  }
  //}

};

//----------------------------------------------------------------------
#endif


