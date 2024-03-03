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
#include "base/utils/sat_tween_manager.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_painter_owner.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/base/sat_window_listener.h"
#include "gui/widgets/sat_root_widget.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"

typedef SAT_Queue<SAT_Widget*,SAT_WINDOW_MAX_DIRTY_WIDGETS> SAT_WidgetQueue;

//----------

class SAT_Window
: public SAT_ImplementedWindow
, public SAT_WidgetOwner
, public SAT_WidgetListener
, public SAT_PainterOwner
, public SAT_TimerListener {

//------------------------------
private:
//------------------------------

  //todo? move all these below to SAT_RootWidget?

  SAT_Widget*         MHoverWidget          = nullptr;
  SAT_Widget*         MModalWidget          = nullptr;
  SAT_Widget*         MMouseCaptureWidget   = nullptr;
  SAT_Widget*         MKeyCaptureWidget     = nullptr;
//SAT_Widget*         MInteractiveWidget    = nullptr;
//SAT_PanelWidget*    MOverlayWidget        = nullptr;

  int32_t             MMouseCurrentXpos     = 0;
  int32_t             MMouseCurrentYpos     = 0;
  int32_t             MMousePreviousXpos    = 0;
  int32_t             MMousePreviousYpos    = 0;
  int32_t             MMouseCurrentCursor   = 0;

  int32_t             MMouseClickedXpos     = 0;
  int32_t             MMouseClickedYpos     = 0;
  uint32_t            MMouseClickedButton   = SAT_BUTTON_NONE;
  uint32_t            MMouseClickedState    = SAT_STATE_NONE;
  uint32_t            MMouseClickedTime     = 0;

  bool                MMouseIsLocked        = false;
  int32_t             MLockedCurrentX       = 0;
  int32_t             MLockedCurrentY       = 0;
  int32_t             MLockedClickedX       = 0;
  int32_t             MLockedClickedY       = 0;

//------------------------------
private:
//------------------------------

  SAT_RootWidget*     MRootWidget           = nullptr;
  SAT_WindowListener* MListener             = nullptr;

  sat_atomic_bool_t   MIsPainting           = false;
  SAT_PaintContext    MPaintContext         = {};
  SAT_Painter*        MPainter              = nullptr;
//SAT_TweenManager    MTweenManager         = {};

  void*               MRenderBuffer         = nullptr;
  uint32_t            MBufferWidth          = 0;
  uint32_t            MBufferHeight         = 0;

  SAT_Timer           MTimer                = SAT_Timer(this);
  double              MPrevTime             = 0.0;
  SAT_WidgetArray     MTimerWidgets         = {};

  int32_t             MInitialWidth         = 0;
  int32_t             MInitialHeight        = 0;
  uint32_t            MWidth                = 0;
  uint32_t            MHeight               = 0;
  double              MScale                = 1.0;

  SAT_WidgetQueue     MDirtyGuiWidgets      = {};
  SAT_WidgetQueue     MDirtyHostWidgets     = {};
  SAT_WidgetQueue     MDirtyListenerWidgets = {};
  SAT_WidgetQueue     MPaintWidgets         = {};

  SAT_Skin            MDefaultSkin          = {};

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, double AScale=1.0, SAT_WindowListener* AListener=nullptr, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {

    //setInitialSize(AWidth,AHeight);

    MWidth = AWidth;
    MHeight = AHeight;
    MScale = AScale;

    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MListener = AListener;

    MScale = recalcScale(AWidth,AHeight);
    //SAT_Print("MScale %f\n",MScale);

    SAT_Assert(AWidth > 0);
    SAT_Assert(AHeight > 0);

    MPainter = new SAT_Painter(this);
    SAT_Assert(MPainter);

    MPaintContext.painter     = MPainter;
    MPaintContext.update_rect = SAT_Rect(0,0,AWidth,AHeight);
    MPaintContext.scale       = 1.0;
    MPaintContext.counter     = 0;
    MPaintContext.window      = this;

    uint32_t width2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(AHeight);
    MRenderBuffer = MPainter->createRenderBuffer(width2,height2);
    SAT_Assert(MRenderBuffer);
    MBufferWidth = width2;
    MBufferHeight = height2;

    //if (AParent) setParent(AParent);

    SAT_Renderer* renderer = getRenderer();
    SAT_Assert(renderer);
    renderer->resetCurrent();

  }

  //----------

  virtual ~SAT_Window() {

    if (MTimer.isRunning()) MTimer.stop();
    if (MRootWidget) {
      //MRootWidget->cleanup(MWindowPainter);
      delete MRootWidget;
    }

    SAT_Assert(MPainter);
    MPainter->deleteRenderBuffer(MRenderBuffer);

    delete MPainter;

  }

//------------------------------
public:
//------------------------------

  //SAT_TweenManager* getTweenManager() {
  //  return &MTweenManager;
  //}

  bool isPainting() {
    return MIsPainting;
  }

//------------------------------
public:
//------------------------------

  // called from:
  // on_window_open()
  // on_window_resize()

  virtual bool markRootWidgetDirty/*FromGui*/() {
    if (MRootWidget) {
      return MDirtyGuiWidgets.write(MRootWidget);
    }
    SAT_Print("MRootWidget is null\n");
    return false;
  }

  //----------

  // called from:
  // on_widgetListener_redraw(), _realign

  virtual bool markWidgetDirtyFromGui(SAT_Widget* AWidget) {
    return MDirtyGuiWidgets.write(AWidget);
  }

  //----------

  // called from:
  // (event handler)

  virtual bool markWidgetDirtyFromHost(SAT_Widget* AWidget) {
    return MDirtyHostWidgets.write(AWidget);
  }

  //----------

  // called from:
  // on_widgetListener_realign

  virtual bool markWidgetDirtyFromListener(SAT_Widget* AWidget) {
    return MDirtyListenerWidgets.write(AWidget);
  }

  //----------

  // if you have a static root widget, and dson't want it to be auto-deleted here,
  // you nmust set the ptr to null so that the destructor doesn't try to delete it..

  virtual void clearRootWidget() {
    MRootWidget = nullptr;
  }

  virtual void setRootWidget(SAT_RootWidget* AWidget) {
    MRootWidget = AWidget;
    MRootWidget->setParent(nullptr);
    MRootWidget->setListenerIfNull(this);
  }

  //----------

  // called from:
  // on_window_open()

  virtual void startTimer(uint32_t AMSInterval, bool AOneShot=false) {
    //HWND hwnd,     
    #ifdef SAT_LINUX
      MTimer.start(AMSInterval,AOneShot);
    #endif
    #ifdef SAT_WIN32
      HWND hwnd = getWin32Window();
      MTimer.start(AMSInterval,hwnd,AOneShot);
    #endif
  }

  //----------

  // called from:
  // on_window_close  

  virtual void stopTimer() {
    MTimer.stop();
  }

  //----------

  virtual void registerTimerWidget(SAT_Widget* AWidget) {
    MTimerWidgets.append(AWidget);
  }

  //----------

  virtual void unregisterTimerWidget(SAT_Widget* AWidget) {
    MTimerWidgets.remove(AWidget);
  }
  
//------------------------------
public: // scale
//------------------------------

  virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;

    MScale = recalcScale(getWidth(),getHeight());
//    MScale = recalcScale(AWidth,AHeight);

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
    MMouseCaptureWidget = AWidget;
  }

  //----------

  virtual void releaseMouse() {
    MMouseCaptureWidget = nullptr;
  }

