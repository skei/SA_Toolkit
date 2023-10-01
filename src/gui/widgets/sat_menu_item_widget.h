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

  SAT_Color MSavedTextColor       = SAT_White;
  SAT_Color MSavedBackgroundColor = SAT_DarkGrey;

//------------------------------
public:
//------------------------------

  SAT_MenuItemWidget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_MenuItemWidget");
    setCursor(SAT_CURSOR_FINGER);
    setDrawBorder(false);
    //setBorderColor(SAT_DarkerGrey);
    setFillBackground(true);
//    setBackgroundColor(MBackgroundColor);
//    setTextColor(MTextColor);
    setAlignment(SAT_WIDGET_ALIGN_TOP_LEFT);
    setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    
  }

  //----------

  virtual ~SAT_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------

//  virtual void setTextColor(SAT_Color AColor)             { MTextColor = AColor; }
//  virtual void setBackgroundColor(SAT_Color AColor)       { MBackgroundColor = AColor; }
  virtual void setHoverTextColor(SAT_Color AColor)        { MHoverTextColor = AColor; }
  virtual void setHoverBackgroundColor(SAT_Color AColor)  { MHoverBackgroundColor = AColor; }
  
//------------------------------
public:
//------------------------------

  void on_widget_mouse_enter(SAT_BaseWidget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
    SAT_TextWidget::on_widget_mouse_enter(AFrom,AXpos,AYpos,ATime);
    //if (isVisible()) {
      MSavedTextColor = getTextColor();
      MSavedBackgroundColor = getBackgroundColor();
      setTextColor(MHoverTextColor);
      setBackgroundColor(MHoverBackgroundColor);
      //parentRedraw();
      do_widgetListener_redraw(this,0);
    //}
  }

  //----------

  // we receive leave msg _after_ the menu has closed.. :-/
  // so we added the 'isVisible' hack..
  // this should probably be fixed so we don't get called when menu closes...

  void on_widget_mouse_leave(SAT_BaseWidget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    SAT_TextWidget::on_widget_mouse_leave(ATo,AXpos,AYpos,ATime);
    if (isVisible()) {
      setTextColor(MSavedTextColor);
      setBackgroundColor(MSavedBackgroundColor);
      //parentRedraw();
      do_widgetListener_redraw(this,0);
    }
  }

  //----------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_TextWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
    if (AButton == SAT_BUTTON_LEFT) {
      setTextColor(MSavedTextColor);
      setBackgroundColor(MSavedBackgroundColor);
      uint32_t index = getIndex();
      //do_widgetListener_notify(this,SAT_WIDGET_NOTIFY_SELECT,index);
      do_widgetListener_select(this,index);
    }
    if (AButton == SAT_BUTTON_RIGHT) {
      setTextColor(MSavedTextColor);
      setBackgroundColor(MSavedBackgroundColor);
      //do_widgetListener_notify(this,SAT_WIDGET_NOTIFY_CLOSE,0);
      do_widgetListener_close(this);
    }
  }

};

//----------------------------------------------------------------------
#endif
