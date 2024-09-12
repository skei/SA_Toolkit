#ifndef sa_ports_included
#define sa_ports_included
//----------------------------------------------------------------------

// signalsmith doesn't like our new/delete..
#undef SAT_DEBUG_MEMTRACE

//----------

#include "sat.h"
#include "base/sat_registry.h"
#include "plugin/sat_entry.h"

#define SAT_NO_ENTRY

#include "ports/sa_compciter.cpp"
#include "ports/sa_event_horizon.cpp"
#include "ports/sa_exciter.cpp"
#include "ports/sa_exciter2.cpp"
#include "ports/sa_freeverb.cpp"
#include "ports/sa_hall_reverb.cpp"
#include "ports/sa_hrtf.cpp"
#include "ports/sa_iirfilter.cpp"
#include "ports/sa_large_room_reverb.cpp"
#include "ports/sa_limiter.cpp"
#include "ports/sa_phaser.cpp"
#include "ports/sa_pitch.cpp"
//#include "ports/sa_reverb.cpp"
#include "ports/sa_saturation.cpp"
#include "ports/sa_small_room_reverb.cpp"
#include "ports/sa_sonic_maximizer.cpp"
#include "ports/sa_stretch.cpp"
#include "ports/sa_tilteq.cpp"
#include "ports/sa_transient_killer.cpp"

//----------------------------------------------------------------------
//
// entry
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
  ARegistry->registerDescriptor(&sa_pitch_descriptor);
//ARegistry->registerDescriptor(&sa_reverb_descriptor);
  ARegistry->registerDescriptor(&sa_saturation_descriptor);
  ARegistry->registerDescriptor(&sa_small_room_reverb_descriptor);
  ARegistry->registerDescriptor(&sa_sonic_maximizer_descriptor);
  ARegistry->registerDescriptor(&sa_stretch_descriptor);
  ARegistry->registerDescriptor(&sa_tilteq_descriptor);
  ARegistry->registerDescriptor(&sa_transient_killer_descriptor);
}

//----------

SAT_ClapPlugin* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  SAT_Plugin* plugin;  
  switch (AIndex) {
    case 0:   return new sa_compciter_plugin(ADescriptor,AHost);
    case 1:   return new sa_event_horizon_plugin(ADescriptor,AHost);
    case 2:   return new sa_exciter_plugin(ADescriptor,AHost);
    case 3:   return new sa_exciter2_plugin(ADescriptor,AHost);
    case 4:   return new sa_freeverb_plugin(ADescriptor,AHost);
    case 5:   return new sa_hall_reverb_plugin(ADescriptor,AHost);
    case 6:   return new sa_hrtf_plugin(ADescriptor,AHost);
    case 7:   return new sa_iirfilter_plugin(ADescriptor,AHost);
    case 8:   return new sa_large_room_reverb_plugin(ADescriptor,AHost);
    case 9:   return new sa_limiter_plugin(ADescriptor,AHost);
    case 10:  return new sa_phaser_plugin(ADescriptor,AHost);
    case 11:  return new sa_pitch_plugin(ADescriptor,AHost);
  //case 12:  return new sa_reverb_plugin(ADescriptor,AHost);
    case 12:  return new sa_saturation_plugin(ADescriptor,AHost);
    case 13:  return new sa_small_room_reverb_plugin(ADescriptor,AHost);
    case 14:  return new sa_sonic_maximizer_plugin(ADescriptor,AHost);
    case 15:  return new sa_stretch_plugin(ADescriptor,AHost);
    case 16:  return new sa_tilteq_plugin(ADescriptor,AHost);
    case 17:  return new sa_transient_killer_plugin(ADescriptor,AHost);
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
