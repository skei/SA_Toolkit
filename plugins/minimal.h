// do nothing

/*

#include "plugin/sat_plugin.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "me/myPlugin", "myPlugin", "me", "", "", "", "0.0.0", "", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

*/

//----------------------------------------------------------------------

// do something

#include "plugin/sat_plugin.h"
const clap_plugin_descriptor_t myDescriptor = { CLAP_VERSION, "me/myPlugin", "myPlugin", "me", "", "", "", "0.0.0", "", (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr } };
class myPlugin : public SAT_Plugin {
public:
  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);
public:
  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("in");
    appendStereoAudioOutputPort("out");
    appendParameter( new SAT_Parameter("gain",0.5,0,1,CLAP_PARAM_IS_AUTOMATABLE + CLAP_PARAM_IS_MODULATABLE) );
    return SAT_Plugin::init();
  }
  void processStereoSample(float* spl0, float* spl1) final {
    *spl0 *= getParameterValue(0);
    *spl1 *= getParameterValue(0);
  }
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

