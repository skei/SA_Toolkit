#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/system/sat_timer.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WindowListener {
public:
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;
#endif

#ifdef SAT_WIN32
  #include "gui/win32/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_ImplementedWindow
, public SAT_WidgetOwner
, public SAT_WidgetListener
, public SAT_TimerListener {

//------------------------------
private:
//------------------------------

  SAT_WindowListener* MListener               = nullptr;
  SAT_Painter*        MWindowPainter          = nullptr;
  SAT_PaintContext    MPaintContext           = {};
  SAT_OpenGL*         MOpenGL                 = nullptr;
  void*               MRenderBuffer           = nullptr;
  SAT_Timer*          MTimer                  = nullptr;
  
  void*               MBuffer                 = nullptr;
  uint32_t            MBufferWidth            = 0;
  uint32_t            MBufferHeight           = 0;
  uint32_t            MAllocatedBufferWidth   = 0;
  uint32_t            MAllocatedBufferHeight  = 0;

  uint32_t            MWidth                  = 0;
  uint32_t            MHeight                 = 0;
  uint32_t            MInitialWidth           = 0;
  uint32_t            MInitialHeight          = 0;
  double              MScale                  = 1.0;

  SAT_WidgetQueue     MDirtyWidgetsQueue      = {};
  SAT_WidgetQueue     MPaintWidgetsQueue      = {};

  SAT_Widget*         MRootWidget             = nullptr;
  SAT_Widget*         MHoverWidget            = nullptr;
  SAT_Widget*         MMouseCapturedWidget    = nullptr;
  SAT_Widget*         MModalWidget            = nullptr;
  SAT_Widget*         MKeyCapturedWidget      = nullptr;
  
  //uint32_t          MMouseState             = 0;
  int32_t             MMouseClickedXpos       = 0;
  int32_t             MMouseClickedYpos       = 0;
  uint32_t            MMouseClickedButton     = 0;
  uint32_t            MMouseClickedState      = 0;
  uint32_t            MMouseClickedTime       = 0;
  
  bool                MMouseIsLocked          = false;
  int32_t             MLockedClickedX         = 0;
  int32_t             MLockedClickedY         = 0;
  int32_t             MLockedXpos             = 0;
  int32_t             MLockedYpos             = 0;

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, SAT_WindowListener* AListener)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    
    MWidth          = AWidth;
    MHeight         = AHeight;
    MInitialWidth   = AWidth;
    MInitialHeight  = AHeight;
    MListener       = AListener;
    
    // opengl
    
    #ifdef SAT_LINUX
      Display* display = getX11Display();
      xcb_window_t window = getX11Window();
      MOpenGL = new SAT_OpenGL(display,window);
    #endif
    
    #ifdef SAT_WIN32
      HWND window = getWin32Window();
      MOpenGL = new SAT_OpenGL(window);
    #endif
    
    MOpenGL->makeCurrent();

    // painter
    MWindowPainter = new SAT_Painter(MOpenGL);
    SAT_Assert(MWindowPainter);
      
    // buffer  
    uint32_t width2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(AHeight);
    //SAT_Print("creating FBO. %i,%i pow2: %i,%i\n",AWidth,AHeight, width2,height2);
    MRenderBuffer = MWindowPainter->createRenderBuffer(width2,height2);
    SAT_Assert(MRenderBuffer);
    MBufferWidth = width2;
    MBufferHeight = height2;
    
    //
    MOpenGL->resetCurrent();
    MTimer = new SAT_Timer(this);
    //SAT_Assert(MTimer);
  }
  
  //----------

  virtual ~SAT_Window() {
    delete MWindowPainter;
    delete MOpenGL;
  }

//------------------------------
public: // buffer
//------------------------------

  virtual void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
    MAllocatedBufferWidth = SAT_NextPowerOfTwo(AWidth);
    MAllocatedBufferHeight = SAT_NextPowerOfTwo(AHeight);
  }
  
  //----------

  virtual void deleteBuffer() {
    MBufferWidth = 0;
    MBufferHeight = 0;
    MAllocatedBufferWidth = 0;
    MAllocatedBufferHeight = 0;
  }

  //----------

  virtual bool resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    // create new buffer
    // copy old to new buffer
    // delete old buffer
    // set new buffer
    return false;
  }

  //----------

  virtual bool checkBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
    if ((w2 != MAllocatedBufferWidth) || (h2 != MAllocatedBufferHeight)) {
      resizeBuffer(w2,h2);
      return true;
    }
    return false;
  }

  //----------
  
  virtual bool activateBuffer() {
    return false;
  }

  //----------

  virtual bool deactivateBuffer() {
    return false;
  }

//------------------------------
public:
//------------------------------

  void appendRootWidget(SAT_Widget* AWidget) {
    MRootWidget = AWidget;
  }


