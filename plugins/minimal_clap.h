
#include "plugin/clap/sat_clap_plugin.h"

//----------------------------------------------------------------------
// descriptor
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin",
  .name         = "my plugin",
  .vendor       = "me",
  .url          = "https://website.com/myPlugin",
  .manual_url   = "https://website.com/myPlugin/manual.pdf",
  .support_url  = "https://website.com/support",
  .version      = "0.0.0",
  .description  = "my precious plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SAT_ClapPlugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
  }

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    return true;
  }

  void destroy() final {
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    return true;
  }

  void deactivate() final {
  }

  bool start_processing() final {
    return true;
  }

  void stop_processing() final {
  }

  void reset() final {
  }

  clap_process_status process(const clap_process_t *process) final {
    return CLAP_PROCESS_CONTINUE;
  }

  const void* get_extension(const char *id) final {
    return nullptr;
  }

  void on_main_thread() final {
  }

};

//----------------------------------------------------------------------
// plugin factory
//----------------------------------------------------------------------

uint32_t get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  SAT_PRINT;
  return 1;
}

const clap_plugin_descriptor_t* get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  SAT_Print("index: %i\n",index);
  if (index == 0) return &myDescriptor;
  return nullptr;
}

const clap_plugin_t* create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  SAT_Print("plugin_id: %s\n",plugin_id);
  if (strcmp(plugin_id,myDescriptor.id) == 0) {
    myPlugin* plugin = new myPlugin(&myDescriptor,host);
    return plugin->getClapPlugin();
  }
  return nullptr;
}

const clap_plugin_factory_t plugin_factory {
  .get_plugin_count      = get_plugin_count_callback,
  .get_plugin_descriptor = get_plugin_descriptor_callback,
  .create_plugin         = create_plugin_callback
};

//----------------------------------------------------------------------
// clap entry
//----------------------------------------------------------------------

bool init_callback(const char *plugin_path) {
  SAT_Print("plugin_path: %s\n",plugin_path);
  return true;
}

void deinit_callback(void) {
  SAT_Print("\n");
}

const void* get_factory_callback(const char* factory_id) {
  SAT_Print("factory_id: %s\n",factory_id);
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) return &plugin_factory;
  return nullptr;
}

CLAP_EXPORT extern const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = init_callback,
  .deinit       = deinit_callback,
  .get_factory  = get_factory_callback
};
