#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_random.h"
#include "gui/widgets/sat_text_widget.h"

class SAT_SelectorWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  SAT_MenuWidget* MMenu = nullptr;

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu)
  : SAT_TextWidget(ARect,AText) {
    MMenu = AMenu;
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

  //----------

  //void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //double value = ASender->getValue();
    //SAT_Print("value %.3f\n",value);
    if (AButton == SAT_BUTTON_LEFT) {
      //if (MMenu && (value > 0.5)) {
      if (MMenu) {
        double S = getWindowScale();
        SAT_Rect rect = getRect();

        rect.x = AXpos;
        rect.y = AYpos;

        //rect.y += (20 * S);

        rect.w = MMenu->getWidth();
        rect.h = MMenu->getHeight();
        //SAT_Print("x %.2f y %.2f w %.2f h %.2f\n",rect.x,rect.y,rect.w,rect.h);
        //if (value > 0.5) {
          MMenu->open(rect);
        //}
        //else {
        //  MMenu->close();
        //}
      }
      //SAT_ButtonWidget::do_widget_update(ASender,AMode,AIndex);
    }
  }


};

//----------------------------------------------------------------------
#endif
