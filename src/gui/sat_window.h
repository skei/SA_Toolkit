#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_tween_manager.h"
#include "gui/sat_gui_base.h"
#include "gui/widget/sat_overlay_widget.h"
#include "gui/widget/sat_root_widget.h"
#include "gui/sat_gui_queues.h"
#include "gui/sat_simple_window.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"

//typedef SAT_AtomicQueue<SAT_Widget*,SAT_WINDOW_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;
////typedef SAT_Queue<SAT_Widget*,SAT_WINDOW_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_SimpleWindow
, public SAT_WidgetListener
, public SAT_WidgetOwner {

//------------------------------
private:
//------------------------------

  SAT_GuiQueues       MQueues               = {};
  
  // widgets

  SAT_RootWidget*     MRootWidget           = nullptr;
  SAT_OverlayWidget*  MOverlayWidget        = nullptr;
  SAT_Widget*         MHoverWidget          = nullptr;
  SAT_Widget*         MModalWidget          = nullptr;
  SAT_Widget*         MMouseCaptureWidget   = nullptr;
  SAT_Widget*         MKeyCaptureWidget     = nullptr;
  SAT_Widget*         MHintWidget           = nullptr;

  // timer

  double              MTimerDelta           = 0;
  SAT_WidgetArray     MTimerListeners       = {};
  SAT_TweenManager    MTweenManager         = {};

  // mouse

  int32_t             MMouseCurrentCursor   = SAT_CURSOR_DEFAULT;
  int32_t             MMouseCurrentXpos     = 0;
  int32_t             MMouseCurrentYpos     = 0;
  int32_t             MMousePreviousXpos    = 0;
  int32_t             MMousePreviousYpos    = 0;
  bool                MMouseLocked          = false;
  int32_t             MMouseLockedXclick    = 0;
  int32_t             MMouseLockedYclick    = 0;
  int32_t             MMouseLockedXpos      = 0;
  int32_t             MMouseLockedYpos      = 0;
  int32_t             MMouseClickedXpos     = 0;
  int32_t             MMouseClickedYpos     = 0;
  uint32_t            MMouseClickedButton   = 0;
  bool                MMouseCaptured        = false;
  int32_t             MMouseCaptureXpos     = 0;
  int32_t             MMouseCaptureYpos     = 0;
  uint32_t            MMouseCaptureButton   = 0;

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, SAT_WindowListener* AListener=nullptr, intptr_t AParent=0)
  : SAT_SimpleWindow(AWidth,AHeight,AListener,AParent) {
    //SAT_PRINT("AListener %p\n",AListener);
  }

  //----------

  virtual ~SAT_Window() {
    if (MRootWidget) delete MRootWidget;
  }

//------------------------------
public: // setup
//------------------------------

  SAT_Widget* getHintWidget() {
    return MHintWidget;
  }

  //----------

  SAT_OverlayWidget* getOverlayWidget() {
    return MOverlayWidget;
  }

  //----------

  SAT_RootWidget* getRootWidget() {
    return MRootWidget;
  }

  //----------

  void setListener(SAT_WindowListener* AListener) {
    SAT_PRINT("AListener %p\n",AListener);
    MListener = AListener;
  }

  //----------

  void setHintWidget(SAT_Widget* AWidget) {
    MHintWidget = AWidget;
  }

  //----------

  void setOverlayWidget(SAT_OverlayWidget* AWidget) {
    MOverlayWidget = AWidget;
  }

  //----------

  void setRootWidget(SAT_RootWidget* ARoot) {
    MRootWidget = ARoot;
  }

//------------------------------
public:
//------------------------------

  bool setupOverlay() override {
    SAT_RootWidget* root = getRootWidget();
    if (root) {
      MOverlayWidget = new SAT_OverlayWidget();
      root->appendChild(MOverlayWidget);
    }
    return true;
  }

//------------------------------
public: // mouse
//------------------------------

  void lockMouseCursor() {
    MMouseLocked        = true;
    MMouseLockedXclick  = MMouseCurrentXpos;
    MMouseLockedYclick  = MMouseCurrentYpos;
    MMouseLockedXpos    = MMouseCurrentXpos;
    MMouseLockedYpos    = MMouseCurrentYpos;
  }

  //----------

  void unlockMouseCursor() {
    MMouseLocked = false;
  }

  //----------

  void updateHover(uint32_t AXpos, uint32_t AYpos, uint32_t ATime) {
    if (MRootWidget) {
      SAT_Widget* hover;
      if (MModalWidget) hover = MModalWidget->findWidget(AXpos,AYpos);
      else hover = MRootWidget->findWidget(AXpos,AYpos);
      if (hover) {
        if (hover != MHoverWidget) {
          if (MHoverWidget) MHoverWidget->on_widget_leave(hover,AXpos,AYpos,ATime);
          hover->on_widget_enter(MHoverWidget,AXpos,AYpos,ATime);
          MHoverWidget = hover;
        }
      }
      else {
        if (MHoverWidget) MHoverWidget->on_widget_leave(nullptr,AXpos,AYpos,ATime);
        MHoverWidget = nullptr;
      }
    }
  }

