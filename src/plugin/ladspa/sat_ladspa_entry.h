#ifndef sat_ladspa_entry_included
#define sat_ladspa_entry_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_host_implementation.h"
#include "plugin/ladspa/sat_ladspa.h"
#include "plugin/ladspa/sat_ladspa_plugin.h"
#include "plugin/ladspa/sat_ladspa_utils.h"

//#include "sat.h"
//#include "plugin/sat_descriptor.h"
////#include "plugin/sat_instance_listener.h"
//#include "plugin/ladspa/sat_ladspa.h"
//#include "plugin/ladspa/sat_ladspa_instance.h"
//#include "plugin/ladspa/sat_ladspa_utils.h"

//----------------------------------------------------------------------

struct SAT_LadspaPorts {
  int*                  descriptors  = nullptr;
  char**                names        = nullptr;
  char*                 namesBuffer  = nullptr;
  LADSPA_PortRangeHint* rangeHints   = nullptr;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LadspaEntry {
  
//------------------------------
private:
//------------------------------

  //LADSPA_Descriptor*    MLadspaDescriptor = {0};

//  SAT_Descriptor*      MDescriptor       = nullptr;
//  //LADSPA_Descriptor*    MLadspaDescriptor = SAT_NULL;
//
//  //int*                  MPortDescriptors  = SAT_NULL;
//  //char**                MPortNames        = SAT_NULL;
//  //char*                 MPortNamesBuffer  = SAT_NULL;
//  //LADSPA_PortRangeHint* MPortRangeHints   = SAT_NULL;

  SAT_LadspaPorts   MPorts      = {0};
  
//  char              MName[SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH] = {0};
//  char              MLabel[SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH] = {0};
//  char              MMaker[SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH] = {0};
//  char              MCopyright[SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH] = {0};

//------------------------------
public:
//------------------------------

  SAT_LadspaEntry() {
    SAT_PRINT;
  }
  
  //----------

  ~SAT_LadspaEntry() {
    SAT_PRINT;
//    if (MDescriptor)        delete MDescriptor;
//    if (MNameBuffer)        free(MNameBuffer);
//    if (MLabelBuffer)       free(MLabelBuffer);
//    if (MMakerBuffer)       free(MMakerBuffer);
//    if (MCopyrightBuffer)   free(MCopyrightBuffer);
    cleanup_ports(&MPorts);
//    if (MLadspaDescriptor)  free(MLadspaDescriptor);
  }

//------------------------------
public:
//------------------------------

  /*
    ? carla calles ladspa_descriptor a second time before deleting the first..
  */

  const LADSPA_Descriptor* ladspa_entrypoint(unsigned long Index) {
    SAT_PRINT;
    
    uint32_t clap_count = SAT_GLOBAL.REGISTRY.getNumDescriptors();
    if (Index >= clap_count) return nullptr;
    
    LADSPA_Descriptor* lad_descriptor = (LADSPA_Descriptor*)malloc(sizeof(LADSPA_Descriptor));    // where is this free'd?
    memset(lad_descriptor,0,sizeof(LADSPA_Descriptor));
    
    const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(Index);
    
    uint32_t unique_id = SAT_HashString(clap_descriptor->id);
    
    //strncpy(MName,       clap_descriptor->name,   SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    //strncpy(MLabel,      clap_descriptor->name,   SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1); // todo: make valid c symbol (see lv2)
    //strncpy(MMaker,      clap_descriptor->vendor, SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    //strncpy(MCopyright,  "",                      SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    //SAT_MakeValidSymbol(MLabel);

    uint32_t numports = setup_ports(clap_descriptor,Index,&MPorts);

    //SAT_Assert(port == numports);
    lad_descriptor->UniqueID            = unique_id;
    lad_descriptor->Label               = clap_descriptor->name;
    lad_descriptor->Properties          = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN
    lad_descriptor->Name                = clap_descriptor->name;
    lad_descriptor->Maker               = clap_descriptor->vendor;
    lad_descriptor->Copyright           = "";
    lad_descriptor->PortCount           = numports;
    lad_descriptor->PortDescriptors     = MPorts.descriptors;
    lad_descriptor->PortNames           = (const char* const*)MPorts.names;
    lad_descriptor->PortRangeHints      = MPorts.rangeHints;
    lad_descriptor->ImplementationData  = this;    //SAT_NULL;
    lad_descriptor->instantiate         = ladspa_instantiate_callback;
    lad_descriptor->connect_port        = ladspa_connect_port_callback;
    lad_descriptor->activate            = ladspa_activate_callback;
    lad_descriptor->run                 = ladspa_run_callback;
    lad_descriptor->run_adding          = ladspa_run_adding_callback;
    lad_descriptor->set_run_adding_gain = ladspa_set_run_adding_gain_callback;
    lad_descriptor->deactivate          = ladspa_deactivate_callback;
    lad_descriptor->cleanup             = ladspa_cleanup_callback;
    return lad_descriptor;
  }
  
  //----------

  /* This member is a function pointer that instantiates a plugin. A
     handle is returned indicating the new plugin instance. The
     instantiation function accepts a sample rate as a parameter. The
     plugin descriptor from which this instantiate function was found
     must also be passed. This function must return NULL if
     instantiation fails.

     Note that instance initialisation should generally occur in
     activate() rather than here. */

  LADSPA_Handle ladspa_instantiate(unsigned long SampleRate) {
    SAT_PRINT;
    return nullptr;
    
    SAT_LadspaPlugin* plugin = new SAT_LadspaPlugin();
    return plugin->getHandle();
    
    // instance deleted in ~SAT_LadspaPlugin()
//    SAT_Instance* instance = new INSTANCE(MDescriptor);
////    instance->on_open();
    // ladspa_plugin deleted in ladspa_cleanup_callback()
//    SAT_LadspaPlugin* ladspa_plugin = new SAT_LadspaPlugin(instance,SampleRate);
//    return (LADSPA_Handle)ladspa_plugin;
  }
  
//------------------------------
public:
//------------------------------

  // returns numports

  uint32_t setup_ports(const clap_plugin_descriptor* clap_descriptor, uint32_t AIndex, SAT_LadspaPorts* APorts) {
    SAT_PRINT;
    
    SAT_HostImplementation* host = new SAT_HostImplementation();
    const clap_host_t* clap_host = host->getHost();
    const clap_plugin_t* clap_plugin = SAT_CreatePlugin(AIndex,clap_descriptor,clap_host);
    SAT_Plugin* plugin = (SAT_Plugin*)clap_plugin->plugin_data;
    clap_plugin->init(clap_plugin);
    
    //uint32_t numin      = ADescriptor->getNumInputs();
    //uint32_t numout     = ADescriptor->getNumOutputs();
    //uint32_t numpar     = ADescriptor->getNumParameters();
    //uint32_t numports   = numin + numout + numpar;

    uint32_t numin = 0;
    if (plugin->getNumAudioInputPorts() > 0) {
      numin = plugin->getAudioInputPort(0)->getInfo()->channel_count;
    }
    uint32_t numout = 0;
    if (plugin->getNumAudioOutputPorts() > 0) {
      numout = plugin->getAudioOutputPort(0)->getInfo()->channel_count;
    }
    uint32_t numpar = plugin->getNumParameters();
    //SAT_Print("numin %i numout %i numpat %i\n",numin,numout,numpar);

    uint32_t numports  = numin + numout + numpar;
    if (SAT_ClapIsInstrument(clap_descriptor)) {
      numports += 1;
    }
    
    APorts->descriptors = (LADSPA_PortDescriptor*)malloc(numports * sizeof(LADSPA_PortDescriptor));
    APorts->names       = (char**)malloc(numports * sizeof(char*));
    APorts->namesBuffer = (char*)malloc(numports * SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
    APorts->rangeHints  = (LADSPA_PortRangeHint*)malloc(numports * sizeof(LADSPA_PortRangeHint));
    uint32_t i = 0;
    uint32_t port = 0;
    for (i=0; i<numin; i++) {
      //const char* port_name = "Audio Input";
      APorts->descriptors[port] = LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT;
      //char* ptr = MPortNamesBuffer + (port * SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
      //SAT_Strncpy(ptr,port_name,SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
      //ptr[SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
      APorts->names[port] = (char*)"input";
      APorts->rangeHints[port].HintDescriptor = 0;
      //MPortRangeHints[port].LowerBound = 0;
      //MPortRangeHints[port].UpperBound = 0;
      port++;
    }
    for (i=0; i<numout; i++) {
      //const char* port_name = "Audio Output";
      APorts->descriptors[port] = LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT;
      //char* ptr = MPortNames + (port * SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
      //SAT_Strncpy(ptr,port_name,SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
      //ptr[SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
      APorts->names[port] = (char*)"output";
      APorts->rangeHints[port].HintDescriptor = 0;
      //MPortRangeHints[port].LowerBound = 0;
      //MPortRangeHints[port].UpperBound = 0;
      port++;
    }
    for (i=0; i<numpar; i++) {
      SAT_Parameter* param = plugin->getParameter(i);
      APorts->descriptors[port] = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;
      //const char* pname = param->getName();
      //const char* port_name = pname;
      //SAT_Trace("%i %s\n",i,port_name);
      char* ptr = APorts->namesBuffer + (port * SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
      strncpy(ptr,param->getName(),SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
      ptr[SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
      APorts->names[port] = ptr;//(char*)"param";
      // LADSPA_HINT_TOGGLED, LADSPA_HINT_SAMPLE_RATE, LADSPA_HINT_LOGARITHMIC
      // LADSPA_HINT_INTEGER,
      APorts->rangeHints[port].HintDescriptor = LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      APorts->rangeHints[port].LowerBound = param->getMinValue();
      APorts->rangeHints[port].UpperBound = param->getMaxValue();
      port++;
    }
    
    clap_plugin->destroy(clap_plugin);
    delete host;
    
    return port;
  }

  //----------

  void cleanup_ports(SAT_LadspaPorts* APorts) {
    SAT_PRINT;
    if (APorts->descriptors)   free(APorts->descriptors);
    if (APorts->names)         free(APorts->names);
    if (APorts->namesBuffer)   free(APorts->namesBuffer);
    if (APorts->rangeHints)    free(APorts->rangeHints);
  }
  

//------------------------------
private: // ladspa callbacks
//------------------------------

  static
  LADSPA_Handle ladspa_instantiate_callback(const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate) {
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Descriptor->ImplementationData;
    LADSPA_Handle instance = nullptr;
    if (plugin) instance = plugin->ladspa_instantiate(SampleRate);
    return (LADSPA_Handle)instance;
  }

  //----------

  static
  void ladspa_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {
    //LADSPA_Trace("ladspa: ladspa_connect_port_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_connect_port(Port,DataLocation);
  }

  //----------

  static
  void ladspa_activate_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_activate_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_activate();
  }

  //----------

  static
  void ladspa_run_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: ladspa_run_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_run(SampleCount);
  }

  //----------

  static
  void ladspa_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: ladspa_run_adding_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_run_adding(SampleCount);
  }

  //----------

  static
  void ladspa_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain) {
    //LADSPA_Trace("ladspa: ladspa_set_run_adding_gain_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_set_run_adding_gain(Gain);
  }

  //----------

  static
  void ladspa_deactivate_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_deactivate_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_deactivate();
  }

