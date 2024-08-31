#ifndef sat_root_widget_included
#define sat_root_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_gui_base.h"
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

  SAT_RootWidget(SAT_WidgetListener* AListener=nullptr)
  : SAT_VisualWidget(0) {
    setName("SAT_RootWidget");
    setHint("SAT_RootWidget");
    MListener = AListener;
    Options.opaque = true;
  }

  //----------

  virtual ~SAT_RootWidget() {
  }

//------------------------------
public:
//------------------------------

  void setListener(SAT_WidgetListener* AListener) {
    MListener = AListener;
  }

//------------------------------
public: // on_widget
//------------------------------

  // void on_Widget_open(SAT_WidgetOwner* AOwner) override {
  // }

  // void on_Widget_close() override {
  // }

  // void on_Widget_move(int32_t AXpos, int32_t AYpos) override {
  // }

  void on_Widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    setSize(AWidth,AHeight);
    SAT_VisualWidget::on_Widget_resize(AWidth,AHeight);
  }

  // void on_Widget_realign() override {
  // }

  // void on_Widget_paint(SAT_PaintContext* AContext) override {
  // }

  // void on_Widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_Widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_Widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_Widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_Widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_Widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  // }

  // void on_Widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  // }

  // void on_Widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
  // }

  // void on_Widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
  // }

  // void on_Widget_timer(uint32_t AId, double ADelta) override {
  // }

//------------------------------
public: // do_widget 
//------------------------------

  // widget -> listener (window)

  void do_Widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    if (MListener) MListener->on_WidgetListener_update(AWidget,AIndex,AMode);
  }
  
  void do_Widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    if (MListener) MListener->on_WidgetListener_redraw(AWidget,AIndex,AMode);
  }

  void do_Widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) override {
    if (MListener) MListener->on_WidgetListener_realign(AWidget,AMode);
  }
  
  void do_Widget_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr=nullptr) override {
    if (MListener) MListener->on_WidgetListener_notify(AWidget,AValue,APtr);
  }
  
  void do_Widget_mouse_capture(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_WidgetListener_mouse_capture(AWidget);
  }
  
  void do_Widget_key_capture(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_WidgetListener_key_capture(AWidget);
  }
  
  void do_Widget_modal(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_WidgetListener_modal(AWidget);
  }

  void do_Widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MListener) MListener->on_WidgetListener_set_cursor(AWidget,ACursor);
  }
  
  void do_Widget_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (MListener) MListener->on_WidgetListener_set_hint(AWidget,AHint);
  }

  void do_Widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    if (MListener) MListener->on_WidgetListener_want_timer(AWidget,AWantTimer);
  }

  void do_Widget_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) override {
    if (MListener) MListener->on_WidgetListener_start_tween(AWidget,ATween);
  }

  void do_Widget_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
    if (MListener) MListener->on_WidgetListener_resize(AWidget,ADeltaX,ADeltaY);
  }

  void do_Widget_set_overlay(SAT_Widget* AWidget, SAT_Color AColor) override {
    if (MListener) MListener->on_WidgetListener_set_overlay(AWidget,AColor);
  }



};

//----------------------------------------------------------------------
#endif