#ifndef sat_menu_item_widget_included
#define sat_menu_item_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widgets/sat_panel_widget.h"

class SAT_MenuItemWidget
: public SAT_PanelWidget {

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
  : SAT_PanelWidget(ARect) {
    setName("SAT_MenuItemWidget");
    setCursor(SAT_CURSOR_FINGER);
    setDrawBorder(false);
    //setBorderColor(SAT_DarkerGrey);
    setFillBackground(true);
    //setBackgroundColor(MBackgroundColor);
    setDrawText(true);
    setText(AText);
    //setTextColor(MTextColor);


//    setAlignment(SAT_WIDGET_ALIGN_TOP_LEFT);
//    setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);

    addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP);
    addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_LEFT);
    addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    
    //setLayoutOuterBorder(SAT_Rect(1,1,1,1));
  }

  //----------

  virtual ~SAT_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------

  //virtual void setTextColor(SAT_Color AColor)             { MTextColor = AColor; }
  //virtual void setBackgroundColor(SAT_Color AColor)       { MBackgroundColor = AColor; }
  virtual void setHoverTextColor(SAT_Color AColor)        { MHoverTextColor = AColor; }
  virtual void setHoverBackgroundColor(SAT_Color AColor)  { MHoverBackgroundColor = AColor; }
  
//------------------------------
public:
//------------------------------

  void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos) override {
    SAT_PanelWidget::on_widget_enter(AFrom,AXpos,AYpos);
    //if (isVisible()) {
      MSavedTextColor = getTextColor();
      MSavedBackgroundColor = getBackgroundColor();
      setTextColor(MHoverTextColor);
      setBackgroundColor(MHoverBackgroundColor);
      //parentRedraw();
      do_widget_redraw(this,0,0);
    //}
  }

  //----------

  // we receive leave msg _after_ the menu has closed.. :-/
  // so we added the 'isVisible' hack..
  // this should probably be fixed so we don't get called when menu closes...

  void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos) override {
    SAT_PanelWidget::on_widget_leave(ATo,AXpos,AYpos);
    if (isVisible()) {
      setTextColor(MSavedTextColor);
      setBackgroundColor(MSavedBackgroundColor);
      //parentRedraw();
      do_widget_redraw(this,0,0);
    }
  }

  //----------

  void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_TextWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
    if (AButton == SAT_BUTTON_LEFT) {
      setTextColor(MSavedTextColor);
      setBackgroundColor(MSavedBackgroundColor);
      uint32_t index = getParentIndex();
      //do_widgetListener_notify(this,SAT_WIDGET_NOTIFY_SELECT,index);
      do_widget_select(this,index);
    }
    if (AButton == SAT_BUTTON_RIGHT) {
      setTextColor(MSavedTextColor);
      setBackgroundColor(MSavedBackgroundColor);
      //do_widgetListener_notify(this,SAT_WIDGET_NOTIFY_CLOSE,0);
      do_widget_close(this);
    }
  }

};

//----------------------------------------------------------------------
#endif