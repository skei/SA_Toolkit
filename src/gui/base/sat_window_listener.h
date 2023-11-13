#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------

// window -> editor

#include "sat.h"

class SAT_Timer;
class SAT_Widget;
class SAT_Wibdow;

//----------------------------------------------------------------------

class SAT_WindowListener {
public:
  virtual void on_windowListener_update(SAT_Widget* AWidget) {}
//virtual void on_windowListener_resize(SAT_Window* AWindow, int32_t AWidth, int32_t AHeight) {}
  virtual void on_windowListener_timer(SAT_Timer* ATimer, double AElapsed) {}
};

//----------------------------------------------------------------------
#endif