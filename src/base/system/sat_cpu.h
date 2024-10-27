#ifndef sat_cpu_included
#define sat_cpu_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_cpu.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_cpu.h"
#endif

//----------------------------------------------------------------------
#endif
