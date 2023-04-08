#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/system/sat_timer.h"
#include "gui/sat_widget.h"
#include "gui/sat_window_listener.h"


#define SAT_WINDOW_BUFFERED

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

  double MPrevTime = 0.0;

//------------------------------
private:
//------------------------------

  SAT_WindowListener* MListener               = nullptr;
  SAT_Painter*        MWindowPainter          = nullptr;
  SAT_PaintContext    MPaintContext           = {};
  SAT_OpenGL*         MOpenGL                 = nullptr;
  void*               MRenderBuffer           = nullptr;
  SAT_Timer*          MTimer                  = nullptr;
  SAT_WidgetArray     MTimerWidgets           = {};
  
  void*               MBuffer                 = nullptr;
  uint32_t            MBufferWidth            = 0;
  uint32_t            MBufferHeight           = 0;

  uint32_t            MWidth                  = 0;
  uint32_t            MHeight                 = 0;
  uint32_t            MInitialWidth           = 0;
  uint32_t            MInitialHeight          = 0;
  double              MScale                  = 1.0;
  //bool              MAutoScaleWidgets       = true;
  
  SAT_WidgetQueue     MPendingDirtyWidgets    = {};
  SAT_WidgetQueue     MPaintDirtyWidgets      = {};

  SAT_Widget*         MRootWidget             = nullptr;
  SAT_Widget*         MHoverWidget            = nullptr;
  SAT_Widget*         MMouseCapturedWidget    = nullptr;
  SAT_Widget*         MModalWidget            = nullptr;
  SAT_Widget*         MKeyCapturedWidget      = nullptr;
  
  //uint32_t          MMouseState             = 0;
  
  int32_t             MMouseCurrentXpos       = 0;
  int32_t             MMouseCurrentYpos       = 0;
  int32_t             MMousePreviousXpos      = 0;
  int32_t             MMousePreviousYpos      = 0;
  int32_t             MMouseCurrentCursor     = 0;

  int32_t             MMouseClickedXpos       = 0;
  int32_t             MMouseClickedYpos       = 0;
  uint32_t            MMouseClickedButton     = 0;
  uint32_t            MMouseClickedState      = 0;
  uint32_t            MMouseClickedTime       = 0;
  
  bool                MMouseIsLocked          = false;
  int32_t             MLockedClickedX         = 0;
  int32_t             MLockedClickedY         = 0;
  int32_t             MLockedCurrentX         = 0;
  int32_t             MLockedCurrentY         = 0;

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
    #ifdef SAT_WINDOW_BUFFERED
      // buffer  
      uint32_t width2 = SAT_NextPowerOfTwo(AWidth);
      uint32_t height2 = SAT_NextPowerOfTwo(AHeight);
      //SAT_Print("creating FBO. %i,%i pow2: %i,%i\n",AWidth,AHeight, width2,height2);
      MRenderBuffer = MWindowPainter->createRenderBuffer(width2,height2);
      SAT_Assert(MRenderBuffer);
      MBufferWidth = width2;
      MBufferHeight = height2;
    #endif
    MOpenGL->resetCurrent();
    // timer
    MTimer = new SAT_Timer(this);
    //SAT_Assert(MTimer);
  }
  
  //----------

  virtual ~SAT_Window() {
    //delete MTimer;
    //delete MWindowPainter;
    //delete MOpenGL;
    if (MTimer) {
      if (MTimer->isRunning()) MTimer->stop();
      delete MTimer;
    }
    if (MRootWidget) {
      MRootWidget->cleanup(MWindowPainter);
      delete MRootWidget;
    }
    MWindowPainter->deleteRenderBuffer(MRenderBuffer);
    delete MWindowPainter;
    delete MOpenGL;
    
  }

