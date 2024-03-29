#include "plugin/sat_plugin.h"
SAT_DEFAULT_PLUGIN_DESCRIPTOR(myDescriptor,"me/myPlugin","myPlugin","me",AUDIO_EFFECT);
class myPlugin : public SAT_Plugin {
public:
  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(myPlugin);
public:
  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");
    appendParameter( new SAT_Parameter("Gain",0.5,0,1,CLAP_PARAM_IS_AUTOMATABLE + CLAP_PARAM_IS_MODULATABLE) );
    return SAT_Plugin::init();
  }
  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    *spl0 *= getModulatedParameterValue(0);
    *spl1 *= getModulatedParameterValue(0);
  }
};
#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);