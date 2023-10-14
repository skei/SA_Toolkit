#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_WindowListener {
  
//------------------------------
public:
//------------------------------

  //virtual void on_windowListener_open() {}
  //virtual void on_windowListener_close() {}
  //virtual void on_windowListener_visible() {}
  virtual void on_windowListener_mouseClick() {}
  virtual void on_windowListener_mouseRelease() {}
  virtual void on_windowListener_mouseMove() {}
  virtual void on_windowListener_keyPress() {}
  virtual void on_windowListener_keyRelease() {}
  virtual void on_windowListener_timer() {}
  virtual void on_windowListener_paint() {}
  virtual void on_windowListener_userMessage(intptr_t AMessage) {}

};

//----------------------------------------------------------------------
#endif