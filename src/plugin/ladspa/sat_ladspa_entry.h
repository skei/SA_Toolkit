#ifndef sat_ladspa_entry_included
#define sat_ladspa_entry_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/ladspa/sat_ladspa.h"
#include "plugin/ladspa/sat_ladspa_plugin.h"

//#include "sat.h"
//#include "plugin/sat_descriptor.h"
////#include "plugin/sat_instance_listener.h"
//#include "plugin/ladspa/sat_ladspa.h"
//#include "plugin/ladspa/sat_ladspa_instance.h"
//#include "plugin/ladspa/sat_ladspa_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LadspaEntry {
  
//------------------------------
private:
//------------------------------

  LADSPA_Descriptor*    MLadspaDescriptor = {0};

//  SAT_Descriptor*      MDescriptor       = nullptr;
//  //LADSPA_Descriptor*    MLadspaDescriptor = SAT_NULL;
//
//  //int*                  MPortDescriptors  = SAT_NULL;
//  //char**                MPortNames        = SAT_NULL;
//  //char*                 MPortNamesBuffer  = SAT_NULL;
//  //LADSPA_PortRangeHint* MPortRangeHints   = SAT_NULL;
//
//  SAT_LadspaPorts   MPorts;
//
//  char*                 MNameBuffer       = nullptr;
//  char*                 MLabelBuffer      = nullptr;
//  char*                 MMakerBuffer      = nullptr;
//  char*                 MCopyrightBuffer  = nullptr;

//------------------------------
public:
//------------------------------

  SAT_LadspaEntry() {
  }
  
  //----------

  ~SAT_LadspaEntry() {
//    //SAT_TRACE;
//    if (MDescriptor)        delete MDescriptor;
//    if (MNameBuffer)        free(MNameBuffer);
//    if (MLabelBuffer)       free(MLabelBuffer);
//    if (MMakerBuffer)       free(MMakerBuffer);
//    if (MCopyrightBuffer)   free(MCopyrightBuffer);
//    SAT_LadspaCleanupPorts(&MPorts);
//    if (MLadspaDescriptor)  free(MLadspaDescriptor);
  }
    

//------------------------------
public:
//------------------------------

  /*
    ? carla calles ladspa_descriptor a second time before deleting the first..
  */

  const LADSPA_Descriptor* ladspa_entrypoint(unsigned long Index) {
    //return nullptr;

    if (Index > 0) return nullptr;
    if (MLadspaDescriptor) return MLadspaDescriptor;
    
//    MDescriptor = new DESCRIPTOR();
    
    MLadspaDescriptor = (LADSPA_Descriptor*)malloc(sizeof(LADSPA_Descriptor));
    memset(MLadspaDescriptor,0,sizeof(LADSPA_Descriptor));
    //setDefaultParamValues();
    
//    MNameBuffer       = (char*)malloc(SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
//    MLabelBuffer      = (char*)malloc(SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
//    MMakerBuffer      = (char*)malloc(SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
//    MCopyrightBuffer  = (char*)malloc(SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
//    strncpy(MNameBuffer,      MDescriptor->getName(),        SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
//    strncpy(MLabelBuffer,     MDescriptor->getName(),        SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1); // todo: make valid c symbol (see lv2)
//    strncpy(MMakerBuffer,     MDescriptor->getAuthor(),      SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
//    strncpy(MCopyrightBuffer, MDescriptor->getLicenseText(), SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);

//    uint32_t numports = SAT_LadspaSetupPorts(MDescriptor,&MPorts);
    
    //SAT_Assert(port == numports);
    MLadspaDescriptor->UniqueID            = 0;       //MDescriptor->getShortId();
    MLadspaDescriptor->Label               = "";      //MLabelBuffer;
    MLadspaDescriptor->Properties          = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN
    MLadspaDescriptor->Name                = "";      //MNameBuffer;
    MLadspaDescriptor->Maker               = "";      //MMakerBuffer;
    MLadspaDescriptor->Copyright           = "";      //MCopyrightBuffer;
    MLadspaDescriptor->PortCount           = 0;       //numports;
    MLadspaDescriptor->PortDescriptors     = nullptr; //MPorts.descriptors;
    MLadspaDescriptor->PortNames           = nullptr; //(const char* const*)MPorts.names;
    MLadspaDescriptor->PortRangeHints      = nullptr; //MPorts.rangeHints;
    MLadspaDescriptor->ImplementationData  = this;    //SAT_NULL;
    MLadspaDescriptor->instantiate         = ladspa_instantiate_callback;
    MLadspaDescriptor->connect_port        = ladspa_connect_port_callback;
    MLadspaDescriptor->activate            = ladspa_activate_callback;
    MLadspaDescriptor->run                 = ladspa_run_callback;
    MLadspaDescriptor->run_adding          = ladspa_run_adding_callback;
    MLadspaDescriptor->set_run_adding_gain = ladspa_set_run_adding_gain_callback;
    MLadspaDescriptor->deactivate          = ladspa_deactivate_callback;
    MLadspaDescriptor->cleanup             = ladspa_cleanup_callback;
    return MLadspaDescriptor;
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

/*__attribute__((visibility("default")))*/                              \
__SAT_EXPORT                                                           \
const LADSPA_Descriptor* sat_ladspa_entrypoint(unsigned long Index) {  \
  SAT_Print("Index %i\n",Index);
  return SAT_GOBAL_LADSPA_ENTRY.ladspa_entrypoint(Index);                              \
}

//----------------------------------------------------------------------
#endif


