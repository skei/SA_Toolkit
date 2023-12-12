#ifndef kode_dssi_plugin_included
#define kode_dssi_plugin_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
//#include "plugin/kode_instance_listener.h"
#include "plugin/dssi/kode_dssi.h"
#include "plugin/dssi/kode_dssi_instance.h"
#include "plugin/ladspa/kode_ladspa.h"
#include "plugin/ladspa/kode_ladspa_utils.h"

//----------------------------------------------------------------------

/*
  A DSSI synth plugin consists of a LADSPA plugin plus an additional framework
  for controlling program settings and transmitting MIDI events.
  A plugin must fully implement the LADSPA descriptor fields as well as the
  required LADSPA functions including instantiate() and (de)activate().
  It should also implement run(), with the same behaviour as if run_synth()
  were called with no synth events.

  In order to instantiate a synth the host calls the LADSPA instantiate
  function, passing in this LADSPA_Descriptor pointer.  The returned
  LADSPA_Handle is used as the argument for the DSSI functions below as well as
  for the LADSPA ones.
*/


//----------------------------------------------------------------------

/*
LADSPA_Handle                   dssi_instantiate_callback(const struct _LADSPA_Descriptor * Descriptor, unsigned long SampleRate);
void                            dssi_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data* DataLocation);
void                            dssi_activate_callback(LADSPA_Handle Instance);
void                            dssi_run_callback(LADSPA_Handle Instance, unsigned long SampleCount);
//void                            dssi_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount);
//void                            dssi_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain);
void                            dssi_deactivate_callback(LADSPA_Handle Instance);
void                            dssi_cleanup_callback(LADSPA_Handle Instance);

char*                           dssi_configure(LADSPA_Handle Instance, const char* Key, const char* Value);
const DSSI_Program_Descriptor*  dssi_get_program(LADSPA_Handle Instance, unsigned long Index);
void                            dssi_select_program(LADSPA_Handle Instance, unsigned long Bank, unsigned long Program);
int                             dssi_get_midi_controller_for_port(LADSPA_Handle Instance, unsigned long Port);
void                            dssi_run_synth(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount);
void                            dssi_run_synth_adding(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount);
//void                          dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts);
//void                          dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts);
*/

//----------------------------------------------------------------------

