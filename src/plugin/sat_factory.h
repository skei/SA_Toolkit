#ifndef sat_factory_included
#define sat_factory_included
//----------------------------------------------------------------------

#include "sat.h"

// #ifdef SAT_PLUGIN_EXE
//   #include "plugin/exe/sat_exe_factory.h"
// #endif

#ifdef SAT_PLUGIN_CLAP
  #include "plugin/clap/sat_clap_factory.h"
#endif

#ifdef SAT_PLUGIN_VST3
  #include "plugin/vst3/sat_vst3_factory.h"
#endif

//----------------------------------------------------------------------
#endif