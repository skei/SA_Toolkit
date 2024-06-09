#ifndef sat_time_included
#define sat_time_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

#if defined (SAT_LINUX)
  #include "base/system/linux/sat_linux_time.h"
#endif

#if defined (SAT_WIN32)
  #include "base/system/win32/sat_win32_time.h"
#endif

//----------------------------------------------------------------------
#endif