//------------------------------
public: // keyboard
//------------------------------

  virtual void captureKeys(SAT_Widget* AWidget) {
    MKeyCaptureWidget = AWidget;
  }

  //----------

  virtual void releaseKeys() {
    MKeyCaptureWidget = nullptr;
  }

//------------------------------
private:
//------------------------------

  // called from: on_window_paint

  SAT_Rect calcClipRect(SAT_Widget* AWidget) {
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

  // called from: on_window_mouseMove, on_window_mouseEnter

  void updateHoverWidget(int32_t AXpos, int32_t AYpos, SAT_Widget* ARoot=nullptr) {
    if (ARoot == nullptr) ARoot = MRootWidget;
    if (ARoot) {
      SAT_Widget* hover = ARoot->findChildWidget(AXpos,AYpos);
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

//------------------------------
public: // window
//------------------------------

  SAT_Painter* getPainter() override {
    return MPainter;
  }

  //----------

  void on_window_open() override {
    startTimer(SAT_WINDOW_TIMER_MS);
    if (MRootWidget) {
      MRootWidget->ownerWindowOpened(this);
      MRootWidget->setSkin(&MDefaultSkin,true,true);
      uint32_t w = getWidth();// * MScale;
      uint32_t h = getHeight();// * MScale;
      MRootWidget->setSize(SAT_Point(w,h));
      MRootWidget->realignChildWidgets();
      markRootWidgetDirty();
    }

  }

  //----------

  void on_window_close() override {
    if (MRootWidget) {
      MRootWidget->ownerWindowClosed(this);
    }
    stopTimer();
  }
  
  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //SAT_Print("AXpos %i AYpos %i\n",AXpos,AYpos);
  }
  
  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    MWidth = AWidth;
    MHeight = AHeight;
    MScale = recalcScale(AWidth,AHeight);
    MScale = recalcScale(AWidth,AHeight);
    if (MRootWidget) {
      MRootWidget->setSize(SAT_Point(AWidth,AHeight));
      MRootWidget->realignChildWidgets();
      markRootWidgetDirty();
    }
    else {
      SAT_Print("no root widget\n");
    }
  }

  //----------

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
  
  //----------

  // move widgets from dirty queues to paint queue
  // we can safely write to MPaintWidgets during timer handling (?)

  void on_window_timer(SAT_Timer* ATimer, double AElapsed) override {

    if (!ATimer->isRunning()) return;

    if (MListener) MListener->on_windowListener_timer(ATimer,AElapsed);
    for (uint32_t i=0; i<MTimerWidgets.size(); i++) MTimerWidgets[i]->on_widget_timer(ATimer,AElapsed);

//    MTweenManager.process(AElapsed);

    // don't add new widgets to paintqueue, if we're still painting the previous batch
    // timer runs in separate thread than gui/painting
    if (MIsPainting) return;

    // we draw everything in the paint queue
    // to keep the buffer always updated..

    SAT_Rect rect = SAT_Rect(0,0,0,0);
    SAT_Widget* widget;

    while (MDirtyListenerWidgets.read(&widget)) {
      //SAT_Print("dirty from listener: %s\n",widget->getName());
      MPaintWidgets.write(widget);
      rect.combine(widget->getRect());
    }
    while (MDirtyHostWidgets.read(&widget)) {
      //SAT_Print("dirty from host: %s\n",widget->getName());
      MPaintWidgets.write(widget);
      rect.combine(widget->getRect());
    }
    while (MDirtyGuiWidgets.read(&widget)) {
      //SAT_Print("dirty from gui: %s\n",widget->getName());
      MPaintWidgets.write(widget);
      rect.combine(widget->getRect());
    }

    // if any widgets were drawn, we invalidate the area
    // later, we will receive EXPOSE/WM_PAINT event (in the gui-thread)
    // where we do the painting, and copy update-rect to screen

    if (rect.isNotEmpty()) {
      //SAT_Print("invalidating %.f,%.f - %.f,%.f\n",rect.x,rect.y,rect.w,rect.h);
      invalidate(rect.x,rect.y,rect.w,rect.h);
    }
    else {
      //SAT_Print("empty rect - no invalidation\n");
    }    
  }

  //----------

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MHoverWidget) {
      if (!MMouseCaptureWidget) {
        // clicked first button
        MMouseCaptureWidget  = MHoverWidget;
        MMouseClickedXpos     = AXpos;
        MMouseClickedYpos     = AYpos;
        MMouseClickedButton   = AButton;
        MMouseClickedState    = AState;
        MMouseClickedTime     = ATime;
        MHoverWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATime);
      }
      //else {} // clicked another button
    }
    else {
      // not hovering over a widget..      
      if (MModalWidget) {
        if ((AButton == SAT_BUTTON_LEFT) || (AButton == SAT_BUTTON_RIGHT)) {
          //MModalWidget->close();
          MModalWidget->do_widget_close(MModalWidget);
        }
      }
    }
  }
  
  //----------

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MMouseCaptureWidget) {
      if (AButton == MMouseClickedButton) {
        // released first button
        MMouseCaptureWidget->on_widget_mouseRelease(AXpos,AYpos,AButton,AState,ATime);
        MMouseCaptureWidget = nullptr;
        //updateHoverWidget(AXpos,AYpos);
      }
      //else {} // released later button
    }
    //else {} // we should never end up here?
  }
  
  //----------

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    MMouseCurrentXpos = AXpos;
    MMouseCurrentYpos = AYpos;
    int32_t x = AXpos;
    int32_t y = AYpos;
    if (MMouseIsLocked) {
      updateLockedMouse(AXpos,AYpos);
      x = MLockedCurrentX;
      y = MLockedCurrentY;
    }
    if (MModalWidget) {
      updateHoverWidget(AXpos,AYpos,MModalWidget);
      MModalWidget->on_widget_mouseMove(x,y,AState,ATime);
    }
    else if (MMouseCaptureWidget) {
      //updateHoverWidget(AXpos,AYpos);
      MMouseCaptureWidget->on_widget_mouseMove(x,y,AState,ATime);
    }
    else {
      updateHoverWidget(AXpos,AYpos);
      if (MHoverWidget) MHoverWidget->on_widget_mouseMove(x,y,AState,ATime);
    }
    MMousePreviousXpos = AXpos;
    MMousePreviousYpos = AYpos;
  }
  
  //----------

  void on_window_keyPress(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
    if (MKeyCaptureWidget) {
      SAT_Print("%s\n",MKeyCaptureWidget->getName());
      MKeyCaptureWidget->on_widget_keyPress(AChar,AKeySym,AState,ATime);
    }
  }
  
  //----------

  void on_window_keyRelease(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
    if (MKeyCaptureWidget) {
      SAT_Print("%s\n",MKeyCaptureWidget->getName());
      MKeyCaptureWidget->on_widget_keyRelease(AChar,AKeySym,AState,ATime);
    }
  }
  
  //----------

  void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_Print("\n");
    //if (MRootWidget) {
    //  MRootWidget->on_widget_enter(nullptr,AXpos,AYpos);
    //}
    MHoverWidget = nullptr;
    updateHoverWidget(AXpos,AYpos);
  }
  
  //----------

  void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
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

  void on_window_clientMessage(uint32_t AData) override {
    //SAT_Print("AData %i\n",AData);
  }

