#ifndef sat_clap_plugin_factory_incuded
#define sat_clap_plugin_factory_incuded
//----------------------------------------------------------------------

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  SAT_Print("\n");
  int32_t num = SAT_GLOBAL.REGISTRY.getNumDescriptors();
  return num;
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  SAT_Print("index %i\n",index);
  const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
  return descriptor;
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  SAT_Print("plugin_id %i\n",plugin_id);
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
  .get_plugin_count      = clap_plugin_factory_get_plugin_count_callback,
  .get_plugin_descriptor = clap_plugin_factory_get_plugin_descriptor_callback,
  .create_plugin         = clap_plugin_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif