#ifndef sat_ladspa_plugin_included
#define sat_ladspa_plugin_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/ladspa/sat_ladspa.h"

//#include "kode.h"
//#include "plugin/kode_descriptor.h"
//#include "plugin/kode_instance.h"
//#include "plugin/kode_process_context.h"
//#include "plugin/ladspa/kode_ladspa.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LadspaPlugin {

//------------------------------
private:
//------------------------------

  SAT_Plugin*               MPlugin           = nullptr;
  SAT_HostImplementation*   MHost             = nullptr;

  const clap_plugin_t*      MClapPlugin       = nullptr;

  const LADSPA_Descriptor*  MLadspaDescriptor = nullptr;
  uint32_t                  MSampleRate       = 0.0f;
  LADSPA_Handle             MLadspaHandle     = nullptr;

  float**                   MInputPtrs        = nullptr;
  float**                   MOutputPtrs       = nullptr;
  float**                   MParameterPtrs    = nullptr;

  uint32_t                  MNumInputs        = 0;
  uint32_t                  MNumOutputs       = 0;
  uint32_t                  MNumParameters    = 0;

  SAT_ParameterArray*       MParameters       = nullptr;
  clap_process_t            MProcess          = {};

  uint32_t                  MNumEvents         = 0;
  char                      MEvents[SAT_PLUGIN_LADSPA_MAX_EVENTS_PER_BLOCK * SAT_PLUGIN_LADSPA_MAX_EVENT_SIZE]  = {0};
  
  SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_LADSPA_MAX_GUI_EVENTS> MHostParamQueue = {}; // gui -> host
  double                    MQueuedHostParamValues[SAT_PLUGIN_VST3_MAX_EVENTS_PER_BLOCK] = {0};

  //float*                    MHostValues       = nullptr;
  //float*                    MProcessValues    = nullptr;
  //SAT_ProcessContext        MProcessContext   = {};
  //clap_input_events_t       MLadspaInputEvents
  //clap_input_events_t       MLadspaOutputEvents

//------------------------------
public:
//------------------------------

  SAT_LadspaPlugin() {
    SAT_PRINT;
  }

  //----------

  virtual ~SAT_LadspaPlugin() {
    SAT_PRINT;
  }
  
  //----------

  LADSPA_Handle getHandle() {
    return MLadspaHandle;
  }

