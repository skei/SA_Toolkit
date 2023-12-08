#ifndef sat_ladspa_hosted_plugin_included
#define sat_ladspa_hosted_plugin_included
//----------------------------------------------------------------------

// ladspa-as-clap

#include "sat.h"
//#include "plugin/clap/sat_clap_host.h"
#include "plugin/ladspa/sat_ladspa.h"

//----------------------------------------------------------------------

#define SAT_LADSPA_MAX_AUDIO_INPUTS       32
#define SAT_LADSPA_MAX_AUDIO_OUTPUTS      32
#define SAT_LADSPA_MAX_CONTROL_INPUTS     1024
#define SAT_LADSPA_MAX_CONTROL_OUTPUTS    1024

//#define SAT_LADSPA_MAX_PORTS              1024
#define SAT_LADSPA_MAX_PORTS              ( SAT_LADSPA_MAX_AUDIO_INPUTS \
                                          + SAT_LADSPA_MAX_AUDIO_OUTPUTS \
                                          + SAT_LADSPA_MAX_CONTROL_INPUTS \
                                          + SAT_LADSPA_MAX_CONTROL_OUTPUTS )

#define SAT_LADSPA_FLAGS_NONE             0
#define SAT_LADSPA_FLAGS_REALTIME         1
#define SAT_LADSPA_FLAGS_INPLACE_BROKEN   2
#define SAT_LADSPA_FLAGS_HARD_RT_CAPABLE  4

#define SAT_LADSPA_PORT_NONE              0
#define SAT_LADSPA_PORT_AUDIO             1
#define SAT_LADSPA_PORT_CONTROL           2
#define SAT_LADSPA_PORT_INPUT             4
#define SAT_LADSPA_PORT_OUTPUT            8

#define SAT_LADSPA_PARAM_NONE             0
#define SAT_LADSPA_PARAM_TOGGLED          1
#define SAT_LADSPA_PARAM_SAMPLERATE       2
#define SAT_LADSPA_PARAM_LOGARITHMIC      4
#define SAT_LADSPA_PARAM_INTEGER          8

struct SAT_LadspaPort {
  const char* name    = nullptr;
  uint32_t    flags   = 0;
  float       minval  = 0.0;
  float       maxval  = 1.0;
  float       defval  = 0.0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LadspaHostedPlugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  char                      MPath[1025]                                         = {0};

  void*                     MLibHandle                                          = nullptr;                // loadLadspaPlugin
  bool                      MIsInstantiated                                     = false;                  // ladspa_instantiate
  bool                      MIsActivated                                        = false;                  // ladspa_activate

  LADSPA_Handle             MLadspaHandle                                       = nullptr;                // ladspa_instantiate
  const LADSPA_Descriptor*  MLadspaDescriptor                                   = nullptr;                // loadLadspaPlugin
  uint32_t                  MLadspaFlags                                        = SAT_LADSPA_FLAGS_NONE;  
  SAT_LadspaPort            MLadspaPorts[SAT_LADSPA_MAX_PORTS]                  = {};
  
  uint32_t                  MNumAudioInputs                                     = 0;
  uint32_t                  MNumAudioOutputs                                    = 0;
  uint32_t                  MNumControlInputs                                   = 0;
  uint32_t                  MNumControlOutputs                                  = 0;

  int32_t                   MAudioInputIndex[SAT_LADSPA_MAX_AUDIO_INPUTS]       = {0};
  int32_t                   MAudioOutputIndex[SAT_LADSPA_MAX_AUDIO_OUTPUTS]     = {0};
  int32_t                   MControlInputIndex[SAT_LADSPA_MAX_CONTROL_INPUTS]   = {0};
  int32_t                   MControlOutputIndex[SAT_LADSPA_MAX_CONTROL_OUTPUTS] = {0};

  float                     MParamValues[SAT_LADSPA_MAX_CONTROL_INPUTS]         = {0};
  float                     MParamOutputs[SAT_LADSPA_MAX_CONTROL_OUTPUTS]       = {0};

//------------------------------
public:
//------------------------------

