#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------
// aka editor
// (from window to editor)

#include "base/system/sat_timer.h"
#include "gui/sat_widget.h"

class SAT_WindowListener {
public:
  virtual void on_windowListener_update(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void on_windowListener_timer(SAT_Timer* ATimer) {}

  // virtual void on_windowListener_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) {}
  // virtual void on_windowListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) {}
  // virtual void on_windowListener_notify(SAT_Widget* AWidget, int32_t AValue) {}
  // virtual void on_windowListener_mouse_capture(SAT_Widget* AWidget) {}
  // virtual void on_windowListener_key_capture(SAT_Widget* AWidget) {}
  // virtual void on_windowListener_modal(SAT_Widget* AWidget) {}
  // virtual void on_windowListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  // virtual void on_windowListener_set_hint(SAT_Widget* AWidget, const char* AHint) {}

  //----------




};

//----------------------------------------------------------------------
#endif