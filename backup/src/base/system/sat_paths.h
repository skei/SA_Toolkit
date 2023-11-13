#ifndef sat_paths_included
#define sat_paths_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_paths.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_paths.h"
#endif

//----------------------------------------------------------------------
#endif

