#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_text_widget.h"
#include "gui/widget/sat_menu_widget.h"

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

  const char*     MInitialText                        = "";
  int32_t         MSelected                           = -1;
  SAT_MenuWidget* MMenu                               = nullptr;
  //char            MSelectedText[SAT_MAX_NAME_LENGTH]  = {0};

//------------------------------
public:
//------------------------------

  SAT_SelectorWidget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu = nullptr)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_SelectorWidget");
    MInitialText = AText;
    MMenu = AMenu;
    //if (MMenu) MMenu->setPos(SAT_Point(ARect.x,ARect.y+ARect.h));
  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMenu(SAT_MenuWidget* AMenu) {
    MMenu = AMenu;
    SAT_Rect rect = getRect();
    //if (MMenu) MMenu->setPos(SAT_Point(rect.x,rect.y+rect.h));
  }

  //----------

  void on_menuListener_select(int32_t AIndex) override {
    //SAT_PRINT("AIndex %i\n",AIndex);
    SAT_PRINT("AValue %i\n",AIndex);
    if (AIndex >= 0) {
      MSelected = AIndex;
      uint32_t numitems = getNumChildren();
      double value = (double)(MSelected - 1) / (double)numitems;
    }
    if (MMenu) MMenu->closeMenu();

  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {

    if (MMenu) {
      switch (AButton) {
        case SAT_BUTTON_LEFT:
          if (MMenu) MMenu->openMenu(AXpos,AYpos);
          break;
        case SAT_BUTTON_RIGHT:
          // if menu is open, close it
          break;
      }
    }

  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) {
    if (MMenu) {
      if (MSelected >= 0) {
        const char* text = MMenu->getItemText(MSelected);
        setText(text);
      }
    }
    SAT_TextWidget::on_widget_paint(AContext);
  }

//------------------------------
public:
//------------------------------

  // void do_widget_notify(SAT_Widget* AWidget, int32_t AValue) override {
  //   SAT_PRINT("AValue %i\n",AValue);
  //   if (AValue == -1) {
  //     // cancel
  //   }
  //   else {
  //     MSelected = AValue;
  //     uint32_t numitems = getNumChildren();
  //     double value = (double)(MSelected - 1) / (double)numitems;
  //   }
  //   if (MMenu) MMenu->closeMenu();
  // }

};

//----------------------------------------------------------------------
#endif