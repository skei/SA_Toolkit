
#include "base/sat.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0",
  .name         = "myPlugin",
  .vendor       = "me",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0",
  .description  = "...",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    appendParameter( new SAT_Parameter("Param1",0.5) );
    appendAudioInputPort( new SAT_AudioPort() );
    appendAudioOutputPort( new SAT_AudioPort() );
    registerAllExtensions();

  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  //void destroy() final {
  //  delete this;
  //}

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_SIMPLE_ENTRY(myDescriptor,myPlugin);

