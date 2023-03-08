#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/system/sat_timer.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"
#include "gui/sat_widget_listener.h"
#include "gui/sat_window_listener.h"


typedef SAT_LockFreeQueue<SAT_Widget*,SAT_WINDOW_MAX_DIRTY_WIDGETS> SAT_DirtyWidgetsQueue;

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

//class SAT_OpenGLWindow
//: public SAT_ImplementedWindow {
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class SAT_BufferedWindow
//: public SAT_OpenGLWindow {
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class SAT_WidgetWindow
//: public SAT_BufferedWindow {
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_ImplementedWindow
, public SAT_WidgetListener
, public SAT_TimerListener {

//------------------------------
private:
//------------------------------

  // window

  SAT_WindowListener*   MListener             = nullptr; // editor
  SAT_Widget*           MRootWidget           = nullptr;
  uint32_t              MWidth                = 0;
  uint32_t              MHeight               = 0;
  double                MInitialWidth         = 0.0;
  double                MInitialHeight        = 0.0;
  double                MScale                = 1.0;
  bool                  MAutoScaleWidgets     = true;

  // opengl

  SAT_OpenGL*           MOpenGL               = nullptr;
  SAT_Painter*          MWindowPainter        = nullptr;
  SAT_PaintContext      MPaintContext         = {};

  // buffer

  void*                 MRenderBuffer         = nullptr;
  uint32_t              MBufferWidth          = 0;
  uint32_t              MBufferHeight         = 0;
  
  // widgets

  SAT_Timer*            MTimer                = nullptr;
  SAT_DirtyWidgetsQueue MPendingDirtyWidgets  = {};
  SAT_DirtyWidgetsQueue MPaintDirtyWidgets    = {};

  SAT_Widget*           MHoverWidget          = nullptr;
  SAT_Widget*           MCapturedWidget       = nullptr;
  SAT_Widget*           MModalWidget          = nullptr;
  SAT_Widget*           MMouseLockedWidget    = nullptr;

  int32_t               MCurrentCursor        = SAT_CURSOR_DEFAULT;
  int32_t               MMouseClickedX        = 0;
  int32_t               MMouseClickedY        = 0;
  uint32_t              MMouseClickedB        = 0;
  int32_t               MMousePreviousX       = 0;
  int32_t               MMousePreviousY       = 0;
  int32_t               MMouseDragX           = 0;
  int32_t               MMouseDragY           = 0;
  

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, SAT_WindowListener* AListener)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {

    MWidth = AWidth;
    MHeight = AHeight;
    MInitialWidth = AWidth;
    MInitialHeight= AHeight;
    MListener = AListener;
    
    // linux

    #ifdef SAT_LINUX
      Display* display = getX11Display();
      xcb_window_t window = getX11Window();
      MOpenGL = new SAT_OpenGL(display,window);
    #endif
    
    // win32

    #ifdef SAT_WIN32
    #endif
    
    //opengl

    MOpenGL->makeCurrent();
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
    
    if (MTimer) {
      if (MTimer->isRunning()) { MTimer->stop(); }
      delete MTimer;
    }
    
    if (MRootWidget) MRootWidget->cleanup(MWindowPainter);
    MWindowPainter->deleteRenderBuffer(MRenderBuffer);
    delete MWindowPainter;
    delete MOpenGL;
  }

//------------------------------
public:
//------------------------------

  SAT_Widget*       getRootWidget()   { return MRootWidget; }
  SAT_PaintContext* getPaintContext() { return &MPaintContext; }
  double            getScale()        { return MScale; }
  uint32_t          getWidth()        { return MWidth; }
  uint32_t          getHeight()       { return MHeight; }

//------------------------------
public:
//------------------------------

  virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MScale = recalcScale(MWindowWidth,MWindowHeight);
  }

  //----------

  virtual double recalcScale(int32_t AWidth, int32_t AHeight) {
    double scale = 1.0;
    if ((MInitialWidth > 0) && (MInitialHeight > 0)) {
      double xscale = AWidth / MInitialWidth;
      double yscale = AHeight / MInitialHeight;
      if (xscale < yscale) scale = xscale;
      else scale =  yscale;
    }
    return scale;
  }

  //----------

  virtual void setRootWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    MRootWidget = AWidget;
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
    uint32_t width = MWindowWidth;
    uint32_t height = MWindowWidth;
    AWidget->setSize(width,height);
  }

