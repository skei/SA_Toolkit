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

  SAT_MenuListener* MMenuListener     = nullptr;
  bool              MRightClickClose  = true;

//------------------------------
public:
//------------------------------

  SAT_MenuWidget(SAT_Rect ARect)
  : SAT_PopupWidget(ARect) {
    setName("SAT_MenuWidget");
    setAlignment(SAT_WIDGET_ALIGN_PARENT);
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

  void do_widgetListener_close(SAT_Widget* ASender) override {
    close();
  }
    
  //----------

  void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex) override {
    if (MMenuListener) MMenuListener->do_menu_select(AIndex);
    close();
  }

};

//----------------------------------------------------------------------
#endif