template<class DESCRIPTOR, class INSTANCE, class EDITOR>
class KODE_DssiPlugin
/*: public KODE_InstanceListener*/ {

private:

  KODE_Descriptor*    MDescriptor       = nullptr;
  DSSI_Descriptor*    MDssiDescriptor   = nullptr;
  LADSPA_Descriptor*  MLadspaDescriptor = nullptr;

  KODE_LadspaPorts MPorts;

  char*               MNameBuffer       = nullptr;
  char*               MLabelBuffer      = nullptr;
  char*               MMakerBuffer      = nullptr;
  char*               MCopyrightBuffer  = nullptr;

public:

  KODE_DssiPlugin() {
    //KODE_PRINT;
    MDescriptor       = new DESCRIPTOR();
    MLadspaDescriptor = (LADSPA_Descriptor*)malloc( sizeof(LADSPA_Descriptor)  );
    MDssiDescriptor   = (DSSI_Descriptor*)malloc( sizeof(DSSI_Descriptor)  );
  }

  ~KODE_DssiPlugin() {
    //KODE_PRINT;
    if (MDescriptor)        delete MDescriptor;
    if (MNameBuffer)        free(MNameBuffer);
    if (MLabelBuffer)       free(MLabelBuffer);
    if (MMakerBuffer)       free(MMakerBuffer);
    if (MCopyrightBuffer)   free(MCopyrightBuffer);
    KODE_LadspaCleanupPorts(&MPorts);
    if (MDssiDescriptor) free(MDssiDescriptor);
    if (MLadspaDescriptor) free(MLadspaDescriptor);

  }

public:

  // see KODE_LadspaPlugin

  const DSSI_Descriptor* entrypoint(unsigned long Index) {
    printf("DSSI\n");

    //KODE_PRINT;

    //KODE_Memset(&MDssiDescriptor,0,sizeof(DSSI_Descriptor));
    //KODE_Memset(&MLadspaDescriptor,0,sizeof(LADSPA_Descriptor));

    if (Index > 0) return nullptr;
    if (MDssiDescriptor) return MDssiDescriptor;

    MDescriptor = new DESCRIPTOR();
    MLadspaDescriptor = (LADSPA_Descriptor*)malloc(sizeof(LADSPA_Descriptor));
    memset(MLadspaDescriptor,0,sizeof(LADSPA_Descriptor));
    MDssiDescriptor = (DSSI_Descriptor*)malloc(sizeof(DSSI_Descriptor));
    memset(MDssiDescriptor,0,sizeof(DSSI_Descriptor));

    MNameBuffer       = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MLabelBuffer      = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MMakerBuffer      = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MCopyrightBuffer  = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);

    strncpy(MNameBuffer,      MDescriptor->getName(),        KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    strncpy(MLabelBuffer,     MDescriptor->getName(),        KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1); // todo: make valid c symbol (see lv2)
    strncpy(MMakerBuffer,     MDescriptor->getAuthor(),      KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    strncpy(MCopyrightBuffer, MDescriptor->getLicenseText(), KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);

    uint32_t numports = KODE_LadspaSetupPorts(MDescriptor,&MPorts);

    MLadspaDescriptor->UniqueID                   = MDescriptor->getShortId();
    MLadspaDescriptor->Label                      = MLabelBuffer;
    MLadspaDescriptor->Properties                 = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN
    MLadspaDescriptor->Name                       = MNameBuffer;
    MLadspaDescriptor->Maker                      = MMakerBuffer;
    MLadspaDescriptor->Copyright                  = MCopyrightBuffer;
    MLadspaDescriptor->PortCount                  = numports;
    MLadspaDescriptor->PortDescriptors            = MPorts.descriptors;
    MLadspaDescriptor->PortNames                  = (const char* const*)MPorts.names;
    MLadspaDescriptor->PortRangeHints             = MPorts.rangeHints;
    MLadspaDescriptor->ImplementationData         = this;//nullptr;
    MLadspaDescriptor->instantiate                = dssi_instantiate_callback;
    MLadspaDescriptor->connect_port               = dssi_connect_port_callback;
    MLadspaDescriptor->activate                   = dssi_activate_callback;
    MLadspaDescriptor->run                        = dssi_run_callback;
    MLadspaDescriptor->run_adding                 = nullptr;//dssi_run_adding_callback;
    MLadspaDescriptor->set_run_adding_gain        = nullptr;//dssi_set_run_adding_gain_callback;
    MLadspaDescriptor->deactivate                 = dssi_deactivate_callback;
    MLadspaDescriptor->cleanup                    = dssi_cleanup_callback;

    MDssiDescriptor->DSSI_API_Version             = 1;
    MDssiDescriptor->LADSPA_Plugin                = MLadspaDescriptor;
    MDssiDescriptor->configure                    = dssi_configure;
    MDssiDescriptor->get_program                  = dssi_get_program;
    MDssiDescriptor->select_program               = dssi_select_program;
    MDssiDescriptor->get_midi_controller_for_port = dssi_get_midi_controller_for_port;
    MDssiDescriptor->run_synth                    = dssi_run_synth;
    MDssiDescriptor->run_synth_adding             = nullptr;//dssi_run_synth_adding;
    MDssiDescriptor->run_multiple_synths          = nullptr;//dssi_run_multiple_synths;
    MDssiDescriptor->run_multiple_synths_adding   = nullptr;//dssi_run_multiple_synths_adding;
    //MInitialized = true;
    return MDssiDescriptor;
  }

  //----------

  LADSPA_Handle instantiate(unsigned long SampleRate) {
    // instance deleted in ~KODE_DssiInstance()
    KODE_Instance* instance = new INSTANCE(MDescriptor);
//    instance->on_open();
    // dssi_instance deleted in dssi_cleanup_callback
    KODE_DssiInstance* dssi_instance = new KODE_DssiInstance(instance,SampleRate);
    return (LADSPA_Handle)dssi_instance;
  }

//------------------------------
private: // ladspa callbacks
//------------------------------

  static
  LADSPA_Handle dssi_instantiate_callback(const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate) {
    KODE_DssiPlugin* plugin = (KODE_DssiPlugin*)Descriptor->ImplementationData;
    LADSPA_Handle instance = nullptr;
    if (plugin) instance = plugin->instantiate(SampleRate);
    return (LADSPA_Handle)instance;
    /*
    // instance deleted in ~KODE_LadspaInstance()
    KODE_Instance* instance = new INST(MDescriptor);
    instance->on_open();
    // ladspa_instance deleted in ladspa_cleanup_callback()
    KODE_LadspaInstance* ladspa_instance = new KODE_LadspaInstance(instance,SampleRate);
    return (LADSPA_Handle)ladspa_instance;
    */
  }

  //----------

  static
  void dssi_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_connect_port(Port,DataLocation);
  }

  //----------

  static
  void dssi_activate_callback(LADSPA_Handle Instance) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_activate();
  }

  //----------

  static
  void dssi_run_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_run(SampleCount);
  }

  //----------

  //static
  //void dssi_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
  //  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  //  if (instance) instance->dssi_run_adding(SampleCount);
  //}

  //----------

  //static
  //void dssi_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain) {
  //  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  //  if (instance) instance->dssi_set_run_adding_gain(Gain);
  //}

  //----------

  static
  void dssi_deactivate_callback(LADSPA_Handle Instance) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_deactivate();
  }

  //----------

  static
  void dssi_cleanup_callback(LADSPA_Handle Instance) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_cleanup();
    //DSSI_Trace("dssi: cleanup -> deleting instance\n");
    delete instance;
  }

