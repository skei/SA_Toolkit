#ifndef sat_basic_window_included
#define sat_basic_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/system/sat_timer.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_window_listener.h"
#include "gui/sat_painter.h"
#include "gui/sat_renderer.h"

//----------------------------------------------------------------------
//
// implemented window
//
//----------------------------------------------------------------------

#ifdef SAT_WINDOW_WAYLAND
  #include "gui/wayland/sat_wayland_window.h"
  typedef SAT_WaylandWindow SAT_ImplementedWindow
#endif

#ifdef SAT_WINDOW_WIN32
  #include "gui/win32/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow
#endif

#ifdef SAT_WINDOW_X11
  #include "gui/x11/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;
#endif

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class SAT_SimpleWindow
: public SAT_ImplementedWindow
, public SAT_TimerListener {

//------------------------------
protected:
//------------------------------

  SAT_WindowListener*     MListener           = nullptr;
  SAT_Timer*              MTimer              = nullptr;
  sat_atomic_bool_t       MIsClosing          { false };

  // painting

  SAT_Renderer*           MWindowRenderer     = nullptr;
  SAT_Painter*            MWindowPainter      = nullptr;
  SAT_PaintContext        MWindowPaintContext = {};
  sat_atomic_bool_t       MIsPainting         { false };

  // buffer

  #ifdef SAT_WINDOW_BUFFERED    

    bool                  MBufferAllocated    = false;
    void*                 MRenderBuffer       = nullptr;
    uint32_t              MBufferWidth        = 0;
    uint32_t              MBufferHeight       = 0;

    // resize

    //SAT_WindowResizeQueue MResizeQueue        = {};
    //sat_atomic_bool_t     MResized            { false };
    uint32_t              MPendingWidth       = 0;
    uint32_t              MPendingHeight      = 0;

  #endif

  // aspect

  int32_t                 MInitialWidth       = 0;
  int32_t                 MInitialHeight      = 0;
  double                  MInitialScale       = 1.0;
  bool                    MProportional       = false;
  double                  MWindowScale        = 1.0;

//------------------------------
public:
//------------------------------

  SAT_SimpleWindow(uint32_t AWidth, uint32_t AHeight, SAT_WindowListener* AListener=nullptr, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MListener = AListener;
    MWindowRenderer = new SAT_Renderer(this,this);
    MWindowRenderer->makeCurrent();
    MWindowPainter = new SAT_Painter(this,this);
    MWindowPaintContext.painter = MWindowPainter;
    MWindowPaintContext.update_rect = {0,0,AWidth,AHeight};
    MWindowRenderer->resetCurrent();
    MTimer = new SAT_Timer(this);
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    #ifdef SAT_WINDOW_BUFFERED    
      MPendingWidth = AWidth;
      MPendingHeight = AHeight;
    #endif
  }

  //----------

  virtual ~SAT_SimpleWindow() {
    MIsClosing = true;
    if (MTimer) {
      MTimer->stop();
      delete MTimer;
    }
    if (MWindowPainter) delete MWindowPainter;
    if (MWindowRenderer) delete MWindowRenderer;
  }

//------------------------------
public:
//------------------------------

  SAT_PaintContext* getPaintContext() { return &MWindowPaintContext; }
  SAT_Painter*      getPainter()      { return MWindowPainter; }
  SAT_Renderer*     getRenderer()     { return MWindowRenderer; }
  bool              isPainting()      { return MIsPainting; }
  bool              isClosing()       { return MIsClosing; }

//------------------------------
public: // setup
//------------------------------

  void setInitialSize(uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MInitialScale = AScale;
    MProportional = AProportional;
    MWindowScale = calcScale(AWidth,AHeight);
  }

  //----------

  double calcScale(int32_t AWidth, int32_t AHeight) {
    double scale = 1.0;
    if ((MInitialWidth > 0) && (MInitialHeight > 0)) {
      double xscale = (double)AWidth / (double)MInitialWidth;
      double yscale = (double)AHeight / (double)MInitialHeight;
      if (xscale < yscale) scale = xscale;
      else scale =  yscale;
    }
    return scale;
  }

//------------------------------
private: // buffer
//------------------------------

#ifdef SAT_WINDOW_BUFFERED    

  virtual bool createRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
    // SAT_Assert(w2 < 10000)
    // SAT_Assert(h2 < 10000)
    if (w2 < SAT_WINDOW_BUFFER_MIN_SIZE) w2 = SAT_WINDOW_BUFFER_MIN_SIZE;
    if (h2 < SAT_WINDOW_BUFFER_MIN_SIZE) h2 = SAT_WINDOW_BUFFER_MIN_SIZE;
    SAT_Painter* painter = getPainter();
    SAT_Assert(painter);
    if (painter) {
      SAT_PRINT("creating %i,%i\n",w2,h2);
      MRenderBuffer = painter->createRenderBuffer(w2,h2);
      SAT_Assert(MRenderBuffer);
      MBufferWidth = w2;
      MBufferHeight = h2;
      return true;
    }
    return false;
  }

  //----------

  virtual void deleteRenderBuffer() {
    SAT_Painter* painter = getPainter();
    if (painter && MRenderBuffer) {
      //SAT_PRINT("\n");
      SAT_PRINT("deleting\n");
      painter->deleteRenderBuffer(MRenderBuffer);
      MBufferWidth = 0;
      MBufferHeight = 0;
      MRenderBuffer = nullptr;
    }
  }

  //----------

  virtual bool resizeRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    //SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
    bool resized = false;
    uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
    if ((w2 == MBufferWidth) && (h2 == MBufferHeight)) return false;
    //SAT_PRINT("w2 %i h2 %i\n",w2,h2);
    //SAT_TRACE;
    SAT_Painter* painter = getPainter();
    SAT_Assert(painter);
    if (painter && MRenderBuffer) {
      SAT_PRINT("resizing %i,%i\n",w2,h2);
      void* buffer = painter->createRenderBuffer(w2,h2);
      SAT_Assert(buffer);
      void* old_buffer = MRenderBuffer;
      // potentially copy buffer here..
      MRenderBuffer = buffer;
      MBufferWidth  = w2;
      MBufferHeight = h2;
      painter->deleteRenderBuffer(old_buffer);
      resized = true;
    }
    return resized;
  }

