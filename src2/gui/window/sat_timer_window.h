#ifndef sat_timer_window_included
#define sat_timer_window_included
//----------------------------------------------------------------------

#include "sat.h"
// #include "gui/widget/sat_widget_listener.h"
// #include "gui/widget/sat_root_widget.h"
// #include "gui/sat_renderer.h"
// #include "gui/sat_painter.h"
// #include "gui/sat_widget.h"
#include "base/system/sat_timer.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TimerWindow
: public SAT_ImplementedWindow
, public SAT_TimerListener {

//------------------------------
private:
//------------------------------

  SAT_Timer*      MTimer          = nullptr;


//------------------------------
public:
//------------------------------

  SAT_TimerWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MTimer = new SAT_Timer(this);
  }

  virtual ~SAT_TimerWindow() {
    if (MTimer) {
      MTimer->stop();
      delete MTimer;
    }
  }

//------------------------------
public:
//------------------------------

  virtual void startTimer(float ms) {
    //SAT_Assert(MTimer);
    if (MTimer) MTimer->start(ms);
  }

  //----------

  virtual void stopTimer() {
    //SAT_Assert(MTimer);
    if (MTimer) MTimer->stop();
  }

//------------------------------
public: // window
//------------------------------

  void on_window_show() override {
    SAT_ImplementedWindow::on_window_show();
    //SAT_TRACE;
    #ifndef SAT_WINDOW_NO_TIMER_AUTOSTART
      //SAT_TRACE;
      startTimer(SAT_WINDOW_TIMER_MS);
    #endif
  }

  //----------

  void on_window_hide() override {
    //SAT_TRACE;
    #ifndef SAT_WINDOW_NO_TIMER_AUTOSTART
      //SAT_TRACE;
      stopTimer();
    #endif
    SAT_ImplementedWindow::on_window_hide();
  }

//------------------------------
public: // timer listener
//------------------------------

  void on_timerListener_callback(SAT_Timer* ATimer, double ADelta) override {
    //SAT_TRACE;
  }

};

//----------------------------------------------------------------------
#endif
