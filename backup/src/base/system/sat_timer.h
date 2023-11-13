#ifndef sat_timer_included
#define sat_timer_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_timer.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_timer.h"
#endif

//----------------------------------------------------------------------
#endif

