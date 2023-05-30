#ifndef sat_lv2_plugin_included
#define sat_lv2_plugin_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/lv2/sat_lv2.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2Plugin {
  
//----------
//------------------------------
private:
//------------------------------

  LV2_Handle                      MHandle               = nullptr;
  const clap_host_t*              MHost                 = nullptr;
  const clap_plugin_t*            MPlugin               = nullptr;
  const clap_plugin_descriptor_t* MDescriptor           = nullptr;
  
//------------------------------
public:
//------------------------------

  LV2_Handle getHandle() {
    return MHandle;
  }

//------------------------------
public:
//------------------------------

  void lv2_connect_port(uint32_t port, void* data_location) {}
  void lv2_activate() {}
  void lv2_run(uint32_t sample_count) {}
  void lv2_deactivate() {}
  void lv2_cleanup() {}
  
};

//----------------------------------------------------------------------
#endif










#if 0



//#include "plugin/base/kode_base_instance.h"

#ifndef KODE_NO_GUI
  #include "plugin/kode_editor.h"
#endif

#include "plugin/kode_process_context.h"
#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_utils.h"

class KODE_Lv2Instance {

//------------------------------
private:
//------------------------------

  KODE_Instance*            MInstance         = nullptr;
  KODE_Descriptor*          MDescriptor       = nullptr;
  #ifndef KODE_NO_GUI
  KODE_Editor*              MEditor           = nullptr;
  #endif
  LV2_URID                  MMidiInputUrid    = 0;
  const LV2_Atom_Sequence*  MAtomSequence     = nullptr;
  float                     MSampleRate       = 0.0f;
  uint32_t                  MNumInputs        = 0;
  uint32_t                  MNumOutputs       = 0;
  uint32_t                  MNumParameters    = 0;
  float**                   MInputPtrs        = nullptr;
  float**                   MOutputPtrs       = nullptr;
  float**                   MParameterPtrs    = nullptr;
  float*                    MParameterValues  = nullptr;
  float*                    MHostValues       = nullptr;
  float*                    MProcessValues    = nullptr;

  KODE_ProcessContext       MProcessContext   = {0};

  //float*                    MEditorParameterValues  = nullptr;
  //float*                    MHostParameterValues    = nullptr;
  //KODE_Lv2UpdateQueue       MHostParameterQueue;

//------------------------------
public:
//------------------------------

  /*
     Note that instance initialisation should generally occur in activate()
     rather than here. If a host calls instantiate(), it MUST call cleanup()
     at some point in the future.

     path: Path to the LV2 bundle which contains this plugin
     binary. It MUST include the trailing directory separator so that simply
     appending a filename will yield the path to that file in the bundle.

     features :A NULL terminated array of LV2_Feature structs which
     represent the features the host supports. Plugins may refuse to
     instantiate if required features are not found here. However, hosts MUST
     NOT use this as a discovery mechanism: instead, use the RDF data to
     determine which features are required and do not attempt to instantiate
     unsupported plugins at all. This parameter MUST NOT be NULL, i.e. a host
     that supports no features MUST pass a single element array containing
     NULL.
  */


  //KODE_Lv2Instance(KODE_Descriptor* ADescriptor/*, float ASampleRate, const char* path, const LV2_Feature* const* features*/)
  KODE_Lv2Instance(KODE_Instance* AInstance)
  /*: KODE_BaseInstance(ADescriptor)*/ {
    MInstance = AInstance;
    //instance->on_open();
    //MInstance->on_initialize(); // open?
    MDescriptor       = AInstance->getDescriptor();
    //MSampleRate       = ASampleRate;
    MNumInputs        = MDescriptor->getNumInputs();
    MNumOutputs       = MDescriptor->getNumOutputs();
    MNumParameters    = MDescriptor->getNumParameters();
    MInputPtrs        = (float**)malloc(MNumInputs     * sizeof(float*));
    MOutputPtrs       = (float**)malloc(MNumOutputs    * sizeof(float*));
    MParameterPtrs    = (float**)malloc(MNumParameters * sizeof(float*));
    MParameterValues  = (float*) malloc(MNumParameters * sizeof(float ));
    MHostValues       = (float*) malloc(MNumParameters * sizeof(float ));
    MProcessValues    = (float*) malloc(MNumParameters * sizeof(float ));
    //#ifdef KODE_DEBUG_LV2
    //  KODE_DPrint("lv2 bundle_path: %s",path);
    //  KODE_DPrint("lv2 features:");
    //  kode_lv2_dump_features(features);
    //#endif
    //
    //LV2_URID_Map* urid_map = (LV2_URID_Map*)kode_lv2_find_feature(LV2_URID__map,features);
    //
    //if (urid_map) {
    //  if (MDescriptor->canReceiveMidi()) {
    //    MMidiInputUrid = kode_lv2_map_urid(LV2_MIDI__MidiEvent,urid_map);
    //  }
    //}
  }

  //----------

  virtual ~KODE_Lv2Instance() {
    if (MInputPtrs)         free(MInputPtrs);
    if (MOutputPtrs)        free(MOutputPtrs);
    if (MParameterPtrs)     free(MParameterPtrs);
    if (MParameterValues)   free(MParameterValues);
    if (MHostValues)        free(MHostValues);
    if (MProcessValues)     free(MProcessValues);
  }

//------------------------------
public:
//------------------------------