//------------------------------
private:
//------------------------------

  // called from
  // - SAT_Window.on_window_open()
  // - SA_Editor.show()

  virtual void prepareWidgets() {
    if (MRootWidget) {
      MRootWidget->prepare(this,true);
      //SAT_Rect R = MRootWidget->getRect();
      MPendingDirtyWidgets.write(MRootWidget);
      invalidateWidget(MRootWidget);
    }
  }

  //----------

  /*
    called from:
    - SAT_Window.prepareWidgets()
  */

  virtual void invalidateWidget(SAT_Widget* AWidget) {
    SAT_Rect R = AWidget->getRect();
    invalidate(R.x,R.y,R.w,R.h);
  }

  //----------

  /*
    called from:
    - do_timer_listener_callback()
  */

  virtual uint32_t flushDirtyWidgets(SAT_Rect* ARect) {
    uint32_t count = 0;
    ARect->set(0);
    SAT_Widget* widget = nullptr;
    while (MPendingDirtyWidgets.read(&widget)) {
      //widget->on_widget_paint(&MPaintContext);
      ARect->combine(widget->getRect());
      MPaintDirtyWidgets.write(widget);
      count += 1;
    }
    return count;
  }

  //----------
  
  /*
    called from
    - on_window_paint()
  */

  virtual uint32_t paintDirtyWidgets(SAT_PaintContext* AContext, SAT_Widget* ARoot=nullptr) {
     int32_t paint_count = MPaintContext.counter;
    uint32_t count = 0;
    SAT_Widget* widget = nullptr;
    if (ARoot) {
      while (MPaintDirtyWidgets.read(&widget)) {
        // we will draw everything anyway, so just empty queue
        //widget->setLastPainted(paint_count);
      }
      ARoot->on_widget_paint(&MPaintContext);
      count = 1;
    }
    else {
      while (MPaintDirtyWidgets.read(&widget)) {
        if (widget->getLastPainted() != paint_count) {
          //SAT_Print("draw\n");
          widget->on_widget_paint(&MPaintContext);
          widget->setLastPainted(paint_count);
          count += 1;
        }
        //else {
        //  SAT_Print("skip draw\n");
        //}
      }
    }
    return count;
  }

  //----------

  virtual void updateHoverWidget(int32_t AXpos, int32_t AYpos, SAT_Widget* ARoot=nullptr) {
    SAT_Widget* hover = nullptr;

    if (ARoot) {
      hover = ARoot->findChildWidget(AXpos,AYpos,true);
    }
    else {
      if (MRootWidget) hover = MRootWidget->findChildWidget(AXpos,AYpos,true);
    }

    if (hover != MHoverWidget) {
      if (MHoverWidget) {
        //MHoverWidget->setHovering(false);
        MHoverWidget->on_widget_mouse_leave(hover,AXpos,AYpos,0);
      }
      if (hover) {
        //hover->setHovering(true);
        hover->on_widget_mouse_enter(MHoverWidget,AXpos,AYpos,0);
      }
    }

    MHoverWidget = hover;
  }

  //----------

  // when releasing mouse cursor after dragging, and entering window
  // (when we don't know the 'from' widget)

  //virtual void updateHoverWidgetFrom(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {
  //  if (MHoverWidget != AFrom) {
  //    if (AFrom) AFrom->on_widget_leave(MHoverWidget,AXpos,AYpos,ATime);
  //    if (MHoverWidget) MHoverWidget->on_widget_enter(AFrom,AXpos,AYpos,ATime);
  //  }
  //}

  //----------

  virtual void copyBuffer(void* ADst, uint32_t ADstXpos, uint32_t ADstYpos, uint32_t ADstWidth, uint32_t ADstHeight, void* ASrc, uint32_t ASrcXpos, uint32_t ASrcYpos, uint32_t ASrcWidth, uint32_t ASrcHeight) {
    MWindowPainter->selectRenderBuffer(ADst,ADstWidth,ADstHeight);
    MWindowPainter->beginFrame(ADstWidth,ADstHeight);
    int32_t image = MWindowPainter->getImageFromRenderBuffer(ASrc);
    MWindowPainter->setFillImage(image, ASrcXpos,ASrcYpos, 1,1, 1.0, 0.0);
    MWindowPainter->fillRect(ASrcXpos,ASrcYpos,ASrcWidth,ASrcHeight);
    MWindowPainter->endFrame();
  }