  // MDescriptor->id:
  // plugin path (.so) + # + hex encoded index

  SAT_LadspaHostedPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost, uint32_t AIndex=0)
  : SAT_Plugin(ADescriptor,AHost) {
    registerDefaultExtensions();
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    strncpy(MPath,ADescriptor->id,1024);
    //const char* ptr = strstr(MPath,"#");  // find #
    //if (ptr) {
    //  *(char*)ptr = 0;  // strip #xxxx from MPath
    //  ptr += 1;         // skip past #, ptr now points to 4-digit hex index
    //  if (*ptr) {
    //    uint32_t index = (uint32_t)strtol(ptr,nullptr,16);
    //    loadLadspaPlugin(MPath,index);
    //  }
    //}
    loadLadspaPlugin(MPath,AIndex);
  }
  
  //----------
  
  virtual ~SAT_LadspaHostedPlugin() {
    SAT_Print("\n");
    unloadLadspaPlugin();
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    SAT_Print("-> true\n");
    //MHost->initExtensions();
    return true;
  }

  //----------

  void destroy() final {
    SAT_Print("\n");
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    SAT_Print("-> true\n");
    ladspa_instantiate(sample_rate);
    ladspa_activate();
    setDefaultParamValues();
    return true;
  }

  //----------

  void deactivate() final {
    SAT_Print("\n");
    ladspa_deactivate();
    ladspa_cleanup();
  }

  //----------

  bool start_processing() final {
    SAT_Print("-> true\n");
    return true;

  }

  //----------

  void stop_processing() final {
    SAT_Print("\n");
  }

  //----------

  void reset() final {
    SAT_Print("\n");
  }

  //----------
  
  /*
    we connect:
    - audio inputs:     directly to clap-provided buffers
    - audio outputs:    directly to clap-provided buffers
    - control inputs:   to parameter values
    - control outputs:  parameter outputs
  */

  clap_process_status process(const clap_process_t *process) final {
    // events
    processEvents(process->in_events,process->out_events);
    // audio inputs
    uint32_t num_inputs = MNumAudioInputs;
    for (uint32_t i=0; i<num_inputs; i++) {
      int32_t port = MAudioInputIndex[i];
      if (port >= 0) {
        SAT_Assert(i <= process->audio_inputs->channel_count);
        ladspa_connect_port(port,process->audio_inputs->data32[i]);
      }
    }
    // audio outputs
    uint32_t num_outputs = MNumAudioOutputs;
    for (uint32_t i=0; i<num_outputs; i++) {
      int32_t port = MAudioOutputIndex[i];
      if (port >= 0) {
        SAT_Assert(i <= process->audio_outputs->channel_count);
        ladspa_connect_port(port,process->audio_outputs->data32[i]);
      }
    }
    // parameters
    uint32_t num_params = MNumControlInputs;//MParams.count(&MPlugin);
    for (uint32_t i=0; i<num_params; i++) {
      int32_t port = MControlInputIndex[i];
      if (port >= 0) {
        //SAT_Print("param %i port %i\n",i,port);
        ladspa_connect_port(port,&MParamValues[port]);
      }
    }
    // param out
    uint32_t num_param_outs = MNumControlOutputs;//MParams.count(&MPlugin);
    for (uint32_t i=0; i<num_param_outs; i++) {
      int32_t port = MControlOutputIndex[i];
      if (port >= 0) {
        //SAT_Print("param %i port %i\n",i,port);
        ladspa_connect_port(port,&MParamOutputs[port]);
      }
    }
    // run
    uint32_t length = process->frames_count;
    ladspa_run(length);
    // return
    return CLAP_PROCESS_CONTINUE;
  }

