
// ../build/compile -i ../build/build.cpp -o build -f exe -g x11 -d
// nc -U -l -k /tmp/sat.socket

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


//#include "../plugins/test/exe.h"
//#include "../plugins/test/gain.h"
//#include "../plugins/test/voices.h"

//----------

//#include "../plugins/templates/template.h"


//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------



//#include "../plugins/sa_automata.h"
//#include "../plugins/sa_botage.h"
//#include "../plugins/sa_bulum.h"
//#include "../plugins/sa_cred.h"
#include "../plugins/sa_demo.h"
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








//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


#if 0

#define SAT_DEBUG_MEMTRACE

#include "sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myMainClass
: public SAT_WindowListener {

//------------------------------
public:
//------------------------------

  int main() {
    SAT_PRINT("Hello world!\n");
    SAT_Window* window = new SAT_Window(this,640,480,0);
    window->setTitle("Hello world!");
    SAT_RootWidget* root = new SAT_RootWidget(window);
    window->setRootWidget(root);
    //window->setOverlayWidget( new SAT_OverlayWidget() );

      SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(270,190,100,100));
      root->appendChild(knob);

      SAT_AnimTestWidget* anim = new SAT_AnimTestWidget(SAT_Rect(100,100,150,100));
      root->appendChild(anim);

    window->show();
    window->eventLoop();
    window->hide();
    delete window;
    return 0;
  }

  //----------

  void on_WindowListener_update(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) final {
    //SAT_TRACE;
  }

  //----------
  
  void on_WindowListener_timer(SAT_Timer* ATimer, double ADelta) final {
    //SAT_TRACE;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  myMainClass main = {};
  return main.main();
}

  // SAT_AtomicQueue<int,32> myQueue;
  // myQueue.write(2);
  // int test = 1;
  // myQueue.read(test);
  // SAT_PRINT("test %i\n",test); // prints 2

#endif // 0














