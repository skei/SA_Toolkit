#ifndef sat_lock_included
#define sat_lock_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_lock.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_lock.h"
#endif

//----------------------------------------------------------------------
#endif
