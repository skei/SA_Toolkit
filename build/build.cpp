
// ../build/compile -i ../build/build.cpp -o build -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

//----------------------------------------------------------------------

#ifdef SAT_PLUGIN_EXE
  #define SAT_DEBUG_MEMTRACE
#else
  #define SAT_PRINT_SOCKET
#endif

//#define SAT_DEBUG_WINDOW
//#define SAT_DEBUG_OBSERVER

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//#include "../plugins/test/exe.h"
//#include "../plugins/test/gain.h"
//#include "../plugins/test/voices.h"

//----------

#include "../plugins/templates/template.h"


//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------


//#include "../plugins/sa_demo.h"

//#include "../plugins/sa_automata.h"
//#include "../plugins/sa_botage.h"
//#include "../plugins/sa_bulum.h"
//#include "../plugins/sa_cred.h"
//#include "../plugins/sa_mael.h"
//#include "../plugins/sa_ports.h"
//#include "../plugins/sa_tyr.h"


//----------------------------------------------------------------------
//
// ports
//
//----------------------------------------------------------------------


//#include "../plugins/ports/sa_compciter.h"
//#include "../plugins/ports/sa_event_horizon.h"
//#include "../plugins/ports/sa_exciter.h"
//#include "../plugins/ports/sa_exciter2.h"
//#include "../plugins/ports/sa_freeverb.h"
//#include "../plugins/ports/sa_hall_reverb.h"
//#include "../plugins/ports/sa_hrtf.h"
//#include "../plugins/ports/sa_iirfilter.h"
//#include "../plugins/ports/sa_large_room_reverb.h"
//#include "../plugins/ports/sa_limiter.h"
//#include "../plugins/ports/sa_phaser.h"
//#include "../plugins/ports/sa_pitch.h"
//#include "../plugins/ports/sa_saturation.h"
//#include "../plugins/ports/sa_small_room_reverb.h"
//#include "../plugins/ports/sa_sonic_maximizer.h"
//#include "../plugins/ports/sa_stretch.h"
//#include "../plugins/ports/sa_tilteq.h"
//#include "../plugins/ports/sa_transient_killer.h"

//----------

//#include "../plugins/ports/sa_reverb.h"


//----------------------------------------------------------------------
//
// test
//
//----------------------------------------------------------------------


// -t, #define SAT_INCLUDE_TESTS

//#include "../test/sat_all_tests.h"

