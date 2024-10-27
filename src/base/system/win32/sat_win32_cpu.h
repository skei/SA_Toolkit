#ifndef sat_win32_cpu_included
#define sat_win32_cpu_included
//----------------------------------------------------------------------

// TEMPORARY HACKS!!


#include "sat.h"
//#include <sys/sysinfo.h>

//----------------------------------------------------------------------


int32_t SAT_GetNumProcessors() {
  return 12;
  // return get_nprocs_conf() // configured processors
}

//----------

uint32_t SAT_GetUptime() {
  // struct sysinfo info;
  // if (sysinfo(&info)) return info.uptime;
  return 0;
}

//----------------------------------------------------------------------
#endif











