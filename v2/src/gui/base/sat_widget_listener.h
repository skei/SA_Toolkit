#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_widget.h"

class SAT_WidgetListener {
public:
  virtual void on_widgetListener_update(SAT_BaseWidget* AFrom) {}
  virtual void on_widgetListener_redraw(SAT_BaseWidget* AFrom) {}
  virtual void on_widgetListener_realign(SAT_BaseWidget* AFrom) {}
};

//----------------------------------------------------------------------
#endif