#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------
//
// implemented window
//
//----------------------------------------------------------------------

#if defined(SAT_GUI_NOGUI)
  ;

#elif defined (SAT_GUI_WAYLAND)
  #include "gui/wayland/sat_wayland_window.h"
  typedef SAT_WaylandWindow SAT_ImplementedWindow;

#elif defined (SAT_GUI_WIN32)
  #include "gui/win32/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow;

#elif defined (SAT_GUI_X11)
  #include "gui/x11/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;

#else
  #error GUI type not defined

#endif

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

#include "base/system/sat_timer.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/base/sat_window_listener.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/widgets/sat_root_widget.h"
#include "gui/sat_widget.h"

typedef SAT_LockFreeQueue<SAT_Widget*,SAT_WINDOW_MAX_DIRTY_WIDGETS> SAT_WidgetQueue;

//----------

class SAT_Window
: public SAT_ImplementedWindow
, public SAT_WidgetOwner
, public SAT_WidgetListener
, public SAT_TimerListener {

//------------------------------
private:
//------------------------------

  SAT_RootWidget*     MRootWidget           = nullptr;
  SAT_WindowListener* MListener             = nullptr;
  sat_atomic_bool_t   MIsPainting           = false;
  SAT_PaintContext    MPaintContext         = {};
  SAT_Timer           MTimer                = SAT_Timer(this);
  SAT_WidgetQueue     MDirtyWidgets         = {};
  SAT_WidgetQueue     MPaintWidgets         = {};
  SAT_Widget*         MHoverWidget          = nullptr;
  SAT_Widget*         MModalWidget          = nullptr;
  SAT_Widget*         MInteractiveWidget    = nullptr;
  SAT_Widget*         MMouseCaptureWidget   = nullptr;
  SAT_Widget*         MKeyCaptureWidget     = nullptr;
  SAT_Widget*         MMouseCapturedWidget  = nullptr;
  SAT_Widget*         MKeyCapturedWidget    = nullptr;

  void*               MRenderBuffer = nullptr;
  uint32_t            MBufferWidth = 0;
  uint32_t            MBufferHeight = 0;

  int32_t             MMouseCurrentXpos     = 0;
  int32_t             MMouseCurrentYpos     = 0;
  int32_t             MMousePreviousXpos    = 0;
  int32_t             MMousePreviousYpos    = 0;
  int32_t             MMouseCurrentCursor   = 0;
  bool                MMouseIsLocked        = false;
  int32_t             MLockedCurrentX       = 0;
  int32_t             MLockedCurrentY       = 0;
  int32_t             MLockedClickedX       = 0;
  int32_t             MLockedClickedY       = 0;

  double MPrevTime = 0.0;

  int32_t MInitialWidth = 0;
  int32_t MInitialHeight = 0;
  double MScale = 1.0;


//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    //SAT_PRINT;
    SAT_BasePainter* painter = getPainter();
    MPaintContext.painter     = painter;
    MPaintContext.update_rect = SAT_Rect(0,0,AWidth,AHeight);
    MPaintContext.scale       = 1.0;
    MPaintContext.counter     = 0;

    // buffer  
    uint32_t width2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(AHeight);
    //SAT_Print("creating FBO. %i,%i pow2: %i,%i\n",AWidth,AHeight, width2,height2);
    MRenderBuffer = painter->createRenderBuffer(width2,height2);
    //SAT_Assert(MRenderBuffer);
    MBufferWidth = width2;
    MBufferHeight = height2;
  }

  //----------

  virtual ~SAT_Window() {
    //delete MPainter;
    //delete MRenderer;

    if (MTimer.isRunning()) MTimer.stop();
    if (MRootWidget) {
      //MRootWidget->cleanup(MWindowPainter);
      delete MRootWidget;
    }

    SAT_BasePainter* painter = getPainter();
    painter->deleteRenderBuffer(MRenderBuffer);

  }

