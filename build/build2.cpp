
// ../build/compile2 -i ../build/build2.cpp -o build -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------------------------------------------------------------------

#ifdef SAT_PLUGIN_EXE
  #define SAT_DEBUG_MEMTRACE
#else
  #define SAT_PRINT_SOCKET
#endif

//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------

//------------------------------
// ports
//------------------------------

//#include "../plugins2/ports/sa_compciter.h"

//----------------------------------------------------------------------
//
// test
//
//----------------------------------------------------------------------

//#include "../plugins2/test2/exe1.h"
//#include "../plugins2/test2/plugin1.h"
#include "../plugins2/test2/plugin_gui1.h"
