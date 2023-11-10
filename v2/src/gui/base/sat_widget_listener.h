#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

// widget -> window

#include "sat.h"
#include "gui/sat_widget.h"

class SAT_WidgetListener {
public:
  virtual void on_widgetListener_update(SAT_Widget* AWidget) {}
  virtual void on_widgetListener_redraw(SAT_Widget* AWidget) {}
  virtual void on_widgetListener_realign(SAT_Widget* AFrAWidgetom) {}
  virtual void on_widgetListener_setCursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void on_widgetListener_setHint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void on_widgetListener_setModal(SAT_Widget* AWidget) {}
  virtual void on_widgetListener_captureKeys(SAT_Widget* ASender) {}
  virtual void on_widgetListener_close(SAT_Widget* ASender) {}
  virtual void do_widgetListener_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1) {}
};

//----------------------------------------------------------------------
#endif