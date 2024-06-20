#ifndef sat_root_widget_included
#define sat_root_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/widget/sat_visual_widget.h"
#include "gui/sat_widget.h"

//----------

class SAT_RootWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  SAT_WidgetListener* MListener     = nullptr;

//------------------------------
public:
//------------------------------

  SAT_RootWidget(SAT_WidgetListener* AListener, SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_RootWidget");
    setHint("SAT_RootWidget");
    MListener = AListener;
  }

  //----------

  virtual ~SAT_RootWidget() {
  }

//------------------------------
public: // on_widget
//------------------------------

  // void on_widget_open(SAT_WidgetOwner* AOwner) override {
  // }

  // void on_widget_close() override {
  // }

  // void on_widget_move(int32_t AXpos, int32_t AYpos) override {
  // }

  void on_widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    setSize(AWidth,AHeight);
    SAT_VisualWidget::on_widget_resize(AWidth,AHeight);
  }

  // void on_widget_realign() override {
  // }

  // void on_widget_paint(SAT_PaintContext* AContext) override {
  // }

  // void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  // }

  // void on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  // }

  // void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
  // }

  // void on_widget_timer(uint32_t AId, double ADelta) override {
  // }

//------------------------------
public: // do_widget 
//------------------------------

  // widget -> listener (window)

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    //SAT_PRINT("%s\n",AWidget->getName());
    if (MListener) MListener->on_widgetListener_update(AWidget,AIndex,AMode);
  }
  
  void do_widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) override {
    if (MListener) MListener->on_widgetListener_redraw(AWidget,AIndex,AMode);
  }

  void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) override {
    if (MListener) MListener->on_widgetListener_realign(AWidget,AMode);
  }
  
  void do_widget_notify(SAT_Widget* AWidget, int32_t AValue) override {
    if (MListener) MListener->on_widgetListener_notify(AWidget,AValue);
  }
  
  void do_widget_mouse_capture(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_mouse_capture(AWidget);
  }
  
  void do_widget_key_capture(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_key_capture(AWidget);
  }
  
  void do_widget_modal(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_modal(AWidget);
  }

  void do_widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MListener) MListener->on_widgetListener_set_cursor(AWidget,ACursor);
  }
  
  void do_widget_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (MListener) MListener->on_widgetListener_set_hint(AWidget,AHint);
  }

  void do_widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    if (MListener) MListener->on_widgetListener_want_timer(AWidget,AWantTimer);
  }

  void do_widget_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) override {
    if (MListener) MListener->on_widgetListener_start_tween(AWidget,ATween);
  }

  void do_widget_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
    if (MListener) MListener->on_widgetListener_resize(AWidget,ADeltaX,ADeltaY);
  }

  void do_widget_set_overlay(SAT_Widget* AWidget, SAT_Color AColor) override {
    if (MListener) MListener->on_widgetListener_set_overlay(AWidget,AColor);
  }



};

//----------------------------------------------------------------------
#endif