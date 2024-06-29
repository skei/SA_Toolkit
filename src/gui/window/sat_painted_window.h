#ifndef sat_painted_window_included
#define sat_painted_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/painter/sat_paint_context.h"
#include "gui/window/sat_basic_window.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PaintedWindow
: public SAT_BasicWindow {

//------------------------------
protected:
//------------------------------

  SAT_Renderer*     MWindowRenderer = nullptr;
  SAT_Painter*      MWindowPainter  = nullptr;
  SAT_PaintContext  MPaintContext   = {};

//------------------------------
public:
//------------------------------

  SAT_PaintedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BasicWindow(AWidth,AHeight,AParent) {
    //SAT_PRINT("\n");
    MWindowRenderer = new SAT_Renderer(this,this);
    MWindowPainter = new SAT_Painter(this,this);
    MPaintContext.painter = MWindowPainter;
    MWindowRenderer->resetCurrent();
  }

  //----------

  virtual ~SAT_PaintedWindow() {
    if (MWindowPainter) delete MWindowPainter;
    if (MWindowRenderer) delete MWindowRenderer;
  }

//------------------------------
public:
//------------------------------

  SAT_PaintContext* getPaintContext() { return &MPaintContext; }
  SAT_Painter*      getPainter()      { return MWindowPainter; }
  SAT_Renderer*     getRenderer()     { return MWindowRenderer; }

//------------------------------
public: // owner
//------------------------------

  // SAT_SurfaceOwner

  #ifdef SAT_PAINTER_NANOVG
  //bool        on_surfaceOwner_isNanoVG()         override { return true; }
    NVGcontext* on_surfaceOwner_getNanoVGContext() override { return MWindowPainter->getNvgContext(); }
  #endif

//------------------------------
public:
//------------------------------

  virtual void on_window_prerender(uint32_t AWidth, uint32_t AHeight) {}
  virtual void on_window_prepaint(uint32_t AWidth, uint32_t AHeight) {}
  virtual void on_window_paint(SAT_PaintContext* AContext) {}
  virtual void on_window_postpaint() {}
  virtual void on_window_postrender() {}

//------------------------------
public: // window
//------------------------------

  // void on_window_show() override {
  //   SAT_BasicWindow::on_window_show();
  // }

  // void on_window_hide() override {
  //   SAT_BasicWindow::on_window_hide();
  // }

  void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();
    on_window_prerender(screenwidth,screenheight);
    MWindowRenderer->beginRendering(screenwidth,screenheight);
    on_window_prepaint(screenwidth,screenheight);
    MWindowPainter->beginPainting(screenwidth,screenheight);
    MWindowPainter->beginFrame(screenwidth,screenheight);
    on_window_paint(&MPaintContext);
    MWindowPainter->endFrame();
    MWindowPainter->endPainting();
    on_window_postpaint();
    MWindowRenderer->endRendering();
    on_window_postrender();
  }

  //----------

  // void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
  //   //SAT_PRINT("w %i h %i\n",AWidth,AHeight);
  // }

};

//----------------------------------------------------------------------
#endif