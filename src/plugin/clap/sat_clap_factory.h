#ifndef sat_clap_factory_included
#define sat_clap_factory_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
// #include "plugin/sat_registry.h"
// #include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

#if defined (SAT_PLUGIN_INCLUDE_CLAP_ARA_FACTORY)
  #include "plugin/clap/factory/sat_clap_ara_factory.h"
#endif

#if defined (SAT_PLUGIN_INCLUDE_CLAP_INVALIDATION_FACTORY)
  #include "plugin/clap/factory/sat_clap_invalidation_factory.h"
#endif

#if defined (SAT_PLUGIN_INCLUDE_CLAP_PLUGIN_FACTORY)
   #include "plugin/clap/factory/sat_clap_plugin_factory.h"
#endif

#if defined (SAT_PLUGIN_INCLUDE_CLAP_PRESET_DISCOVERY_FACTORY)
  #include "plugin/clap/factory/sat_clap_preset_discovery_factory.h"
#endif

#if defined (SAT_PLUGIN_INCLUDE_CLAP_STATE_CONVERTER_FACTORY)
  #include "plugin/clap/factory/sat_clap_state_converter_factory.h"
#endif

//----------------------------------------------------------------------
#endif
