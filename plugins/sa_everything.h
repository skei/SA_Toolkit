#ifndef sa_everything_included
#define sa_everything_included
//----------------------------------------------------------------------

#define SAT_PLUGIN_CLAP
#define SAT_PLUGIN_VST3

//----------

#include "base/sat.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_registry.h"

#define SAT_NO_ENTRY

// plugins
#include "../test/test_synth.h"
#include "../plugins/sa_bulum.h"
#include "../plugins/sa_cred.h"
#include "../plugins/sa_gain.h"
#include "../plugins/sa_pitch.h"
#include "../plugins/sa_demo.h"

// ports
#include "../plugins/ports/sa_compciter.h"
#include "../plugins/ports/sa_event_horizon.h"
#include "../plugins/ports/sa_exciter.h"
#include "../plugins/ports/sa_exciter2.h"
#include "../plugins/ports/sa_freeverb.h"
#include "../plugins/ports/sa_hall_reverb.h"
#include "../plugins/ports/sa_hrtf.h"
#include "../plugins/ports/sa_iirfilter.h"
#include "../plugins/ports/sa_large_room_reverb.h"
#include "../plugins/ports/sa_limiter.h"
#include "../plugins/ports/sa_phaser.h"
#include "../plugins/ports/sa_saturation.h"
#include "../plugins/ports/sa_small_room_reverb.h"
#include "../plugins/ports/sa_sonic_maximizer.h"
#include "../plugins/ports/sa_stretch.h"
#include "../plugins/ports/sa_tilteq.h"
#include "../plugins/ports/sa_transient_killer.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void SAT_Register(SAT_Registry* ARegistry) {
  
  // plugins
  
  ARegistry->registerDescriptor(&sa_gain_descriptor);
  ARegistry->registerDescriptor(&sa_demo_descriptor);
  ARegistry->registerDescriptor(&test_synth_descriptor);
  ARegistry->registerDescriptor(&sa_bulum_descriptor);
  ARegistry->registerDescriptor(&sa_cred_descriptor);
  ARegistry->registerDescriptor(&sa_pitch_descriptor);
  
  // ports
  
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
    
    // plugins
    
    case 0:   plugin = new sa_gain_plugin(ADescriptor,AHost);               return plugin->getPlugin();
    case 1:   plugin = new sa_demo_plugin(ADescriptor,AHost);               return plugin->getPlugin();
    case 2:   plugin = new test_synth_plugin(ADescriptor,AHost);            return plugin->getPlugin();
    case 3:   plugin = new sa_bulum_plugin(ADescriptor,AHost);              return plugin->getPlugin();
    case 4:   plugin = new sa_cred_plugin(ADescriptor,AHost);               return plugin->getPlugin();
    case 5:   plugin = new sa_pitch_plugin(ADescriptor,AHost);              return plugin->getPlugin();

  //case :    plugin = new sa_botage_plugin(ADescriptor,AHost);             return plugin->getPlugin();
  //case :    plugin = new sa_tyr_plugin(ADescriptor,AHost);                return plugin->getPlugin();
    
    // ports
    
    case 6:   plugin = new sa_compciter_plugin(ADescriptor,AHost);          return plugin->getPlugin();
    case 7:   plugin = new sa_event_horizon_plugin(ADescriptor,AHost);      return plugin->getPlugin();
    case 8:   plugin = new sa_exciter_plugin(ADescriptor,AHost);            return plugin->getPlugin();
    case 9:   plugin = new sa_exciter2_plugin(ADescriptor,AHost);           return plugin->getPlugin();
    case 10:  plugin = new sa_freeverb_plugin(ADescriptor,AHost);           return plugin->getPlugin();
    case 11:  plugin = new sa_hall_reverb_plugin(ADescriptor,AHost);        return plugin->getPlugin();
    case 12:  plugin = new sa_hrtf_plugin(ADescriptor,AHost);               return plugin->getPlugin();
    case 13:  plugin = new sa_iirfilter_plugin(ADescriptor,AHost);          return plugin->getPlugin();
    case 14:  plugin = new sa_large_room_reverb_plugin(ADescriptor,AHost);  return plugin->getPlugin();
    case 15:  plugin = new sa_limiter_plugin(ADescriptor,AHost);            return plugin->getPlugin();
    case 16:  plugin = new sa_phaser_plugin(ADescriptor,AHost);             return plugin->getPlugin();
    case 17:  plugin = new sa_saturation_plugin(ADescriptor,AHost);         return plugin->getPlugin();
    case 18:  plugin = new sa_small_room_reverb_plugin(ADescriptor,AHost);  return plugin->getPlugin();
    case 19:  plugin = new sa_sonic_maximizer_plugin(ADescriptor,AHost);    return plugin->getPlugin();
    case 20:  plugin = new sa_stretch_plugin(ADescriptor,AHost);            return plugin->getPlugin();
    case 21:  plugin = new sa_tilteq_plugin(ADescriptor,AHost);             return plugin->getPlugin();
    case 22:  plugin = new sa_transient_killer_plugin(ADescriptor,AHost);   return plugin->getPlugin();
    
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
