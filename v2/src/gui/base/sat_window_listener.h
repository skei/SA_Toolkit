#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_widget.h"

class SAT_WindowListener {
public:
  virtual void on_windowListener_resize(SAT_BaseWindow* AWindow) {}
};

//----------------------------------------------------------------------
#endif