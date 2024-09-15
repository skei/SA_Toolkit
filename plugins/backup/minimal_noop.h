#include "plugin/sat_plugin.h"
SAT_DEFAULT_PLUGIN_DESCRIPTOR(myDescriptor,"me/myPlugin","myPlugin","me",AUDIO_EFFECT);
class myPlugin : public SAT_Plugin {
public:
  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin);
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);