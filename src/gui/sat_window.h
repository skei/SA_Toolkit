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
  SAT_WidgetArray     MPrePaintWidgets        = {};
  SAT_WidgetArray     MPostPaintWidgets       = {};
  
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
    //SAT_PRINT;
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

  SAT_Widget*       getRootWidget()     { return MRootWidget; }
  SAT_PaintContext* getPaintContext()   { return &MPaintContext; }
  double            getScale()          { return MScale; }
  uint32_t          getWidth()          { return MWidth; }
  uint32_t          getHeight()         { return MHeight; }
  double            getInitialWidth()   { return MInitialWidth; }
  double            getInitialHeight()  { return MInitialHeight; }
  
  //SAT_TweenManager* getTweens()         { return &MTweens; }
  
  SAT_OpenGL*       getOpenGL()         { return MOpenGL; }

//------------------------------
public:
//------------------------------

  virtual SAT_Widget* appendRootWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
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

  virtual void copyBuffer(void* ADst, int32_t ADstXpos, int32_t ADstYpos, uint32_t ADstWidth, uint32_t ADstHeight, void* ASrc, int32_t ASrcXpos, int32_t ASrcYpos, uint32_t ASrcWidth, uint32_t ASrcHeight) {
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
  
  //----------

  virtual SAT_Rect calcClipRect(SAT_Widget* AWidget) {
    SAT_Rect rect = AWidget->getRect();
    SAT_Widget* parent = AWidget->getParentWidget();
    while (parent) {
      SAT_Rect parentrect = parent->getRect();
      rect.overlap(parentrect);
      SAT_Widget* next = parent->getParentWidget();
      parent = next;
    }
    return rect;
  }
  
  //----------
  
//  /*
//    select render buffer:
//    void nvgluBindFramebuffer(NVGLUframebuffer* fb) {
//      if (defaultFBO == -1) glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
//      glBindFramebuffer(GL_FRAMEBUFFER, fb != NULL ? fb->fbo : defaultFBO);
//    }
//  */    
  
  // opengl context is made current
  
  virtual void prepaint(SAT_PaintContext* AContext) {
    for (uint32_t i=0; i<MPrePaintWidgets.size(); i++) {
      MPrePaintWidgets[i]->on_widget_prepaint(AContext);
    }
  }

  //----------

  // opengl context is still current
  
  virtual void postpaint(SAT_PaintContext* AContext) {
    for (uint32_t i=0; i<MPostPaintWidgets.size(); i++) {
      MPostPaintWidgets[i]->on_widget_postpaint(AContext);
    }
  }


void appendPrePaint(SAT_Widget* AWidget) {
  MPrePaintWidgets.append(AWidget);
}

void appendPostPaint(SAT_Widget* AWidget) {
  MPostPaintWidgets.append(AWidget);
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
        //if (AHover->autoHoverCursor()) do_widgetListener_set_cursor(AHover,AHover->getMouseCursor());
        //if (AHover->autoHoverHint()) do_widgetListener_set_hint(AHover,0,AHover->getHint());
      }
      MHoverWidget = AHover;
    }
  }

//------------------------------
public:
//------------------------------

  void show() override {
    //SAT_PRINT;
    SAT_ImplementedWindow::show();
    //if (MRootWidget) {
    //  MRootWidget->realignChildWidgets();
    //}
    #ifdef SAT_WIN32
      MTimer->start(SAT_WINDOW_TIMER_MS,getWin32Window(),false);
    #else
      MTimer->start(SAT_WINDOW_TIMER_MS,false);
    #endif
  }
  
  //----------
  
  void hide() override {
    //SAT_PRINT;
    MTimer->stop();
    SAT_ImplementedWindow::hide();
  }

