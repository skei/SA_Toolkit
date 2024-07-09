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

  bool      MBufferAllocated  = false;
  void*     MRenderBuffer     = nullptr;
  uint32_t  MBufferWidth      = 0;
  uint32_t  MBufferHeight     = 0;

//------------------------------
public:
//------------------------------

  SAT_BufferedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_PaintedWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_BufferedWindow() {
  }

//------------------------------
public:
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
    SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
    SAT_PaintedWindow::on_window_resize(AWidth,AHeight);
    if (MBufferAllocated) {
      resizeRenderBuffer(AWidth,AHeight);
    }
  }

  //----------

  // void on_window_prerender(uint32_t AWidth, uint32_t AHeight) override {
  //   SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
  //   SAT_PaintedWindow::on_window_prerender(AWidth,AHeight);
  // }

  //----------

  void on_window_prepaint(uint32_t AWidth, uint32_t AHeight) override {
    SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
    SAT_PaintedWindow::on_window_prepaint(AWidth,AHeight);
  }

  //----------

  // void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
  //   SAT_PRINT("AXpos %i AYpos %i AWidth %i AHeight %i\n",AXpos,AYpos,AWidth,AHeight);
  //   SAT_PaintedWindow::on_window_paint(AXpos,AYpos,AWidth,AHeight);
  // }

  //----------

  void on_window_postpaint() override {
    SAT_TRACE;
    SAT_PaintedWindow::on_window_postpaint();
  }

  //----------

  // void on_window_postrender() override {
  //   SAT_TRACE;
  //   SAT_PaintedWindow::on_window_postrender();
  // }

  //----------

  // void on_window_realign() override {
  //   SAT_TRACE;
  //   SAT_PaintedWindow::on_window_realign();
  // }

  //----------

  #endif // buffered

//------------------------------
private:
//------------------------------

  bool createRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
    SAT_Painter* painter = getPainter();
    SAT_Assert(painter);
    if (painter) {
      MBufferWidth = AWidth;
      MBufferHeight = AHeight;
      MRenderBuffer = painter->createRenderBuffer(AWidth,AHeight);
      //SAT_PRINT("painter %p MRenderBuffer %p\n",painter,MRenderBuffer);
      SAT_Assert(MRenderBuffer);
      return true;
    }
    return false;
  }

  //----------

  void deleteRenderBuffer() {
    SAT_Painter* painter = getPainter();
    if (painter && MRenderBuffer) {
      painter->deleteRenderBuffer(MRenderBuffer);
      MBufferWidth = 0;
      MBufferHeight = 0;
      MRenderBuffer = nullptr;
    }
  }

  //----------

  bool resizeRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    SAT_PRINT("MBufferWidth %i MBufferHeight %i -> AWidth %i AHeight %i\n",MBufferWidth,MBufferHeight,AWidth,AHeight);
    deleteRenderBuffer();
    return createRenderBuffer(AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif




  // v1

  #if 0

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
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

    // if there are dirty widgets coming in after this,
    // they have to wait until next timer tick
    
    MIsPainting = true;

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
    painter->endFrame();

    // copy buffer to screen

    painter->selectRenderBuffer(nullptr);
    renderer->setViewport(0,0,window_width,window_height);
    painter->beginFrame(window_width,window_height,1.0);
    painter->resetClip();

    int32_t image = painter->getImageFromRenderBuffer(MRenderBuffer);
    painter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
    if (resized) painter->fillRect(0,0,window_width,window_height);
    else painter->fillRect(AXpos,AYpos,AWidth,AHeight);

    painter->endFrame();

    //

    painter->endPaint();
    renderer->endRendering();

  }

  #endif // 0
