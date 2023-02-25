#ifndef sat_button_widget_included
#define sat_button_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ButtonWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  bool        MIsToggle = false;
  const char* MOnText   = "ON";
  const char* MOffText  = "OFF";

//------------------------------
public:
//------------------------------

  SAT_ButtonWidget(SAT_Rect ARect)
  : SAT_TextWidget(ARect,"") {
    setCursor(SAT_CURSOR_FINGER);
  }

  //----------

  virtual ~SAT_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  void setIsToggle(bool AState)    { MIsToggle = AState; }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MIsToggle) {
        if (getValue() < 0.5) setValue(1.0);
        else setValue(0,0);
      }
      else {
        setValue(1.0);
      }
      do_widget_update(this,0);
      do_widget_redraw(this,0);
    }
  }

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (!MIsToggle) {
        setValue(0.0);
        do_widget_update(this,0);
        do_widget_redraw(this,0);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    fillBackground(AContext);
    if (getValue() > 0.5) setText(MOnText);
    else setText(MOffText);
    drawText(AContext);
    //paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif




