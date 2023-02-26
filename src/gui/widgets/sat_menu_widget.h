#ifndef sat_menu_widget_included
#define sat_menu_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_popup_widget.h"

//

class SAT_MenuWidget
: public SAT_PopupWidget {

//------------------------------
public:
//------------------------------

  SAT_MenuWidget(SAT_Rect ARect)
  : SAT_PopupWidget(ARect) {
  }

  //----------

  virtual ~SAT_MenuWidget() {
  }

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_RIGHT) close();
    SAT_PopupWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
  }



};

//----------------------------------------------------------------------
#endif