  //KODE_Descriptor* getDescriptor() override {
  //  return MDescriptor;
  //}

  //----------

  // (shouldn't be public)

  //void _setMidiInputUrid(LV2_URID lv2_midi_input_urid) {
  //  MMidiInputUrid = lv2_midi_input_urid;
  //}

//------------------------------
public:
//------------------------------

//  void setDefaultParameterValues() override {
//    uint32_t num = MDescriptor->getNumParameters();
//    for (uint32_t i=0; i<num; i++) {
//      KODE_Parameter* parameter = MDescriptor->getParameter(i);
//      float value = parameter->getDefValue();
//      MParameterValues[i] = value;
//      //MHostParameterValues[i] = value;
//    }
//  }

  //----------

//  void updateAllParameters() override {
//    //KODE_PRINT;
//    uint32_t num = MDescriptor->getNumParameters();
//    for (uint32_t i=0; i<num; i++) {
//      KODE_Parameter* parameter = MDescriptor->getParameter(i);
//      float value = MParameterValues[i];
//      float v = parameter->from01(value);
//      on_plugin_parameter(0,i,v);
//      // if editor is open ...
//    }
//  }

  //----------

//  void updateAllEditorParameters(KODE_BaseEditor* AEditor, bool ARedraw=true) override {
//    uint32_t num = MDescriptor->getNumParameters();
//    for (uint32_t i=0; i<num; i++) {
//      float value = MParameterValues[i];
//      //KODE_Parameter* parameter = MDescriptor->getParameter(i);
//      //float v = parameter->from01(value);
//      float v = value;
//      //on_plugin_parameter(0,i,v);
//      AEditor->updateParameterFromHost(i,v,ARedraw);
//    }
//  }

  //----------

//  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
////    //MEditorParameterValues[AIndex] = AValue;
////    queueParameterToHost(AIndex,AValue);
//  }

//----------------------------------------------------------------------
// lv2
//----------------------------------------------------------------------

  void lv2_setup(float ASampleRate, const char* path, const LV2_Feature* const* features) {
    #ifdef KODE_DEBUG_LV2
      KODE_DPrint("lv2 bundle_path: %s",path);
      KODE_DPrint("lv2 features:");
      kode_lv2_dump_features(features);
    #endif
    MSampleRate = ASampleRate;
    LV2_URID_Map* urid_map = (LV2_URID_Map*)KODE_Lv2FindFeature(LV2_URID__map,features);
    if (urid_map) {
      if (MDescriptor->canReceiveMidi()) {
        MMidiInputUrid = KODE_Lv2MapUrid(LV2_MIDI__MidiEvent,urid_map);
      }
    }

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
    //KODE_Print("lv2_connect_port: port %i data_location 0x%x\n",port,data_location);

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
    if (MDescriptor->canReceiveMidi()) {
      MAtomSequence = (const LV2_Atom_Sequence*)data_location;
      port -= 1;
    }
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
    KODE_Print("lv2_activate\n");
//    on_plugin_open();
//    on_plugin_activate();
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
    //KODE_Print("lv2_run: %i\n",sample_count);

    // parameters

    for (uint32_t i=0; i<MNumParameters; i++) {
      float v = *MParameterPtrs[i];
      MHostValues[i] = v;
      if (v != MProcessValues[i]) { // todo: slmilar_to?
        MProcessValues[i] = v;
        // to/from01 ??
        //KODE_Parameter* param = MPlugin->getParameter(i);
        //if (param) v = param->from01(v);
        MInstance->on_plugin_parameter(i,v);
      }
    }

    // midi

    if (MDescriptor->canReceiveMidi()) {
      uint32_t offset = 0;
      LV2_ATOM_SEQUENCE_FOREACH(MAtomSequence, ev) {
        if (ev->body.type == MMidiInputUrid) {
          const uint8_t* const msg = (const uint8_t*)(ev + 1);
          offset = (uint32_t)ev->time.frames;
          MInstance->on_plugin_midi(offset,msg[0],msg[1],msg[2]);
        }
      }
    }

    MProcessContext.numinputs = MDescriptor->getNumInputs();
    MProcessContext.numoutputs = MDescriptor->getNumOutputs();
    for (uint32_t i=0; i<MProcessContext.numinputs; i++)  { MProcessContext.inputs[i]  = MInputPtrs[i]; }
    for (uint32_t i=0; i<MProcessContext.numoutputs; i++) { MProcessContext.outputs[i] = MOutputPtrs[i]; }
    MProcessContext.numsamples  = sample_count;
    MProcessContext.samplerate  = MSampleRate;
    //context.offset      = 0;
    //context.oversample  = 1;

    MInstance->on_plugin_process(&MProcessContext);

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
    KODE_Print("lv2_deactivate\n");
//    on_plugin_close();
//    on_plugin_deactivate();
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
    KODE_Print("lv2_cleanup\n");
//    on_plugin_terminate();
  }

  //----------

  //static
  //const void* lv2_extension_data_callback(const char* uri) {
  //  KODE_Print("lv2_extension_data_callback: '%s'\n",uri);
  //  return nullptr;
  //}

};


#endif // 0

