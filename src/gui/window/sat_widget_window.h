#ifndef sat_widget_window_included
#define sat_widget_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_tween_manager.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/window/sat_buffered_window.h"
#include "gui/window/sat_window_listener.h"
#include "gui/widget/sat_overlay_widget.h"
#include "gui/widget/sat_root_widget.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"

typedef SAT_Queue<SAT_Widget*,1024> SAT_WidgetQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WidgetWindow
: public SAT_BufferedWindow
, public SAT_WidgetListener
, public SAT_WidgetOwner {

//------------------------------
private:
//------------------------------

  SAT_WindowListener* MListener           = nullptr;
  SAT_TweenManager    MTweenManager       = {};

  SAT_RootWidget*     MRootWidget         = nullptr;
  SAT_WidgetQueue     MDirtyWidgets       = {};
  SAT_WidgetQueue     MPaintWidgets       = {};
  
  SAT_Widget*         MHoverWidget        = nullptr;
  SAT_Widget*         MModalWidget        = nullptr;
  SAT_Widget*         MMouseCaptureWidget = nullptr;
  SAT_Widget*         MKeyCaptureWidget   = nullptr;
  SAT_Widget*         MHintWidget         = nullptr;
  SAT_OverlayWidget*  MOverlayWidget      = nullptr;

  int32_t             MMouseCurrentCursor = SAT_CURSOR_DEFAULT;
  SAT_WidgetArray     MTimerListeners     = {};

  int32_t             MInitialWidth       = 0;
  int32_t             MInitialHeight      = 0;
  double              MInitialScale       = 1.0;
  bool                MProportional       = false;
  double              MWindowScale        = 1.0;

  int32_t             MMouseCurrentXpos   = 0;
  int32_t             MMouseCurrentYpos   = 0;
  int32_t             MMousePreviousXpos  = 0;
  int32_t             MMousePreviousYpos  = 0;
  
  bool                MMouseLocked        = false;
  int32_t             MMouseLockedXclick  = 0;
  int32_t             MMouseLockedYclick  = 0;
  int32_t             MMouseLockedXpos    = 0;
  int32_t             MMouseLockedYpos    = 0;

  int32_t             MMouseClickedXpos   = 0;
  int32_t             MMouseClickedYpos   = 0;
  uint32_t            MMouseClickedButton = 0;

  bool                MMouseCaptured      = false;
  int32_t             MMouseCaptureXpos   = 0;
  int32_t             MMouseCaptureYpos   = 0;
  uint32_t            MMouseCaptureButton = 0;

//------------------------------
public:
//------------------------------

  SAT_WidgetWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BufferedWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_WidgetWindow() {
    if (MRootWidget) delete MRootWidget;
  }

//------------------------------
public:
//------------------------------

  void setListener(SAT_WindowListener* AListener) {
    MListener = AListener;
  }

  //----------

  void setRootWidget(SAT_RootWidget* ARoot) {
    MRootWidget = ARoot;
  }

  SAT_RootWidget* getRootWidget() {
    return MRootWidget;
  }

  //----------

  void setHintWidget(SAT_Widget* AWidget) {
    MHintWidget = AWidget;
  }

  SAT_Widget* getHintWidget() {
    return MHintWidget;
  }

  //----------

  SAT_OverlayWidget* getOverlayWidget() {
    return MOverlayWidget;
  }

  //----------

  void setInitialSize(uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false) {
    //SAT_PRINT("AWidth %i AHeight %i AScale %.3f\n",AWidth,AHeight,AScale);
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MInitialScale = AScale;
    MProportional = AProportional;
    MWindowScale = calcScale(AWidth,AHeight);
  }

//------------------------------
public:
//------------------------------

// called by SAT_EmbeddedEditor.setupOverlay()

  bool setupOverlay() override {
    SAT_RootWidget* root = getRootWidget();
    if (root) {
      MOverlayWidget = new SAT_OverlayWidget();
      root->appendChild(MOverlayWidget);
    }
    return true;
  }

  //----------

  void paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    //SAT_BufferedWindow::on_window_paint(AContext);
    SAT_Painter* painter = AContext->painter;
    //MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    uint32_t screenwidth = getWidth();
    uint32_t screenheight = getHeight();
    painter->setClipRect(SAT_Rect(0,0,screenwidth,screenheight));
    painter->setClip(0,0,screenwidth,screenheight);
    flushPaintWidgets(AContext);
    painter->resetClip();
  }

