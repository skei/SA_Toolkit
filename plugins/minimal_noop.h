#include "plugin/sat_plugin.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "me/myPlugin", "myPlugin", "me", "", "", "", "0.0.0", "", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);