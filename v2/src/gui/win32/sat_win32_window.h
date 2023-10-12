#ifndef sat_win32_window_included
#define sat_win32_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32Window
: public SAT_BaseWindow {

//------------------------------
public:
//------------------------------

  SAT_Win32Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_BaseWindow() {
  }


};

//----------------------------------------------------------------------
#endif