#endif

//------------------------------
public: // painting
//------------------------------

  // override these to 'do your things'..

  virtual void preRender(uint32_t AWidth, uint32_t AHeight) {}
  virtual void prePaint(uint32_t AWidth, uint32_t AHeight) {}
  virtual void paint(SAT_PaintContext* AContext) {}
  virtual void paintRoot(SAT_PaintContext* AContext) {}
  virtual void postPaint() {}
  virtual void postRender() {}

//------------------------------
public: // timer
//------------------------------

  virtual void startTimer(float ms) {
    #ifdef SAT_LINUX
      if (MTimer) MTimer->start(ms);
    #endif
    #ifdef SAT_WIN32
      HWND hwnd = getWin32Window();
      if (MTimer) MTimer->start(ms,hwnd);
    #endif
  }

  //----------

  virtual void stopTimer() {
    if (MTimer) MTimer->stop();
  }

  //----------

  // (overridden by SAT_Window)

  // void on_TimerListener_update(SAT_Timer* ATimer, double ADelta) override {
  //   //SAT_TRACE;
  //   sendClientMessage(SAT_WINDOW_THREAD_TIMER,0);
  // }

//------------------------------
public: // window
//------------------------------

  void on_window_show() override {
    #ifdef SAT_WINDOW_BUFFERED    
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
    #endif    
    #ifdef SAT_WINDOW_TIMER_AUTOSTART
      startTimer(SAT_WINDOW_TIMER_MS);
    #endif
    MIsClosing = false;
  }

  //----------

  void on_window_hide() override {
    MIsClosing = true;
    #ifdef SAT_WINDOW_TIMER_AUTOSTART
      stopTimer();
    #endif
    #ifdef SAT_WINDOW_BUFFERED    
      if (MBufferAllocated) {
        SAT_Renderer* renderer = getRenderer();
        renderer->makeCurrent();
        deleteRenderBuffer();
        MBufferAllocated = false;
        renderer->resetCurrent();
      }
    #endif
  }

  //----------

  /*
    we don't risk getting overlapping on_window_paint and on_window_resize events, do we?

    should we make the renderer conrtext current during resizing?
  */

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    SAT_Assert(AWidth < 32768);
    SAT_Assert(AHeight < 32768);
    //SAT_Assert(MIsPainting);
    //SAT_Assert(MResized);
    // if (AWidth >= 32768) AWidth = 1;
    // if (AHeight >= 32768) AHeight = 1;
    MWindowScale = calcScale(AWidth,AHeight);
    #ifdef SAT_WINDOW_BUFFERED    
      //MResized = true;
      //MResizeQueue.write(true);
      MPendingWidth = SAT_NextPowerOfTwo(AWidth);
      MPendingHeight = SAT_NextPowerOfTwo(AHeight);
      // resize buffer
      // SAT_Assert(MWindowRenderer);
      // MWindowRenderer->makeCurrent();
      // resizeRenderBuffer(MPendingWidth,MPendingHeight);
      // MWindowRenderer->resetCurrent();
    #endif
    //SAT_PRINT("AWidth %i AHeight %i -> MPeidngWidth %i MPendingHeight %i\n",AWidth,AHeight,MPendingWidth,MPendingHeight);
  }

  //----------

  // on_window_resize and on_window_paint will not be called concurrently, will they?
  // ..resize while we're painting...

  // (we don't care about the update rect..)

  void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {

    if (MIsClosing || MIsPainting) return;

    MWindowPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();

    MIsPainting = true;
    
    preRender(screenwidth,screenheight);
    MWindowRenderer->beginRendering(screenwidth,screenheight);

    prePaint(screenwidth,screenheight);
    MWindowPainter->beginPainting(screenwidth,screenheight);
    
    #ifdef SAT_WINDOW_BUFFERED

      // --- resize buffer if needed
      bool resized = resizeRenderBuffer(MPendingWidth,MPendingHeight);

      // --- paint widgets to buffer
      MWindowPainter->selectRenderBuffer(MRenderBuffer);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowRenderer->setViewport(0,0,MBufferWidth,MBufferHeight);

      // if we have a transparent widget on top of for example a knob,
      // the following lines will only redraw the knob, and not
      // the transp widget on top of it..
      // we have to draw entirely from the root to 'catch' all layers.-
      // :-/

      // if (resized) paintRoot(&MWindowPaintContext);
      // else paint(&MWindowPaintContext);

      paintRoot(&MWindowPaintContext);

      // --- copy buffer to screen
      MWindowPainter->selectRenderBuffer(nullptr);
      MWindowRenderer->setViewport(0,0,screenwidth,screenheight);
      MWindowPainter->beginFrame(screenwidth,screenheight,1.0);
      if (MRenderBuffer) {
        int32_t image = MWindowPainter->getImageFromRenderBuffer(MRenderBuffer);
          MWindowPainter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
          //MWindowPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
          MWindowPainter->fillRect(0,0,screenwidth,screenheight);
      }
      MWindowPainter->endFrame();

    #else // not buffered

      // if we're not buffering, we need to redraw the entire thing

      MWindowPainter->beginFrame(screenwidth,screenheight);
      paintRoot(&MWindowPaintContext);
      MWindowPainter->endFrame();

    #endif // buffered

    MWindowPainter->endPainting();
    postPaint();
    MWindowRenderer->endRendering();
    postRender();

    MIsPainting = false;

  }

  //----------

  // void on_window_realign() override {
  // }

  //----------

  // void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  //----------

  // void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  //----------

  // void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
  // }

  //----------

  // void on_window_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  // }

  //----------

  // void on_window_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  // }

  //----------

  // void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  // }

  //----------

  // void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  // }

  //----------

  // void on_window_clientMessage(uint32_t AData) override {
  // }

  //----------

  // void on_window_timer() override {
  //   SAT_TRACE;
  // }

};

//----------------------------------------------------------------------
#endif