//------------------------------
public: // painting
//------------------------------

  void preRender(uint32_t AWidth, uint32_t AHeight) override {
  }

  //----------

  void prePaint(uint32_t AWidth, uint32_t AHeight) override {
  }

  //----------

  // called from
  //   SAT_SimpleWindow.on_window_paint

  void paint(SAT_PaintContext* AContext) override {
    SAT_Painter* painter = AContext->painter;
    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();
    painter->setClipRect(SAT_Rect(0,0,screenwidth,screenheight));
    painter->setClip(0,0,screenwidth,screenheight);
    #ifdef SAT_WINDOW_QUEUE_WIDGETS
      MQueues.flushRealignWidgets(AContext->counter);
      #ifdef SAT_WINDOW_BUFFERED
        MQueues.flushPaintWidgets(AContext);
      #else
        MQueues.flushPaintWidgets(AContext,MRootWidget);
      #endif
    #endif
    painter->resetClip();
  }

  //----------

  // called from
  //   SAT_SimpleWindow.on_window_paint

  void paintRoot(SAT_PaintContext* AContext, bool AResized=false) override {
    SAT_Painter* painter = AContext->painter;
    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();
    painter->setClipRect(SAT_Rect(0,0,screenwidth,screenheight));
    painter->setClip(0,0,screenwidth,screenheight);
    if (AResized) {
      MWindowScale = calcScale(screenwidth,screenheight);
      if (MRootWidget) {
        MRootWidget->on_widget_resize(screenwidth,screenheight);
        MRootWidget->realignChildren();
      }
    }
    MQueues.flushRealignWidgets(AContext->counter);
    MQueues.flushPaintWidgets(AContext,MRootWidget);
    painter->resetClip();
  }

  //----------

  void postPaint() override {
  }

  //----------

  void postRender() override {
  }

//------------------------------
public: // timer
//------------------------------

  // if called from on_TimerListener_update() : timer thread
  // if called from on_window_timer() : gui (x11 event thread) thread

  void handleTimer(double ADelta) {
    MTimerDelta += ADelta;
    if (MIsClosing)  return;
    if (MIsPainting) return;

    // timer updates

    if (MListener) MListener->on_WindowListener_timer(MTimerDelta);
    for (uint32_t i=0; i<MTimerListeners.size(); i++) {
      MTimerListeners[i]->on_widget_timer(MTimerDelta);
    }
    MTweenManager.process(MTimerDelta);
    MTimerDelta = 0;

    // paint dirty widgets

    #ifdef SAT_WINDOW_QUEUE_WIDGETS
      #ifdef SAT_WINDOW_BUFFERED
        SAT_Rect rect = MQueues.flushRedrawToPaint();
        if (rect.isNotEmpty()) invalidate(rect.x,rect.y,rect.w,rect.h);
      #else
        //SAT_Rect rect = MQueues.flushRedrawToPaint(MRootWidget);
        MQueues.flushRedrawToPaint(MRootWidget);
        SAT_Rect rect = MRootWidget->getRect();
        invalidate(rect.x,rect.y,rect.w,rect.h);
      #endif
    #endif

  }

  //----------

  /*
    called via
    SAT_SimpleWindow.on_TimerListener_update -> userMessage -> event/gui thread -> on_window_timer
  */

  // [TIMER THREAD]

  void on_TimerListener_update(SAT_Timer* ATimer, double ADelta) override {
    //sendClientMessage(SAT_WINDOW_THREAD_TIMER,0);
    handleTimer(ADelta);
  }

  //----------

  // [MAIN/GUI THREAD]

  // void on_window_timer(double ADelta) override {
  //   handleTimer(ADelta);
  // }

//------------------------------
public: // widget owner
//------------------------------

  SAT_Painter*  on_WidgetOwner_getPainter() override { return MWindowPainter; }
  uint32_t      on_WidgetOwner_getWidth()   override { return getWidth(); }
  uint32_t      on_WidgetOwner_getHeight()  override { return getHeight(); }
  double        on_WidgetOwner_getScale()   override { return MWindowScale; }

