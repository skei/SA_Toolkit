#ifndef sa_plugins_included
#define sa_plugins_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/sat_registry.h"
#include "plugin/sat_entry.h"

#define SAT_NO_ENTRY

#include "sa_automaton.h"
#include "sa_botage.h"
#include "sa_bulum.h"
#include "sa_cred.h"
#include "sa_mael.h"
#include "sa_tyr.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  ARegistry->registerDescriptor(&sa_automaton_descriptor);
  ARegistry->registerDescriptor(&sa_botage_descriptor);
  ARegistry->registerDescriptor(&sa_bulum_descriptor);
  ARegistry->registerDescriptor(&sa_cred_descriptor);
  ARegistry->registerDescriptor(&sa_mael_descriptor);
  ARegistry->registerDescriptor(&sa_tyr_descriptor);
}

//----------

SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  SAT_Plugin* plugin;  
  switch (AIndex) {
    case 0: return new sa_automaton_plugin(ADescriptor,AHost);
    case 1: return new sa_botage_plugin(ADescriptor,AHost);
    case 2: return new sa_bulum_plugin(ADescriptor,AHost);
    case 3: return new sa_cred_plugin(ADescriptor,AHost);
    case 4: return new sa_mael_plugin(ADescriptor,AHost);
    case 5: return new sa_tyr_plugin(ADescriptor,AHost);
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
