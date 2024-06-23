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

  // void*               MRenderBuffer         = nullptr;
  // uint32_t            MBufferWidth          = 0;
  // uint32_t            MBufferHeight         = 0;


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

//------------------------------
public:
//------------------------------

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

};

//----------------------------------------------------------------------
#endif