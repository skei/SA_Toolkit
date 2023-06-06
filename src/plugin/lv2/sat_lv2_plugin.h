#ifndef sat_lv2_plugin_included
#define sat_lv2_plugin_included
//----------------------------------------------------------------------

// clap-as-lv2

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/lv2/sat_lv2.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2Plugin {
  
//------------------------------
private:
//------------------------------

  LV2_Handle                      MHandle         = nullptr;
  const LV2_Descriptor*           MDescriptor     = nullptr;
  const char*                     MBundlePath     = nullptr;
  const LV2_Feature* const*       MFeatures       = nullptr;
  double                          MSampleRate     = 0.0;
  
  const clap_host_t*              MClapHost       = nullptr;
  const clap_plugin_t*            MClapPlugin     = nullptr;
  //const clap_plugin_descriptor_t* MClapDescriptor = nullptr;

  SAT_HostImplementation*         MHost           = nullptr;
  SAT_Plugin*                     MPlugin         = nullptr;
  
  uint32_t                        MNumInputs        = 0;
  uint32_t                        MNumOutputs       = 0;
  uint32_t                        MNumParameters    = 0;

  float**                         MInputPtrs        = nullptr;
  float**                         MOutputPtrs       = nullptr;
  float**                         MParameterPtrs    = nullptr;

  clap_process_t                  MProcess          = {};
  SAT_ProcessContext              MProcessContext   = {0};

  uint32_t                  MNumEvents         = 0;
  char                      MEvents[SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK * SAT_PLUGIN_MAX_EVENT_SIZE]  = {0};
  
  SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK> MHostParamQueue = {}; // gui -> host
  double                    MQueuedHostParamValues[SAT_PLUGIN_MAX_PARAMETERS] = {0};

  //LV2_URID                        MMidiInputUrid          = 0;
  //const LV2_Atom_Sequence*        MAtomSequence           = nullptr;
  //float*                          MParameterValues        = nullptr;
  //float*                          MHostValues             = nullptr;
  //float*                          MProcessValues          = nullptr;
  //float*                          MEditorParameterValues  = nullptr;
  //float*                          MHostParameterValues    = nullptr;
  //SAT_Lv2UpdateQueue              MHostParameterQueue;
  
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

  LV2_Handle getHandle() {
    return MHandle;
  }

//------------------------------
public:
//------------------------------

  /*
    oops...
    we need to know the index (or plugin_id) for the plugin we will create..
    we have the descriptor, which contains the lv2 uri, which we have set to 'urn:' + the clap id..
  */

  LV2_Handle lv2_instantiate(const LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
    
    MDescriptor  = descriptor;
    MSampleRate     = sample_rate;
    
    //LV2_URID_Map* urid_map = (LV2_URID_Map*)SAT_Lv2FindFeature(LV2_URID__map,features);
    //if (urid_map) {
    //  if (MDescriptor->canReceiveMidi()) {
    //    MMidiInputUrid = SAT_Lv2MapUrid(LV2_MIDI__MidiEvent,urid_map);
    //  }
    //}
    
    
//    MBundlePath     = bundle_path;
//    MFeatures       = features;

    MHost = new SAT_HostImplementation();
    const clap_host_t* clap_host = MHost->getHost();
    
    char plugin_id_buffer[SAT_MAX_NAME_LENGTH];
    strcpy(plugin_id_buffer,descriptor->URI);
    char* id = plugin_id_buffer + 4; // skip 'urn:'
    uint32_t index = SAT_GLOBAL.REGISTRY.findDescriptor(id);
    
    const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    MClapPlugin = SAT_CreatePlugin(index,clap_descriptor,clap_host);
    MClapPlugin->init(MClapPlugin);
    MPlugin = (SAT_Plugin*)MClapPlugin->plugin_data;
    //SAT_Print("MPlugin %p\n",MPlugin);
    
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

    //LV2_URID_Map* urid_map = (LV2_URID_Map*)sat_lv2_find_feature(LV2_URID__map,features);
    //if (urid_map) {
    //  if (MDescriptor->canReceiveMidi()) {
    //    MMidiInputUrid = sat_lv2_map_urid(LV2_MIDI__MidiEvent,urid_map);
    //  }
    //}

    return MHandle;
  }

  //----------

  /*
     Connect a port on a plugin instance to a memory location.

     Plugin writers should be aware that the host may elect to use the same
     buffer for more than one port and even use the same buffer for both
     input and output (see lv2:inPlaceBroken in lv2.ttl).

     If the plugin has the feature lv2:hardRTCapable then there are various
     things that the plugin MUST NOT do within the connect_port() function;
     see lv2core.ttl for details.

     connect_port() MUST be called at least once for each port before run()
     is called, unless that port is lv2:connectionOptional. The plugin must
     pay careful attention to the block size passed to run() since the block
     allocated may only just be large enough to contain the data, and is not
     guaranteed to remain constant between run() calls.

     connect_port() may be called more than once for a plugin instance to
     allow the host to change the buffers that the plugin is reading or
     writing. These calls may be made before or after activate() or
     deactivate() calls.

     @param instance Plugin instance containing the port.

     @param port Index of the port to connect. The host MUST NOT try to
     connect a port index that is not defined in the plugin's RDF data. If
     it does, the plugin's behaviour is undefined (a crash is likely).

     @param data_location Pointer to data of the type defined by the port
     type in the plugin's RDF data (for example, an array of float for an
     lv2:AudioPort). This pointer must be stored by the plugin instance and
     used to read/write data when run() is called. Data present at the time
     of the connect_port() call MUST NOT be considered meaningful.
  */

  // we assume the ports are in this order:
  // inputs, outputs, parameters, midi_in

  void lv2_connect_port(uint32_t port, void* data_location) {
    SAT_PRINT;
    //SAT_Print("lv2_connect_port: port %i data_location 0x%x\n",port,data_location);
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

//    if (MDescriptor->canReceiveMidi()) {
//      MAtomSequence = (const LV2_Atom_Sequence*)data_location;
//      port -= 1;
//    }

  }
  
  //----------

  /*
     Initialise a plugin instance and activate it for use.

     This is separated from instantiate() to aid real-time support and so
     that hosts can reinitialise a plugin instance by calling deactivate()
     and then activate(). In this case the plugin instance MUST reset all
     state information dependent on the history of the plugin instance except
     for any data locations provided by connect_port(). If there is nothing
     for activate() to do then this field may be NULL.

     When present, hosts MUST call this function once before run() is called
     for the first time. This call SHOULD be made as close to the run() call
     as possible and indicates to real-time plugins that they are now live,
     however plugins MUST NOT rely on a prompt call to run() after
     activate().

     The host MUST NOT call activate() again until deactivate() has been
     called first. If a host calls activate(), it MUST call deactivate() at
     some point in the future. Note that connect_port() may be called before
     or after activate().
  */

  void lv2_activate() {
    SAT_PRINT;
    //on_plugin_open();
    //on_plugin_activate();
    MPlugin->activate(MSampleRate,0,1024);
  }
  
  //----------

  /*
     Run a plugin instance for a block.

     Note that if an activate() function exists then it must be called before
     run(). If deactivate() is called for a plugin instance then run() may
     not be called until activate() has been called again.

     If the plugin has the feature lv2:hardRTCapable then there are various
     things that the plugin MUST NOT do within the run() function (see
     lv2core.ttl for details).

     As a special case, when `sample_count` is 0, the plugin should update
     any output ports that represent a single instant in time (for example,
     control ports, but not audio ports). This is particularly useful for
     latent plugins, which should update their latency output port so hosts
     can pre-roll plugins to compute latency. Plugins MUST NOT crash when
     `sample_count` is 0.

     @param instance Instance to be run.

     @param sample_count The block size (in samples) for which the plugin
     instance must run.
  */

  void lv2_run(uint32_t sample_count) {
    SAT_PRINT;
    //SAT_Print("lv2_run: %i\n",sample_count);

    // parameters

    for (uint32_t i=0; i<MNumParameters; i++) {
      float v = *MParameterPtrs[i];
      //MHostValues[i] = v;
      //if (v != MProcessValues[i]) { // todo: slmilar_to?
      //  MProcessValues[i] = v;
      if (v != MPlugin->getParameterValue(i)) { // todo: almost_equal?
        MPlugin->setParameterValue(i,v); 
        // to/from01 ??
        //SAT_Parameter* param = MPlugin->getParameter(i);
        //if (param) v = param->from01(v);
        //MInstance->on_plugin_parameter(i,v);
      }
    }
    
    // midi
    
    //if (MDescriptor->canReceiveMidi()) {
    //  uint32_t offset = 0;
    //  LV2_ATOM_SEQUENCE_FOREACH(MAtomSequence, ev) {
    //    if (ev->body.type == MMidiInputUrid) {
    //      const uint8_t* const msg = (const uint8_t*)(ev + 1);
    //      offset = (uint32_t)ev->time.frames;
    //      MInstance->on_plugin_midi(offset,msg[0],msg[1],msg[2]);
    //    }
    //  }
    //}
    
    //MProcessContext.numinputs = MDescriptor->getNumInputs();
    //MProcessContext.numoutputs = MDescriptor->getNumOutputs();
    //for (uint32_t i=0; i<MProcessContext.numinputs; i++)  { MProcessContext.inputs[i]  = MInputPtrs[i]; }
    //for (uint32_t i=0; i<MProcessContext.numoutputs; i++) { MProcessContext.outputs[i] = MOutputPtrs[i]; }
    //MProcessContext.numsamples  = sample_count;
    //MProcessContext.samplerate  = MSampleRate;
    ////context.offset      = 0;
    ////context.oversample  = 1;

    //MInstance->on_plugin_process(&MProcessContext);
    
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
    
    //MProcess.in_events            = &MLadspaInputEvents;
    //MProcess.out_events           = &MLadspaOutputEvents;

    //MProcessContext.process       = &MProcess;
    //MProcessContext.samplerate    = 0.0;
    //MProcessContext.minbufsize    = 0;
    //MProcessContext.maxbufsize    = 0;
    //MProcessContext.parameters    = MParameters;
    //MProcessContext.counter       = 0;
    
    MPlugin->process(&MProcess);
    MProcess.steady_time += sample_count;
    
    //todo: flush midi
    
  }
  
  //----------

  /*
     Deactivate a plugin instance (counterpart to activate()).

     Hosts MUST deactivate all activated instances after they have been run()
     for the last time. This call SHOULD be made as close to the last run()
     call as possible and indicates to real-time plugins that they are no
     longer live, however plugins MUST NOT rely on prompt deactivation. If
     there is nothing for deactivate() to do then this field may be NULL

     Deactivation is not similar to pausing since the plugin instance will be
     reinitialised by activate(). However, deactivate() itself MUST NOT fully
     reset plugin state. For example, the host may deactivate a plugin, then
     store its state (using some extension to do so).

     Hosts MUST NOT call deactivate() unless activate() was previously
     called. Note that connect_port() may be called before or after
     deactivate().
  */

  void lv2_deactivate() {
    SAT_PRINT;
    MPlugin->deactivate();
  }
  
  //----------

  /*
     Clean up a plugin instance (counterpart to instantiate()).

     Once an instance of a plugin has been finished with it must be deleted
     using this function. The instance handle passed ceases to be valid after
     this call.

     If activate() was called for a plugin instance then a corresponding call
     to deactivate() MUST be made before cleanup() is called. Hosts MUST NOT
     call cleanup() unless instantiate() was previously called.
  */

  void lv2_cleanup() {
    SAT_PRINT;

    if (MClapPlugin) MClapPlugin->destroy(MClapPlugin);
    if (MHost) delete MHost;
    
    if (MInputPtrs) free(MInputPtrs);
    if (MOutputPtrs) free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);

    //if (MParameterValues) free(MParameterValues);
    //if (MHostValues) free(MHostValues);
    //if (MProcessValues) free(MProcessValues);
    
  }
  
//------------------------------
private: // in_events
//------------------------------

  uint32_t input_events_size() {
    return MNumEvents;
  }

  //----------

  const clap_event_header_t* input_events_get(uint32_t index) {
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

  void queueHostParam(uint32_t AIndex, double AValue) {
    MQueuedHostParamValues[AIndex] = AValue;
    MHostParamQueue.write(AIndex);
  }

  //----------

  void flushHostParams() {
    uint32_t index;
    while (MHostParamQueue.read(&index)) {
      double value = MQueuedHostParamValues[index];
      SAT_Print("flush! %i = %.3f\n",index,value);
    }
  }

};

//----------------------------------------------------------------------
#endif


