#ifndef sat_entry_included
#define sat_entry_included
//----------------------------------------------------------------------

#include "sat.h"

//#ifdef SAT_PLUGIN_CLAP
  #include "plugin/entry/sat_clap_entry.h"
//#endif

#ifdef SAT_PLUGIN_DSSI
  #include "plugin/entry/sat_dssi_entry.h"
#endif

#ifdef SAT_PLUGIN_EXE
  #include "plugin/entry/sat_exe_entry.h"
#endif

// #ifdef SAT_PLUGIN_EXELIB
//   #include "plugin/entry/sat_exelib_entry.h"
// #endif

#ifdef SAT_PLUGIN_LADSPA
  #include "plugin/entry/sat_ladspa_entry.h"
#endif

#ifdef SAT_PLUGIN_LV2
  #include "plugin/entry/sat_lv2_entry.h"
#endif

#ifdef SAT_PLUGIN_VST2
  #include "plugin/entry/sat_vst2_entry.h"
#endif

#ifdef SAT_PLUGIN_VST3
  #include "plugin/entry/sat_vst3_entry.h"
#endif

//----------------------------------------------------------------------
#endif
