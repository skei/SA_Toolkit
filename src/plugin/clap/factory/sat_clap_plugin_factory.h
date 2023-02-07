#ifndef sat_clap_plugin_factory_incuded
#define sat_clap_plugin_factory_incuded
//----------------------------------------------------------------------

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  int32_t num = SAT_GLOBAL.REGISTRY.getNumDescriptors();
  SAT_Log("CLAP: FACTORY.get_plugin_count -> %i\n",num);
  return num;
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
  SAT_Log("CLAP: FACTORY.get_plugin_descriptor (index %i) -> %p\n",index,descriptor);
  return descriptor;
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  if (SAT_CreatePlugin) {
    int32_t index = SAT_GLOBAL.REGISTRY.findDescriptor(plugin_id);
    //LOG.print("  - index %i\n",index);
    if (index >= 0) {
      const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
      const clap_plugin_t* plugin = SAT_CreatePlugin(index,descriptor,host);//,host);
      SAT_Log("CLAP: FACTORY.create_plugin (plugin_id %s) -> %p\n",plugin_id,plugin);
      return plugin;

    }
  }
  SAT_Log("CLAP: FACTORY.create_plugin (plugin_id %s) -> null\n",plugin_id);
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
