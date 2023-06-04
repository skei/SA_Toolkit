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

struct SAT_LadspaPorts {
  int*                  descriptors  = nullptr;
  char**                names        = nullptr;
  char*                 namesBuffer  = nullptr;
  LADSPA_PortRangeHint* rangeHints   = nullptr;
};

//----------

struct SAT_LadspaEntryData {
  uint32_t            index;
  LADSPA_Descriptor*  descriptor;
  SAT_LadspaPorts*    ports;
};


//----------------------------------------------------------------------
#endif

