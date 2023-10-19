#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/base/sat_window_listener.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------
//
// basic window
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
// paint window
//
//----------------------------------------------------------------------

class SAT_PaintWindow
: public SAT_ImplementedWindow {

//------------------------------
protected:
//------------------------------

  SAT_WindowListener* MListener           = nullptr;
  SAT_PaintContext    MPaintContext       = {};

//------------------------------
public:
//------------------------------

  SAT_PaintWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MPaintContext.painter = getPainter();
    MPaintContext.update_rect = SAT_Rect(0,0,AWidth,AHeight);
  }

  //----------

  virtual ~SAT_PaintWindow() {
  }

//------------------------------
public:
//------------------------------

  virtual void setListener(SAT_WindowListener* AListener) {
    MListener = AListener;
  }

//------------------------------
public: // base window
//------------------------------

  void on_window_open() override {
    //SAT_Print("\n");
  }

  void on_window_close() override {
    //SAT_Print("\n");
  }
  
  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("\n");
  }

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("\n");
    // SAT_BaseRenderer* renderer = getRenderer();
    // renderer->beginRendering(0,0,AWidth,AHeight);
    // //...
    // renderer->endRendering();
  }
  
};

//----------------------------------------------------------------------
//
// widget window
//
//----------------------------------------------------------------------

class SAT_WidgetWindow
: public SAT_PaintWindow
, public SAT_WidgetOwner
, public SAT_WidgetListener {

//------------------------------
private:
//------------------------------

  SAT_Widget*         MRootWidget         = nullptr;
  SAT_Widget*         MHoverWidget        = nullptr;
  SAT_Widget*         MModalWidget        = nullptr;
  SAT_Widget*         MInteractiveWidget  = nullptr;
  SAT_Widget*         MMouseCaptureWidget = nullptr;
  SAT_Widget*         MKeyCaptureWidget   = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WidgetWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_PaintWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_WidgetWindow() {
  }

//------------------------------
public:
//------------------------------

  virtual void setRootWidget(SAT_Widget* AWidget) {
    MRootWidget = AWidget;
    MRootWidget->setParent(nullptr);
    //MRootWidget->setListener(this);
    MRootWidget->setOwner(this);
  }

  virtual void setListener(SAT_WindowListener* AListener) {
    MListener = AListener;
  }

//------------------------------
public: // base window
//------------------------------

  void on_window_open() override {
    //SAT_Print("\n");
    //SAT_PaintWindow::on_window_open();
    if (MRootWidget) {
      //MRootWidget->on_widget_open(this);
      MRootWidget->setOwner(this);
    }
  }

  void on_window_close() override {
    //SAT_Print("\n");
    //SAT_PaintWindow::on_window_close();
  }
  
  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //SAT_Print("\n");
    //SAT_PaintWindow::on_window_move(AXpos,AYpos);
  }
  
  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("\n");
    //SAT_PaintWindow::on_window_move(AWidth,AHeight);
    if (MRootWidget) {
      MRootWidget->setSize(SAT_Point(AWidth,AHeight));
      MRootWidget->realignChildWidgets();
      //repaint?
    }
  }

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //SAT_Print("\n");
    //SAT_PaintWindow::on_window_paint(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.update_rect = SAT_Rect(AXpos,AYpos,AWidth,AHeight);
    SAT_BaseRenderer* renderer = getRenderer();
    renderer->beginRendering(0,0,AWidth,AHeight);
    SAT_BasePainter* painter = MPaintContext.painter;
    painter->beginPaint(AXpos,AYpos,AWidth,AHeight);
    painter->beginFrame(AWidth,AHeight,1);
    if (MRootWidget) {
      MRootWidget->on_widget_paint(&MPaintContext);
    }
    painter->endFrame();
    painter->endPaint();
    renderer->endRendering();
  }
  
  void on_window_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  void on_window_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
  void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //SAT_Print("\n");
  }
  
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

  //void on_widgetListener_update_value(SAT_Widget* AFrom) override {
  //  //SAT_PRINT;
  //}

  //void on_widgetListener_redraw(SAT_Widget* AFrom) override {
  //  //SAT_PRINT;
  //}

  //void on_widgetListener_realign(SAT_Widget* AFrom) override {
  //  //SAT_PRINT;
  //}

};

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

typedef SAT_WidgetWindow SAT_Window;

//----------------------------------------------------------------------
#endif