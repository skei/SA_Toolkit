#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------

// window -> editor

#include "sat.h"
#include "gui/base/sat_base_window.h"

class SAT_WindowListener {
public:
  //virtual void on_windowListener_update(SAT_Widget* AWidget) {}
  //virtual void on_windowListener_resize(SAT_BaseWindow* AWindow, int32_t AWidth, int32_t AHeight) {}
  virtual void on_windowListener_timer(SAT_BaseWindow* AWindow, double AElapsed) {}
};

//----------------------------------------------------------------------
#endif