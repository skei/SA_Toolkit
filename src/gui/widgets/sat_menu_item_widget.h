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
  SAT_Color MHoverBackgroundColor = SAT_DarkerGray;

  SAT_Color MTextColor            = SAT_White;
  SAT_Color MBackgroundColor      = SAT_DarkGray;

//------------------------------
public:
//------------------------------

  SAT_MenuItemWidget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setCursor(SAT_CURSOR_FINGER);
    setBorderColor(SAT_DarkerGray);
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
    setTextColor(MHoverTextColor);
    setBackgroundColor(MHoverBackgroundColor);
    redraw();
    SAT_TextWidget::on_widget_mouse_enter(AFrom,AXpos,AYpos,ATime);

  }

  void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    setTextColor(MTextColor);
    setBackgroundColor(MBackgroundColor);
    redraw();
    SAT_TextWidget::on_widget_mouse_leave(ATo,AXpos,AYpos,ATime);
  }

  void on_widget_notify(uint32_t AReason=0, int32_t AValue=0) override {
    SAT_TextWidget::on_widget_notify(AReason,AValue);
  }




};

//----------------------------------------------------------------------
#endif
