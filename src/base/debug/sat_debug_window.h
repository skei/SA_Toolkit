#ifndef sat_debug_window_included
#define sat_debug_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_window.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DebugWindow
: public SAT_Window {
  
//------------------------------
private:
//------------------------------

  SAT_PanelWidget* panel = nullptr;
  
//------------------------------
public:
//------------------------------
  
  SAT_DebugWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0, SAT_WindowListener* AListener=nullptr)
  : SAT_Window(AWidth,AHeight,AParent,AListener) {
    SAT_PRINT;
    setTitle("SAT_DebugWindow");
    panel = new SAT_PanelWidget(100);
    appendRootWidget(panel);
    panel->setDrawBorder(true);
    panel->setFillBackground(true);
    panel->setBackgroundColor(SAT_Red);
  }
  
  //----------
  
  virtual ~SAT_DebugWindow() {
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------
  
};

//----------------------------------------------------------------------
#endif