//------------------------------
public:
//------------------------------

  virtual void setRootWidget(SAT_RootWidget* AWidget) {
    MRootWidget = AWidget;
    MRootWidget->setParent(nullptr);
    //MRootWidget->setListener(this);
    //MRootWidget->setOwner(this);
  }

  //----------

  virtual void setListener(SAT_WindowListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual void markWidgetDirty(SAT_Widget* AWidget) {
    MDirtyWidgets.write(AWidget);
  }

  //----------

  virtual void startTimer(uint32_t AMSInterval, bool AOneShot=false) {
    MTimer.start(AMSInterval,AOneShot);
  }

  //----------

  virtual void stopTimer() {
    MTimer.stop();
  }

  //----------

  bool isPainting() {
    return MIsPainting;
  }

//------------------------------
public: // scale
//------------------------------

  //----------

  virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MScale = recalcScale(getWidth(),getHeight());
  }

  //----------

  virtual double recalcScale(int32_t AWidth, int32_t AHeight) {
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
public: // mouse
//------------------------------

  virtual void lockMouseCursor() {
    MMouseIsLocked  = true;
    MLockedClickedX = MMouseCurrentXpos;
    MLockedClickedY = MMouseCurrentYpos;
    MLockedCurrentX = MMouseCurrentXpos;
    MLockedCurrentY = MMouseCurrentYpos;
  }

  //----------

  virtual void unlockMouseCursor() {
    MMouseIsLocked = false;
  }

  //----------
  
  virtual void updateLockedMouse(int32_t AXpos, int32_t AYpos) {
    if ((AXpos == MLockedClickedX) && (AYpos == MLockedClickedY)) {
      MMousePreviousXpos = AXpos;
      MMousePreviousYpos = AYpos;
      return;
    }
    int32_t xdiff = AXpos - MLockedClickedX;
    int32_t ydiff = AYpos - MLockedClickedY;
    MLockedCurrentX += xdiff;
    MLockedCurrentY += ydiff;
    setMousePos(MLockedClickedX,MLockedClickedY);
  }

  //----------

  virtual void captureMouse(SAT_Widget* AWidget) {
    MMouseCapturedWidget = AWidget;
  }

  //----------

  virtual void releaseMouse() {
    MMouseCapturedWidget = nullptr;
  }

//------------------------------
public: // keyboard
//------------------------------

  virtual void captureKeys(SAT_Widget* AWidget) {
    MKeyCapturedWidget = AWidget;
  }

  //----------

  virtual void releaseKeys() {
    MKeyCapturedWidget = nullptr;
  }

//------------------------------
private:
//------------------------------

  virtual SAT_Rect calcClipRect(SAT_Widget* AWidget) {
    SAT_Rect rect = AWidget->getRect();
    SAT_Widget* parent = AWidget->getParent();
    while (parent) {
      SAT_Rect parentrect = parent->getRect();
      rect.overlap(parentrect);
      SAT_Widget* next = parent->getParent();
      parent = next;
    }
    return rect;
  }

  //----------

  // this is called every time the mouse moves
  // if we enter a new widger, we let both the old and new know
  // also (should) set the widget's mouse cursor, and send its hint upwards

  void updateHoverWidget(int32_t AXpos, int32_t AYpos) {
    if (MRootWidget) {
      SAT_Widget* hover = MRootWidget->findChildWidget(AXpos,AYpos);
      if (hover) {
        //SAT_Print("%s\n",hover->getName());
        if (hover != MHoverWidget) {
          if (MHoverWidget) {
            //SAT_Print("leaving %s\n",MHoverWidget->getName());
            MHoverWidget->on_widget_leave(hover,AXpos,AYpos);
          }
          //SAT_Print("entering %s\n",hover->getName());
          hover->on_widget_enter(MHoverWidget,AXpos,AYpos);
          MHoverWidget = hover;
          // cursor
          // hint
        }
      }
      else {
        if (MHoverWidget) {
          //SAT_Print("leaving %s\n",MHoverWidget->getName());
          MHoverWidget->on_widget_leave(hover,AXpos,AYpos);
          MHoverWidget = nullptr;
        }
      }
    }
  }

  //----------

  // null = off

  //void modalize(SAT_Widget* AWidget) {
  //  //if (MModalWidget) MModalWidget.unmodalize();
  //  //MModalWidget->modalize();
  //  MModalWidget = AWidget;
  //}

//------------------------------
private:
//------------------------------

  bool checkAndPossiblyResizeBuffer(int32_t AWidth, int32_t AHeight) {
    uint32_t width2  = SAT_NextPowerOfTwo(getWidth());
    uint32_t height2 = SAT_NextPowerOfTwo(getHeight());
    if ((width2 != MBufferWidth) || (height2 != MBufferHeight)) {
      // if size has changed: create new buffer, copy old to new, delete old
      SAT_BasePainter* painter = getPainter();
      void* buffer = painter->createRenderBuffer(width2,height2);
      SAT_Assert(buffer);
      // we don't need to copy the buffer if we're redrawing the entire thing anyway, do we?
      //copyBuffer(buffer,0,0,width2,height2,MRenderBuffer,0,0,MBufferWidth,MBufferHeight);
      painter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = width2;
      MBufferHeight = height2;
      return true;
    }
    return false;
  }

  //----------
  
  void paintQueuedWidgetsToBuffer() {
    SAT_BasePainter* painter = getPainter();
    painter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
    //prepaint(&MPaintContext);
    painter->beginFrame(MBufferWidth,MBufferHeight,1.0);
//    painter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
    SAT_Widget* widget;
    //int32_t paint_count = MPaintContext.counter;
    while (MPaintWidgets.read(&widget)) {
      //if (widget->isRecursivelyVisible()) {
      //if (widget->isVisible()) {
        if (widget->getLastPainted() != MPaintContext.counter) { //paint_count) {
          SAT_Rect cliprect = calcClipRect(widget);
          // if cliprect visible?
          painter->pushClip(cliprect);
          widget->on_widget_paint(&MPaintContext);
          painter->popClip();
          widget->setLastPainted(MPaintContext.counter); //paint_count);
          //count += 1;
        }
      //}
    }
    painter->endFrame();
  }

  //----------
  
  void paintRootWidgetToBuffer() {
    SAT_BasePainter* painter = getPainter();
     painter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
     //prepaint(&MPaintContext);
     painter->beginFrame(MBufferWidth,MBufferHeight,1.0);
     painter->setClip(0,0,getWidth(),getHeight());

     // clear paint queue
     SAT_Widget* widget;
     while (MPaintWidgets.read(&widget)) {}

     //count = 1;
     if (MRootWidget) {
       MRootWidget->on_widget_paint(&MPaintContext);
       MRootWidget->setLastPainted(MPaintContext.counter); //paint_count);
     }
     painter->endFrame();
     //postpaint(&MPaintContext);
  }

  //----------
  
  void copyBufferToScreen(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) {
    //copyBuffer(nullptr,0,0,getWidth(),getHeight(),MRenderBuffer,AXpos,AYpos,AWidth,AHeight);
    //void copyBuffer(void* ADst, int32_t ADstXpos, int32_t ADstYpos, uint32_t ADstWidth, uint32_t ADstHeight, void* ASrc, int32_t ASrcXpos, int32_t ASrcYpos, uint32_t ASrcWidth, uint32_t ASrcHeight) {
    SAT_BasePainter* painter = getPainter();
    painter->selectRenderBuffer(nullptr,getWidth(),getHeight());
    painter->beginFrame(getWidth(),getHeight(),1.0);
    int32_t image = painter->getImageFromRenderBuffer(MRenderBuffer);
    painter->setFillImage(image, 0,0, 1,1, 1.0, 0.0);
    painter->fillRect(AXpos,AYpos,AWidth,AHeight);
    painter->endFrame();
    //}  
  }

//------------------------------
public: // base window
//------------------------------

  void on_window_open() override {
    SAT_Print("\n");
    //MRootWidget->on_widget_open(this);
    if (MRootWidget) {
      MRootWidget->setOwner(this);
      uint32_t w = getWidth();
      uint32_t h = getHeight();
      MRootWidget->setSize(SAT_Point(w,h));
      MRootWidget->realignChildWidgets();
      markWidgetDirty(MRootWidget);
    }
    startTimer(SAT_WINDOW_TIMER_MS);

  }

  //----------

  void on_window_close() override {
    SAT_Print("\n");
    stopTimer();
  }
  
  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //SAT_Print("AXpos %i AYpos %i\n",AXpos,AYpos);
  }
  
  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("AWidth %i AHeight %i\n",AWidth,AHeight);
    if (MRootWidget) {
      MRootWidget->setSize(SAT_Point(AWidth,AHeight));
      MRootWidget->realignChildWidgets();
      markWidgetDirty(MRootWidget);
    }
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("AXpos %i AYpos %i AWidth %i AHeight %i counter %i\n",AXpos,AYpos,AWidth,AHeight,MPaintContext.counter);
    MIsPainting = true;

    SAT_BasePainter* painter  = getPainter();
    MPaintContext.painter     = painter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.scale       = 1.0;
    MPaintContext.counter     += 1; // start from 1 (and increase)..

    painter->beginPaint(AXpos,AYpos,AWidth,AHeight);
    if (checkAndPossiblyResizeBuffer(getWidth(),getHeight())) {
      paintRootWidgetToBuffer();
    }
    else {
      paintQueuedWidgetsToBuffer();
    }
    copyBufferToScreen(AXpos,AYpos,AWidth,AHeight);
    painter->endPaint();
    MIsPainting = false;
  }
  
  //----------

  void on_window_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  //----------

  void on_window_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
    MMouseCurrentXpos = AXpos;
    MMouseCurrentYpos = AYpos;
    updateHoverWidget(AXpos,AYpos);
  }
  
  //----------

  void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_Print("\n");
    //if (MRootWidget) {
    //  MRootWidget->on_widget_enter(nullptr,AXpos,AYpos);
    //}
    MHoverWidget = nullptr;
    updateHoverWidget(AXpos,AYpos);
  }
  
  //----------

  void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_Print("\n");
    if (MHoverWidget) {
      MHoverWidget->on_widget_leave(nullptr,AXpos,AYpos);
      MHoverWidget = nullptr;
    }
    //if (MRootWidget) {
    //  MRootWidget->on_widget_leave(nullptr,AXpos,AYpos);
    //}
  }
  
  //----------

  void on_window_client_message(uint32_t AData) override {
    //SAT_Print("AData %i\n",AData);
  }

