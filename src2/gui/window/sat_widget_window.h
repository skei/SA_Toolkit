#ifndef sat_widget_window_included
#define sat_widget_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_tween_manager.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/window/sat_buffered_window.h"
#include "gui/window/sat_window_listener.h"
#include "gui/widget/sat_root_widget.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"

// SAT_EditorQueues

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
  
  SAT_Widget*         MHoverWidget        = nullptr;
  SAT_Widget*         MModalWidget        = nullptr;
  SAT_Widget*         MMouseCaptureWidget = nullptr;
  SAT_Widget*         MKeyCaptureWidget   = nullptr;

  int32_t             MMouseCurrentCursor = SAT_CURSOR_DEFAULT;
  SAT_WidgetArray     MTimerListeners     = {};

  int32_t             MMouseCurrentXpos   = 0;
  int32_t             MMouseCurrentYpos   = 0;
  
  bool                MMouseLocked        = false;
  int32_t             MMouseLockedX       = 0;
  int32_t             MMouseLockedY       = 0;

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

  void setListener(SAT_WindowListener* AListener) {
    MListener = AListener;
  }

  //----------

  void setRootWidget(SAT_RootWidget* ARoot) {
    MRootWidget = ARoot;
  }

//------------------------------
public:
//------------------------------

  // void show() override {
  //   SAT_TRACE;
  //   if (MRootWidget) MRootWidget->on_widget_realign();
  //   SAT_BufferedWindow::show();
  // };

  //----------

  // void hide() override {
  //   SAT_BufferedWindow::hide();
  // };

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
          // if (MMouseCaptured) {
          // }
          // else {
          // }
          if (MHoverWidget) MHoverWidget->on_widget_leave(hover,AXpos,AYpos,ATime);
          hover->on_widget_enter(MHoverWidget,AXpos,AYpos,ATime);
          MHoverWidget = hover;
          //SAT_PRINT("hover: %s\n",MHoverWidget->getName());
        }
      }
      else {
        if (MHoverWidget) MHoverWidget->on_widget_leave(nullptr,AXpos,AYpos,ATime);
        MHoverWidget = nullptr;
        //SAT_PRINT("hover: %s\n",MHoverWidget->getName());
      }
    }
  }

  //----------

  // called from
  // on_widgetListener_redraw

  void queueDirtyWidget(SAT_Widget* AWidget) {
    // SAT_PRINT("%s\n",AWidget->getName());
    if (!MDirtyWidgets.write(AWidget)) {
    }
  }

  //----------

  // called from timer

  void flushDirtyWidgets() {
    // SAT_TRACE;
    uint32_t count = 0;
    SAT_Widget* widget;
    while (MDirtyWidgets.read(&widget)) {
      count += 1;

      SAT_Rect rect = widget->getRect();
      invalidate(rect.x,rect.y,rect.w,rect.h);

    }
  }

  //----------

  virtual void lockMouseCursor() {
    MMouseLocked  = true;
    MMouseLockedX = MMouseCurrentXpos;
    MMouseLockedY = MMouseCurrentYpos;
    // MMouseClickedXpos = MMouseXpos;
    // MMouseClickedYpos = MMouseYpos;
  }

  //----------

  virtual void unlockMouseCursor() {
    MMouseLocked = false;
  }  

