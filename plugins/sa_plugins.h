#ifndef sa_plugins_included
#define sa_plugins_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_VST3

#define SAT_NO_ENTRY

#include "base/sat.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_registry.h"

#include "../test/test_synth.h"
#include "../plugins/sa_bulum.h"
#include "../plugins/sa_cred.h"
#include "../plugins/sa_filter.h"
#include "../plugins/sa_gain.h"
#include "../plugins/sa_pitch.h"
#include "../plugins/sa_demo.h"
#include "../plugins/sa_synth.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  ARegistry->registerDescriptor(&test_synth_descriptor);
  ARegistry->registerDescriptor(&sa_bulum_descriptor);
  ARegistry->registerDescriptor(&sa_cred_descriptor);
  ARegistry->registerDescriptor(&sa_filter_descriptor);
  ARegistry->registerDescriptor(&sa_gain_descriptor);
  ARegistry->registerDescriptor(&sa_pitch_descriptor);
  ARegistry->registerDescriptor(&sa_demo_descriptor);
  ARegistry->registerDescriptor(&sa_synth_descriptor);
}

//----------

const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  SAT_Plugin* plugin;  
  switch (AIndex) {
    case 0: plugin = new test_synth_plugin(ADescriptor,AHost);  return plugin->getPlugin();
    case 1: plugin = new sa_bulum_plugin(ADescriptor,AHost);    return plugin->getPlugin();
    case 2: plugin = new sa_cred_plugin(ADescriptor,AHost);     return plugin->getPlugin();
    case 3: plugin = new sa_filter_plugin(ADescriptor,AHost);   return plugin->getPlugin();
    case 4: plugin = new sa_gain_plugin(ADescriptor,AHost);     return plugin->getPlugin();
    case 5: plugin = new sa_pitch_plugin(ADescriptor,AHost);    return plugin->getPlugin();
    case 6: plugin = new sa_demo_plugin(ADescriptor,AHost);     return plugin->getPlugin();
    case 7: plugin = new sa_synth_plugin(ADescriptor,AHost);    return plugin->getPlugin();
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
