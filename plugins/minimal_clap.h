
#include "sat.h"
#include "plugin/clap/sat_clap_plugin.h"

//----------------------------------------------------------------------
//
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
//
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
// entry
//----------------------------------------------------------------------

//#include "plugin/clap/sat_clap_entry.h"
//SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

//----------------------------------------------------------------------

#if 0

#include "plugin/clap/sat_clap_entry.h"

void SAT_Register(SAT_Registry* ARegistry) {
  ARegistry->registerDescriptor(&myDescriptor);
}

SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) return new myPlugin(ADescriptor,AHost);
  return nullptr;
}

#endif // 0

//----------------------------------------------------------------------

//------------------------------
// factory
//------------------------------

uint32_t get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  SAT_PRINT;
  return 1;
}

const clap_plugin_descriptor_t* get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  SAT_PRINT;
  if (index == 0) return &myDescriptor;
  return nullptr;
}

const clap_plugin_t* create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  SAT_PRINT;
  myPlugin* plugin = new myPlugin(&myDescriptor,host);
  return plugin->getClapPlugin();
}

const clap_plugin_factory_t plugin_factory {
  .get_plugin_count      = get_plugin_count_callback,
  .get_plugin_descriptor = get_plugin_descriptor_callback,
  .create_plugin         = create_plugin_callback
};

//------------------------------
// entry
//------------------------------

bool init_callback(const char *plugin_path) {
  SAT_PRINT;
  return true;
}

void deinit_callback(void) {
  SAT_PRINT;
}

const void* get_factory_callback(const char* factory_id) {
  SAT_PRINT;
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) return &plugin_factory;
  return nullptr;
}

//__SAT_EXPORT
CLAP_EXPORT extern const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = init_callback,
  .deinit       = deinit_callback,
  .get_factory  = get_factory_callback
};
