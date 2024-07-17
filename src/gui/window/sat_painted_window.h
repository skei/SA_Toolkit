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

  SAT_Renderer*     MWindowRenderer     = nullptr;
  SAT_Painter*      MWindowPainter      = nullptr;
  SAT_PaintContext  MWindowPaintContext = {};

  sat_atomic_bool_t MIsPainting         = false;
  sat_atomic_bool_t MIsClosing          = false;

//------------------------------
public:
//------------------------------

  SAT_PaintedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BasicWindow(AWidth,AHeight,AParent) {
    //SAT_PRINT("\n");
    MWindowRenderer = new SAT_Renderer(this,this);
    MWindowPainter = new SAT_Painter(this,this);
    MWindowPaintContext.painter = MWindowPainter;
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

  SAT_PaintContext* getPaintContext() { return &MWindowPaintContext; }
  SAT_Painter*      getPainter()      { return MWindowPainter; }
  SAT_Renderer*     getRenderer()     { return MWindowRenderer; }

  bool isPainting() { return MIsPainting; }
  bool isClosing()  { return MIsClosing; }

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

  // virtual void preRender(uint32_t AWidth, uint32_t AHeight) {}
  // virtual void prePaint(uint32_t AWidth, uint32_t AHeight) {}
  // virtual void paint(SAT_PaintContext* AContext) {}
  // virtual void postPaint() {}
  // virtual void postRender() {}

//------------------------------
public: // window
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
    MWindowPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();

    MIsPainting = true;

    preRender(screenwidth,screenheight);
    MWindowRenderer->beginRendering(screenwidth,screenheight);
    prePaint(screenwidth,screenheight);
    MWindowPainter->beginPainting(screenwidth,screenheight);
    MWindowPainter->beginFrame(screenwidth,screenheight);
    paint(&MWindowPaintContext,false);
    MWindowPainter->endFrame();
    MWindowPainter->endPainting();
    postPaint();
    MWindowRenderer->endRendering();
    postRender();

    MIsPainting = false;

  }

};

//----------------------------------------------------------------------
#endif