//------------------------------
private: // clap audio ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) final {
    //SAT_Print("is_input %i\n",is_input);
    if (is_input) return 1;//MNumAudioInputs;
    else return 1;//MNumAudioOutputs;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) final {
    //SAT_Print("index %i is_input %i\n",index,is_input);
    if (is_input) {
      int32_t port_index = MAudioInputIndex[index];
      const char* name = MLadspaPorts[port_index].name;
      strncpy(info->name,name,CLAP_NAME_SIZE-1);
      info->id = index;
      info->flags = CLAP_AUDIO_PORT_IS_MAIN;
//TODO: always stereo.. remap via ini file, (double_mono)
      info->channel_count = MNumAudioInputs;
      if (MNumAudioInputs == 1) info->port_type = CLAP_PORT_MONO;
      else if (MNumAudioInputs == 2) info->port_type = CLAP_PORT_STEREO;
      else info->port_type = 0;
//
      info->in_place_pair = CLAP_INVALID_ID;
    }
    else {
      int32_t port_index = MAudioOutputIndex[index];
      const char* name = MLadspaPorts[port_index].name;
      strncpy(info->name,name,CLAP_NAME_SIZE-1);
      info->id = index;
      info->flags = CLAP_AUDIO_PORT_IS_MAIN;
//TODO: same as inputs
      info->channel_count = MNumAudioOutputs;
      if (MNumAudioOutputs == 1) info->port_type = CLAP_PORT_MONO;
      else if (MNumAudioOutputs == 2) info->port_type = CLAP_PORT_STEREO;
      else info->port_type = 0;
//
      info->in_place_pair = CLAP_INVALID_ID;
    }
    return true;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() final {
    //SAT_Print("%i\n",MNumControlInputs);
    return MNumControlInputs;
  }

  //----------

  /*
    append 'fake' parameter, so we can control it via gui?
  */

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) final {
    int32_t port_index = MControlInputIndex[param_index];
    if (port_index >= 0) {
      param_info->id            = param_index;
      param_info->flags         = CLAP_PARAM_IS_AUTOMATABLE;
      param_info->cookie        = nullptr;
      param_info->min_value     = MLadspaPorts[port_index].minval;
      param_info->max_value     = MLadspaPorts[port_index].maxval;
      param_info->default_value = MLadspaPorts[port_index].defval;
      strcpy(param_info->name,MLadspaPorts[port_index].name);
      strcpy(param_info->module,"");
      //SAT_Print("%i '%s' : def %.3f min %.3f max %.3f\n",param_index,MLadspaPorts[port_index].name,MLadspaPorts[port_index].defval,MLadspaPorts[port_index].minval,MLadspaPorts[port_index].maxval);

      //SAT_Parameter* parameter = new SAT_Parameter(param_info);
      //appendParameter(parameter);

      return true;
    }
    return false;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) final {
    //SAT_Print("%i = %.3f\n",param_id,MParamValues[param_id]);
    *value = MParamValues[param_id];
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    sprintf(display,"%.3f",value);
    //SAT_Print("%.3f -> '%s'\n",value,display);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    *value = atof(display);
    //SAT_Print("'%s' -> %.3f\n",display,*value);
    return true;
  }

  //----------

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) final {
    SAT_Print("TODO\n");
    processEvents(in,out);
  }

//------------------------------
private: // state
//------------------------------

  bool state_save(const clap_ostream_t *stream) final {
    //SAT_Print("\n");
    uint32_t version = 0;
    /*int64_t result =*/ stream->write(stream, &version,sizeof(uint32_t));
    return true;
  }

  //----------

  bool state_load(const clap_istream_t *stream) final {
    //SAT_Print("\n");
    uint32_t version = 0;
    /*int64_t result =*/ stream->read(stream, &version,sizeof(uint32_t));
    return true;
  }

//------------------------------
private: // events
//------------------------------

  void processEvents(const clap_input_events_t *in_events, const clap_output_events_t *out_events) {
    //const clap_input_events_t* in_events = process->in_events;
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {
          case CLAP_EVENT_PARAM_VALUE: {
            const clap_event_param_value_t* event = (const clap_event_param_value_t*)header;
            uint32_t index = event->param_id;
            double value = event->value;
            SAT_Assert(index <= MNumControlInputs);
            int32_t port = MControlInputIndex[index];
            if (port >= 0) {
              MParamValues[port] = value;
            }
            break;
          }
        }
      }
    }
  }

