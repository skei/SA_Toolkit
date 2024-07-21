#ifndef sat_widget_owner_included
#define sat_widget_owner_included
//----------------------------------------------------------------------

// aka window (from root widget)

#include "sat.h"
#include "gui/sat_painter.h"

// #ifdef SAT_SURFACE_X11
//   #include "gui/lib/sat_x11.h"
// #endif

class SAT_Window;

//----------------------------------------------------------------------

class SAT_WidgetOwner {
public:
  virtual SAT_Painter*  on_WidgetOwner_getPainter() { return nullptr; }
  virtual uint32_t      on_WidgetOwner_getWidth() { return 0; }
  virtual uint32_t      on_WidgetOwner_getHeight() { return 0; }
  virtual double        on_WidgetOwner_getScale() { return 0.0; } // 1.0


  // virtual SAT_Window* on_WidgetOwner_getWindow() { return nullptr; }
  // virtual void on_WidgetOwner_wantTimerEvents(bool AWant=true) {}
};

//----------------------------------------------------------------------
#endif