//------------------------------
public: // window
//------------------------------

  /*
    don't realign every time we hide/show editor? (in plugin)
    only first time we open it
  */

  void on_window_show() override {
//    SAT_PRINT("show start\n");
    SAT_BufferedWindow::on_window_show();
    if (MRootWidget) {
      MRootWidget->ownerWindowOpened(this);
      // MRootWidget->setSkin(&MDefaultSkin,true,true);
      uint32_t w = getWidth();// * MScale;
      uint32_t h = getHeight();// * MScale;
      MRootWidget->setSize(w,h);
      // MRootWidget->realignChildren();
      // markRootWidgetDirty();
    }
//    SAT_PRINT("show end\n");
  }

  //----------

  void on_window_hide() override {
    //SAT_PRINT("\n");
    if (MRootWidget) MRootWidget->closeOwnerWindow(this);
    SAT_BufferedWindow::on_window_hide();
  }

  //----------
  
  void on_window_paint(SAT_PaintContext* AContext) override {
    //SAT_BufferedWindow::on_window_paint(AContext);
    SAT_Rect rect = AContext->update_rect;
    if (MRootWidget) {
      //SAT_PRINT("%.f,%.f,%.f,%.f\n",rect.x,rect.y,rect.w,rect.h);
      MRootWidget->on_widget_paint(AContext);
    }
  }

  //----------

  // void on_window_move(int32_t AXpos, int32_t AYpos) override {
  //   //SAT_BufferedWindow::on_window_move(AXpos,AYpos);
  //   //SAT_PRINT("x %i y %i\n",AXpos,AYpos);
  // }

  //----------
  
  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    //SAT_BufferedWindow::on_window_resize(AWidth,AHeight);
    SAT_PRINT("w %i h %i\n",AWidth,AHeight);
    if (MRootWidget) {
      // resize buffer?
      MRootWidget->on_widget_resize(AWidth,AHeight);
      MRootWidget->realignChildren();
    }
  }

  //----------
  
  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_PRINT("x %i y %i b %i s %i\n",AXpos,AYpos,AButton,AState);
    MMouseClickedXpos   = AXpos;
    MMouseClickedYpos   = AYpos;
    MMouseClickedButton = AButton;

    // if widget is already captured, 
    // send further clicks to the same widget..
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
    //SAT_PRINT("x %i y %i b %i s %i\n",AXpos,AYpos,AButton,AState);

    // MMouseClickedXpos   = AXpos;
    // MMouseClickedYpos   = AYpos;
    // MMouseClickedButton = AButton;

    if (MMouseCaptured) {

      MMouseCaptureWidget->on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATime);

      // end capture if we release the same button as
      // the one we started the dragging with
      if (AButton == MMouseCaptureButton) {
        //SAT_PRINT("release\n");
        MMouseCaptured = false;

        updateHover(AXpos,AYpos,ATime);

      }

    }

    // else {
    //   // if mouse not capture, we do nothing
    // }

  }

  //----------
  
  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    //SAT_PRINT("x %i y %i s %i\n",AXpos,AYpos,AState);
    MMouseCurrentXpos = AXpos;
    MMouseCurrentYpos = AYpos;

    // when we set the mouse cursor back to its locked pos, the os/system might send us a new mouse move event as a result.. ignore it..
    if (MMouseLocked) {
      if ((AXpos == MMouseLockedX) && (AYpos == MMouseLockedY)) {
        // SAT_TRACE;
        return;
      }
    }

    // send mouse move events only to captured widget
    if (MMouseCaptured) {
      MMouseCaptureWidget->on_widget_mouse_move(AXpos,AYpos,AState,ATime);
      // updateHoverCaptured(AXpos,AYpos,ATime);
    }
    else {
      updateHover(AXpos,AYpos,ATime);
    }

    // if mouse locked, set it back to locked position
    // (after processing it)

    if (MMouseLocked) {
      setMousePos(MMouseLockedX,MMouseLockedY);
    }

  }

  //----------
  
  void on_window_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
    //SAT_PRINT("k %i c %i s %i\n",AKey,AChar,AState);
    if (MKeyCaptureWidget) {
      //SAT_PRINT("key press\n");
      MKeyCaptureWidget->on_widget_key_press(AKey,AChar,AState,ATime);
    }
  }

  //----------
  
  void on_window_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
    //SAT_PRINT("k %i c %i s %i\n",AKey,AChar,AState);
    if (MKeyCaptureWidget) {
      //SAT_PRINT("key release\n");
      MKeyCaptureWidget->on_widget_key_release(AKey,AChar,AState,ATime);
    }
  }

  //----------
  
  void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_PRINT("x %i y %i\n",AXpos,AYpos);
    updateHover(AXpos,AYpos,ATime);
  }

  //----------
  
  void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_PRINT("x %i y %i\n",AXpos,AYpos);
    //updateHover(AXpos,AYpos,ATime);
    if (MHoverWidget) MHoverWidget->on_widget_leave(nullptr,AXpos,AYpos,ATime);
    MHoverWidget = nullptr;
    //SAT_PRINT("hover: %p\n",MHoverWidget);

  }

  //----------
  
  void on_window_clientMessage(uint32_t AData) override {
    //SAT_PRINT("d %i\n",AData);
  }

//------------------------------
public: // timer listener
//------------------------------

  void on_timerListener_callback(SAT_Timer* ATimer, double ADelta) override {
    //SAT_TRACE;
    if (MListener) MListener->on_windowListener_timer(ATimer,ADelta);
    for (uint32_t i=0; i<MTimerListeners.size(); i++) {
      MTimerListeners[i]->on_widget_timer(ADelta);
    }
    // todo: tweening
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
    return getScale();
  }

  //----------

  // SAT_Window* _getWindow() override {
  //   return this;
  // }

  //void on_widgetOwner_wantTimerEvents(bool AWant=true) override {
  //}

//------------------------------
public: // widget listener
//------------------------------

  void on_widgetListener_update(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    if (MListener) {
      //SAT_TRACE;
      MListener->on_windowListener_update(AWidget,AMode);
    }
  }

  //----------

  void on_widgetListener_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) override {
    //SAT_PRINT("%s\n",AWidget->getName());
    //markWidgetDirtyFromGui(AWidget);
    #ifdef SAT_WINDOW_TIMER_REFRESH_WIDGETS
      queueDirtyWidget(AWidget);
    #else
      SAT_Rect rect = AWidget->getRect();
      invalidate(rect.x,rect.y,rect.w,rect.h);
    #endif

  }

  //----------

  void on_widgetListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) override {
    // SAT_Widget* parent = AWidget->getParent();
    // if (parent) {
    //   parent->realignChildWidgets();
    //   markWidgetDirtyFromGui(parent);
    // }    
  }

  //----------

  void on_widgetListener_notify(SAT_Widget* AWidget, int32_t AValue) override {
    // if (MListener) MListener->on_windowListener_notify(AWidget,AValue);
  }
  
  //----------

  void on_widgetListener_mouse_capture(SAT_Widget* AWidget) override {
    MMouseCaptureWidget = AWidget;
    // if (MListener) MListener->on_windowListener_mouse_capture(AWidget);
  }
  
  //----------

  void on_widgetListener_key_capture(SAT_Widget* AWidget) override {
    MKeyCaptureWidget = AWidget;
    // if (MListener) MListener->on_windowListener_key_capture(AWidget);
  }
  
  //----------

  void on_widgetListener_modal(SAT_Widget* AWidget) override {
    MModalWidget = AWidget;
    // if (MListener) MListener->on_windowListener_modal(AWidget);
  }

  //----------

  void on_widgetListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    // //SAT_Print("%s\n",AWidget->getName());
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
      // if (MListener) MListener->on_windowListener_set_hint(AWidget,AHint);
    }
  }

  //----------

  void on_widgetListener_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    //SAT_TRACE;
    if (AWantTimer) MTimerListeners.append(AWidget);
    else MTimerListeners.remove(AWidget);
  }

};

//----------------------------------------------------------------------
#endif