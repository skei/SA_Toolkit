
// ../build/compile -i ../build/build.cpp -o build -f exe -g x11 -d
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

//#include "../plugins/ports/sa_compciter.h"

//----------------------------------------------------------------------
//
// test
//
//----------------------------------------------------------------------

//#include "../plugins/test/exe1.h"
//#include "../plugins/test/plugin1.h"
//#include "../plugins/test/plugin_gui1.h"
#include "../plugins/test/plugin_gui2.h"
