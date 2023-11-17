#ifndef sat_registry_included
#define sat_registry_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/debug/sat_debug_print.h"
#include "base/utils/sat_inifile.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"

class SAT_Registry;
extern void SAT_Register(SAT_Registry* ARegistry) __SAT_WEAK;
//extern const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __SAT_WEAK;
extern SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __SAT_WEAK;

typedef SAT_Array<const clap_plugin_descriptor_t*>  SAT_DescriptorArray;
typedef SAT_Array<const clap_plugin_factory_t*>     SAT_FactoryArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Registry {

//------------------------------
private:
//------------------------------

  bool                  MIsInitialized  = false;
  SAT_DescriptorArray   MDescriptors    = {};
  SAT_FactoryArray      MFactories      = {};
  SAT_ConstCharPtrArray MFactoryIds     = {};

//SAT_IniFile           MIniFile        = {};

//------------------------------
public:
//------------------------------

  SAT_Registry() {
  }

  //----------

  ~SAT_Registry() {
  }

//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    if (MIsInitialized) return;
    //MIniFile.load();
    if (SAT_Register) SAT_Register(this);
    MIsInitialized = true;
  }

  //----------

  bool isInitialized() {
    return MIsInitialized;
  }

  //----------

  void cleanup() {
    if (MIsInitialized) {
      //MIniFile.save();
    }
  }

//------------------------------
public: // descriptors
//------------------------------

  // returns index of descriptor

  uint32_t registerDescriptor(const clap_plugin_descriptor_t* descriptor) {
    uint32_t index = MDescriptors.size();
    MDescriptors.push_back(descriptor);
    return index;
  }

  //----------

  uint32_t getNumDescriptors() {
    return MDescriptors.size();
  }

  // returns nullptr if index out of range

  const clap_plugin_descriptor_t* getDescriptor(uint32_t index) {
    if (index < MDescriptors.size()) {
      return MDescriptors[index];
    }
    return nullptr;
  }

  //----------

  // returns -1 if plugin not found
  
  int32_t findDescriptor(const char* plugin_id) {
    uint32_t num = MDescriptors.size();
    for (uint32_t i=0; i<num; i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return i;
    }
    return -1;
  }

//------------------------------
public: // factories
//------------------------------

  // returns index of factory

  uint32_t registerFactory(const char* factory_id, const clap_plugin_factory_t* factory) {
    uint32_t index = MFactories.size();
    MFactoryIds.push_back(factory_id);
    MFactories.push_back(factory);
    return index;
  }

  //----------

  uint32_t getNumFactories() {
    return MFactories.size();
  }

  // returns nullptr if index out of range

  const clap_plugin_factory_t* getFactory(uint32_t index) {
    if (index < MFactories.size()) {
      return MFactories[index];
    }
    return nullptr;
  }

  //----------

  // returns -1 if plugin not found
  
  int32_t findFactory(const char* factory_id) {
    uint32_t num = MFactories.size();
    for (uint32_t i=0; i<num; i++) {
      if (strcmp(factory_id,MFactoryIds[i]) == 0) return i;
    }
    return -1;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define SAT_PLUGIN_ENTRY(DESC,PLUG)                                                                                                   \
                                                                                                                                      \
  /* #include "plugin/sat_entry.h" */                                                                                                 \
                                                                                                                                      \
  void SAT_Register(SAT_Registry* ARegistry) {                                                                                        \
    uint32_t index = ARegistry->getNumDescriptors();                                                                                  \
    /*SAT_Print("index %i = id %s\n",index,DESC.id);*/                                                                                \
    ARegistry->registerDescriptor(&DESC);                                                                                             \
  }                                                                                                                                   \
                                                                                                                                      \
  /* ----- */                                                                                                                         \
                                                                                                                                      \
  SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {          \
    /*SAT_Print("index %i\n",AIndex);*/                                                                                               \
    if (AIndex == 0) {                                                                                                                \
      SAT_Plugin* plugin = new PLUG(ADescriptor,AHost); /* deleted in: ... */                                                         \
      return plugin;                                                                                                                  \
    }                                                                                                                                 \
    return nullptr;                                                                                                                   \
  }

//----------------------------------------------------------------------
#endif
