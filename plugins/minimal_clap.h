
#include "sat.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/clap/sat_clap_entry.h"

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
//
//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  ARegistry->registerDescriptor(&myDescriptor);
}

//----------

SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) return new myPlugin(ADescriptor,AHost);
  return nullptr;
}
