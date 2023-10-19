#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

// widget -> window

#include "sat.h"
#include "gui/sat_widget.h"

class SAT_WidgetListener {
public:
  virtual void on_widgetListener_update_value(SAT_Widget* AFrom) {}
  virtual void on_widgetListener_redraw(SAT_Widget* AFrom) {}
  //virtual void on_widgetListener_realign(SAT_Widget* AFrom) {}
};

//----------------------------------------------------------------------
#endif