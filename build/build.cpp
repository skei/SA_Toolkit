
/*
  ../build/compile -i ../build/build.cpp -o build -f exe -g x11 -d
  nc -U -l -k /tmp/sat.socket
*/

//----------------------------------------------------------------------


#ifdef SAT_PLUGIN_EXE
  //#define SAT_DEBUG_MEMTRACE
#else
  #define SAT_PRINT_SOCKET
#endif

//#define SAT_DEBUG_WINDOW
//#define SAT_DEBUG_OBSERVER
//#define SAT_WINDOW_BUFFERED

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//#include "../plugins/test/exe.cpp"
//#include "../plugins/test/gain.cpp"
//#include "../plugins/test/voices.cpp"

//----------

//#include "../plugins/templates/template.cpp"


//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------


//#include "../plugins/sa_automata.cpp"
//#include "../plugins/sa_botage.cpp"
//#include "../plugins/sa_bulum.cpp"
//#include "../plugins/sa_cred.cpp"
#include "../plugins/sa_demo.cpp"
//#include "../plugins/sa_mael.cpp"
//#include "../plugins/sa_ports.cpp"


//----------------------------------------------------------------------
//
// ports
//
//----------------------------------------------------------------------


//#include "../plugins/ports/sa_compciter.cpp"
//#include "../plugins/ports/sa_event_horizon.cpp"
//#include "../plugins/ports/sa_exciter.cpp"
//#include "../plugins/ports/sa_exciter2.cpp"
//#include "../plugins/ports/sa_freeverb.cpp"
//#include "../plugins/ports/sa_hall_reverb.cpp"
//#include "../plugins/ports/sa_hrtf.cpp"
//#include "../plugins/ports/sa_iirfilter.cpp"
//#include "../plugins/ports/sa_large_room_reverb.cpp"
//#include "../plugins/ports/sa_limiter.cpp"
//#include "../plugins/ports/sa_phaser.cpp"
//#include "../plugins/ports/sa_pitch.cpp"
//#include "../plugins/ports/sa_saturation.cpp"
//#include "../plugins/ports/sa_small_room_reverb.cpp"
//#include "../plugins/ports/sa_sonic_maximizer.cpp"
//#include "../plugins/ports/sa_stretch.cpp"
//#include "../plugins/ports/sa_tilteq.cpp"
//#include "../plugins/ports/sa_transient_killer.cpp"

//----------

//#include "../plugins/ports/sa_reverb.cpp"


//----------------------------------------------------------------------
//
// test
//
//----------------------------------------------------------------------


// -t, #define SAT_INCLUDE_TESTS

//#include "../test/sat_all_tests.cpp"


