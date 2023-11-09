#ifndef sat_root_widget_included
#define sat_root_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/widgets/sat_panel_widget.h"
//#include "gui/sat_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_RootWidget
//: public SAT_Widget {
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  SAT_WidgetListener* MListener = nullptr;

//------------------------------
public:
//------------------------------

  SAT_RootWidget(SAT_Rect ARect, SAT_WidgetListener* AListener)
  //: SAT_Widget(ARect) {
  : SAT_PanelWidget(ARect) {
    MListener = AListener;
    setName("MRootWidget");
  }

  //----------

  virtual ~SAT_RootWidget() {
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_update(AWidget);
  }

  //----------

  void do_widget_redraw(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_redraw(AWidget);
  }

  //----------

  void do_widget_realign(SAT_Widget* AWidget) override {
    if (MListener) MListener->on_widgetListener_realign(AWidget);
  }

  //----------

  void do_widget_setCursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MListener) MListener->on_widgetListener_setCursor(AWidget,ACursor);
  }

};



//----------------------------------------------------------------------
#endif
