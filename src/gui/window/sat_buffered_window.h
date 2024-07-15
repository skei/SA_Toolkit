#ifndef sat_buffered_window_included
#define sat_buffered_window_included
//----------------------------------------------------------------------

#define SAT_WINDOW_BUFFERED

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

  bool              MBufferAllocated    = false;
  void*             MRenderBuffer       = nullptr;
  uint32_t          MBufferWidth        = 0;
  uint32_t          MBufferHeight       = 0;

  bool              MResized            = false;
  uint32_t          MPendingWidth       = 0;
  uint32_t          MPendingHeight      = 0;

//SAT_Painter*      MBufferPainter      = nullptr;
//SAT_PaintContext  MBufferPaintContext = {};

//------------------------------
public:
//------------------------------

  SAT_BufferedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_PaintedWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_BufferedWindow() {
    //deleteRenderBuffer();
  }

//------------------------------
public: // window
//------------------------------

  #ifdef SAT_WINDOW_BUFFERED

  //----------

  void on_window_show() override {
    SAT_TRACE;
    SAT_PaintedWindow::on_window_show();
    if (!MBufferAllocated) {
      SAT_Renderer* renderer = getRenderer();
      renderer->makeCurrent();
      uint32_t w = getWidth();
      uint32_t h = getHeight();
      // w = SAT_NextPowerOfTwo(w);
      // h = SAT_NextPowerOfTwo(w);
      createRenderBuffer(w,h);
      MBufferAllocated = true;
      renderer->resetCurrent();
    }
  }

  //----------

  void on_window_hide() override {
    SAT_TRACE;
    SAT_PaintedWindow::on_window_hide();
    if (MBufferAllocated) {
      SAT_Renderer* renderer = getRenderer();
      renderer->makeCurrent();
      deleteRenderBuffer();
      MBufferAllocated = false;
      renderer->resetCurrent();
    }
  }

  //----------

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
    SAT_PaintedWindow::on_window_resize(AWidth,AHeight);
    MResized = true;
    MPendingWidth = SAT_NextPowerOfTwo(AWidth);
    MPendingHeight = SAT_NextPowerOfTwo(AHeight);
    // if (MBufferAllocated) {
    //   resizeRenderBuffer(AWidth,AHeight);
    // }
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
    MWindowPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);

    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();
    preRender(screenwidth,screenheight);

    MWindowRenderer->beginRendering(screenwidth,screenheight);
    //MWindowRenderer->beginRendering();

    if (MResized) {
      //SAT_PRINT("resizing\n");
      resizeRenderBuffer(MPendingWidth,MPendingHeight);
    }

    //MWindowRenderer->setViewport(0,0,screenwidth,screenheight);

    prePaint(screenwidth,screenheight);
    MWindowPainter->beginPainting(screenwidth,screenheight);

    // paint widgets

    MWindowPainter->selectRenderBuffer(MRenderBuffer);
      //MWindowPainter->beginFrame(screenwidth,screenheight);
      //MWindowRenderer->setViewport(0,0,screenwidth,screenheight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowRenderer->setViewport(0,0,MBufferWidth,MBufferHeight);

      MWindowPainter->setClipRect(SAT_Rect(0,0,screenwidth,screenheight));
      MWindowPainter->setClip(0,0,screenwidth,screenheight);
      paint(&MWindowPaintContext);
      MWindowPainter->endFrame();

    // copy updated part of buffer to screen/window

    MWindowPainter->selectRenderBuffer(nullptr);
      MWindowRenderer->setViewport(0,0,screenwidth,screenheight);
      MWindowPainter->beginFrame(screenwidth,screenheight,1.0);
      MWindowPainter->resetClip();
      int32_t image = MWindowPainter->getImageFromRenderBuffer(MRenderBuffer);
      if (MResized) {
        MWindowPainter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
        MWindowPainter->fillRect(0,0,screenwidth,screenheight);
      }
      else {
        MWindowPainter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
        MWindowPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
      }
      MWindowPainter->endFrame();

    MWindowPainter->endPainting();
    //postPaint();
    MWindowRenderer->endRendering();
    //postRender();

    MResized = false;
  }

