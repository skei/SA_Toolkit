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
};

//----------------------------------------------------------------------
#endif