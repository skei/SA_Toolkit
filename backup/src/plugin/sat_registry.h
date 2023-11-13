#ifndef sat_registry_included
#define sat_registry_included
//----------------------------------------------------------------------

#include "plugin/clap/sat_clap.h"
//#include "plugin/clap/sat_clap_plugin.h"

//----------

class SAT_Registry;
extern void SAT_Register(SAT_Registry* ARegistry) __SAT_WEAK;
extern const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __SAT_WEAK;


//----------

typedef SAT_Array<const clap_plugin_descriptor_t*> SAT_DescriptorArray;
typedef SAT_Array<const clap_plugin_factory_t*> SAT_FactoryArray;
//typedef SAT_Array<const clap_preset_discovery_provider_t*> SAT_PresetDiscoveryProviderArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Registry {

//------------------------------
private:
//------------------------------

  bool                              MIsInitialized            = false;
  SAT_DescriptorArray               MDescriptors              = {};
  SAT_FactoryArray                  MFactories                = {};
  SAT_ConstCharPtrArray             MFactoryIds               = {};
  //SAT_PresetDiscoveryProviderArray  MPresetDiscoveryProviders = {};

//------------------------------
public:
//------------------------------

  SAT_Registry() {
    //initialize();
  }

  //----------

  ~SAT_Registry() {
  }

//------------------------------
public:
//------------------------------

  void initialize() {
    if (MIsInitialized) return;
    if (SAT_Register) SAT_Register(this);
    MIsInitialized = true;
  }

  //----------

  void cleanup() {
    // delete appended descriptors, factories, etc..
  }

//------------------------------
public: // descriptors
//------------------------------

  int32_t registerDescriptor(const clap_plugin_descriptor_t* ADescriptor) {
    int32_t index = MDescriptors.size();
    MDescriptors.append(ADescriptor);
    return index;
  }

  //----------

  uint32_t getNumDescriptors() {
    return MDescriptors.size();
  }

  //----------

  const clap_plugin_descriptor_t* getDescriptor(uint32_t AIndex) {
    return MDescriptors[AIndex];
  }

  //----------

  /*const clap_plugin_descriptor_t**/ int32_t findDescriptor(const char* AId) {
    for (uint32_t i=0; i<MDescriptors.size(); i++) {
      if (strcmp(MDescriptors[i]->id,AId) == 0) return i;//MDescriptors[i];

    }
    return -1;//nullptr;
  }

//------------------------------
public: // factories
//------------------------------

  int32_t registerFactory(const clap_plugin_factory_t* AFactory, const char* AId) {
    int32_t index = MFactories.size();
    MFactories.append(AFactory);
    MFactoryIds.append(AId);
    return index;
  }

  //----------

  uint32_t getNumFactories() {
    return MFactories.size();
  }

  //----------

  const clap_plugin_factory_t* getFactory(uint32_t AIndex) {
    return MFactories[AIndex];
  }

  //----------

  int32_t findFactory(const char* AId) {
    for (uint32_t i=0; i<MFactoryIds.size(); i++) {
      if (strcmp(MFactoryIds[i],AId) == 0) return i;
    }
    return -1;
  }

//------------------------------
public: // preset discovery providers
//------------------------------

//  int32_t registerPresetDiscoveryProvider(const clap_preset_discovery_provider_t* AProvider) {
//    int32_t index = MPresetDiscoveryProviders.size();
//    MPresetDiscoveryProviders.append(AProvider);
//    return index;
//  }

  //----------

//  uint32_t getNumPresetDiscoveryProviders() {
//    return MPresetDiscoveryProviders.size();
//  }

  //----------

//  const clap_preset_discovery_provider_t* getPresetDiscoveryProvider(uint32_t AIndex) {
//    return MPresetDiscoveryProviders[AIndex];
//  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#define SAT_PLUGIN_ENTRY(DESC,PLUG)                                                                                               \
                                                                                                                                  \
  /* #include "plugin/sat_entry.h" */                                                                                             \
                                                                                                                                  \
  void SAT_Register(SAT_Registry* ARegistry) {                                                                                    \
    uint32_t index = ARegistry->getNumDescriptors();                                                                              \
    SAT_Log("SAT_Register -> id %s index %i\n",DESC.id,index);                                                                    \
    ARegistry->registerDescriptor(&DESC);                                                                                         \
  }                                                                                                                               \
                                                                                                                                  \
  /* ----- */                                                                                                                     \
                                                                                                                                  \
  const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) { \
    SAT_Log("SAT_CreatePlugin (index %i)\n",AIndex);                                                                              \
    if (AIndex == 0) {                                                                                                            \
      SAT_Plugin* plugin = new PLUG(ADescriptor,AHost); /* deleted in: ... */                                                     \
      return plugin->getClapPlugin();                                                                                             \
    }                                                                                                                             \
    return nullptr;                                                                                                               \
  }

//----------------------------------------------------------------------
#endif

