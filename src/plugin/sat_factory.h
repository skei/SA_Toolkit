#ifndef sat_factory_included
#define sat_factory_included
//----------------------------------------------------------------------

#include "sat.h"

// #ifdef SAT_PLUGIN_EXE
//   #include "plugin/wrapper/exe/sat_exe_factory.h"
// #endif

#ifdef SAT_PLUGIN_CLAP
  #include "plugin/factory/sat_clap_factory.h"
#endif

#ifdef SAT_PLUGIN_VST3
  #include "plugin/factory/sat_vst3_factory.h"
#endif

//----------------------------------------------------------------------
#endif