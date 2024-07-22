#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------
// aka SAT_Plugin

#include "sat.h"

//----------

class SAT_Timer;
class SAT_Widget;

class SAT_WindowListener {
public:
  virtual void on_WindowListener_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void on_WindowListener_timer(SAT_Timer* ATimer, double ADelta) {}

  // virtual void on_WindowListener_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_GUI) {}
  // virtual void on_WindowListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) {}
  // virtual void on_WindowListener_notify(SAT_Widget* AWidget, int32_t AValue) {}
  // virtual void on_WindowListener_mouse_capture(SAT_Widget* AWidget) {}
  // virtual void on_WindowListener_key_capture(SAT_Widget* AWidget) {}
  // virtual void on_WindowListener_modal(SAT_Widget* AWidget) {}
  // virtual void on_WindowListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  // virtual void on_WindowListener_set_hint(SAT_Widget* AWidget, const char* AHint) {}

};

//----------------------------------------------------------------------
#endif