#ifndef sat_clap_entry_included
#define sat_clap_entry_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_factory.h"

//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  SAT_Print("plugin_path %s\n",plugin_path);
  SAT_GLOBAL.setPluginPath(plugin_path);
  return true;
}

//----------

void clap_entry_deinit_callback(void) {
  SAT_Print("\n");
}

//----------

const void* clap_entry_get_factory_callback(const char* factory_id) {
  SAT_Print("factory_id: %s\n",factory_id);
  int32_t index = SAT_GLOBAL.REGISTRY.findFactory(factory_id);
  if (index > 0) {
    const clap_plugin_factory_t* factory = SAT_GLOBAL.REGISTRY.getFactory(index);
    //SAT_Print("CLAP: ENTRY.get_factory (factory_id %s) -> %p\n",factory_id,factory);
    return factory;
  }
  // no factory found.. try default ones
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &SAT_CLAP_PLUGIN_FACTORY;
  }
  #ifdef SAT_PLUGIN_USE_PRESET_DISCOVERY_FACTORY
    if (strcmp(factory_id,CLAP_PRESET_DISCOVERY_FACTORY_ID) == 0) {
      return &SAT_CLAP_PRESET_DISCOVERY_FACTORY;
    }
  #endif
  //if (strcmp(factory_id,CLAP_PLUGIN_INVALIDATION_FACTORY_ID) == 0) {
  //  return &SAT_CLAP_PLUGIN_INVALIDATION_FACTORY;
  //}
  #ifdef SAT_PLUGIN_USE_ARA_FACTORY
    if (strcmp(factory_id,CLAP_EXT_ARA_FACTORY) == 0) {
      return &SAT_CLAP_ARA_FACTORY;
    }
  #endif
  // none..
  //SAT_Print("CLAP: ENTRY.get_factory (factory_id %s) -> null (no factory found)\n",factory_id);
  return nullptr;
}

//----------------------------------------------------------------------

//__SAT_EXPORT
CLAP_EXPORT extern const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = clap_entry_init_callback,
  .deinit       = clap_entry_deinit_callback,
  .get_factory  = clap_entry_get_factory_callback
};

//----------------------------------------------------------------------
#endif