//------------------------------
public: // painting
//------------------------------

  //renderer->setViewport(0,0,MBufferWidth,MBufferHeight);

  // void preRender(uint32_t AWidth, uint32_t AHeight) override {
  //   //SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
  //   SAT_PaintedWindow::preRender(AWidth,AHeight);
  // }

  //----------

  // void postRender() override {
  //   //SAT_PRINT("\n");
  //   SAT_PaintedWindow::postRender();
  // }

  //----------


  // void prePaint(uint32_t AWidth, uint32_t AHeight) override {
  //   //SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
  //   SAT_PaintedWindow::prePaint(AWidth,AHeight);
  //   // painter->selectRenderBuffer(MRenderBuffer);
  //   // painter->beginFrame(MBufferWidth,MBufferHeight,1.0);
  //   // painter->setClipRect(SAT_Rect(0,0,window_width,window_height));
  //   // painter->setClip(0,0,window_width,window_height);
  // }

  //----------

  // void postPaint() override {
  //   //SAT_PRINT("\n");
  //   SAT_PaintedWindow::postPaint();
  // }

  //----------

  // void paint(SAT_PaintContext* AContext) override {
  //   SAT_PRINT("\n");
  //   SAT_PaintedWindow::paint(AContext);
  // }
  
//------------------------------
private: // buffer
//------------------------------

  bool createRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
    SAT_Painter* painter = getPainter();
    SAT_Assert(painter);
    if (painter) {
      MRenderBuffer = painter->createRenderBuffer(w2,h2);
      SAT_Assert(MRenderBuffer);
      MBufferWidth = w2;
      MBufferHeight = h2;
      SAT_PRINT("AWidth %i AHeight %i MRenderBuffer %p -> %i,%i\n",AWidth,AHeight,MRenderBuffer,w2,h2);
      return true;
    }
    return false;
  }

  //----------

  void deleteRenderBuffer() {
    SAT_Painter* painter = getPainter();
    if (painter && MRenderBuffer) {
      SAT_PRINT("MRenderBuffer %p\n",MRenderBuffer);
      painter->deleteRenderBuffer(MRenderBuffer);
      MBufferWidth = 0;
      MBufferHeight = 0;
      MRenderBuffer = nullptr;
    }
  }

  //----------

  /*
    note: does the context have to be current before resizing?
    should we only resize buffer during painting?
  */      

  bool resizeRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    // bool resized = false;
    uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
    SAT_PRINT("MBufferWidth %i MBufferHeight %i -> w2 %i h2 %i (AWidth %i AHeight %i)\n",MBufferWidth,MBufferHeight,w2,h2,AWidth,AHeight);
    if ((w2 == MBufferWidth) && (h2 == MBufferHeight)) return true;
    SAT_Painter* painter = getPainter();
    if (painter && MRenderBuffer) {
      void* buffer = painter->createRenderBuffer(w2,h2);
      SAT_Assert(buffer);
      // potentially copy buffer here..
      painter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = w2;
      MBufferHeight = h2;
      // resized = true;
      return true;
    }
    //deleteRenderBuffer();
    //return createRenderBuffer(AWidth,AHeight);
    return false;
  }

  //----------

  #endif // SAT_WINDOW_BUFFERED

};

//----------------------------------------------------------------------
#endif




