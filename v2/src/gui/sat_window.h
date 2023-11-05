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

  SAT_RootWidget*     MRootWidget         = nullptr;
  SAT_WindowListener* MListener           = nullptr;

  sat_atomic_bool_t   MIsPainting         = false;
  SAT_PaintContext    MPaintContext       = {};
  SAT_Timer           MTimer              = SAT_Timer(this);
  SAT_WidgetQueue     MDirtyWidgets       = {};
  SAT_WidgetQueue     MPaintWidgets       = {};

  SAT_Widget*         MHoverWidget        = nullptr;
  SAT_Widget*         MModalWidget        = nullptr;
  SAT_Widget*         MInteractiveWidget  = nullptr;
  SAT_Widget*         MMouseCaptureWidget = nullptr;
  SAT_Widget*         MKeyCaptureWidget   = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    SAT_PRINT;
    MPaintContext.painter = getPainter();
    MPaintContext.update_rect = SAT_Rect(0,0,AWidth,AHeight);
    MPaintContext.counter = 0;
  }

  //----------

  virtual ~SAT_Window() {
    //delete MPainter;
    //delete MRenderer;
  }

//------------------------------
public:
//------------------------------

  bool isPainting() {
    return MIsPainting;
  }

  //----------

  virtual void setRootWidget(SAT_RootWidget* AWidget) {
    MRootWidget = AWidget;
    MRootWidget->setParent(nullptr);
    //MRootWidget->setListener(this);
    MRootWidget->setOwner(this);
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

  virtual void startTimer(uint32_t AMSInterval, bool AOneShot) {
    MTimer.start(AMSInterval,AOneShot);
  }

  //----------

  virtual void stopTimer() {
    MTimer.stop();
  }

  //----------

  // virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
  //   MInitialWidth = AWidth;
  //   MInitialHeight = AHeight;
  //   MScale = recalcScale(MWindowWidth,MWindowHeight);
  // }

  //----------

  // virtual double recalcScale(int32_t AWidth, int32_t AHeight) {
  //   double scale = 1.0;
  //   if ((MInitialWidth > 0) && (MInitialHeight > 0)) {
  //     double xscale = (double)AWidth / (double)MInitialWidth;
  //     double yscale = (double)AHeight / (double)MInitialHeight;
  //     if (xscale < yscale) scale = xscale;
  //     else scale =  yscale;
  //   }
  //   return scale;
  // }
  
//------------------------------
public: // cursor
//------------------------------

  // virtual void lockMouseCursor() {
  //   MMouseIsLocked  = true;
  //   MLockedClickedX = MMouseCurrentXpos;
  //   MLockedClickedY = MMouseCurrentYpos;
  //   MLockedCurrentX = MMouseCurrentXpos;
  //   MLockedCurrentY = MMouseCurrentYpos;
  // }

  //----------

  // virtual void unlockMouseCursor() {
  //   MMouseIsLocked = false;
  // }

//------------------------------
private: // buffer
//------------------------------

  void createBuffer() {
  }

  void deleteBuffer() {
  }

  void resizeBuffer() {
  }

  // void copyBuffer(void* ADst, int32_t ADstXpos, int32_t ADstYpos, uint32_t ADstWidth, uint32_t ADstHeight, void* ASrc, int32_t ASrcXpos, int32_t ASrcYpos, uint32_t ASrcWidth, uint32_t ASrcHeight) {
  //   MWindowPainter->selectRenderBuffer(ADst,ADstWidth,ADstHeight);
  //   MWindowPainter->beginFrame(ADstWidth,ADstHeight);
  //   int32_t image = MWindowPainter->getImageFromRenderBuffer(ASrc);
  //   MWindowPainter->setFillImage(image, ADstXpos,ADstYpos, 1,1, 1.0, 0.0);
  //   MWindowPainter->fillRect(ASrcXpos,ASrcYpos,ASrcWidth,ASrcHeight);
  //   MWindowPainter->endFrame();
  // }  

//------------------------------
private:
//------------------------------

  // (should be) called from a timer.. checks for widgets marked dirtys,
  // and inserts them all into the paint queue..
  // then we (should, in the timer) send an 'invalidate' message to the sys/os
  // to signal a 'redraw, please!')
  // (and we continue  in paintQueuedWidgets, in the gui thread)
  //todo: check doubles, ..

  SAT_Rect handleDirtyWidgets() {
    SAT_Rect rect;
    SAT_Widget* widget;
    while (MDirtyWidgets.read(&widget)) {
      MPaintWidgets.write(widget);
      rect.combine(widget->getRect());
    }
    return rect;
  }

  //----------

  // (should be) called during on_window_paint
  // we paint all widgets to the background buffer to keep it up to date
  // (but blir only the update rectangle)

  void paintQueuedWidgets(SAT_PaintContext* AContext) {
    SAT_Widget* widget;
    uint32_t count = 0;
    while (MPaintWidgets.read(&widget)) {
      count += 1;
      widget->on_widget_paint(AContext);
    }
    SAT_Print("%i\n",count);
    SAT_Rect rect = AContext->update_rect;
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
            SAT_Print("leaving %s\n",MHoverWidget->getName());
            MHoverWidget->on_widget_leave(hover,AXpos,AYpos);
          }
          SAT_Print("entering %s\n",hover->getName());
          hover->on_widget_enter(MHoverWidget,AXpos,AYpos);
          MHoverWidget = hover;
          // cursor
          // hint
        }
      }
      else {
        if (MHoverWidget) {
          SAT_Print("leaving %s\n",MHoverWidget->getName());
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

  //----------
  
  // void updateLockedMouse(int32_t AXpos, int32_t AYpos) {
  //   if ((AXpos == MLockedClickedX) && (AYpos == MLockedClickedY)) {
  //     MMousePreviousXpos = AXpos;
  //     MMousePreviousYpos = AYpos;
  //     return;
  //   }
  //   int32_t xdiff = AXpos - MLockedClickedX;
  //   int32_t ydiff = AYpos - MLockedClickedY;
  //   MLockedCurrentX += xdiff;
  //   MLockedCurrentY += ydiff;
  //   setMouseCursorPos(MLockedClickedX,MLockedClickedY);
  // }

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
  }

  //----------

  void on_window_close() override {
    SAT_Print("\n");
  }
  
  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    SAT_Print("AXpos %i AYpos %i\n",AXpos,AYpos);
  }
  
  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    SAT_Print("AWidth %i AHeight %i\n",AWidth,AHeight);
    if (MRootWidget) {
      MRootWidget->setSize(SAT_Point(AWidth,AHeight));
      MRootWidget->realignChildWidgets();
      markWidgetDirty(MRootWidget);
    }
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    SAT_Print("AXpos %i AYpos %i AWidth %i AHeight %i\n",AXpos,AYpos,AWidth,AHeight);

    // transfers all widgets in dirtyWidgets to paintQueue
    // this shuld be called from a timer
    handleDirtyWidgets();

    //

    SAT_BasePainter* painter  = getPainter();
    MPaintContext.painter     = painter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.scale       = 1.0;
    MPaintContext.counter     += 1; // start from 1 (and increase)..

    MIsPainting = true;  // signal timer to f.off

    painter->beginPaint(AXpos,AYpos,AWidth,AHeight); // set up nanovg
    // todo: select render buffer
    painter->beginFrame(AWidth,AHeight,1);

      // calls on_widget_paint on all widgets in paintQueue
      // (added by handleDirtyWidgets)
      paintQueuedWidgets(&MPaintContext);

    painter->endFrame(); // nanovg cleanup?

    // todo: unselect render buffer
    // blit updaterecr from render buffer

    painter->endPaint();  // swapBuffers

    MIsPainting = false; // timer ok..

  }
  
  //----------

  void on_window_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    SAT_Print("\n");
  }
  
  //----------

  void on_window_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
    updateHoverWidget(AXpos,AYpos);
  }
  
  //----------

  void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_Print("\n");
    //if (MRootWidget) {
    //  MRootWidget->on_widget_enter(nullptr,AXpos,AYpos);
    //}
    MHoverWidget = nullptr;
    updateHoverWidget(AXpos,AYpos);
  }
  
  //----------

  void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_Print("\n");
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
    SAT_Print("AData %i\n",AData);
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

  void on_widgetListener_update_value(SAT_Widget* AWidget) override {
    SAT_Print("\n");
  }

  //----------

  void on_widgetListener_redraw(SAT_Widget* AWidget) override {
    SAT_Print("\n");
    //MDirtyWidgets.write(AWidget);
  }

  //----------

  void on_widgetListener_realign(SAT_Widget* AWidget) override {
    SAT_Print("\n");
  }

  //----------

  void on_widgetListener_setCursor(SAT_Widget* AWidget, uint32_t ACursor) override {
    SAT_Print("ACursor %i\n",ACursor);
  }

//------------------------------
public: // timer listener
//------------------------------

  // if we are currently painting (not finished with the previous frame),
  // we just return..
  // and test again next time the timer fires..

  void do_timerListener_callback(SAT_Timer* ATimer) override {
    SAT_Print("\n");
    if (MIsPainting) return;
    SAT_Rect rect = handleDirtyWidgets();
    invalidate(rect.x,rect.y,rect.w,rect.h);
  }


};

//----------------------------------------------------------------------
#endif