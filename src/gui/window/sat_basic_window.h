#ifndef sat_basic_window_included
#define sat_basic_window_included
//---------------------------------------------------------------------

#include "sat.h"
#include "base/system/sat_timer.h"
#include "gui/window/sat_implemented_window.h"

//---------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------

class SAT_BasicWindow
: public SAT_ImplementedWindow
, public SAT_TimerListener {

//------------------------------
private:
//------------------------------

  SAT_Timer* MTimer = nullptr;

//------------------------------
public:
//------------------------------

  SAT_BasicWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MTimer = new SAT_Timer(this);
  }

  //----------

  virtual ~SAT_BasicWindow() {
    if (MTimer) {
      MTimer->stop();
      delete MTimer;
    }
  }

//------------------------------
public:
//------------------------------

  virtual void startTimer(float ms) {
    //SAT_TRACE;
    if (MTimer) MTimer->start(ms);
  }

  //----------

  virtual void stopTimer() {
    //SAT_TRACE;
    if (MTimer) MTimer->stop();
  }

//------------------------------
public:
//------------------------------

  void on_timerListener_callback(SAT_Timer* ATimer, double ADelta) override {
    //SAT_TRACE;
  }

  //----------

  // // make it shut up about unimplemented virtual abstract methods..
  // #ifdef SAT_PAINTER_NANOVG
  //   NVGcontext* on_surfaceOwner_getNanoVGContext()  override { return nullptr; }
  // #endif

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

};

//---------------------------------------------------------------------
#endif