//------------------------------
public: // widget listener
//------------------------------

  // [GUI THREAD]

  void on_WidgetListener_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    //SAT_PRINT("MListener: %p\n",MListener);;
    if (MListener) {
      MListener->on_WindowListener_update(AWidget,AIndex,AMode);
    }
  }

  //----------

  void on_WidgetListener_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    #ifdef SAT_WINDOW_QUEUE_WIDGETS

      MQueues.queueRedraw(AWidget);
      switch (AMode) {
        case SAT_WIDGET_REDRAW_SELF: {
          MQueues.queueRedraw(AWidget);
          break;
        }
        case SAT_WIDGET_REDRAW_PARENT: {
          SAT_Widget* parent = AWidget->getParent();
          if (parent) MQueues.queueRedraw(parent);
          break;
        }
        case SAT_WIDGET_REDRAW_ROOT: {
          MQueues.queueRedraw(MRootWidget);
          break;
        }
      }

    #else

      SAT_Rect rect = AWidget->getRect();
      invalidate(rect.x,rect.y,rect.w,rect.h);

    #endif
  }

  //----------

  void on_WidgetListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) override {
    #ifdef SAT_WINDOW_QUEUE_WIDGETS    

      switch (AMode) {
        case SAT_WIDGET_REALIGN_SELF: {
          MQueues.queueRealign(AWidget);
          //MQueues.queueRedraw(AWidget);
          break;
        }
        case SAT_WIDGET_REALIGN_PARENT: {
          SAT_Widget* parent = AWidget->getParent();
          if (parent) {
            MQueues.queueRealign(parent);
            //MQueues.queueRedraw(parent);
          }
          break;
        }
        case SAT_WIDGET_REALIGN_ROOT: {
          break;
        }
      }

    #else

      switch (AMode) {
        case SAT_WIDGET_REALIGN_SELF: {
          AWidget->realignChildren();
          AWidget->do_widget_redraw(AWidget);
          break;
        }
        case SAT_WIDGET_REALIGN_PARENT: {
          SAT_Widget* parent = AWidget->getParent();
          if (parent) {
            parent->realignChildren();
            parent->do_widget_redraw(parent);

          }
          break;
        }
        case SAT_WIDGET_REALIGN_ROOT: {
          break;
        }
      }

    #endif

  }

  //----------

  void on_WidgetListener_notify(SAT_Widget* AWidget, int32_t AValue) override {
  }
  
  //----------

  void on_WidgetListener_mouse_capture(SAT_Widget* AWidget) override {
    MMouseCaptureWidget = AWidget;
  }
  
  //----------

  void on_WidgetListener_key_capture(SAT_Widget* AWidget) override {
    MKeyCaptureWidget = AWidget;
  }
  
  //----------

  void on_WidgetListener_modal(SAT_Widget* AWidget) override {
    MModalWidget = AWidget;
  }

  //----------

  void on_WidgetListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    switch(ACursor) {
      case SAT_CURSOR_LOCK:
        lockMouseCursor();
        break;
      case SAT_CURSOR_UNLOCK:
        unlockMouseCursor();
        break;
      case SAT_CURSOR_SHOW:  
        showMouseCursor();
        setMouseCursorShape(MMouseCurrentCursor);
        break;
      case SAT_CURSOR_HIDE:
        hideMouseCursor();
        break;
      default:
        if (ACursor != MMouseCurrentCursor) {
          setMouseCursorShape(ACursor);
          MMouseCurrentCursor = ACursor;
        }
    }
  }
  
  //----------

  void on_WidgetListener_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (AHint[0]) {
      if (MHintWidget) MHintWidget->on_widget_hint(AWidget,AHint);
    }
  }

  //----------

  void on_WidgetListener_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    if (AWantTimer) MTimerListeners.append(AWidget);
    else MTimerListeners.remove(AWidget);
  }

  //----------

   // restart?
  
  void on_WidgetListener_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) override {
    AWidget->clearPrevTween();
    MTweenManager.appendChain(ATween);
  }

  //----------

  void on_WidgetListener_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
  }

  //----------

  void on_WidgetListener_set_overlay(SAT_Widget* AWidget, SAT_Color AColor) override {
    SAT_OverlayWidget* overlay = getOverlayWidget();
    if (overlay) {
      overlay->setColor(AColor);
      SAT_RootWidget* root = getRootWidget();
      root->do_widget_redraw(root,0,SAT_WIDGET_REDRAW_SELF);
    }
  }

