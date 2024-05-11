#ifndef sat_buffered_window_included
#define sat_buffered_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/window/sat_painted_window.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BufferedWindow
: public SAT_PaintedWindow {

//------------------------------
public:
//------------------------------

  SAT_BufferedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_PaintedWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_BufferedWindow() {
  }

  // void on_window_show() override {
  //   SAT_PaintedWindow::on_window_show();
  // }

  // void on_window_hide() override {
  //   SAT_PaintedWindow::on_window_hide();
  // }


};

//----------------------------------------------------------------------
#endif