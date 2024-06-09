#ifndef sat_paths_included
#define sat_paths_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

#if defined (SAT_LINUX)
  #include "base/system/linux/sat_linux_paths.h"
#endif

#if defined (SAT_MAC)
  #include "base/system/mac/sat_mac_paths.h"
#endif

#if defined (SAT_WIN32)
  #include "base/system/win32/sat_win32_paths.h"
#endif

//----------------------------------------------------------------------
#endif
