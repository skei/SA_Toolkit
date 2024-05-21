#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "base/system/sat_timer.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/widget/sat_root_widget.h"
#include "gui/window/sat_widget_window.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_WidgetWindow {

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_WidgetWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_Window() {
  }

};

//----------------------------------------------------------------------
#endif