//------------------------------
private: // dssi callbacks
//------------------------------

  static
  char* dssi_configure(LADSPA_Handle Instance, const char* Key, const char* Value) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) return instance->dssi_configure(Key,Value);
    return nullptr;
  }

  //----------

  static
  const DSSI_Program_Descriptor* dssi_get_program(LADSPA_Handle Instance, unsigned long Index) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) return instance->dssi_get_program(Index);
    return nullptr;
  }

  //----------

  static
  void dssi_select_program(LADSPA_Handle Instance, unsigned long Bank, unsigned long Program) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_select_program(Bank,Program);
  }

  //----------

  static
  int dssi_get_midi_controller_for_port(LADSPA_Handle Instance, unsigned long Port) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) return instance->dssi_get_midi_controller_for_port(Port);
    return 0;
  }

  //----------

  static
  void dssi_run_synth(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount) {
    KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
    if (instance) instance->dssi_run_synth(SampleCount,Events,EventCount);
  }

  //----------

  //static
  //void dssi_run_synth_adding(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount) {
  //  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  //  if (instance) instance->dssi_run_synth_adding(SampleCount,Events,EventCount);
  //}

  //----------

  //static
  //void dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
  //  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  //  if (instance) instance->dssi_run_multiple_synths(InstanceCount,Instances,SampleCount,Events,EventCounts);
  //}

  //----------

  //static
  //void dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
  //  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  //  if (instance) instance->dssi_run_multiple_synths_adding(InstanceCount,Instances,SampleCount,Events,EventCounts);
  //}


};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/**
 * DSSI supports a plugin discovery method similar to that of LADSPA:
 *
 * - DSSI hosts may wish to locate DSSI plugin shared object files by
 *    searching the paths contained in the DSSI_PATH and LADSPA_PATH
 *    environment variables, if they are present.  Both are expected
 *    to be colon-separated lists of directories to be searched (in
 *    order), and DSSI_PATH should be searched first if both variables
 *    are set.
 *
 * - Each shared object file containing DSSI plugins must include a
 *   function dssi_descriptor(), with the following function prototype
 *   and C-style linkage.  Hosts may enumerate the plugin types
 *   available in the shared object file by repeatedly calling
 *   this function with successive Index values (beginning from 0),
 *   until a return value of NULL indicates no more plugin types are
 *   available.  Each non-NULL return is the DSSI_Descriptor
 *   of a distinct plugin type.
 */

//----------------------------------------------------------------------

#define KODE_DSSI_MAIN_SYMBOL asm ("dssi_descriptor");
const DSSI_Descriptor* kode_dssi_entrypoint(unsigned long Index) KODE_DSSI_MAIN_SYMBOL

//----------

#define KODE_DSSI_MAIN(D,I,E)                                         \
                                                                      \
  KODE_DssiPlugin<D,I,E> _DSSI_PLUGIN;                                \
                                                                      \
  /*__attribute__((visibility("default")))*/                          \
  __KODE_DLLEXPORT                                                    \
  const DSSI_Descriptor* kode_dssi_entrypoint(unsigned long Index) {  \
    return _DSSI_PLUGIN.entrypoint(Index);                            \
  }

//----------------------------------------------------------------------
#endif
