#ifndef sat_thread_included
#define sat_thread_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_thread.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_thread.h"
#endif

//----------------------------------------------------------------------
#endif
