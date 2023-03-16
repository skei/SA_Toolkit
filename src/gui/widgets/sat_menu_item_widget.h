#ifndef sat_menu_item_widget_included
#define sat_menu_item_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_text_widget.h"

class SAT_MenuItemWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  SAT_Color MHoverTextColor       = SAT_White;
  SAT_Color MHoverBackgroundColor = SAT_DarkerGrey;

  SAT_Color MTextColor            = SAT_White;
  SAT_Color MBackgroundColor      = SAT_DarkGrey;

//------------------------------
public:
//------------------------------

  SAT_MenuItemWidget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setCursor(SAT_CURSOR_FINGER);
    setBorderColor(SAT_DarkerGrey);
    setFillBackground(true);
    setTextColor(MTextColor);
    setBackgroundColor(MBackgroundColor);
  }

  //----------

  virtual ~SAT_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
    SAT_TextWidget::on_widget_mouse_enter(AFrom,AXpos,AYpos,ATime);
    //if (isVisible()) {
      setTextColor(MHoverTextColor);
      setBackgroundColor(MHoverBackgroundColor);
      redraw();
    //}
  }

  //----------

  // we receive leave msg _after_ the menu has closed.. :-/
  // so we added the 'isVisible' hack..
  // this should probably be fixed so we don't get called when menu closes...

  void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    SAT_TextWidget::on_widget_mouse_leave(ATo,AXpos,AYpos,ATime);
    if (isVisible()) {
      setTextColor(MTextColor);
      setBackgroundColor(MBackgroundColor);
      redraw();
    }
  }

  //----------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_TextWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
    if (AButton == SAT_BUTTON_LEFT) {
      setTextColor(MTextColor);
      setBackgroundColor(MBackgroundColor);
      uint32_t index = getIndex();
      do_widget_notify(this,SAT_WIDGET_NOTIFY_SELECT,index);
    }
    if (AButton == SAT_BUTTON_RIGHT) {
      setTextColor(MTextColor);
      setBackgroundColor(MBackgroundColor);
      do_widget_notify(this,SAT_WIDGET_NOTIFY_CLOSE,0);
    }
  }

};

//----------------------------------------------------------------------
#endif
