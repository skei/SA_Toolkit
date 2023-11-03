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
    SAT_PRINT;
  }

  //----------

  virtual ~SAT_Window() {
  }

//------------------------------
public:
//------------------------------

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

//------------------------------
public: // base window
//------------------------------

  void on_window_open() override {
    SAT_Print("\n");
    //MRootWidget->on_widget_open(this);
    //SAT_PaintWindow::on_window_open();
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
    //SAT_PaintWindow::on_window_close();
  }
  
  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    SAT_Print("AXpos %i AYpos %i\n",AXpos,AYpos);
    //SAT_PaintWindow::on_window_move(AXpos,AYpos);
  }
  
  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    SAT_Print("AWidth %i AHeight %i\n",AWidth,AHeight);
    //SAT_PaintWindow::on_window_move(AWidth,AHeight);
    if (MRootWidget) {
      MRootWidget->setSize(SAT_Point(AWidth,AHeight));
      MRootWidget->realignChildWidgets();
      markWidgetDirty(MRootWidget);
    }
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIsPainting = true;
    SAT_Print("AXpos %i AYpos %i AWidth %i AHeight %i\n",AXpos,AYpos,AWidth,AHeight);
    SAT_BasePainter* painter  = getPainter();
    MPaintContext.painter     = painter;
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.scale       = 1.0;
    //SAT_BaseRenderer* renderer = getRenderer();         // -> lower level (x11/wayland, ..)
    //renderer->beginRendering(0,0,AWidth,AHeight);       // -> lower level
    painter->beginPaint(AXpos,AYpos,AWidth,AHeight);

    // queued widgets -> buffer

    painter->beginFrame(AWidth,AHeight,1);
    //paintRootWidget(&MPaintContext);

      // hack, paints directly to screen, now..
      // called from on_timerListener_timer()
      handleDirtyWidgets();

    paintQueuedWidgets(&MPaintContext);
    painter->endFrame();

    // buffer -> screen
    // todo

    painter->endPaint();
    //renderer->endRendering();                           // -> lower level
    MPaintContext.counter += 1;
    MIsPainting = false;
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
  }
  
  //----------

  void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_Print("\n");
  }
  
  //----------

  void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_Print("\n");
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

  void do_timerListener_callback(SAT_Timer* ATimer) override {
    SAT_Print("\n");
    if (MIsPainting) return;
    SAT_Rect rect = handleDirtyWidgets();
    invalidate(rect.x,rect.y,rect.w,rect.h);
  }

//------------------------------
public:
//------------------------------

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

//------------------------------
private:
//------------------------------

  //void paintRootWidget(SAT_PaintContext* AContext) {
  //  if (MRootWidget) {
  //    MRootWidget->on_widget_paint(AContext);
  //  }
  //}

  //----------

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

  void updateHoverWidget(int32_t AXpos, int32_t AYpos) {
  }

  //----------

  // null = off

  //void modalize(SAT_Widget* AWidget) {
  //  //if (MModalWidget) MModalWidget.unmodalize();
  //  //MModalWidget->modalize();
  //  MModalWidget = AWidget;
  //}

};

//----------------------------------------------------------------------
#endif