#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------
// aka window
// (from root widget to window)

// #include "sat.h"
#include "gui/sat_widget.h"

class SAT_WidgetListener {
public:
  virtual void on_WidgetListener_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void on_WidgetListener_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_GUI) {}
  virtual void on_WidgetListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_GUI) {}
  virtual void on_WidgetListener_notify(SAT_Widget* AWidget, int32_t AValue) {}
  virtual void on_WidgetListener_mouse_capture(SAT_Widget* AWidget) {}
  virtual void on_WidgetListener_key_capture(SAT_Widget* AWidget) {}
  virtual void on_WidgetListener_modal(SAT_Widget* AWidget) {}
  virtual void on_WidgetListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void on_WidgetListener_set_hint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void on_WidgetListener_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) {}
  virtual void on_WidgetListener_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) {}
  virtual void on_WidgetListener_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) {}
  virtual void on_WidgetListener_set_overlay(SAT_Widget* AWidget, SAT_Color AColor) {}

};

//----------------------------------------------------------------------
#endif