//------------------------------
public:
//------------------------------

  /*
    This member is a function pointer that instantiates a plugin. A
    handle is returned indicating the new plugin instance. The
    instantiation function accepts a sample rate as a parameter. The
    plugin descriptor from which this instantiate function was found
    must also be passed. This function must return NULL if
    instantiation fails.

    Note that instance initialisation should generally occur in
    activate() rather than here.
  */

  void ladspa_instantiate(const LADSPA_Descriptor* Descriptor, uint32_t ASampleRate) {
    SAT_PRINT;
    
    MLadspaDescriptor = Descriptor;
    MSampleRate = ASampleRate;

    MHost = new SAT_HostImplementation();
    const clap_host_t* clap_host = MHost->getHost();
    SAT_LadspaDescriptorInfo* descriptor_info = (SAT_LadspaDescriptorInfo*)MLadspaDescriptor->ImplementationData;
    uint32_t index = descriptor_info->index;
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
    
    //MHostValues     = (float*)malloc(MNumParameters * sizeof(float ));
    //MProcessValues  = (float*)malloc(MNumParameters * sizeof(float ));
    //instance->on_open();
    //MInstance->on_initialize(); // open?

  }
  
  //----------

  /*
    This member is a function pointer that connects a port on an
    instantiated plugin to a memory location at which a block of data
    for the port will be read/written. The data location is expected
    to be an array of LADSPA_Data for audio ports or a single
    LADSPA_Data value for control ports. Memory issues will be
    managed by the host. The plugin must read/write the data at these
    locations every time run() or run_adding() is called and the data
    present at the time of this connection call should not be
    considered meaningful.

    connect_port() may be called more than once for a plugin instance
    to allow the host to change the buffers that the plugin is
    reading or writing. These calls may be made before or after
    activate() or deactivate() calls.

    connect_port() must be called at least once for each port before
    run() or run_adding() is called. When working with blocks of
    LADSPA_Data the plugin should pay careful attention to the block
    size passed to the run function as the block allocated may only
    just be large enough to contain the block of samples.

    Plugin writers should be aware that the host may elect to use the
    same buffer for more than one port and even use the same buffer
    for both input and output (see LADSPA_PROPERTY_INPLACE_BROKEN).
    However, overlapped buffers or use of a single buffer for both
    audio and control data may result in unexpected behaviour.
  */

  //----------

  void ladspa_connect_port(unsigned long Port, LADSPA_Data * DataLocation) {
    //SAT_PRINT;
    //SAT_Print("ladspa: connect_port Port:%i DataLocation:%p\n",Port,DataLocation);
    if (Port < MNumInputs) {
      MInputPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumInputs;
    if (Port < MNumOutputs) {
      MOutputPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumOutputs;
    if (Port < MNumParameters) {
      MParameterPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumParameters;
  }

  //----------

  /*
    This member is a function pointer that initialises a plugin
    instance and activates it for use. This is separated from
    instantiate() to aid real-time support and so that hosts can
    reinitialise a plugin instance by calling deactivate() and then
    activate(). In this case the plugin instance must reset all state
    information dependent on the history of the plugin instance
    except for any data locations provided by connect_port() and any
    gain set by set_run_adding_gain(). If there is nothing for
    activate() to do then the plugin writer may provide a NULL rather
    than an empty function.

    When present, hosts must call this function once before run() (or
    run_adding()) is called for the first time. This call should be
    made as close to the run() call as possible and indicates to
    real-time plugins that they are now live. Plugins should not rely
    on a prompt call to run() after activate(). activate() may not be
    called again unless deactivate() is called first. Note that
    connect_port() may be called before or after a call to
    activate().
  */

  void ladspa_activate() {
    SAT_PRINT;
    if (MPlugin) {
      MPlugin->activate(MSampleRate,0,1024);
      //MSampleRate = MPlugin->getSampleRate();
      //MInstance->on_stateChange(kps_initialize);
      //MInstance->on_initialize();
      //MInstance->on_activate();
    }
  }

  //----------

  /*
    This method is a function pointer that runs an instance of a
    plugin for a block. Two parameters are required: the first is a
    handle to the particular instance to be run and the second
    indicates the block size (in samples) for which the plugin
    instance may run.

    Note that if an activate() function exists then it must be called
    before run() or run_adding(). If deactivate() is called for a
    plugin instance then the plugin instance may not be reused until
    activate() has been called again.

    If the plugin has the property LADSPA_PROPERTY_HARD_RT_CAPABLE
    then there are various things that the plugin should not do
    within the run() or run_adding() functions (see above).
  */

  void ladspa_run(unsigned long SampleCount) {
    //SAT_PRINT;
    if (MPlugin) {
      
      MNumEvents = 0;
      for (uint32_t i=0; i<MNumParameters; i++) {
        float v = *MParameterPtrs[i];
        //MHostValues[i] = v;
        if (v != MPlugin->getParameterValue(i)) { // todo: almost_equal?
          MPlugin->setParameterValue(i,v); 
          //SAT_Parameter* param = MPlugin->getParameter(i);
          //v = param->denormalizeValue(v); // from01(v);
          MPlugin->setParameterValue(i,v);
          uint32_t pos = SAT_PLUGIN_LADSPA_MAX_EVENT_SIZE * MNumEvents++;
          clap_event_param_value_t* param_value_event = (clap_event_param_value_t*)&MEvents[pos];
          memset(param_value_event,0,sizeof(clap_event_param_value_t));
          param_value_event->header.size     = sizeof(clap_event_param_value_t);
          param_value_event->header.time     = 0;
          param_value_event->header.space_id = CLAP_CORE_EVENT_SPACE_ID;
          param_value_event->header.type     = CLAP_EVENT_PARAM_VALUE;
          param_value_event->header.flags    = 0;
          param_value_event->param_id        = i; //paramQueue->getParameterId();
          param_value_event->note_id         = -1;
          param_value_event->port_index      = 0;
          param_value_event->channel         = 0;
          param_value_event->key             = 0;
          param_value_event->value           = v;
        }
      }
      
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

      MProcess.frames_count         = SampleCount;
      MProcess.transport            = nullptr;
      MProcess.audio_inputs         = &audio_inputs;
      MProcess.audio_outputs        = &audio_outputs;
      MProcess.audio_inputs_count   = MNumInputs;
      MProcess.audio_outputs_count  = MNumOutputs;
      MProcess.in_events            = &MLadspaInputEvents;
      MProcess.out_events           = &MLadspaOutputEvents;

      //MProcessContext.process     = &MProcess;
      //MProcessContext.samplerate  = 0.0;
      //MProcessContext.minbufsize  = 0;
      //MProcessContext.maxbufsize  = 0;
      //MProcessContext.parameters  = MParameters;
      //MProcessContext.counter     = 0;
      
      MPlugin->process(&MProcess);
      MProcess.steady_time += SampleCount;

    }
  }

  //----------

  /*
    This method is a function pointer that runs an instance of a
    plugin for a block. This has identical behaviour to run() except
    in the way data is output from the plugin. When run() is used,
    values are written directly to the memory areas associated with
    the output ports. However when run_adding() is called, values
    must be added to the values already present in the memory
    areas. Furthermore, output values written must be scaled by the
    current gain set by set_run_adding_gain() (see below) before
    addition.

    run_adding() is optional. When it is not provided by a plugin,
    this function pointer must be set to NULL. When it is provided,
    the function set_run_adding_gain() must be provided also.
  */

  void ladspa_run_adding(unsigned long SampleCount) {
    //SAT_PRINT;
  }

  //----------

  /* This method is a function pointer that sets the output gain for
     use when run_adding() is called (see above). If this function is
     never called the gain is assumed to default to 1. Gain
     information should be retained when activate() or deactivate()
     are called.

     This function should be provided by the plugin if and only if the
     run_adding() function is provided. When it is absent this
     function pointer must be set to NULL.
  */

  void ladspa_set_run_adding_gain(LADSPA_Data Gain) {
    //SAT_PRINT;
  }

  //----------

  /*
    This is the counterpart to activate() (see above). If there is
    nothing for deactivate() to do then the plugin writer may provide
    a NULL rather than an empty function.

    Hosts must deactivate all activated units after they have been
    run() (or run_adding()) for the last time. This call should be
    made as close to the last run() call as possible and indicates to
    real-time plugins that they are no longer live. Plugins should
    not rely on prompt deactivation. Note that connect_port() may be
    called before or after a call to deactivate().

    Deactivation is not similar to pausing as the plugin instance
    will be reinitialised when activate() is called to reuse it.
  */

  void ladspa_deactivate() {
    SAT_PRINT;
    if (MPlugin) {
      MPlugin->deactivate();
      //MInstance->on_deactivate();
      //MInstance->on_stop();
    }
  }

  //----------

  /*
    Once an instance of a plugin has been finished with it can be
    deleted using the following function. The instance handle passed
    ceases to be valid after this call.

    If activate() was called for a plugin instance then a
    corresponding call to deactivate() must be made before cleanup()
    is called.
  */

  void ladspa_cleanup() {
    SAT_PRINT;

    //if (MInstance) {
    //  //MInstance->on_terminate();
    //  //MInstance->on_close();
    //}

    SAT_LadspaDescriptorInfo* descriptor_info = (SAT_LadspaDescriptorInfo*)MLadspaDescriptor->ImplementationData;
    if (descriptor_info) {
      if (descriptor_info->descriptor) free(descriptor_info->descriptor);
      if (descriptor_info->ports) {
        //cleanup_ports(&MPorts);
        if (descriptor_info->ports->descriptors)   free(descriptor_info->ports->descriptors);
        if (descriptor_info->ports->names)         free(descriptor_info->ports->names);
        if (descriptor_info->ports->namesBuffer)   free(descriptor_info->ports->namesBuffer);
        if (descriptor_info->ports->rangeHints)    free(descriptor_info->ports->rangeHints);
        free(descriptor_info->ports);
      }
      free(descriptor_info);
    }
    
    if (MClapPlugin) MClapPlugin->destroy(MClapPlugin);
    if (MHost) delete MHost;
    
    if (MInputPtrs)     free(MInputPtrs);
    if (MOutputPtrs)    free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
    
//    if (MHostValues)    free(MHostValues);
//    if (MProcessValues) free(MProcessValues);

  }

//------------------------------
private: // in_events
//------------------------------

  uint32_t input_events_size() {
    return MNumEvents;
  }

  //----------

  const clap_event_header_t* input_events_get(uint32_t index) {
    uint32_t pos = SAT_PLUGIN_LADSPA_MAX_EVENT_SIZE * index;
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
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)list->ctx;
    return plugin->input_events_size();
  }

  //----------

  static const clap_event_header_t* input_events_get_callback(const struct clap_input_events *list, uint32_t index) {
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)list->ctx;
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
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)list->ctx;
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