  //----------

  static
  void ladspa_cleanup_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_cleanup_callback\n");
    SAT_LadspaPlugin* plugin = (SAT_LadspaPlugin*)Instance;
    if (plugin) plugin->ladspa_cleanup();
    //LADSPA_Trace("ladspa: cleanup -> deleting instance\n");
// !!!
    delete plugin;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_LadspaEntry SAT_GOBAL_LADSPA_ENTRY = {};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  Accessing a Plugin:

  The exact mechanism by which plugins are loaded is host-dependent,
  however all most hosts will need to know is the name of shared
  object file containing the plugin types. To allow multiple hosts to
  share plugin types, hosts may wish to check for environment
  variable LADSPA_PATH. If present, this should contain a
  colon-separated path indicating directories that should be searched
  (in order) when loading plugin types.

  A plugin programmer must include a function called
  "ladspa_descriptor" with the following function prototype within
  the shared object file. This function will have C-style linkage (if
  you are using C++ this is taken care of by the `extern "C"' clause
  at the top of the file).

  A host will find the plugin shared object file by one means or
  another, find the ladspa_descriptor() function, call it, and
  proceed from there.

  Plugin types are accessed by index (not ID) using values from 0
  upwards. Out of range indexes must result in this function
  returning NULL, so the plugin count can be determined by checking
  for the least index that results in NULL being returned.
*/

//----------------------------------------------------------------------

#define SAT_LADSPA_MAIN_SYMBOL asm ("ladspa_descriptor");
const LADSPA_Descriptor* sat_ladspa_entrypoint(unsigned long Index) SAT_LADSPA_MAIN_SYMBOL

//----------

/*__attribute__((visibility("default")))*/
__SAT_EXPORT
const LADSPA_Descriptor* sat_ladspa_entrypoint(unsigned long Index) {
  SAT_Print("Index %i\n",Index);
  return SAT_GOBAL_LADSPA_ENTRY.ladspa_entrypoint(Index);
}

//----------------------------------------------------------------------
#endif