//------------------------------
public: // capture
//------------------------------

  virtual void captureKeys(SAT_Widget* AWidget) {
    MKeyCapturedWidget = AWidget;
  }

  //----------

  virtual void releaseKeys() {
    MKeyCapturedWidget = nullptr;
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
public: // modal
//------------------------------

  virtual void beginModal(SAT_Widget* AWidget) {
    MModalWidget = AWidget;
  }

  //----------

  virtual void endModal() {
    MModalWidget = nullptr;
  }

//------------------------------
public: // cursor
//------------------------------

  virtual void lockMouseCursor(int32_t AXpos, int32_t AYpos) {
    MMouseIsLocked = true;
    MLockedClickedX = AXpos;
    MLockedClickedY = AYpos;
    MLockedXpos = AXpos;
    MLockedYpos = AYpos;
  }

  //----------

  virtual void unlockMouseCursor() {
    MMouseIsLocked = false;
  }

  //----------

  virtual void updateLockedMouse(int32_t AXpos, int32_t AYpos) {
    int32_t xdiff = AXpos - MLockedClickedX;
    int32_t ydiff = AYpos - MLockedClickedY;
    MLockedXpos += xdiff;
    MLockedYpos += ydiff;
    setMouseCursorPos(MLockedClickedX,MLockedClickedY);
  }

//------------------------------
public: // hover
//------------------------------

  virtual SAT_Widget* findHoverWidget(int32_t AXpos, int32_t AYpos, SAT_Widget* AWidget=nullptr) {
    SAT_Widget* hover = nullptr;
    if (AWidget) {
      hover = AWidget->findChildWidget(AXpos,AYpos);
    }
    else {
      if (MRootWidget) hover = MRootWidget->findChildWidget(AXpos,AYpos);
    }
    return hover;
  }

  //----------

  virtual void updateHoverState(SAT_Widget* AHover, int32_t AXpos, int32_t AYpos, uint32_t AMode=0) {
    if (AHover != MHoverWidget) {
      if (MHoverWidget) MHoverWidget->on_widget_mouse_leave(AHover,AXpos,AYpos,0);
      if (AHover) {
        AHover->on_widget_mouse_enter(MHoverWidget,AXpos,AYpos,0);
        if (AHover->autoHoverCursor()) do_widget_set_cursor(AHover,AHover->getMouseCursor());
        if (AHover->autoHoverHint()) do_widget_set_hint(AHover,0,AHover->getHint());
      }
    }
    MHoverWidget = AHover;
  }

//------------------------------
public: // queues
//------------------------------

  // gui interaction
  // do_widget_redraw, on_window_resize

  virtual bool queueDirtyWidgetFromGui(SAT_Widget* AWidget) {
    MDirtyWidgetsQueue.write(AWidget);
    return true;
  }

  //----------
  
  // flushed by timer
  
  virtual bool flushDirtyWidgetsFromGui() {
    SAT_Widget* widget;
    while (MDirtyWidgetsQueue.read(&widget)) {
      // check last painted, etc..
      MPaintWidgetsQueue.write(widget);
    }
    return false;
  }

  //------------------------------
  
  // parameter changes from audio/process thread (event handling)

  //virtual bool queueDirtyWidgetFromEvents(SAT_Widget* AWidget) {
  //  return false;
  //}

  //----------
  
  // flushed by timer

  //virtual bool flushDirtyWidgetsFromEvents() {
  //  return false;
  //}

  //----------
  
  // written to by timer (after thinning/sorting)

  virtual bool queuePaintWidget(SAT_Widget* AWidget) {
    MPaintWidgetsQueue.write(AWidget);
    return false;
  }

  //----------
  
  // finally flushed by EXPOSE, WM_PAINT
  // all painted at once
  // then updaterect only is copied to screen

  virtual bool flushPaintWidgets() {
    return false;
  }

//------------------------------
public: // painting
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {//override {
    //SAT_ImplementedWindow::beginPaint();
    MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
  }

  //----------

  void endPaint() {//override {
    MWindowPainter->resetClip();
    //SAT_ImplementedWindow::endPaint();
  }

//------------------------------
public: // window
//------------------------------

  virtual void on_window_open() {
    if (MRootWidget) {
      MRootWidget->prepareWidgets(this);
      MRootWidget->realignChildWidgets();
      //MRootWidget->markDirty();
    }
  }

  //----------

  virtual void on_window_close() {
  }

  //----------

  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {
  }

  //----------

  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {
    bool resized = checkBuffer(AWidth,AHeight);
    if (resized) {}
    //MWidth = AWidth;
    //MHeight = AHeight;
    if (MRootWidget) {
      MRootWidget->on_widget_resize(AWidth,AHeight);
      MRootWidget->realignChildWidgets();
      queueDirtyWidgetFromGui(MRootWidget);
    }
  }
  
  //----------

  virtual void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
    //bool dblclick = checkDoubleClick(ATimEsStamp);
    //bool scroll = checkScrollWheel(AButton);
    if (MHoverWidget) {
      if (!MMouseCapturedWidget) {
        // clicked first button
        MMouseCapturedWidget = MHoverWidget;
        MMouseClickedXpos   = AXpos;
        MMouseClickedYpos   = AYpos;
        MMouseClickedButton = AButton;
        MMouseClickedState  = AState;
        MMouseClickedTime   = ATimestamp;
        MHoverWidget->on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATimestamp);
      }
      //else {
      //  // clicked another button
      //}
    }
    //else {
    //  // not hovering over a widget..
    //}
  }

  //----------

  virtual void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
    if (MMouseCapturedWidget) {
      if (AButton == MMouseClickedButton) {
        // released first button
        MMouseCapturedWidget = nullptr;
        MMouseCapturedWidget->on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATimestamp);
      }
      //else {
      //  // released later button
      //}
    }
    //else {
    //  SAT_Assert(0!=0) // we should never end up here?
    //}
  }

  //----------

  virtual void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimestamp) {
    SAT_Widget* hover = findHoverWidget(AXpos,AYpos);
    int32_t x = AXpos;
    int32_t y = AYpos;
    if (MMouseIsLocked) {
      updateLockedMouse(AXpos,AYpos);
      x = MLockedXpos;
      y = MLockedYpos;
    }
    if (MMouseCapturedWidget) {
      // check hover if drag'n'drop?
      //updateHoverState(hover,x,y,0);
      MMouseCapturedWidget->on_widget_mouse_move(x,y,AState,ATimestamp);
    }
    else {
      updateHoverState(hover,x,y,0);
      if (hover) hover->on_widget_mouse_move(x,y,AState,ATimestamp);
    }
  }

  //----------

  virtual void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, SAT_Widget* AFrom) {
    SAT_Widget* hover = findHoverWidget(AXpos,AYpos);
    updateHoverState(hover,AXpos,AYpos,0);
  }

  //----------

  virtual void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, SAT_Widget* ATo) {
    updateHoverState(nullptr,AXpos,AYpos,0);
  }

  //----------

  virtual void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATimestamp) {
    if (MKeyCapturedWidget) {
      MKeyCapturedWidget->on_widget_key_press(AKey,AState,ATimestamp);
    }
  }

  //----------

  virtual void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATimestamp) {
    if (MKeyCapturedWidget) {
      MKeyCapturedWidget->on_widget_key_release(AKey,AState,ATimestamp);
    }
  }

  //----------
  
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    MPaintContext.painter = MWindowPainter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    beginPaint(AXpos,AYpos,AWidth,AHeight);
      flushPaintWidgets();
    endPaint();
    MPaintContext.counter += 1;
  }
  
  //----------
  
  virtual void on_window_timer() {
    //double current = getCurrentTime();
    //double delta = current - prev;
    //prev = current;
    //for (uint32_t i=0; i<MTimerWidgets; i++) {
    //  MTimerWidgets[i]->on_widget_timer(i,delta);
    //}
    
    //flushDirtyWidgetsFromAudio
    //flushDirtyWidgetsFromGui
    
  }

