#ifndef sat_menu_widget_included
#define sat_menu_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_menu_item_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MenuListener {
public:
  virtual void on_menuListener_select(int32_t AIndex) {};
};

//----------------------------------------------------------------------

class SAT_MenuWidget
: public SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  int32_t           MSelected = -1;
  SAT_MenuListener* MListener = nullptr;

//------------------------------
public:
//------------------------------

  SAT_MenuWidget(SAT_Rect ARect, SAT_MenuListener* AListener=nullptr)
  : SAT_VisualWidget(ARect) {
    setName("SAT_MenuWidget");
    setHint("SAT_MenuWidget");
    MListener = AListener;
    Options.realignInvisible = true;
    State.visible = false;
    State.active = false;
  }

  //----------

  virtual ~SAT_MenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setListener(SAT_MenuListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual void openMenu(int32_t AXpos, int32_t AYpos) {
    //SAT_TRACE;
    double x = AXpos;
    double y = AYpos;

    SAT_Rect mrect = getRect();
    double window_width = getWindowWidth();
    double window_height = getWindowHeight();
    //SAT_PRINT("%.1f, %.1f\n",window_width,window_height);
    //SAT_PRINT("%.1f, %.1f, %.1f, %.1f\n",mrect.x,mrect.y,mrect.w,mrect.h);
    if ((AXpos + mrect.w) >= window_width)  x = window_width  - mrect.w;
    if ((AYpos + mrect.h) >= window_height) y = window_height - mrect.h;

    setActive(true);
    setVisible(true);
    SAT_Widget* parent = getParent();
    if (parent) {

      // setRectAndBase();
      SAT_Rect baserect = getBaseRect();
      baserect.x = x;
      baserect.y = y;
      double scale = getWindowScale();
      if (scale > 0.0) {
        baserect.x = x / scale;
        baserect.y = y / scale;
      }
      setBaseRect(baserect);

      parent->realignChildren();
      parent->do_widget_redraw(parent);
    }
    do_widget_modal(this);
  }

  //----------

  virtual void closeMenu() {
    //SAT_TRACE;
    setActive(false);
    setVisible(false);
    SAT_Widget* parent = getParent();
    if (parent) {
      parent->realignChildren();
      parent->do_widget_redraw(parent);
    }
    do_widget_modal(nullptr);
    // deleteChildren();
  }

  //----------

  virtual int32_t appendItem(const char* AText) {
    int32_t index = getNumChildren();
    SAT_MenuItemWidget* menuitem = new SAT_MenuItemWidget(20,AText);
    appendChild(menuitem);
    return 0;
  }

  //----------

  virtual const char* getItemText(uint32_t AIndex) {
    SAT_MenuItemWidget* menuitem = (SAT_MenuItemWidget*)getChild(AIndex);
    return menuitem->getText();
  }

//------------------------------
public:
//------------------------------

  void do_widget_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr=nullptr) override {
    //SAT_PRINT("AValue %i\n",AValue);
    if (AValue == -1) {
      // cancel
    }
    else {
      // selected index AValue
      MSelected = AValue;
    }

    if (MListener) MListener->on_menuListener_select(AValue);

    // do_widget_modal(this,false);
    // SAT_Widget* parent = getParent();
    // if (parent) {
    //   parent->realignChildren();
    //   parent->do_widget_redraw(parent);
    // }
    // closeMenu();
    //SAT_VisualWidget::do_widget_notify(this,MSelected);
  }

};

//----------------------------------------------------------------------
#endif