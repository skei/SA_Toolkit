#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_random.h"
#include "gui/widgets/sat_menu_widget.h"
#include "gui/widgets/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SelectorWidget
: public SAT_TextWidget
, public SAT_MenuListener {

//------------------------------
private:
//------------------------------

  SAT_MenuWidget* MMenu   = nullptr;

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_SelectorWidget");
    setCursor(SAT_CURSOR_FINGER);
    MMenu = AMenu;
    MMenu->setMenuListener(this);
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

//------------------------------
public: // widget
//------------------------------

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

//------------------------------
public: // menu listener
//------------------------------

  void do_menu_select(int32_t AIndex) override {
    SAT_TextWidget* widget = (SAT_TextWidget*)MMenu->getChildWidget(AIndex);
    const char* text = widget->getText();
    setText(text);
    parentRedraw();
  }

  //----------

  void do_menu_cancel() override {
  }

};

//----------------------------------------------------------------------
#endif
