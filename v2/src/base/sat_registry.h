#ifndef sat_registry_included
#define sat_registry_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/debug/sat_debug_print.h"
#include "plugin/clap/sat_clap.h"

class SAT_Registry;
extern void SAT_Register(SAT_Registry* ARegistry) __SAT_WEAK;
//extern const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __SAT_WEAK;

typedef SAT_Array<const clap_plugin_descriptor_t*>  SAT_DescriptorArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Registry {

//------------------------------
private:
//------------------------------

  bool                MIsInitialized  = false;
  SAT_DescriptorArray MDescriptors    = {};

//------------------------------
public:
//------------------------------

  SAT_Registry() {
  }

  //----------

  ~SAT_Registry() {
    // delete appended descriptors, factories, etc..
  }

//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    if (MIsInitialized) return;
    if (SAT_Register) SAT_Register(this);
    MIsInitialized = true;
  }

  //----------

  void cleanup() {
    if (MIsInitialized) {
    }
  }

//------------------------------
public:
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

  const clap_plugin_descriptor_t* findDescriptor(const char* plugin_id) {
    uint32_t num = MDescriptors.size();
    for (uint32_t i=0; i<num; i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return MDescriptors[i];
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define SAT_PLUGIN_ENTRY(DESC,PLUG)                                                                                               \
                                                                                                                                  \
  /* #include "plugin/sat_entry.h" */                                                                                             \
                                                                                                                                  \
  void SAT_Register(SAT_Registry* ARegistry) {                                                                                    \
    uint32_t index = ARegistry->getNumDescriptors();                                                                              \
    /*SAT_Log("SAT_Register -> id %s index %i\n",DESC.id,index);*/                                                                \
    ARegistry->registerDescriptor(&DESC);                                                                                         \
  }                                                                                                                               \
                                                                                                                                  \
  /* ----- */                                                                                                                     \
                                                                                                                                  \
  const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) { \
    /*SAT_Log("SAT_CreatePlugin (index %i)\n",AIndex);*/                                                                          \
    if (AIndex == 0) {                                                                                                            \
      SAT_Plugin* plugin = new PLUG(ADescriptor,AHost); /* deleted in: ... */                                                     \
      return plugin->getClapPlugin();                                                                                             \
    }                                                                                                                             \
    return nullptr;                                                                                                               \
  }

//----------------------------------------------------------------------
#endif
