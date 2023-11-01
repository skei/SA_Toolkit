#ifndef sat_clap_factory_included
#define sat_clap_factory_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
// #include "plugin/sat_registry.h"
// #include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

#ifdef SAT_PLUGIN_USE_ARA_FACTORY
  #include "plugin/clap/factories/sat_clap_ara_factory.h"
#endif

#ifdef SAT_PLUGIN_USE_INVALIDATION_FACTORY
  #include "plugin/clap/factories/sat_clap_invalidation_factory.h"
#endif

#ifdef SAT_PLUGIN_USE_PLUGIN_FACTORY
   #include "plugin/clap/factories/sat_clap_plugin_factory.h"
#endif

#ifdef SAT_PLUGIN_USE_PRESET_DISCOVERY_FACTORY
  #include "plugin/clap/factories/sat_clap_preset_discovery_factory.h"
#endif

//----------------------------------------------------------------------
#endif
