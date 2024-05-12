#ifndef sat_painted_window_included
#define sat_painted_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/system/sat_timer.h"
#include "gui/painter/sat_paint_context.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PaintedWindow
: public SAT_TimerWindow
{

//------------------------------
private:
//------------------------------

  int32_t           MInitialWidth   = 0;
  int32_t           MInitialHeight  = 0;
  double            MScale          = 1.0;

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
  : SAT_TimerWindow(AWidth,AHeight,AParent) {
    //SAT_PRINT("\n");
    MWindowRenderer = new SAT_Renderer(this,this);
    MWindowPainter = new SAT_Painter(/*this,*/this);
    MPaintContext.painter = MWindowPainter;

    // #ifdef SAT_PAINTER_NANOVG
    //   SAT_GLOBAL.GUI.nanoVGContext = MWindowPainter.getNanoVGContext();
    // #endif

    // #ifdef SAT_PAINTER_X11
    //   SAT_GLOBAL.GUI.xcbConnection  = _getXcbConnection();
    //   SAT_GLOBAL.GUI.xcbVisual      = _getXcbVisual();
    //   SAT_GLOBAL.GUI.xcbDrawable    = _getXcbDrawable();
    //   SAT_GLOBAL.GUI.xcbDepth       = _getDepth();
    //   SAT_PRINT("xcbConnection: %p\n",_getXcbConnection());
    //   SAT_PRINT("xcbVisual:     %i\n",_getXcbVisual());
    //   SAT_PRINT("xcbDrawable:   %i\n",_getXcbDrawable());
    //   SAT_PRINT("xcbDepth:      %i\n",_getDepth());
    // #endif

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

  #ifdef SAT_PAINTER_NANOVG
    bool        _isNanoVG()         override { return true; }
    NVGcontext* _getNanoVGContext() override { return MWindowPainter->getNvgContext(); }
  #endif

//------------------------------
public:
//------------------------------

  virtual void on_window_paint(SAT_PaintContext* AContext) {
    //SAT_TRACE;
  }

//------------------------------
public: // window
//------------------------------

  // void on_window_show() override {
  //   SAT_TimerWindow::on_window_show();
  // }

  // void on_window_hide() override {
  //   SAT_TimerWindow::on_window_hide();
  // }

  void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);

    //MWindowRenderer->makeCurrent();
    MWindowRenderer->beginRendering(AWidth,/*-*/AHeight);
    MWindowPainter->beginPainting(AWidth,AHeight);
    MWindowPainter->beginFrame(AWidth,AHeight);

    on_window_paint(&MPaintContext);

    MWindowPainter->endFrame();
    MWindowPainter->endPainting();
    MWindowRenderer->endRendering();
    //MWindowRenderer->swapBuffers();
    //MWindowRenderer->resetCurrent();

  }

  //----------

  // void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
  //   //SAT_PRINT("w %i h %i\n",AWidth,AHeight);
  // }

//------------------------------
public: // scale
//------------------------------

  virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MScale = recalcScale( getWidth(), getHeight() );
    // MScale = recalcScale(AWidth,AHeight);
    //SAT_Print("MScale %f\n",MScale);
  }

  //----------

  // calculates the maximal (or minimal) scale to use for the gui,
  // that will fit inside the given width/height

  virtual double recalcScale(int32_t AWidth, int32_t AHeight) {
    double scale = 1.0;
    if ((MInitialWidth > 0) && (MInitialHeight > 0)) {
      double xscale = (double)AWidth / (double)MInitialWidth;
      double yscale = (double)AHeight / (double)MInitialHeight;
      if (xscale < yscale) scale = xscale;
      else scale =  yscale;
    }
    //SAT_Print("scale: %f\n",scale);
    return scale;
  }

};

//----------------------------------------------------------------------
#endif