//------------------------------
public: // SAT_PainterOwner
//------------------------------

//------------------------------
public: // widget owner
//------------------------------

  SAT_Window* on_widgetOwner_getWindow() override {
    return this;
  }

  double on_widgetOwner_getWidth() override {
    //return getWidth();
    return MWidth;
  }

  double on_widgetOwner_getHeight() override {
    //return getHeight();
    return MHeight;
  }

  double on_widgetOwner_getScale() override {
    //SAT_Print("MScale %f\n",MScale);
    return MScale;
  }

//------------------------------
public: // widget listener
//------------------------------

  void on_widgetListener_update(SAT_Widget* AWidget, uint32_t AMode=0) override {
    //SAT_Print("%s\n",AWidget->getName());
    if (MListener) MListener->on_windowListener_update(AWidget,AMode);
  }

  //----------

  void on_widgetListener_redraw(SAT_Widget* AWidget) override {
    //SAT_Print("%s\n",AWidget->getName());
    markWidgetDirtyFromGui(AWidget);
  }

  //----------

  void on_widgetListener_realign(SAT_Widget* AWidget) override {
    //SAT_Print("%s\n",AWidget->getName());
    SAT_Widget* parent = AWidget->getParent();
    if (parent) {
      parent->realignChildWidgets();

      markWidgetDirtyFromGui(parent);
      
    }
  }

  //----------

  void on_widgetListener_setCursor(SAT_Widget* AWidget, int32_t ACursor) override {
    //SAT_Print("%s\n",AWidget->getName());
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

  //----------

  void on_widgetListener_setHint(SAT_Widget* AWidget, const char* AHint) override {
    if (AHint[0]) {
      //SAT_Print("hint: %s\n",AHint);
    }
  }

  //----------

  void on_widgetListener_setModal(SAT_Widget* AWidget) override {
    MModalWidget = AWidget;
  }

  //----------

  void on_widgetListener_captureKeys(SAT_Widget* AWidget) override {
    MKeyCaptureWidget = AWidget;
  }

  //----------

  void on_widgetListener_close(SAT_Widget* AWidget) override {
  }

  //----------

  void on_widgetListener_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1) override{
  }

  //----------

  //void on_widgetListener_resized(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
  //}

//------------------------------
public: // timer listener
//------------------------------

  void on_timerListener_callback(SAT_Timer* ATimer) override {
    if (!ATimer->isRunning()) return;
    double now = SAT_GetTime();
    double elapsed = now - MPrevTime;
    MPrevTime = now;
    on_window_timer(ATimer,elapsed);
  }

};

//----------------------------------------------------------------------
#endif