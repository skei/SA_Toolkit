#ifndef sat_selector_widget_included
#define sat_selector_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_menu_widget.h"
#include "gui/widget/sat_symbol_widget.h"
#include "gui/widget/sat_text_widget.h"

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
    setHint("SAT_SelectorWidget");
    setCursor(SAT_CURSOR_FINGER);
    setFillBackground(true);
    setFillGradient(true);
    Layout.innerBorder = {0,0,3,3};
    MInitialText = AText;
    MMenu = AMenu;
    //if (MMenu) MMenu->setPos(SAT_Point(ARect.x,ARect.y+ARect.h));
    SAT_SymbolWidget* symbol = new SAT_SymbolWidget(7,SAT_SYMBOL_FILLED_TRI_DOWN);
    appendChild(symbol);
    symbol->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_RIGHT;
    symbol->setActive(false);
    symbol->setColor(SAT_DarkerGrey);

  }

  //----------

  virtual ~SAT_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMenu(SAT_MenuWidget* AMenu) {
    MMenu = AMenu;
    //SAT_Rect rect = getRect();
    //if (MMenu) MMenu->setPos(SAT_Point(rect.x,rect.y+rect.h));
  }

  //----------

  virtual void select(int32_t AIndex) {
    MSelected = AIndex;
    double value = MSelected;
    if (MSelected >= 0) {
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        value = param->normalize(AIndex);
        //SAT_PRINT("AIndex %i = value %.3f\n",AIndex,value);
        SAT_TextWidget::setValue(value);
      }
    }
  }

  //----------

  void on_menuListener_select(int32_t AIndex) override {
    //SAT_PRINT("AValue %i\n",AIndex);
    if (AIndex >= 0) {
      select(AIndex);
      do_widget_update(this);
      //do_widget_redraw(this);
    }
    if (MMenu) MMenu->closeMenu();
    do_widget_set_overlay(this,SAT_Color(0,0,0,0));

  }

  //----------

  void setValue(double AValue) override {
    //SAT_PRINT("AValue %.3f\n",AValue);
    MSelected = AValue;
    double value = AValue;
    SAT_TextWidget::setValue(value);
  }


//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {

    if (MMenu) {
      switch (AButton) {
        case SAT_BUTTON_LEFT:
          if (MMenu) {
            MMenu->openMenu(AXpos,AYpos);
            do_widget_set_overlay(this,SAT_Color(0,0,0,0.2));
          }
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

};

//----------------------------------------------------------------------
#endif