//  void processParamValue(const clap_event_param_value_t* event) final {
//    uint32_t index = event->param_id;
//    double value = event->value;
//    SAT_Assert(index <= MNumControlInputs);
//    int32_t port = MControlInputIndex[index];
//    if (port >= 0) {
//      MParamValues[port] = value;
//    }
//  }

//  void processAudio(const clap_process_t* process) {
//  }

//------------------------------
private: // ladspa
//------------------------------

  void ladspa_instantiate(uint32_t ASampleRate) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->instantiate) {
        MLadspaHandle = MLadspaDescriptor->instantiate(MLadspaDescriptor,ASampleRate);
        if (MLadspaHandle) MIsInstantiated = true;
      }
    }
  }

  //----------

  void ladspa_connect_port(uint32_t APort, float* APtr) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->connect_port) {
          
//TODO: remap

          MLadspaDescriptor->connect_port(MLadspaHandle,APort,APtr);
        }
      }
    }
  }

  //----------

  void ladspa_activate(void) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->activate) {
          MLadspaDescriptor->activate(MLadspaHandle);
          MIsActivated = true;
        }
      }
    }
  }

  //----------

  void ladspa_run(uint32_t ANum) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->run) {
          MLadspaDescriptor->run(MLadspaHandle,ANum);
        }
      }
    }
  }

  //----------

  void ladspa_run_adding(uint32_t ANum) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->run_adding) {
          MLadspaDescriptor->run_adding(MLadspaHandle,ANum);
        }
      }
    }
  }

  //----------

  void ladspa_set_run_adding_gain(float AGain) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->set_run_adding_gain) {
          MLadspaDescriptor->set_run_adding_gain(MLadspaHandle,AGain);
        }
      }
    }
  }

  //----------

  void ladspa_deactivate(void) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->deactivate) {
          MLadspaDescriptor->deactivate(MLadspaHandle);
          MIsActivated = false;
        }
      }
    }
  }

  //----------

  void ladspa_cleanup(void) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->cleanup) {
          MLadspaDescriptor->cleanup(MLadspaHandle);
          MIsInstantiated = false;
        }
      }
    }
  }