//------------------------------
public: // widget owner
//------------------------------

  double do_widget_owner_get_scale() {
    return MScale;
  }

  //----------

  //double do_widgetOwner_getWidth() {
  //  return MWidth;
  //}

  //----------

  //double do_widgetOwner_getHeight() {
  //  return MHeight;
  //} 

  //----------

  SAT_Painter* do_widget_owner_get_painter() {
    return MWindowPainter;
  } 

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    //if (MListener) MListener->do_windowListener_updateWidget(ASender,AMode,AIndex);
  }

  //----------

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    queueDirtyWidgetFromGui(ASender);
  }

  //----------

  void do_widget_realign(SAT_Widget* ASender) override {
  }

  //----------

  void do_widget_close(SAT_Widget* ASender) override {
  }

  //----------

  void do_widget_select(SAT_Widget* ASender, int32_t AIndex) override {
  }

  //----------

  void do_widget_set_cursor(SAT_Widget* ASender, int32_t ACursor) override {
    //switch(ACursor) {
    //  case SAT_CURSOR_HIDE:     hideMouseCursor();    break;
    //  case SAT_CURSOR_SHOW:     hideMouseCursor();    break;
    //  case SAT_CURSOR_LOCK:     lockMouseCursor();    break;
    //  case SAT_CURSOR_UNLOCK:   unlockMouseCursor();  break;
    //  case SAT_CURSOR_DEFAULT:  break;
    //  default:
    //    if (ACursor != MCurrentCursor) {
    //      setMouseCursor(ACursor);
    //      MCurrentCursor = ACursor);
    //    }
    //}
  }

  //----------

  void do_widget_set_hint(SAT_Widget* ASender, uint32_t AMode, const char* AHint) override {
  }

  //----------

  void do_widget_set_modal(SAT_Widget* ASender) override {
    if (ASender) beginModal(ASender);
    else endModal();
  }
  
  //----------

  //void do_widget_notify(SAT_Widget* ASender, uint32_t AMessage, int32_t AValue)  override {
  //  return true;
  //}

//------------------------------
public: // timer listener
//------------------------------

  void do_timer_callback(SAT_Timer* ATimer) override {
    on_window_timer();
  }

};

//----------------------------------------------------------------------
#endif

