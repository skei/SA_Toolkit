#ifndef sat_library_included
#define sat_library_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_library.h"
#endif

#ifdef SAT_MAC
  #include "base/system/mac/sat_mac_library.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_library.h"
#endif

//----------------------------------------------------------------------
#endif
