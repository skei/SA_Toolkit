#ifndef sat_registry_included
#define sat_registry_included
//----------------------------------------------------------------------

#include "base/sat_print.h"
#include "plugin/lib/sat_clap.h"

typedef SAT_Array<const clap_plugin_descriptor_t*> SAT_ClapDescriptorArray;
typedef SAT_Array<const void*> SAT_ClapFactoryArray;
typedef SAT_Array<const char*> SAT_ClapFactoryIdArray;

class SAT_Registry;
//extern void SAT_Register(SAT_Registry* ARegistry) __attribute__ ((weak));
extern void SAT_Register(SAT_Registry* ARegistry) __SAT_WEAK;

class SAT_ClapPlugin;
//extern SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __attribute__ ((weak));
extern SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __SAT_WEAK;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Registry {

//------------------------------
private:
//------------------------------

  bool                    MInitialized      = false;
  SAT_Print*              MPrint            = nullptr;
  SAT_ClapDescriptorArray MDescriptors      = {};
  SAT_ClapFactoryArray    MFactories        = {};
  SAT_ClapFactoryIdArray  MFactoryIds       = {};
  char                    MPluginPath[1024] = {0};

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

  bool initialize(SAT_Print* APrint) {
    //APrint->print("SAT_Registry.initialize\n");
    if (MInitialized) return true;
    MPrint = APrint;
    //MIniFile.load();
    if (SAT_Register) SAT_Register(this);
    MInitialized = true;
    return true;
  }

  //----------

  void cleanup() {
    //SAT_PRINT("SAT_Registry.cleanup\n");
    if (MInitialized) {
      //MIniFile.save();
    }
  }

//------------------------------
public: // descriptors
//------------------------------

  // returns index of descriptor

  uint32_t registerDescriptor(const clap_plugin_descriptor_t* descriptor) {
    //SAT_PRINT("SAT_Registry.registerDescriptor\n");
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

  uint32_t registerFactory(const char* factory_id, const void* factory) {
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

  const void* getFactory(uint32_t index) {
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

#define SAT_PLUGIN_ENTRY(DESC,PLUG)                                                                                           \
                                                                                                                              \
  void SAT_Register(SAT_Registry* ARegistry) {                                                                                \
    ARegistry->registerDescriptor(&DESC);                                                                                     \
  }                                                                                                                           \
                                                                                                                              \
  SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {  \
    if (AIndex == 0) {                                                                                                        \
      SAT_Plugin* plugin = new PLUG(ADescriptor,AHost);                                                                       \
      return plugin;                                                                                                          \
    }                                                                                                                         \
    return nullptr;                                                                                                           \
  }

//----------------------------------------------------------------------
#endif