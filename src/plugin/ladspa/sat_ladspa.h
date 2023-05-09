#ifndef sat_ladspa_included
#define sat_ladspa_included
//----------------------------------------------------------------------

#include "base/sat.h"

#ifdef SAT_LINUX
  #include <ladspa.h>
#endif

#ifdef SAT_WIN32
  #include "/usr/include/ladspa.h"
#endif

#define SAT_PLUGIN_LADSPA_MAX_NAME_LENGTH      256
#define SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH 64

//----------------------------------------------------------------------
#endif

