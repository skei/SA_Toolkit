#ifndef sa_ports_included
#define sa_ports_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_VST3

#include "base/sat.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_registry.h"

#define SAT_NO_ENTRY
#include "../../SA_Plugins/ports/sa_compciter.h"
#include "../../SA_Plugins/ports/sa_event_horizon.h"
#include "../../SA_Plugins/ports/sa_exciter.h"
#include "../../SA_Plugins/ports/sa_exciter2.h"
#include "../../SA_Plugins/ports/sa_freeverb.h"
#include "../../SA_Plugins/ports/sa_hall_reverb.h"
#include "../../SA_Plugins/ports/sa_hrtf.h"
#include "../../SA_Plugins/ports/sa_iirfilter.h"
#include "../../SA_Plugins/ports/sa_large_room_reverb.h"
#include "../../SA_Plugins/ports/sa_limiter.h"
#include "../../SA_Plugins/ports/sa_phaser.h"
#include "../../SA_Plugins/ports/sa_saturation.h"
#include "../../SA_Plugins/ports/sa_small_room_reverb.h"
#include "../../SA_Plugins/ports/sa_sonic_maximizer.h"
#include "../../SA_Plugins/ports/sa_stretch.h"
#include "../../SA_Plugins/ports/sa_tilteq.h"
#include "../../SA_Plugins/ports/sa_transient_killer.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  ARegistry->registerDescriptor(&sa_compciter_descriptor);
  ARegistry->registerDescriptor(&sa_event_horizon_descriptor);
  ARegistry->registerDescriptor(&sa_exciter_descriptor);
  ARegistry->registerDescriptor(&sa_exciter2_descriptor);
  ARegistry->registerDescriptor(&sa_freeverb_descriptor);
  ARegistry->registerDescriptor(&sa_hall_reverb_descriptor);
  ARegistry->registerDescriptor(&sa_hrtf_descriptor);
  ARegistry->registerDescriptor(&sa_iirfilter_descriptor);
  ARegistry->registerDescriptor(&sa_large_room_reverb_descriptor);
  ARegistry->registerDescriptor(&sa_limiter_descriptor);
  ARegistry->registerDescriptor(&sa_phaser_descriptor);
  ARegistry->registerDescriptor(&sa_saturation_descriptor);
  ARegistry->registerDescriptor(&sa_small_room_reverb_descriptor);
  ARegistry->registerDescriptor(&sa_sonic_maximizer_descriptor);
  ARegistry->registerDescriptor(&sa_stretch_descriptor);
  ARegistry->registerDescriptor(&sa_tilteq_descriptor);
  ARegistry->registerDescriptor(&sa_transient_killer_descriptor);
}

//----------

const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  SAT_Plugin* plugin;  
  switch (AIndex) {
    case 0:   plugin = new sa_compciter_plugin(ADescriptor,AHost);          return plugin->getClapPlugin();
    case 1:   plugin = new sa_event_horizon_plugin(ADescriptor,AHost);      return plugin->getClapPlugin();
    case 2:   plugin = new sa_exciter_plugin(ADescriptor,AHost);            return plugin->getClapPlugin();
    case 3:   plugin = new sa_exciter2_plugin(ADescriptor,AHost);           return plugin->getClapPlugin();
    case 4:   plugin = new sa_freeverb_plugin(ADescriptor,AHost);           return plugin->getClapPlugin();
    case 5:   plugin = new sa_hall_reverb_plugin(ADescriptor,AHost);        return plugin->getClapPlugin();
    case 6:   plugin = new sa_hrtf_plugin(ADescriptor,AHost);               return plugin->getClapPlugin();
    case 7:   plugin = new sa_iirfilter_plugin(ADescriptor,AHost);          return plugin->getClapPlugin();
    case 8:   plugin = new sa_large_room_reverb_plugin(ADescriptor,AHost);  return plugin->getClapPlugin();
    case 9:   plugin = new sa_limiter_plugin(ADescriptor,AHost);            return plugin->getClapPlugin();
    case 10:  plugin = new sa_phaser_plugin(ADescriptor,AHost);             return plugin->getClapPlugin();
    case 11:  plugin = new sa_saturation_plugin(ADescriptor,AHost);         return plugin->getClapPlugin();
    case 12:  plugin = new sa_small_room_reverb_plugin(ADescriptor,AHost);  return plugin->getClapPlugin();
    case 13:  plugin = new sa_sonic_maximizer_plugin(ADescriptor,AHost);    return plugin->getClapPlugin();
    case 14:  plugin = new sa_stretch_plugin(ADescriptor,AHost);            return plugin->getClapPlugin();
    case 15:  plugin = new sa_tilteq_plugin(ADescriptor,AHost);             return plugin->getClapPlugin();
    case 16:  plugin = new sa_transient_killer_plugin(ADescriptor,AHost);   return plugin->getClapPlugin();
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