//------------------------------
private:
//------------------------------

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

  //----------

  void lockMouseCursor() {
    MMouseLocked  = true;
    MMouseLockedXclick = MMouseCurrentXpos;
    MMouseLockedYclick = MMouseCurrentYpos;
    MMouseLockedXpos = MMouseCurrentXpos;
    MMouseLockedYpos = MMouseCurrentYpos;
  }

  //----------

  void unlockMouseCursor() {
    MMouseLocked = false;
  }  

  //----------

  // calculates the maximal (or minimal) scale to use for the gui,
  // that will fit inside the given width/height

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

  //----------

  // called from
  //   SAT_WidgetWindow.on_widgetListener_redraw()

  void queueDirtyWidget(SAT_Widget* AWidget) {
    if (!MDirtyWidgets.write(AWidget)) {
    }
  }

  //----------

  // called from
  //   on_timerListener_callback

  // TODO: check widget if already set to be drawn (render frame #)

  void flushDirtyWidgets() {
    uint32_t count = 0;
    SAT_Widget* widget;
    bool has_update = false;
    SAT_Rect update_rect;
    while (MDirtyWidgets.read(&widget)) {
      count += 1;
      SAT_Rect rect = widget->getRect();
      queuePaintWidget(widget);
      if (has_update) {
        update_rect.combine(rect);
      }
      else {
        update_rect = rect;
        has_update = true;
      }
    }
    if (has_update) {
      //SAT_PRINT("invalidating %.f,%.f %.f,%.f\n",update_rect.x,update_rect.y,update_rect.w,update_rect.h);
      invalidate(update_rect.x,update_rect.y,update_rect.w,update_rect.h);
    }
  }

  //----------

  // called from
  // SAT_WidgetWindow.flushDirtyWidgets()

  void queuePaintWidget(SAT_Widget* AWidget) {
    MPaintWidgets.write(AWidget);
  }

  //----------

  // called from
  //   SAT_WidgetWindow.on_window_paint()

  // TODO:
  // - paint to buffer
  // - copy update rect

  void flushPaintWidgets(SAT_PaintContext* AContext) {
    
    // SAT_Widget* widget = nullptr;
    // while (MPaintWidgets.read(&widget)) {}
    // if (MRootWidget) MRootWidget->on_widget_paint(AContext);

    SAT_Painter* painter = AContext->painter;
    SAT_Widget* widget = nullptr;
    while (MPaintWidgets.read(&widget)) {
      painter->pushClip(widget->getRect());
      SAT_Widget* parent = widget->findOpaqueParent();
      if (parent) {
        painter->pushOverlappingClip( parent->getRect() );
        parent->on_widget_paint(AContext);
        painter->popClip();
      }
      else {
        //painter->pushOverlappingClip( widget->getRect() );
        widget->on_widget_paint(AContext);
      }
      painter->popClip();
    }

  }

//------------------------------
public: // window
//------------------------------

  // don't realign every time we hide/show editor? (in plugin)
  // only first time we open it

  void on_window_show() override {
    SAT_BufferedWindow::on_window_show();
    if (MRootWidget) {
      uint32_t w = getWidth();
      uint32_t h = getHeight();
      MWindowScale = calcScale(w,h);
      MRootWidget->setSize(w,h);
      MRootWidget->realignChildren();
      MRootWidget->ownerWindowOpened(this);
      queueDirtyWidget(MRootWidget);
    }
  }

  //----------

  void on_window_hide() override {
    if (MRootWidget) MRootWidget->ownerWindowClose(this);
    SAT_BufferedWindow::on_window_hide();
  }

  //----------

  // void on_window_move(int32_t AXpos, int32_t AYpos) override {
  //   SAT_BufferedWindow::on_window_move(AXpos,AYpos);
  // }

  //----------
  
  // looks like Reaper is sending us negative coords (int16_t), 
  // but in the x11 event, w,h is uint16_t,
  // so we get widths like 65532, etc..

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);

    if (AWidth >= 32768) AWidth = 0;
    if (AHeight >= 32768) AHeight = 0;

    SAT_BufferedWindow::on_window_resize(AWidth,AHeight);

    if (MRootWidget) {
      MWindowScale = calcScale(AWidth,AHeight);
      MRootWidget->on_widget_resize(AWidth,AHeight);
      MRootWidget->realignChildren();
      queueDirtyWidget(MRootWidget);
    }
  }

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
        setMousePos(MMouseLockedXclick,MMouseLockedYclick);
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

