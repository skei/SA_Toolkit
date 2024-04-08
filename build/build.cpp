/*
  in '/bin':    ../build/compile -i ../build/build.cpp -o build.exe -f exe -g x11 -d
  validator:    ../Tools/clap-validator validate ./build.clap
  debug output: nc -U -l -k /tmp/sat.socket
*/

//#define SAT_DEBUG_PRINT_SOCKET
//#define SAT_DEBUG_MEMTRACE

//----------------------------------------------------------------------
//
// templates
//
//----------------------------------------------------------------------

//#include "../plugins/templates/analyzer.h"
//#include "../plugins/templates/audio_effect.h"
//#include "../plugins/templates/instrument.h"
//#include "../plugins/templates/note_detector.h"
//#include "../plugins/templates/note_effect.h"

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
//#include "../plugins/ports/sa_reverb.h"
//#include "../plugins/ports/sa_saturation.h"
//#include "../plugins/ports/sa_small_room_reverb.h"
//#include "../plugins/ports/sa_sonic_maximizer.h"
//#include "../plugins/ports/sa_stretch.h"
//#include "../plugins/ports/sa_tilteq.h"
//#include "../plugins/ports/sa_transient_killer.h"

//#include "../plugins/sa_ports.h"

//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------

  
//#include "../plugins/sa_automata.h"
#include "../plugins/sa_botage.h"
//#include "../plugins/sa_bulum.h"
//#include "../plugins/sa_cred.h"
//#include "../plugins/sa_mael.h"
//#include "../plugins/sa_tyr.h"

//#include "../plugins/sa_plugins.h"

//----------------------------------------------------------------------
//
// misc, test, ..
//
//----------------------------------------------------------------------

//#include "../plugins/misc/demo.h"
//#include "../plugins/misc/demo2.h"
//#include "../plugins/misc/interceptor.h"
//#include "../plugins/misc/minimal.h"
//#include "../plugins/misc/minimal_clap.h"
//#include "../plugins/misc/minimal_noop.h"
//#include "../plugins/misc/pure_clap.h"
//#include "../plugins/misc/standalone.h"

//#include "../plugins/test/test_params.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

