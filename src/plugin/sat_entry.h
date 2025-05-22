#ifndef sat_entry_included
#define sat_entry_included
//----------------------------------------------------------------------

#include "sat.h"

//#ifdef SAT_PLUGIN_CLAP
  #include "plugin/clap/sat_clap_entry.h"
//#endif

#ifdef SAT_PLUGIN_DSSI
  #include "plugin/dssi/sat_dssi_entry.h"
#endif

#ifdef SAT_PLUGIN_EXE
  #include "plugin/exe/sat_exe_entry.h"
#endif

#ifdef SAT_PLUGIN_EXECUTABLE_LIBRARY
  #include "plugin/exe/sat_exelib_entry.h"
#endif

#ifdef SAT_PLUGIN_LADSPA
  #include "plugin/ladspa/sat_ladspa_entry.h"
#endif

#ifdef SAT_PLUGIN_LV2
  #include "plugin/lv2/sat_lv2_entry.h"
#endif

#ifdef SAT_PLUGIN_VST2
  #include "plugin/vst2/sat_vst2_entry.h"
#endif

#ifdef SAT_PLUGIN_VST3
  #include "plugin/vst3/sat_vst3_entry.h"
#endif

//----------------------------------------------------------------------
#endif
