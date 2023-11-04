#ifndef sat_root_widget_included
#define sat_root_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget.h"
#include "gui/base/sat_widget_listener.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_RootWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

  SAT_WidgetListener* MListener = nullptr;

//------------------------------
public:
//------------------------------

  SAT_RootWidget(SAT_Rect ARect, SAT_WidgetListener* AListener)
  : SAT_Widget(ARect) {
    MListener = AListener;
    setName("MRootWidget");
  }

  //----------

  virtual ~SAT_RootWidget() {
  }

//------------------------------
public: // widget listener
//------------------------------

  virtual void do_widget_update_value(SAT_Widget* AWidget) {
    if (MListener) MListener->on_widgetListener_update_value(AWidget);
  }

  //----------

  virtual void do_widget_redraw(SAT_Widget* AWidget) {
    if (MListener) MListener->on_widgetListener_redraw(AWidget);
  }

  //----------

  virtual void do_widget_realign(SAT_Widget* AWidget) {
    if (MListener) MListener->on_widgetListener_realign(AWidget);
  }

  //----------

  virtual void do_widget_setCursor(SAT_Widget* AWidget, uint32_t ACursor) {
    if (MListener) MListener->on_widgetListener_setCursor(AWidget,ACursor);
  }

};



//----------------------------------------------------------------------
#endif