//------------------------------
public: // window
//------------------------------

  void on_window_open() override {
    //SAT_PRINT;
    if (MRootWidget) {
      MRootWidget->prepare(this);
      MRootWidget->realignChildWidgets();
      //MRootWidget->markDirty();
      MPendingDirtyWidgets.write(MRootWidget);
    }
  }

  //----------

  void on_window_close() override {
    //SAT_PRINT;
  }

  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //SAT_PRINT;
  }

  //----------
  
  void on_window_resize(int32_t AWidth, int32_t AHeight) override {

    MWidth = AWidth;
    MHeight = AHeight;
    MScale = recalcScale(AWidth,AHeight);

    // or should we check in paint()?
    //#ifdef SAT_WINDOW_BUFFERED
    //  bool resized = checkBuffer(AWidth,AHeight);
    //  if (resized) {}
    //#endif
    
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
    // if hover = null (outside of window/modal-widget), and we click, we send close notification
    // if we click _outside_ modal widget, we send close notification
      
    // right click brings up context/popup menu
    uint32_t num_popup = MHoverWidget->getNumPopupMenuItems();
    if ((AButton == SAT_BUTTON_RIGHT) && (num_popup > 0)) {
      for (uint32_t i=0; i<num_popup; i++) {
        const char* item = MHoverWidget->getPopupMenuItem(i);
        if (item) {
          SAT_Print("%i. %s\n",i,item);
        }
      }
    }

    if ((ATime - MPrevButtonTime) < SAT_WINDOW_DBLCLICK_MS) {
      // less than x ms since last click means double_click
      // todo: check if same button, and over same widget..
      SAT_Print("dblclick\n");
    }
  */
  
  //bool dblclick = checkDoubleClick(ATimEsStamp);
  //bool scroll = checkScrollWheel(AButton);
  //bool rightclick = checkContextMenu();

  void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) override {
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
      //else {} // clicked another button
    }
    else {
      // not hovering over a widget..      
      if (MModalWidget) {
        if ((AButton == SAT_BUTTON_LEFT) || (AButton == SAT_BUTTON_RIGHT)) {
          //SAT_Print("nope.. closeing modal\n");
          //MModalWidget->close();
          MModalWidget->do_widgetListener_close(MModalWidget);
        }
      }
    }
  }

  //----------
  
  void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) override {
    if (MMouseCapturedWidget) {
      if (AButton == MMouseClickedButton) {
        // released first button
        MMouseCapturedWidget->on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATimestamp);
        MMouseCapturedWidget = nullptr;
        //updateHoverWidget(AXpos,AYpos);
      }
      //else {} // released later button
    }
    //else {} // we should never end up here?
  }

  //----------

  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimestamp) override {
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
      SAT_Widget* hover = findHoverWidget(AXpos,AYpos,MModalWidget);
      updateHoverState(hover,x,y,0);
      MModalWidget->on_widget_mouse_move(x,y,AState,ATimestamp);
    }
    else if (MMouseCapturedWidget) {
      //SAT_Widget* hover = findHoverWidget(AXpos,AYpos,MMouseCapturedWidget);
      //updateHoverState(hover,x,y,0);
      MMouseCapturedWidget->on_widget_mouse_move(x,y,AState,ATimestamp);
    }
    else {
      SAT_Widget* hover = findHoverWidget(AXpos,AYpos);
      updateHoverState(hover,x,y,0);
      if (hover) hover->on_widget_mouse_move(x,y,AState,ATimestamp);
    }
    
    MMousePreviousXpos = AXpos;
    MMousePreviousYpos = AYpos;
  }

  //----------

  void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATimestamp) override {
    SAT_Widget* hover = findHoverWidget(AXpos,AYpos);
    updateHoverState(hover,AXpos,AYpos,0);
  }

  //----------

  void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATimestamp) override {
    updateHoverState(nullptr,AXpos,AYpos,0);
  }

  //----------

  void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATimestamp) override {
    if (MKeyCapturedWidget) {
      MKeyCapturedWidget->on_widget_key_press(AKey,AState,ATimestamp);
    }
  }

  //----------

  void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATimestamp) override {
    if (MKeyCapturedWidget) {
      MKeyCapturedWidget->on_widget_key_release(AKey,AState,ATimestamp);
    }
  }

  //----------
  
  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MPaintContext.painter = MWindowPainter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    MOpenGL->makeCurrent();
    
    //prepaint(&MPaintContext);
    
    uint32_t width2  = SAT_NextPowerOfTwo(MWidth);
    uint32_t height2 = SAT_NextPowerOfTwo(MHeight);
    if ((width2 != MBufferWidth) || (height2 != MBufferHeight)) {
      // if size has changed: create new buffer, copy old to new, delete old
      void* buffer = MWindowPainter->createRenderBuffer(width2,height2);
      SAT_Assert(buffer);
      
      // we don't need to copy the buffer if we're redrawing the entire thing anyway, do we?
      //copyBuffer(buffer,0,0,width2,height2,MRenderBuffer,0,0,MBufferWidth,MBufferHeight);

      MWindowPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBuffer = buffer;
      MBufferWidth  = width2;
      MBufferHeight = height2;
      // paint dirty widgets to render buffer
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);
      
