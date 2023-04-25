#ifndef sa_ports_included
#define sa_ports_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_VST3

#include "base/sat.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_registry.h"

#define SAT_NO_ENTRY
#include "../plugins/ports/sa_compciter.h"
#include "../plugins/ports/sa_eventhor.h"
#include "../plugins/ports/sa_exciter.h"
#include "../plugins/ports/sa_exciter2.h"
#include "../plugins/ports/sa_freeverb.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  ARegistry->registerDescriptor(&sa_compciter_descriptor);
  ARegistry->registerDescriptor(&sa_eventhor_descriptor);
  ARegistry->registerDescriptor(&sa_exciter_descriptor);
  ARegistry->registerDescriptor(&sa_exciter2_descriptor);
  ARegistry->registerDescriptor(&sa_freeverb_descriptor);
}

//----------

const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  SAT_Plugin* plugin;  
  switch (AIndex) {
    case 0: plugin = new sa_compciter_plugin(ADescriptor,AHost);  return plugin->getPlugin();
    case 1: plugin = new sa_eventhor_plugin(ADescriptor,AHost);   return plugin->getPlugin();
    case 2: plugin = new sa_exciter_plugin(ADescriptor,AHost);    return plugin->getPlugin();
    case 3: plugin = new sa_exciter2_plugin(ADescriptor,AHost);   return plugin->getPlugin();
    case 4: plugin = new sa_freeverb_plugin(ADescriptor,AHost);   return plugin->getPlugin();
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