//------------------------------
public: // ladspa
//------------------------------

  #ifdef SAT_LINUX
  
    void loadLadspaPlugin(const char* APath, uint32_t AIndex) {
      SAT_Print("path '%s' index %i\n",APath,AIndex);
      MLibHandle = dlopen(APath,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW, RTLD_LAZY
      if (MLibHandle) {
        LADSPA_Descriptor_Function get_ladspa_descriptor = (LADSPA_Descriptor_Function)dlsym(MLibHandle,"ladspa_descriptor");
        MLadspaDescriptor = get_ladspa_descriptor(AIndex);
        if (MLadspaDescriptor) {
          setupLadspaPorts();
        }
      }
    }
    
    void unloadLadspaPlugin() {
      SAT_Print("\n");
      if (MLibHandle) {
        dlclose(MLibHandle);
        MLibHandle = nullptr;
      }
    }

  #endif

  //----------

  #ifdef SAT_WIN32
  
    void loadLadspaPlugin(const char* APath, uint32_t AIndex) {
      #error not implemented for win32 yet (dlopen)
    }
    
    void unloadLadspaPlugin() {
      #error not implemented for win32 yet (dlclose)
    }

  #endif

  //----------

  void resetLadspaPorts(void) {
    MNumAudioInputs     = 0;
    MNumAudioOutputs    = 0;
    MNumControlInputs   = 0;
    MNumControlOutputs  = 0;
    MLadspaFlags        = SAT_LADSPA_FLAGS_NONE;
    memset((void*)MLadspaPorts, 0, SAT_LADSPA_MAX_PORTS           * sizeof(SAT_LadspaPort));
    memset(MAudioInputIndex,    0, SAT_LADSPA_MAX_AUDIO_INPUTS    * sizeof(int32_t));
    memset(MAudioOutputIndex,   0, SAT_LADSPA_MAX_AUDIO_INPUTS    * sizeof(int32_t));
    memset(MControlInputIndex,  0, SAT_LADSPA_MAX_AUDIO_INPUTS    * sizeof(int32_t));
    memset(MControlOutputIndex, 0, SAT_LADSPA_MAX_AUDIO_INPUTS    * sizeof(int32_t));
    memset(MParamValues,        0, SAT_LADSPA_MAX_CONTROL_INPUTS  * sizeof(float));
    memset(MParamOutputs,       0, SAT_LADSPA_MAX_CONTROL_OUTPUTS * sizeof(float));
  }
  
  //----------
    
  void setupLadspaPorts(void) {
    //SAT_Assert(MLadspaDescriptor != nullptr);
    //SAT_Print("\n");
    MNumAudioInputs     = 0;
    MNumAudioOutputs    = 0;
    MNumControlInputs   = 0;
    MNumControlOutputs  = 0;
    MLadspaFlags        = SAT_LADSPA_FLAGS_NONE;
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_REALTIME)         { MLadspaFlags |= SAT_LADSPA_FLAGS_REALTIME; }
      if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_INPLACE_BROKEN)   { MLadspaFlags |= SAT_LADSPA_FLAGS_INPLACE_BROKEN; }
      if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_HARD_RT_CAPABLE)  { MLadspaFlags |= SAT_LADSPA_FLAGS_HARD_RT_CAPABLE; }
      for (uint32_t i=0; i<MLadspaDescriptor->PortCount; i++) {
        MLadspaPorts[i].name    = MLadspaDescriptor->PortNames[i];
        MLadspaPorts[i].flags   = SAT_LADSPA_FLAGS_NONE;
        MLadspaPorts[i].minval  = 0;
        MLadspaPorts[i].maxval  = 1;
        MLadspaPorts[i].defval  = 0;
        LADSPA_PortDescriptor pd = MLadspaDescriptor->PortDescriptors[i];
        if (pd & LADSPA_PORT_AUDIO)   { MLadspaPorts[i].flags |= SAT_LADSPA_PORT_AUDIO; }
        if (pd & LADSPA_PORT_CONTROL) { MLadspaPorts[i].flags |= SAT_LADSPA_PORT_CONTROL; }
        if (pd & LADSPA_PORT_INPUT)   { MLadspaPorts[i].flags |= SAT_LADSPA_PORT_INPUT; }
        if (pd & LADSPA_PORT_OUTPUT)  { MLadspaPorts[i].flags |= SAT_LADSPA_PORT_OUTPUT; }
        uint32_t hints = MLadspaDescriptor->PortRangeHints[i].HintDescriptor;
        if (hints & LADSPA_HINT_BOUNDED_BELOW)  { MLadspaPorts[i].minval = MLadspaDescriptor->PortRangeHints[i].LowerBound; }
        if (hints & LADSPA_HINT_BOUNDED_ABOVE)  { MLadspaPorts[i].maxval = MLadspaDescriptor->PortRangeHints[i].UpperBound; }
        if (hints & LADSPA_HINT_TOGGLED)        { MLadspaPorts[i].flags |= SAT_LADSPA_PARAM_TOGGLED; }
        if (hints & LADSPA_HINT_SAMPLE_RATE)    { MLadspaPorts[i].flags |= SAT_LADSPA_PARAM_SAMPLERATE; }
        if (hints & LADSPA_HINT_LOGARITHMIC)    { MLadspaPorts[i].flags |= SAT_LADSPA_PARAM_LOGARITHMIC; }
        if (hints & LADSPA_HINT_INTEGER)        { MLadspaPorts[i].flags |= SAT_LADSPA_PARAM_INTEGER; }
        uint32_t defmask = hints & LADSPA_HINT_DEFAULT_MASK;
        if (defmask == LADSPA_HINT_DEFAULT_MINIMUM) MLadspaPorts[i].defval =  MLadspaPorts[i].minval;
        if (defmask == LADSPA_HINT_DEFAULT_LOW)     MLadspaPorts[i].defval = (MLadspaPorts[i].minval * 0.75 + MLadspaPorts[i].maxval * 0.25);
        if (defmask == LADSPA_HINT_DEFAULT_MIDDLE)  MLadspaPorts[i].defval = (MLadspaPorts[i].minval * 0.50 + MLadspaPorts[i].maxval * 0.50);
        if (defmask == LADSPA_HINT_DEFAULT_HIGH)    MLadspaPorts[i].defval = (MLadspaPorts[i].minval * 0.25 + MLadspaPorts[i].maxval * 0.75);
        if (defmask == LADSPA_HINT_DEFAULT_MAXIMUM) MLadspaPorts[i].defval =  MLadspaPorts[i].maxval;
        if (defmask == LADSPA_HINT_DEFAULT_0)       MLadspaPorts[i].defval = 0;
        if (defmask == LADSPA_HINT_DEFAULT_1)       MLadspaPorts[i].defval = 1;
        if (defmask == LADSPA_HINT_DEFAULT_100)     MLadspaPorts[i].defval = 100;
        if (defmask == LADSPA_HINT_DEFAULT_440)     MLadspaPorts[i].defval = 440;
        if ( (MLadspaPorts[i].flags & SAT_LADSPA_PORT_AUDIO)   && (MLadspaPorts[i].flags & SAT_LADSPA_PORT_INPUT)  ) MAudioInputIndex[   MNumAudioInputs++]    = i;
        if ( (MLadspaPorts[i].flags & SAT_LADSPA_PORT_AUDIO)   && (MLadspaPorts[i].flags & SAT_LADSPA_PORT_OUTPUT) ) MAudioOutputIndex[  MNumAudioOutputs++]   = i;
        if ( (MLadspaPorts[i].flags & SAT_LADSPA_PORT_CONTROL) && (MLadspaPorts[i].flags & SAT_LADSPA_PORT_INPUT)  ) MControlInputIndex[ MNumControlInputs++]  = i;
        if ( (MLadspaPorts[i].flags & SAT_LADSPA_PORT_CONTROL) && (MLadspaPorts[i].flags & SAT_LADSPA_PORT_OUTPUT) ) MControlOutputIndex[MNumControlOutputs++] = i;
        //SAT_Print("port %i\n",i);
        //SAT_Print("  name: '%s'\n",MLadspaPorts[i].name);
        //SAT_Print("  flags: %04x\n",MLadspaPorts[i].flags);
      }
    }
    //SAT_Print("\n");
    SAT_Print("control inputs:  %i\n",MNumControlInputs);
    for (uint32_t i=0; i<MNumControlInputs; i++) {
      int32_t p = MControlInputIndex[i];
      SAT_Print("    %i. %s\n",i,MLadspaPorts[p].name);
    }
    SAT_Print("control outputs: %i\n",MNumControlOutputs);
    for (uint32_t i=0; i<MNumControlOutputs; i++) {
      int32_t p = MAudioOutputIndex[i];
      SAT_Print("    %i. %s\n",i,MLadspaPorts[p].name);
    }
    SAT_Print("audio inputs:    %i\n",MNumAudioInputs);
    for (uint32_t i=0; i<MNumAudioInputs; i++) {
      int32_t p = MAudioInputIndex[i];
      SAT_Print("    %i. %s\n",i,MLadspaPorts[p].name);
    }
    SAT_Print("audio outputs:   %i\n",MNumAudioOutputs);
    for (uint32_t i=0; i<MNumAudioOutputs; i++) {
      int32_t p = MAudioOutputIndex[i];
      SAT_Print("    %i. %s\n",i,MLadspaPorts[p].name);
    }
    SAT_Print("\n");
  }

  //----------

  void setDefaultParamValues() {
    uint32_t num_params = MNumControlInputs;
    //SAT_Print("num_params %i\n",num_params);
    for (uint32_t i=0; i<num_params; i++) {
      int32_t port = MControlInputIndex[i];
      if (port >= 0) {
        MParamValues[i] = MLadspaPorts[i].defval;
        //SAT_Print("%i = %.3f\n",i,MParamValues[i]);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
