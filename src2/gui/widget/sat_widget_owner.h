#ifndef sat_widget_owner_included
#define sat_widget_owner_included
//----------------------------------------------------------------------

// aka window (from root widget)

// #include "sat.h"

// #ifdef SAT_SURFACE_X11
//   #include "gui/lib/sat_x11.h"
// #endif

class SAT_Window;

//----------------------------------------------------------------------

class SAT_WidgetOwner {
public:
  // virtual SAT_Window* on_widgetOwner_getWindow() { return nullptr; }
  // virtual void on_widgetOwner_wantTimerEvents(bool AWant=true) {}
};

//----------------------------------------------------------------------
#endif