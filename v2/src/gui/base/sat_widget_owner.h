#ifndef sat_widget_owner_included
#define sat_widget_owner_included
//----------------------------------------------------------------------

// window

#include "sat.h"
#include "gui/base/sat_base_window.h"

class SAT_WidgetOwner {
public:
  virtual SAT_BaseWindow* on_widgetOwner_getWindow() { return nullptr; }
};

//----------------------------------------------------------------------
#endif