//------------------------------
public:
//------------------------------

  void show() override {
    SAT_ImplementedWindow::show();
    MTimer->start(SAT_WINDOW_TIMER_MS,false);
  }

  //----------

  void hide() override {
    MTimer->stop();
    SAT_ImplementedWindow::hide();
  }

//------------------------------
public: // window
//------------------------------

  /*
    called from:
    - SAT_X11Window.processEvent(), XCB_MAP_NOTIFY
    - SAT_Editor.show()
  */

  void on_window_open() override {
    //SAT_Print("\n");
    prepareWidgets();
  }

  //----------

  /*
    called from:
    - SAT_X11Window.processEvent(), XCB_UNMAP_NOTIFY
    - ~SAT_Editor()
  */

  void on_window_close() override {
    //SAT_Print("\n");
  }

  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //SAT_Print("%i,%i\n",AXpos,AYpos);
  }

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("%i,%i\n",AWidth,AHeight);
    MWidth = AWidth;
    MHeight = AHeight;
    MScale = recalcScale(AWidth,AHeight);
    if (MRootWidget) {
      if (MAutoScaleWidgets) MRootWidget->scaleWidget(MScale);
      MRootWidget->setSize(AWidth,AHeight);
      MRootWidget->realignChildWidgets();
      MPendingDirtyWidgets.write(MRootWidget);
    }
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("%i,%i,%i,%i\n",AXpos,AYpos,AWidth,AHeight);
    MPaintContext.painter       = MWindowPainter;
    MPaintContext.update_rect.x = AXpos;
    MPaintContext.update_rect.y = AYpos;
    MPaintContext.update_rect.w = AWidth;
    MPaintContext.update_rect.h = AHeight;
    MPaintContext.window_width  = MWindowWidth;
    MPaintContext.window_height = MWindowHeight;
    MPaintContext.window_scale  = MScale;

    MOpenGL->makeCurrent();
    
    //uint32_t width2  = SAT_NextPowerOfTwo(MWindowWidth);
    //uint32_t height2 = SAT_NextPowerOfTwo(MWindowHeight);
    uint32_t width2  = SAT_NextPowerOfTwo(MWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(MHeight);
    
    if ((width2 != MBufferWidth) || (height2 != MBufferHeight)) {
      //SAT_Print("creating new FBO. window: %i,%i pow2: %i,%i\n",MWindowWidth,MWindowHeight, width2,height2);
      void* buffer = MWindowPainter->createRenderBuffer(width2,height2);
      SAT_Assert(buffer);
      copyBuffer(buffer,0,0,width2,height2,MRenderBuffer,0,0,MBufferWidth,MBufferHeight);
      //SAT_Print("deleting previous FBO\n");
      MWindowPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = width2;
      MBufferHeight = height2;
      
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      paintDirtyWidgets(&MPaintContext,MRootWidget);      
      MWindowPainter->endFrame();
      
    }
    else {
      
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      paintDirtyWidgets(&MPaintContext);
      MWindowPainter->endFrame();
      
    }
    
    copyBuffer(nullptr,0,0,MWindowWidth,MWindowHeight,MRenderBuffer,AXpos,AYpos,AWidth,AHeight);
    MOpenGL->swapBuffers();
    MOpenGL->resetCurrent();
    MPaintContext.counter += 1;
  }

  //----------

  void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
  }

  //----------

  void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
  }

  //----------

  void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {

    if (MHoverWidget == nullptr) {
      //SAT_Print("hover = null\n");
      if (MModalWidget) {
        //SAT_Print("modal != null\n");
        MModalWidget->on_widget_notify(SAT_WIDGET_NOTIFY_CLOSE);
      }
    }

    else if (!MCapturedWidget) {
      MMouseClickedX = AXpos;
      MMouseClickedY = AYpos;
      MMouseClickedB = AButton;
      MMouseDragX = AXpos;
      MMouseDragY = AYpos;

      if (MModalWidget) {
        if (!MModalWidget->getRect().contains(AXpos,AYpos)) {
          MModalWidget->on_widget_notify(SAT_WIDGET_NOTIFY_CLOSE);
        }
      } // modal

      if (MHoverWidget) {
        //SAT_Print("click\n");
        //MMouseLockedWidget = MHoverWidget;
        MCapturedWidget = MHoverWidget;
        MHoverWidget->on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
      } // hover

    } // !captured
  }

  //----------

  void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //if (MMouseClicked) {
    //}
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

  //----------

  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (MMouseLockedWidget) { // todo: also if mouse_clicked?
      if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
        MMousePreviousX = AXpos;
        MMousePreviousY = AYpos;
        return;
      }
      int32_t deltax = AXpos - MMouseClickedX;
      int32_t deltay = AYpos - MMouseClickedY;
      MMouseDragX += deltax;
      MMouseDragY += deltay;
      //setCursorPos(MMouseClickedX,MMouseClickedY);
      if (MCapturedWidget) {
        MCapturedWidget->on_widget_mouse_move(MMouseDragX,MMouseDragY,AState,ATime);
      }
      setCursorPos(MMouseClickedX,MMouseClickedY);
    }
    else {

      if (MModalWidget) {
        updateHoverWidget(AXpos,AYpos,MModalWidget); // only from modal widget and downwards?
        MModalWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
      }

      else if (MCapturedWidget) {
        MCapturedWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
      }

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

  /*
  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (MMouseEventRecipient) {
      MMouseEventRecipient->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
    }
  }
  */

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  }

  //----------

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  }

  //----------

  void on_window_client_message(uint32_t AData) override {
    SAT_Print("data %i\n",AData);
  }

  //----------

  //void on_window_timer() override {
  //}

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    //SAT_PRINT;
    if (MListener) MListener->do_window_listener_update_widget(ASender,AMode,AIndex);
  }

  //----------

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    //SAT_PRINT;
    if (!ASender) ASender = MRootWidget;
    MPendingDirtyWidgets.write(ASender);
    //invalidateWidget(ASender);
    if (MListener) MListener->do_window_listener_redraw_widget(ASender,AMode,AIndex);
  }

  //----------

  void do_widget_set_state(SAT_Widget* ASender, uint32_t AState) override {
    //if (MListener) MListener->do_window_listener_set_widget_state(ASender,AState);
    switch (AState) {
      case SAT_WIDGET_STATE_MODAL: {
        MModalWidget = ASender;
        break;
      }
      case SAT_WIDGET_STATE_NORMAL: {
        MModalWidget = nullptr;
        break;
      }
    }
  }

  //----------

  void do_widget_set_cursor(SAT_Widget* ASender, int32_t ACursor) override {
    //if (MListener) MListener->do_window_listener_set_cursor(ASender,ACursor);
    switch (ACursor) {
      case SAT_CURSOR_LOCK:
        MMouseLockedWidget = ASender;
        break;
      case SAT_CURSOR_UNLOCK:
        MMouseLockedWidget = nullptr;
        break;
      case SAT_CURSOR_SHOW:
        showCursor();
        setCursor(MCurrentCursor);
        break;
      case SAT_CURSOR_HIDE:
        hideCursor();
        break;
      default:
        if (ACursor != MCurrentCursor) {
          setCursor(ACursor);
          MCurrentCursor = ACursor;
        }
        break;
    }
  }

  //----------

  //void do_widget_set_cursor_pos(SAT_Widget* ASender, int32_t AXpos, int32_t AYpos) override {
  //  setCursorPos(AXpos,AYpos);
  //}

  //----------

  void do_widget_set_hint(SAT_Widget* AWidget, const char* AHint) override {
  }

//------------------------------
public: // timer listener
//------------------------------

  void do_timer_listener_callback(SAT_Timer* ATimer) override {
    if (MListener) MListener->do_window_listener_timer(this);
    //on_window_timer();
    SAT_Rect rect;
    uint32_t num = flushDirtyWidgets(&rect);
    if (num > 0) { // && (rect.isNotEmpty()) {
      invalidate(rect.x,rect.y,rect.w,rect.h);
      //#ifdef SAT_DEBUG
      //SAT_GLOBAL.DEBUG.reportNumDirtyWidgets(num);
      //#endif
    }
  }

};

//----------------------------------------------------------------------
#endif
