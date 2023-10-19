#ifndef sat_registry_included
#define sat_registry_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/debug/sat_debug_print.h"

class SAT_Registry;
extern void SAT_Register(SAT_Registry* ARegistry) __SAT_WEAK;
//extern const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __SAT_WEAK;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Registry {

//------------------------------
private:
//------------------------------

  bool MIsInitialized = false;

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
  }

//------------------------------
public:
//------------------------------

  // returns index of descriptor

  uint32_t registerDescriptor() {
    return false;
  }

  //----------

  // returns nullptr if index out of range

  void* getDescriptor(uint32_t index) {
    return nullptr;
  }

  //----------

  // returns -1 if plugin not found

  int32_t findPlugin(const char* plugin_id) {
    return -1;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#if 0

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

#endif // 0

//----------------------------------------------------------------------
#endif
