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

  //float*                    MHostValues       = nullptr;
  //float*                    MProcessValues    = nullptr;
  
  SAT_ProcessContext       MProcessContext   = {};

//------------------------------
public:
//------------------------------

  //SAT_LadspaPlugin(const struct _LADSPA_Descriptor* Descriptor) {
  SAT_LadspaPlugin(const LADSPA_Descriptor* Descriptor, uint32_t ASampleRate) {
    SAT_Print("Descriptor %p ASampleRate %i\n",Descriptor,ASampleRate);
    MLadspaDescriptor = Descriptor;
    MSampleRate = ASampleRate;
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

  void ladspa_instantiate() {
    SAT_PRINT;
    
    MHost = new SAT_HostImplementation();
    const clap_host_t* clap_host = MHost->getHost();
    
    SAT_LadspaEntryData* entrydata = (SAT_LadspaEntryData*)MLadspaDescriptor->ImplementationData;
    uint32_t index = entrydata->index;
    const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    MClapPlugin = SAT_CreatePlugin(index,clap_descriptor,clap_host);
    MPlugin = (SAT_Plugin*)MClapPlugin->plugin_data;
    
    clap_audio_port_info_t info;
    if (MPlugin->audio_ports_get(0,true,&info))  { MNumInputs = info.channel_count; }
    if (MPlugin->audio_ports_get(0,false,&info)) { MNumOutputs = info.channel_count; }
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
    SAT_PRINT;
    //LADSPA_Trace("ladspa: connect_port Port:%i DataLocation:%p\n",Port,DataLocation);
    //if (Port < 0) return;
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
    SAT_PRINT;
    if (MPlugin) {
      
      for (uint32_t i=0; i<MNumParameters; i++) {
        float v = *MParameterPtrs[i];
        //MHostValues[i] = v;
        if (v != MPlugin->getParameterValue(i)) {
          MPlugin->setParameterValue(i,v); // almostequal
          SAT_Parameter* param = MPlugin->getParameter(i);
          //v = param->denormalizeValue(v); // from01(v);
          //MInstance->on_parameterChange(i,v);
        }
      }
      
      uint32_t num_in  = MNumInputs;//MDescriptor->getNumInputs();
      uint32_t num_out = MNumOutputs;//MDescriptor->getNumOutputs();
      
//      for (uint32_t i=0; i<num_in; i++)  { MProcessContext.inputs[i]  = MInputPtrs[i]; }
//      for (uint32_t i=0; i<num_out; i++) { MProcessContext.outputs[i] = MOutputPtrs[i]; }
//      MProcessContext.playstate     = 0;//KODE_PLUGIN_PLAYSTATE_NONE;
//      MProcessContext.samplepos     = 0;
//      MProcessContext.beatpos       = 0.0f;
//      MProcessContext.tempo         = 0.0f;
//      MProcessContext.timesignum    = 0;
//      MProcessContext.timesigdenom  = 0;
//      MProcessContext.numinputs     = num_in;
//      MProcessContext.numoutputs    = num_out;
//      MProcessContext.numsamples    = SampleCount;
//      MProcessContext.samplerate    = MSampleRate;
//      MInstance->on_plugin_process(&MProcessContext);

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
    SAT_PRINT;
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
    SAT_PRINT;
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

    SAT_LadspaEntryData* entrydata = (SAT_LadspaEntryData*)MLadspaDescriptor->ImplementationData;
    if (entrydata) {
      if (entrydata->descriptor) free(entrydata->descriptor);
      if (entrydata->ports) {
        //cleanup_ports(&MPorts);
        if (entrydata->ports->descriptors)   free(entrydata->ports->descriptors);
        if (entrydata->ports->names)         free(entrydata->ports->names);
        if (entrydata->ports->namesBuffer)   free(entrydata->ports->namesBuffer);
        if (entrydata->ports->rangeHints)    free(entrydata->ports->rangeHints);
        free(entrydata->ports);
      }
      free(entrydata);
    }
    
    if (MClapPlugin) MClapPlugin->destroy(MClapPlugin);
    if (MHost) delete MHost;
    
    if (MInputPtrs)     free(MInputPtrs);
    if (MOutputPtrs)    free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
    
    //if (MHostValues)    free(MHostValues);
    //if (MProcessValues) free(MProcessValues);

  }

  //----------

};

//----------------------------------------------------------------------
#endif