//------------------------------
public: // timer listener
//------------------------------

  // timer-thread
  // (could gui-thread be pushing dirty widgets at the same time?)

  void on_timerListener_callback(SAT_Timer* ATimer, double ADelta) override {

    if (MIsClosing) {
      //SAT_PRINT("isClosing\n");
      return;
    }

    if (MIsPainting) {
      //SAT_PRINT("isPainting\n");
      return;
    }

    if (MListener) MListener->on_windowListener_timer(ATimer,ADelta);
    for (uint32_t i=0; i<MTimerListeners.size(); i++) {
      MTimerListeners[i]->on_widget_timer(ADelta);
    }
    MTweenManager.process(ADelta);
    #ifdef SAT_WINDOW_TIMER_REFRESH_WIDGETS
      flushDirtyWidgets();
    #endif
  }

//------------------------------
public: // widget owner
//------------------------------

  uint32_t on_widgetOwner_getWidth() override {
    return getWidth();
  }

  uint32_t on_widgetOwner_getHeight() override {
    return getHeight();
  }

  double on_widgetOwner_getScale() override {
    return MWindowScale;
  }

  SAT_Painter* on_widgetOwner_getPainter() override {
    return MWindowPainter;
  }

//------------------------------
public: // widget listener
//------------------------------

  void on_widgetListener_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    if (MListener) {
      MListener->on_windowListener_update(AWidget,AIndex,AMode);
    }
  }

  //----------

  void on_widgetListener_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) override {
    #ifdef SAT_WINDOW_TIMER_REFRESH_WIDGETS
      queueDirtyWidget(AWidget);
    #else
      SAT_Rect rect = AWidget->getRect();
      invalidate(rect.x,rect.y,rect.w,rect.h);
    #endif

  }

  //----------

  void on_widgetListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) override {
    SAT_Widget* parent = AWidget->getParent();
    if (parent) {
      parent->realignChildren();
      parent->do_widget_redraw(parent);
    }    
  }

  //----------

  void on_widgetListener_notify(SAT_Widget* AWidget, int32_t AValue) override {
  }
  
  //----------

  void on_widgetListener_mouse_capture(SAT_Widget* AWidget) override {
    MMouseCaptureWidget = AWidget;
  }
  
  //----------

  void on_widgetListener_key_capture(SAT_Widget* AWidget) override {
    MKeyCaptureWidget = AWidget;
  }
  
  //----------

  void on_widgetListener_modal(SAT_Widget* AWidget) override {
    MModalWidget = AWidget;
  }

  //----------

  void on_widgetListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
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

  void on_widgetListener_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (AHint[0]) {
      if (MHintWidget) MHintWidget->on_widget_hint(AWidget,AHint);
    }
  }

  //----------

  void on_widgetListener_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    if (AWantTimer) MTimerListeners.append(AWidget);
    else MTimerListeners.remove(AWidget);
  }

  //----------

  void on_widgetListener_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) override {
    AWidget->clearPrevTween();
    MTweenManager.appendChain(ATween);
  }

  //----------

  void on_widgetListener_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
  }

  //----------

  void on_widgetListener_set_overlay(SAT_Widget* AWidget, SAT_Color AColor) override {
    SAT_OverlayWidget* overlay = getOverlayWidget();
    if (overlay) {
      overlay->setColor(AColor);
      SAT_RootWidget* root = getRootWidget();
      root->do_widget_redraw(root);
    }
  }

};

//----------------------------------------------------------------------
#endif