prepaint(&MPaintContext);
      
      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
      SAT_Widget* widget;

      while (MPaintDirtyWidgets.read(&widget)) {} // widget->on_widget_paint(&MPaintContext);
      //count = 1;
      MRootWidget->on_widget_paint(&MPaintContext);
      MRootWidget->setLastPainted(MPaintContext.counter); //paint_count);

      MWindowPainter->endFrame();
      
postpaint(&MPaintContext);
      
    }
    else {
      MWindowPainter->selectRenderBuffer(MRenderBuffer,MBufferWidth,MBufferHeight);

prepaint(&MPaintContext);

      MWindowPainter->beginFrame(MBufferWidth,MBufferHeight);
      MWindowPainter->setClipRect(SAT_Rect(0,0,MWindowWidth,MWindowHeight));
      SAT_Widget* widget;
      //int32_t paint_count = MPaintContext.counter;
      while (MPaintDirtyWidgets.read(&widget)) {
        if (widget->isVisible()) {
          if (widget->getLastPainted() != MPaintContext.counter) { //paint_count) {
            SAT_Rect cliprect = calcClipRect(widget);
            // if cliprect visible
            MWindowPainter->pushClip(cliprect);
            widget->on_widget_paint(&MPaintContext);
            MWindowPainter->popClip();
            widget->setLastPainted(MPaintContext.counter); //paint_count);
            //count += 1;
          }
        }
      }
      MWindowPainter->endFrame();
      
postpaint(&MPaintContext);
      
    }
    copyBuffer(nullptr,0,0,MWindowWidth,MWindowHeight,MRenderBuffer,AXpos,AYpos,AWidth,AHeight);
    
    //postpaint(&MPaintContext);
    
    MOpenGL->swapBuffers();
    MOpenGL->resetCurrent();
    MPaintContext.counter += 1;
  }
  
//------------------------------
public:
//------------------------------

  virtual void on_window_timer(double AElapsed) {
    if (MListener) MListener->do_windowListener_timer(this,AElapsed);
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

  double do_widgetOwner_get_scale() override {
    return MScale;
  }

  //----------

  double do_widgetOwner_get_width() override {
    return MWidth;
  }

  //----------

  double do_widgetOwner_get_height() override {
    return MHeight;
  } 

  //----------

  SAT_Painter* do_widgetOwner_get_painter() override {
    return MWindowPainter;
  } 

//------------------------------
public: // widget listener
//------------------------------

  void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_windowListener_update_widget(ASender,AMode,AIndex);
  }

  //----------
  
  void do_widgetListener_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (!ASender) ASender = MRootWidget;
    MPendingDirtyWidgets.write(ASender);
    if (MListener) MListener->do_windowListener_redraw_widget(ASender,AMode,AIndex);
  }

  //----------

  void do_widgetListener_realign(SAT_Widget* ASender) override {
  }

  //----------

  void do_widgetListener_close(SAT_Widget* ASender) override {
  }

  //----------

  void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex) override {
  }

  //----------

  void do_widgetListener_set_cursor(SAT_Widget* ASender, int32_t ACursor) override {
    switch(ACursor) {
      case SAT_CURSOR_LOCK:
        lockMouseCursor();
        break;
      case SAT_CURSOR_UNLOCK:
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

  void do_widgetListener_set_hint(SAT_Widget* ASender, uint32_t AMode, const char* AHint) override {
  }

  //----------

  void do_widgetListener_set_modal(SAT_Widget* ASender) override {
    if (ASender) beginModal(ASender);
    else endModal();
  }
  
//------------------------------
public: // timer listener
//------------------------------

  void do_timerListener_callback(SAT_Timer* ATimer) override {
    double now = SAT_GetTime();
    double elapsed = now - MPrevTime;
    MPrevTime = now;
    on_window_timer(elapsed);
  }

};

//----------------------------------------------------------------------
#endif