//------------------------------
public: // widget owner
//------------------------------

  SAT_BaseWindow* on_widgetOwner_getWindow() override {
    return this;
  }

//------------------------------
public: // widget listener
//------------------------------

  // these are called from the root widget

  //----------

  void on_widgetListener_update(SAT_Widget* AWidget) override {
    //SAT_Print("\n");
  }

  //----------

  void on_widgetListener_redraw(SAT_Widget* AWidget) override {
    //SAT_Print("\n");
    //MDirtyWidgets.write(AWidget);
    if (!AWidget) AWidget = MRootWidget;
    if (!MDirtyWidgets.write(AWidget)) {
      SAT_Print("couldn't write to MPendingDirtyWidgets queue\n");
    }
    //if (MListener) MListener->do_windowListener_redraw(AWidget);
  }

  //----------

  void on_widgetListener_realign(SAT_Widget* AWidget) override {
    //SAT_Print("\n");
  }

  //----------

  void on_widgetListener_setCursor(SAT_Widget* ASender, int32_t ACursor) override {
    switch(ACursor) {
      case SAT_CURSOR_LOCK:
        lockMouseCursor();
        break;
      case SAT_CURSOR_UNLOCK:
        unlockMouseCursor();
        break;
      case SAT_CURSOR_SHOW:  
        showMouse();
        setMouseCursor(MMouseCurrentCursor);
        break;
      case SAT_CURSOR_HIDE:
        hideMouse();
        break;
      default:
        if (ACursor != MMouseCurrentCursor) {
          setMouseCursor(ACursor);
          MMouseCurrentCursor = ACursor;
        }
    }
  }  

