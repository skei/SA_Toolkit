#ifndef sat_clap_entry_included
#define sat_clap_entry_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "plugin/factory/sat_clap_factory.h"

//----------------------------------------------------------------------

bool sat_clap_entry_init_callback(const char *plugin_path) {
  SAT_PRINT("plugin_path: %s\n",plugin_path);
  //SAT_GLOBAL.setPluginPath(plugin_path);
  SAT_GLOBAL.clapInit(plugin_path);
  return true;
}

//----------

void sat_clap_entry_deinit_callback(void) {
  SAT_PRINT("\n");
  SAT_GLOBAL.clapDeInit();
}

//----------

const void* sat_clap_entry_get_factory_callback(const char* factory_id) {

  //SAT_PRINT("factory_id: %s\n",factory_id);

  int32_t index = SAT_GLOBAL.REGISTRY.findFactory(factory_id);
  if (index > 0) {
    const clap_plugin_factory_t* factory = (const clap_plugin_factory_t*)SAT_GLOBAL.REGISTRY.getFactory(index);
    return factory;
  }

  // no factory found.. try default ones

  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    //SAT_PRINT("plugin factory\n");
    return &SAT_CLAP_PLUGIN_FACTORY;
  }

  #ifdef SAT_PLUGIN_INCLUDE_CLAP_PRESET_DISCOVERY_FACTORY
    if (strcmp(factory_id,CLAP_PRESET_DISCOVERY_FACTORY_ID) == 0) {
      return &SAT_CLAP_PRESET_DISCOVERY_FACTORY;
    }
    if (strcmp(factory_id,CLAP_PRESET_DISCOVERY_FACTORY_ID_COMPAT) == 0) {
      return &SAT_CLAP_PRESET_DISCOVERY_FACTORY;
    }
  #endif

  //if (strcmp(factory_id,CLAP_PLUGIN_INVALIDATION_FACTORY_ID) == 0) {
  //  return &SAT_CLAP_PLUGIN_INVALIDATION_FACTORY;
  //}

  #ifdef SAT_PLUGIN_INCLUDE_CLAP_ARA_FACTORY
    if (strcmp(factory_id,CLAP_EXT_ARA_FACTORY) == 0) {
      return &SAT_CLAP_ARA_FACTORY;
    }
  #endif
  
  // none..
  return nullptr;
}

//----------------------------------------------------------------------

//__SAT_EXPORT
CLAP_EXPORT extern const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = sat_clap_entry_init_callback,
  .deinit       = sat_clap_entry_deinit_callback,
  .get_factory  = sat_clap_entry_get_factory_callback
};

//----------------------------------------------------------------------
#endif
