#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_random.h"
#include "gui/widgets/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SelectorWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  SAT_MenuWidget* MMenu   = nullptr;

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu)
  : SAT_TextWidget(ARect,AText) {
    setCursor(SAT_CURSOR_FINGER);
    MMenu = AMenu;
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

  //----------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MMenu) {
        SAT_Rect rect = getRect();
        rect.x = AXpos;
        rect.y = AYpos;
        rect.w = MMenu->getWidth();
        rect.h = MMenu->getHeight();
        MMenu->open(rect);
      }
    }
  }


};

//----------------------------------------------------------------------
#endif