//------------------------------
public: // timer listener
//------------------------------

  void do_timerListener_callback(SAT_Timer* ATimer) override {
    //SAT_Print("\n");
    if (!ATimer->isRunning()) return;

    double now = SAT_GetTime();
    double elapsed = now - MPrevTime;
    MPrevTime = now;
    //on_window_timer(ATimer,elapsed);

    if (MIsPainting) return;

    // flush param/mod fromHostToGui
    if (MListener) MListener->on_windowListener_timer(this,elapsed);
    // widget timers
    //for (uint32_t i=0; i<MTimerWidgets.size(); i++) MTimerWidgets[i]->on_widget_timer(0,AElapsed);
    //MTweens.process(elapsed,MScale);

    // always starts from 0,0, meaning the recrt will always be drawn from upper left corner
    // and encompass all dirty widgets..
    // should we reduce this, to start at
    // upper left corner of upper-left-most widget?

    SAT_Rect rect; // 0,0,0,0

    SAT_Widget* widget;
    while (MDirtyWidgets.read(&widget)) {
      //SAT_Print("%s\n",widget->getName());
      rect.combine(widget->getRect());
      if (!MPaintWidgets.write(widget)) {
        SAT_Print("couldn't write to MPaintWidgets queue\n");
      }
    }
    if (rect.isNotEmpty()) invalidate(rect.x,rect.y,rect.w,rect.h);

  }

};

//----------------------------------------------------------------------
#endif