//------------------------------
public:
//------------------------------

  void show() override {
    SAT_PRINT;
    SAT_ImplementedWindow::show();
    //if (MRootWidget) {
    //  MRootWidget->realignChildWidgets();
    //}
    MTimer->start(SAT_WINDOW_TIMER_MS,false);
  }
  
  //----------
  
  void hide() override {
    SAT_PRINT;
    MTimer->stop();
    SAT_ImplementedWindow::hide();
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendRootWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    //SAT_Print("%s\n",AWidget->getName());
    MRootWidget = AWidget;
    MRootWidget->setParentWidget(nullptr);
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
    uint32_t width = MWindowWidth;
    uint32_t height = MWindowHeight;
    AWidget->setSize(width,height);
    return AWidget;
  }
  
  //----------
  
  virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MScale = recalcScale(MWindowWidth,MWindowHeight);
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
  
  //----------

  virtual void copyBuffer(void* ADst, uint32_t ADstXpos, uint32_t ADstYpos, uint32_t ADstWidth, uint32_t ADstHeight, void* ASrc, uint32_t ASrcXpos, uint32_t ASrcYpos, uint32_t ASrcWidth, uint32_t ASrcHeight) {
    MWindowPainter->selectRenderBuffer(ADst,ADstWidth,ADstHeight);
    MWindowPainter->beginFrame(ADstWidth,ADstHeight);
    int32_t image = MWindowPainter->getImageFromRenderBuffer(ASrc);
    MWindowPainter->setFillImage(image, ADstXpos,ADstYpos, 1,1, 1.0, 0.0);
    MWindowPainter->fillRect(ASrcXpos,ASrcYpos,ASrcWidth,ASrcHeight);
    MWindowPainter->endFrame();
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
public: // buffer
//------------------------------

  #ifdef SAT_WINDOW_BUFFERED

  //virtual void createBuffer(uint32_t AWidth, uint32_t AHeight) {
  //}
  
  //----------

  //virtual void deleteBuffer() {
  //}

  //----------

  //virtual bool resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
  //  return false;
  //}

  //----------

  //virtual bool checkBuffer(uint32_t AWidth, uint32_t AHeight) {
  //  //uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
  //  //uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
  //  //if ((w2 != MAllocatedBufferWidth) || (h2 != MAllocatedBufferHeight)) {
  //  //  resizeBuffer(w2,h2);
  //  //  return true;
  //  //}
  //  return false;
  //}

  #endif // buffered

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
  
  
  
/*
 if (MMouseLockedWidget) { // todo: also if mouse_clicked?
      if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
        MMousePreviousX = AXpos;
        MMousePreviousY = AYpos;
        return;
      }
      if (MCapturedWidget) {
        int32_t deltax = AXpos - MMouseClickedX;
        int32_t deltay = AYpos - MMouseClickedY;
        MMouseDragX += deltax;
        MMouseDragY += deltay;
        MCapturedWidget->on_widget_mouse_move(MMouseDragX,MMouseDragY,AState,ATime);
      }
      setMouseCursorPos(MMouseClickedX,MMouseClickedY);
    }
    else {
*/  
  
  
  
  
  

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
    //SAT_Print("%i,%i current %i,%i\n",AXpos,AYpos,MLockedCurrentX,MLockedCurrentY);
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

  //virtual bool queueDirtyWidgetFromGui(SAT_Widget* AWidget) {
  //  MDirtyWidgetsQueue.write(AWidget);
  //  return true;
  //}

  //----------
  
  // flushed by timer
  
  //virtual bool flushDirtyWidgetsFromGui() {
  //  SAT_Widget* widget;
  //  while (MDirtyWidgetsQueue.read(&widget)) {
  //    // check last painted, etc..
  //    MPaintWidgetsQueue.write(widget);
  //  }
  //  return false;
  //}

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

  //virtual bool queuePaintWidget(SAT_Widget* AWidget) {
  //  MPaintWidgetsQueue.write(AWidget);
  //  return false;
  //}

  //----------
  
  // finally flushed by EXPOSE, WM_PAINT
  // all painted at once
  // then updaterect only is copied to screen

  //virtual bool flushPaintWidgets() {
  //  SAT_PRINT;
  //  return false;
  //}

//------------------------------
public: // painting
//------------------------------

  //void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {//override {
  //  //SAT_ImplementedWindow::beginPaint();
  //  MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
  //}

  //----------

  //void endPaint() {//override {
  //  MWindowPainter->resetClip();
  //  //SAT_ImplementedWindow::endPaint();
  //}

//------------------------------
public: // window
//------------------------------

  virtual void on_window_open() {
    SAT_PRINT;
    if (MRootWidget) {
      MRootWidget->prepare(this);
      MRootWidget->realignChildWidgets();
      //MRootWidget->markDirty();
      MPendingDirtyWidgets.write(MRootWidget);
    }
  }

  //----------

  virtual void on_window_close() {
    SAT_PRINT;
  }

  //----------

  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {
    //SAT_PRINT;
  }

  //----------
  
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {

    MWidth = AWidth;
    MHeight = AHeight;
    MScale = recalcScale(AWidth,AHeight);

    // or should we check in paint()?
    #ifdef SAT_WINDOW_BUFFERED
      //bool resized = checkBuffer(AWidth,AHeight);
      //if (resized) {}
    #endif
    
    if (MRootWidget) {

      //if (MAutoScaleWidgets)
        MRootWidget->scaleWidget(MScale);
      MRootWidget->setSize(AWidth,AHeight);
      MRootWidget->realignChildWidgets();

      MPendingDirtyWidgets.write(MRootWidget);
      
    }
  }
  
  //----------
  
  /*
  void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MHoverWidget == nullptr) {
      // if hover = null (outside of window/modal-widget), and we click,
      // we send close notification
      if (MModalWidget) {
        //SAT_Print("modal != null\n");
        MModalWidget->on_widget_notify(SAT_WIDGET_NOTIFY_CLOSE);
      }
    }
    // hover widget is not null.. we are hovering!..
    // captured = null means no widget is currently clicked/dragged
    else if (!MCapturedWidget) {
      // (we're only here if hover widget != null)
      // remember things
      MMouseClickedX = AXpos;
      MMouseClickedY = AYpos;
      MMouseClickedB = AButton;
      MMouseDragX = AXpos;
      MMouseDragY = AYpos;
      // if we click _outside_ modal widget, we send close notification
      if (MModalWidget) {
        if (!MModalWidget->getRect().contains(AXpos,AYpos)) {
          MModalWidget->on_widget_notify(SAT_WIDGET_NOTIFY_CLOSE);
        }
      } // modal
      // we clock on a widget..
      if (MHoverWidget) {
        //SAT_Print("click\n");
        // right click brings up context/popup menu
        uint32_t num_popup = MHoverWidget->getNumPopupMenuItems();
        if ((AButton == SAT_BUTTON_RIGHT) && (num_popup > 0)) {
          for (uint32_t i=0; i<num_popup; i++) {
            const char* item = MHoverWidget->getPopupMenuItem(i);
            if (item) {
              //TODO
              //SAT_Print("%i. %s\n",i,item);
            }
          }
        }
        else {
          if ((ATime - MPrevButtonTime) < SAT_WINDOW_DBLCLICK_MS) {
            // less than x ms since last click means double_click
            // todo: check if same button, and over same widget..
            SAT_Print("dblclick\n");
            MCapturedWidget = MHoverWidget;
            MHoverWidget->on_widget_mouse_dblclick(AXpos,AYpos,AButton,AState,ATime);
          }
          else {
            // or else.. normal mouse click
            // we capture the widget, so it will receive related mouse release events
            //MMouseLockedWidget = MHoverWidget;
            MCapturedWidget = MHoverWidget;
            MHoverWidget->on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
          }
        } // right
      } // hover
    } // !captured
    // remember things (mainly for double clicking)
    MPrevButton = AButton;
    MPrevButtonTime = ATime;
  }  
  */

  virtual void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
    //bool dblclick = checkDoubleClick(ATimEsStamp);
    //bool scroll = checkScrollWheel(AButton);
    if (MHoverWidget) {
      if (!MMouseCapturedWidget) {
        // clicked first button
        MMouseCapturedWidget  = MHoverWidget;
        MMouseClickedXpos     = AXpos;
        MMouseClickedYpos     = AYpos;
        MMouseClickedButton   = AButton;
        MMouseClickedState    = AState;
        MMouseClickedTime     = ATimestamp;
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
  
  /*
  void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    // if we captured a widget, notify it we released the button, and find new hovering widget
    if (MCapturedWidget) {
      if (MMouseClickedB == AButton) {
        //AT_Print("release\n");
        //MMouseLockedWidget = nullptr;
        MCapturedWidget->on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATime);
        MCapturedWidget = nullptr;
        updateHoverWidget(AXpos,AYpos);
      }
    }
  }  
  */

  virtual void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
    if (MMouseCapturedWidget) {
      if (AButton == MMouseClickedButton) {
        // released first button
        MMouseCapturedWidget->on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATimestamp);
        MMouseCapturedWidget = nullptr;
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
  
  /*
  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    // if mouse is locked, we do the 'regular mouse things' with 'adjusted' (dragging) coords
    // and then reset the cursor back to where it was..
    if (MMouseLockedWidget) { // todo: also if mouse_clicked?
      if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
        MMousePreviousX = AXpos;
        MMousePreviousY = AYpos;
        return;
      }
      if (MCapturedWidget) {
        int32_t deltax = AXpos - MMouseClickedX;
        int32_t deltay = AYpos - MMouseClickedY;
        MMouseDragX += deltax;
        MMouseDragY += deltay;
        MCapturedWidget->on_widget_mouse_move(MMouseDragX,MMouseDragY,AState,ATime);
      }
      setMouseCursorPos(MMouseClickedX,MMouseClickedY);
    }
    else {
      // if modal widget, we check only from that and downwards..
      if (MModalWidget) {
        updateHoverWidget(AXpos,AYpos,MModalWidget); // only from modal widget and downwards?
        MModalWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
      }
      // if captured widget, we call it directly
      else if (MCapturedWidget) {
        MCapturedWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
      }
      // or we have to find what widget we are hovering over..
      else {
        updateHoverWidget(AXpos,AYpos);
        if (MHoverWidget) {
          //if (MHoverWidget->wantHoverEvents()) {
          MHoverWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
          //}
        }
      }
    }
    MMousePreviousX = AXpos;
    MMousePreviousY = AYpos;
  }  
  */

  virtual void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimestamp) {
    MMouseCurrentXpos = AXpos;
    MMouseCurrentYpos = AYpos;
    SAT_Widget* hover = findHoverWidget(AXpos,AYpos);
    int32_t x = AXpos;
    int32_t y = AYpos;
    if (MMouseIsLocked) {
      updateLockedMouse(AXpos,AYpos);
      x = MLockedCurrentX;
      y = MLockedCurrentY;
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
    MMousePreviousXpos = AXpos;
    MMousePreviousYpos = AYpos;
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
  
  /*
  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MPaintContext.painter       = MWindowPainter;
    MPaintContext.update_rect.x = AXpos;
    MPaintContext.update_rect.y = AYpos;
    MPaintContext.update_rect.w = AWidth;
    MPaintContext.update_rect.h = AHeight;
    MPaintContext.window_width  = MWindowWidth;
    MPaintContext.window_height = MWindowHeight;
    MPaintContext.window_scale  = MScale;
    MOpenGL->makeCurrent();
    uint32_t width2  = SAT_NextPowerOfTwo(MWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(MHeight);
    if ((width2 != MBufferWidth) || (height2 != MBufferHeight)) {
      void* buffer = MWindowPainter->createRenderBuffer(width2,height2);
      SAT_Assert(buffer);
      copyBuffer(buffer,0,0,width2,height2,MRenderBuffer,0,0,MBufferWidth,MBufferHeight);
      MWindowPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = width2;
      MBufferHeight = height2;
      // paint root
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
      paintDirtyWidgets(&MPaintContext,MRootWidget);
      MWindowPainter->endFrame();
    }
    else {
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
      // paint dirty widgets
      paintDirtyWidgets(&MPaintContext);
      MWindowPainter->endFrame();
    }
    copyBuffer(nullptr,0,0,MWindowWidth,MWindowHeight,MRenderBuffer,AXpos,AYpos,AWidth,AHeight);
    MOpenGL->swapBuffers();
    MOpenGL->resetCurrent();
    MPaintContext.counter += 1;
  }
  */
  
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    MPaintContext.painter = MWindowPainter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    MOpenGL->makeCurrent();
    
    uint32_t width2  = SAT_NextPowerOfTwo(MWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(MHeight);
    if ((width2 != MBufferWidth) || (height2 != MBufferHeight)) {
      void* buffer = MWindowPainter->createRenderBuffer(width2,height2);
      SAT_Assert(buffer);
      copyBuffer(buffer,0,0,width2,height2,MRenderBuffer,0,0,MBufferWidth,MBufferHeight);
      MWindowPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = width2;
      MBufferHeight = height2;
      // paint dirty widgets to render buffer
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
      SAT_Widget* widget;
      while (MPaintDirtyWidgets.read(&widget)) {} // widget->on_widget_paint(&MPaintContext);
      MRootWidget->on_widget_paint(&MPaintContext);
      MWindowPainter->endFrame();
    }
    else {
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
      SAT_Widget* widget;
      while (MPaintDirtyWidgets.read(&widget)) widget->on_widget_paint(&MPaintContext);
      MWindowPainter->endFrame();
    }
    copyBuffer(nullptr,0,0,MWindowWidth,MWindowHeight,MRenderBuffer,AXpos,AYpos,AWidth,AHeight);
    MOpenGL->swapBuffers();
    MOpenGL->resetCurrent();
    MPaintContext.counter += 1;
  }
  
  //----------
  
  virtual void on_window_timer(double AElapsed) {
    if (MListener) MListener->do_window_timer(this,AElapsed);

    for (uint32_t i=0; i<MTimerWidgets.size(); i++) MTimerWidgets[i]->on_widget_timer(0,AElapsed);
    //MTweens.process(elapsed,MScale);
    
    SAT_Rect rect;
    SAT_Widget* widget;
    while (MPendingDirtyWidgets.read(&widget)) {
      //SAT_Print("%s\n",widget->getName());
      rect.combine(widget->getRect());
      MPaintDirtyWidgets.write(widget);
    }
    if (rect.isNotEmpty()) invalidate(rect.x,rect.y,rect.w,rect.h);
    
  }

//------------------------------
public: // widget owner
//------------------------------

  double do_widget_get_scale() override {
    return MScale;
  }

  //----------

  //double do_widgetr_getWidth() override {
  //  return MWidth;
  //}

  //----------

  //double do_widget_getHeight() override {
  //  return MHeight;
  //} 

  //----------

  SAT_Painter* do_widget_get_painter() override {
    return MWindowPainter;
  } 

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_window_update_widget(ASender,AMode,AIndex);
  }

  //----------
  
  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    SAT_Assert(ASender);
    //if (!ASender) ASender = MRootWidget;
    MPendingDirtyWidgets.write(ASender);
    if (MListener) MListener->do_window_redraw_widget(ASender,AMode,AIndex);
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
    switch(ACursor) {
      case SAT_CURSOR_LOCK:
        //MMouseLockedWidget = ASender;
        lockMouseCursor();
        break;
      case SAT_CURSOR_UNLOCK:
        //MMouseLockedWidget = nullptr;
        unlockMouseCursor();
        break;
      case SAT_CURSOR_SHOW:  
        showMouseCursor();
        setMouseCursor(MMouseCurrentCursor);
        break;
      case SAT_CURSOR_HIDE:
        hideMouseCursor();
        break;
      default:
        if (ACursor != MMouseCurrentCursor) {
          setMouseCursor(ACursor);
          MMouseCurrentCursor = ACursor;
        }
    }
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
    double now = SAT_GetTime();
    double elapsed = now - MPrevTime;
    MPrevTime = now;
    on_window_timer(elapsed);
  }

};

//----------------------------------------------------------------------
#endif

