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

//----------------------------------------------------------------------
#endif