//------------------------------
public: // window
//------------------------------

  // [EVENT THREAD]

  void on_window_show() override {
    SAT_SimpleWindow::on_window_show();
    if (MRootWidget) {
      uint32_t w = getWidth();
      uint32_t h = getHeight();
      MWindowScale = calcScale(w,h);
      MRootWidget->setSize(w,h);
      MRootWidget->realignChildren();
      MRootWidget->ownerWindowOpened(this);

      // initial painting.. (without this, the screen is black)
      MQueues.queueRedraw(MRootWidget);

    }
  }

  //----------

  void on_window_hide() override {
    if (MRootWidget) MRootWidget->ownerWindowClose(this);
    SAT_SimpleWindow::on_window_hide();
  }

  //----------

  // void on_window_move(int32_t AXpos, int32_t AYpos) override {
  // }

  //----------

//  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
//
//    // if (AWidth >= 32768) AWidth = 0;
//    // if (AHeight >= 32768) AHeight = 0;
//    SAT_Assert(AWidth < 32768);
//    SAT_Assert(AHeight < 32768);
//    if (MRootWidget) {
//      MWindowScale = calcScale(AWidth,AHeight);
//      MRootWidget->on_widget_resize(AWidth,AHeight);
//      MRootWidget->realignChildren();
//      // #ifndef SAT_PLUGIN_EXE
//      //   queueDirtyWidget(MRootWidget);
//      // #endif
//    }
//
//    //MQueues.queueResize(AWidth,AHeight);
//    SAT_SimpleWindow::on_window_resize(AWidth,AHeight);
//  }

  //----------

  // void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
  //   SAT_SimpleWindow::on_window_paint(AXpos,AYpos,AWidth,AHeight);
  // }

  //----------

  // void on_window_realign() override {
  // }

  //----------

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    MMouseClickedXpos   = AXpos;
    MMouseClickedYpos   = AYpos;
    MMouseClickedButton = AButton;
    // if widget is already captured, send further clicks to the same widget..
    if (MMouseCaptured) {
      MMouseCaptureWidget->on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
    }
    // else, capture the widget we are hovering above
    else {
      if (MHoverWidget) {
        MMouseCaptured = true;
        MMouseCaptureWidget = MHoverWidget;
        MMouseCaptureXpos = AXpos;
        MMouseCaptureYpos = AYpos;
        MMouseCaptureButton = AButton;
        MMouseCaptureWidget->on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
      }
    }
  }

  //----------

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (MMouseCaptured) {
      MMouseCaptureWidget->on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATime);
      // end capture if we release the same button as the one we started the dragging with
      if (AButton == MMouseCaptureButton) {
        MMouseCaptured = false;
        updateHover(AXpos,AYpos,ATime);
      }
    }
  }

  //----------

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    MMouseCurrentXpos = AXpos;
    MMouseCurrentYpos = AYpos;
    int32_t deltax = AXpos - MMousePreviousXpos;
    int32_t deltay = AYpos - MMousePreviousYpos;
    // when we set the mouse cursor back to its locked pos, the os/system might send us a new mouse move event as a result.. ignore it..
    if (MMouseLocked) {
      if ((AXpos != MMouseLockedXclick) || (AYpos != MMouseLockedYclick)) {
        MMouseLockedXpos += deltax;
        MMouseLockedYpos += deltay;
        if (MMouseCaptured) MMouseCaptureWidget->on_widget_mouse_move(MMouseLockedXpos,MMouseLockedYpos,AState,ATime);
        // set it back to locked position
        setMouseCursorPos(MMouseLockedXclick,MMouseLockedYclick);
      }
    }
    else {
      // send mouse move events only to captured widget
      if (MMouseCaptured) {
        MMouseCaptureWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
      }
      else {
        updateHover(AXpos,AYpos,ATime);
        if (MHoverWidget && MHoverWidget->Options.wantHoverEvents) {
          MHoverWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
        }
      }
    }
    MMousePreviousXpos = AXpos;
    MMousePreviousYpos = AYpos;
  }

  //----------

  void on_window_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
    if (MKeyCaptureWidget) {
      MKeyCaptureWidget->on_widget_key_press(AKey,AChar,AState,ATime);
    }
  }

  //----------

  void on_window_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
    if (MKeyCaptureWidget) {
      MKeyCaptureWidget->on_widget_key_release(AKey,AChar,AState,ATime);
    }
  }

  //----------

  void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    updateHover(AXpos,AYpos,ATime);
  }

  //----------

  void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MHoverWidget) MHoverWidget->on_widget_leave(nullptr,AXpos,AYpos,ATime);
    MHoverWidget = nullptr;
  }

  //----------

  void on_window_clientMessage(uint32_t AData) override {
  }


};

//----------------------------------------------------------------------
#endif