#if 0

  // void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
  //   //SAT_PRINT("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
  //   //SAT_PaintedWindow::on_window_paint(AXpos,AYpos,AWidth,AHeight);
  //   MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
  //   uint32_t screenwidth = getWidth();
  //   uint32_t screenheight = getHeight();
  //   MWindowRenderer->beginRendering(screenwidth,screenheight);
  //   MWindowPainter->beginPainting(screenwidth,screenheight);
  //   MWindowPainter->beginFrame(screenwidth,screenheight);
  //   //on_window_paint(&MPaintContext);
  //   MWindowPainter->endFrame();
  //   MWindowPainter->endPainting();
  //   MWindowRenderer->endRendering();
  // }

  //----------

  // v1

  void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
    //SAT_Print("AXpos %i AYpos %i AWidth %i AHeight %i counter %i MRenderBuffer %p (%i,%i)\n",AXpos,AYpos,AWidth,AHeight,MPaintContext.counter,MRenderBuffer,MBufferWidth,MBufferHeight);
    SAT_Renderer* renderer = getRenderer();
    SAT_Painter* painter = getPainter();
    SAT_Assert(renderer);
    SAT_Assert(painter);
    MPaintContext.painter = painter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.scale = MScale;
    MPaintContext.counter += 1;
    uint32_t window_width = getWidth();
    uint32_t window_height = getHeight();

    // start rendering to buffer

    renderer->beginRendering(window_width,window_height);
    painter->beginPaint(window_width,window_height);

    // resize buffer

    //uint32_t num_painted = 0;
    bool resized = false;
    uint32_t width2  = SAT_NextPowerOfTwo(getWidth());
    uint32_t height2 = SAT_NextPowerOfTwo(getHeight());
    //SAT_Print("bufferwidth %i bufferheight %i getWidth %i getHeight %i width2 %i height2 %i\n",MBufferWidth,MBufferHeight,getWidth(),getHeight(),width2,height2);
    if ((width2 != MBufferWidth) || (height2 != MBufferHeight)) {
      void* buffer = painter->createRenderBuffer(width2,height2);
      SAT_Assert(buffer);
      // potentially copy buffer here..
      painter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = width2;
      MBufferHeight = height2;
      resized = true;
    }

    // render paint queue
    painter->selectRenderBuffer(MRenderBuffer);
    renderer->setViewport(0,0,MBufferWidth,MBufferHeight);
    painter->beginFrame(MBufferWidth,MBufferHeight,1.0);
    painter->setClipRect(SAT_Rect(0,0,window_width,window_height));
    painter->setClip(0,0,window_width,window_height);
    // if there are dirty widgets coming in after this, they have to wait until next timer tick
    MIsPainting = true;

    // if we have resized the window, we redraw everything (from the rootup)

    if (resized) {
      // root widget
      SAT_Widget* widget;
      while (MPaintWidgets.read(&widget)) {}
      if (MRootWidget) {
        // huh? this isn't needed.. we're resized!
        if (MRootWidget->getLastPainted() != MPaintContext.counter) {
          //painter->resetClip();
          //SAT_Print("root widget\n");
          MRootWidget->on_widget_paint(&MPaintContext);
          MRootWidget->setLastPainted(MPaintContext.counter);
        }
      }
    }

    // else we paint only widgets in the paint queue

    else {
      // paint queue
      SAT_Widget* widget;
      while (MPaintWidgets.read(&widget)) {
        if (widget->isRecursivelyVisible()) {
        //if (widget->isVisible()) {
          if (widget->getLastPainted() != MPaintContext.counter) {
            SAT_Rect cliprect = calcClipRect(widget);
            // if cliprect visible?
            painter->pushClip(cliprect);
            //SAT_Print("%s\n",widget->getName());
            widget->on_widget_paint(&MPaintContext);
            painter->popClip();
            widget->setLastPainted(MPaintContext.counter);
          }
        }
      }
    }
    MIsPainting = false;

    // end buffer painting

    painter->endFrame();
    // copy buffer to screen
    painter->selectRenderBuffer(nullptr);

    // copy updated part of buffer to screen/window

    renderer->setViewport(0,0,window_width,window_height);
    painter->beginFrame(window_width,window_height,1.0);
    painter->resetClip();
    int32_t image = painter->getImageFromRenderBuffer(MRenderBuffer);
    painter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
    if (resized) painter->fillRect(0,0,window_width,window_height);
    else painter->fillRect(AXpos,AYpos,AWidth,AHeight);
    painter->endFrame();
    painter->endPaint();
    renderer->endRendering();
  }

#endif // 0
