#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_button_widget.h"

class SAT_SelectorWidget
: public SAT_ButtonWidget {

//------------------------------
private:
//------------------------------

  SAT_MenuWidget* MMenu = nullptr;

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, SAT_MenuWidget* AMenu)
  : SAT_ButtonWidget(ARect) {
    MMenu = AMenu;
    setIsToggle(false);
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

  //----------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    double value = ASender->getValue();
    SAT_Print("value %.3f\n",value);
    if (MMenu && (value > 0.5)) {
      double S = getWindowScale();
      SAT_Rect rect = getRect();
      rect.y += (20 * S);
      rect.w = MMenu->getWidth();
      rect.h = MMenu->getHeight();
      SAT_Print("x %.2f y %.2f w %.2f h %.2f\n",rect.x,rect.y,rect.w,rect.h);
      if (value > 0.5) {
        MMenu->open(rect);
      }
      else {
        MMenu->close();
      }
    }
    SAT_ButtonWidget::do_widget_update(ASender,AMode,AIndex);
  }


};

//----------------------------------------------------------------------
#endif
