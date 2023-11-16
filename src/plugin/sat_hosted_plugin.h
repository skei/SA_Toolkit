#ifndef sat_hosted_plugin_included
#define sat_hosted_plugin_included
//----------------------------------------------------------------------

// aka SAT_PluginReference

#include "plugin/clap/sat_clap_hosted_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostedPlugin
: public SAT_ClapHostedPlugin {

public:

  SAT_HostedPlugin(clap_host_t* AHost, clap_plugin_t* APlugin) {
  }

  virtual ~SAT_HostedPlugin() {
  }

};

//----------------------------------------------------------------------
#endif