#ifndef sat_widget_owner_included
#define sat_widget_owner_included
//----------------------------------------------------------------------

// window

#include "sat.h"

class SAT_Window;

//----------------------------------------------------------------------

class SAT_WidgetOwner {
public:
  virtual SAT_Window* on_widgetOwner_getWindow() { return nullptr; }
  virtual double      on_widgetOwner_getWidth() { return 0.0; }
  virtual double      on_widgetOwner_getHeight() { return 0.0; }
  virtual double      on_widgetOwner_getScale() { return 0.0; }


};

//----------------------------------------------------------------------
#endif