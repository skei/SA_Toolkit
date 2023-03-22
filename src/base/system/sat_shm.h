#ifndef sat_shm_included
#define sat_shm_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------

#ifdef SAT_LINUX
  #include "base/system/linux/sat_linux_shm.h"
#endif

#ifdef SAT_WIN32
  #include "base/system/win32/sat_win32_shm.h"
#endif

//----------------------------------------------------------------------
#endif
