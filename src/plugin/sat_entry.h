#ifndef sat_entry_included
#define sat_entry_included
//----------------------------------------------------------------------

#ifdef SAT_PLUGIN_CLAP
  #include "plugin/clap/sat_clap_entry.h"
#endif

#ifdef SAT_PLUGIN_EXE
  #include "plugin/exe/sat_exe_entry.h"
#endif

#ifdef SAT_PLUGIN_VST2
  #include "plugin/vst2/sat_vst2_entry.h"
#endif

#ifdef SAT_PLUGIN_VST3
  #include "plugin/vst3/sat_vst3_entry.h"
#endif

//----------------------------------------------------------------------
#endif
