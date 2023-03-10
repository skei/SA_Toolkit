#ifndef sat_menu_widget_included
#define sat_menu_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_popup_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_MenuListener {
public:
  virtual void do_menu_select(int32_t AIndex) {}
  virtual void do_menu_cancel() {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
//

class SAT_MenuWidget
: public SAT_PopupWidget {

//------------------------------
private:
//------------------------------

  SAT_MenuListener* MMenuListener         = nullptr;
  bool              MRightClickClose  = true;

//------------------------------
public:
//------------------------------

  SAT_MenuWidget(SAT_Rect ARect)
  : SAT_PopupWidget(ARect) {
  }

  //----------

  virtual ~SAT_MenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setRightClickClose(bool AClose=true)         { MRightClickClose = AClose; }
  virtual void setMenuListener(SAT_MenuListener* AListener) { MMenuListener = AListener; }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_PopupWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
    if (MRightClickClose && (AButton == SAT_BUTTON_RIGHT)) close();
  }

  //----------

  // from menu item

  void do_widget_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
    //SAT_PopupWidget::do_widget_notify(ASender,AReason,AValue);
    switch (AReason) {
      case SAT_WIDGET_NOTIFY_SELECT: {
        if (MMenuListener) MMenuListener->do_menu_select(AValue);
        close();
        break;
      }
      case SAT_WIDGET_NOTIFY_CLOSE: {
        if (MMenuListener) MMenuListener->do_menu_cancel();
        close();
        break;
      }
    }
  }


};

//----------------------------------------------------------------------
#endif
