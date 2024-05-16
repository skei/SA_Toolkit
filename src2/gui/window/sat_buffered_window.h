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
private:
//------------------------------

  // void* MRenderBuffer = nullptr;

//------------------------------
public:
//------------------------------

  SAT_BufferedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_PaintedWindow(AWidth,AHeight,AParent) {
    //SAT_Painter* painter = getPainter();
    //SAT_PRINT("painter: %p\n",painter);
    //void* MRenderBuffer = painter->createRenderBuffer(AWidth,AHeight);
    //SAT_PRINT("MRenderBuffer: %p\n",MRenderBuffer);
  }

  //----------

  virtual ~SAT_BufferedWindow() {
    //SAT_Painter* painter = getPainter();
    //if (painter) {
    //   if (MRenderBuffer) painter->deleteRenderBuffer(MRenderBuffer);
    //}
  }

//------------------------------
public:
//------------------------------

  // void on_window_show() override {
  //   SAT_TRACE;
  //   SAT_PaintedWindow::on_window_show();
  //   SAT_Painter* painter = getPainter();
  //   SAT_PRINT("painter: %p\n",painter);
  //   void* MRenderBuffer = painter->createRenderBuffer(getWidth(),getHeight());
  //   SAT_PRINT("MRenderBuffer: %p\n",MRenderBuffer);
  // }

  //----------

  // void on_window_hide() override {
  //   SAT_TRACE;
  //   SAT_PaintedWindow::on_window_hide();
  //   SAT_Painter* painter = getPainter();
  //   if (painter && MRenderBuffer) painter->deleteRenderBuffer(MRenderBuffer);
  // }

  //----------

  // resize
  // dirty, redraw, ..

};

//----------------------------------------------------------------------
#endif