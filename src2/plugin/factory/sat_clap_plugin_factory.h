#ifndef sat_clap_plugin_factory_incuded
#define sat_clap_plugin_factory_incuded
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "plugin/plugin/sat_clap_plugin.h"

//----------------------------------------------------------------------

uint32_t sat_clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  int32_t num = SAT_GLOBAL.REGISTRY.getNumDescriptors();
  //SAT_PRINT("num: %i\n",num);
  return num;
}

//----------

const clap_plugin_descriptor_t* sat_clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  //SAT_PRINT("index: %i\n",index);
  const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
  return descriptor;
}

//----------

const clap_plugin_t* sat_clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  //SAT_PRINT("plugin_id: %s\n",plugin_id);
  if (SAT_CreatePlugin) {
    int32_t index = SAT_GLOBAL.REGISTRY.findDescriptor(plugin_id);
    if (index >= 0) {
      const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
      SAT_ClapPlugin* plugin = SAT_CreatePlugin(index,descriptor,host);
      return  plugin->getClapPlugin();
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------

const clap_plugin_factory_t SAT_CLAP_PLUGIN_FACTORY {
  .get_plugin_count      = sat_clap_plugin_factory_get_plugin_count_callback,
  .get_plugin_descriptor = sat_clap_plugin_factory_get_plugin_descriptor_callback,
  .create_plugin         = sat_clap_plugin_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif