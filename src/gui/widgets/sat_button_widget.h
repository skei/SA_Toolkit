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

  bool        MIsToggle     = false;

  char        MOnText[256]  = "ON";
  char        MOffText[256] = "OFF";
  
  SAT_Color   MOnTextColor  = SAT_Black;
  SAT_Color   MOffTextColor = SAT_LightGrey;

  SAT_Color   MOnBackgroundColor  = SAT_LightGrey;
  SAT_Color   MOffBackgroundColor = SAT_DarkerGrey;


//------------------------------
public:
//------------------------------

  SAT_ButtonWidget(SAT_Rect ARect, bool AIsToggle=false)
  : SAT_TextWidget(ARect,"") {
    setName("SAT_ButtonWidget");
    setCursor(SAT_CURSOR_FINGER);
    MIsToggle = AIsToggle;
  }

  //----------

  virtual ~SAT_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setIsToggle(bool AState) {
    MIsToggle = AState;
  }

  virtual void setTexts(const char* AOnText, const char* AOffText) {
    strcpy(MOnText,AOnText);
    strcpy(MOffText,AOffText);
  }
  
  virtual void setTextColors(SAT_Color AOnColor, SAT_Color AOffColor) {
    MOnTextColor = AOnColor;
    MOffTextColor = AOffColor;
  }

  virtual void setBackgroundColors(SAT_Color AOnColor, SAT_Color AOffColor) {
    MOnBackgroundColor = AOnColor;
    MOffBackgroundColor = AOffColor;
  }

//------------------------------

//  void setText(const char* AText) override {
//    setText(AText,AText);
//  }
//  
//  void setTextColor(SAT_Color AColor) override {
//    setTextColors(AColor,AColor);
//  }
//
//  void setBackgroundColor(SAT_Color AColor) override {
//    setBackgroundColors(AColor,AColor);
//  }

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
      do_widgetListener_update(this,0);
      do_widgetListener_redraw(this,0);
    }
  }
  
  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (!MIsToggle) {
        setValue(0.0);
        do_widgetListener_update(this,0);
        do_widgetListener_redraw(this,0);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    
    if (getValue() > 0.5) {
      setBackgroundColor(MOnBackgroundColor);
      setTextColor(MOnTextColor);
      SAT_TextWidget::setText(MOnText);
    }
    else {
      setBackgroundColor(MOffBackgroundColor);
      setTextColor(MOffTextColor);
      SAT_TextWidget::setText(MOffText);
    }
    
    fillBackground(AContext);
    //if (getValue() > 0.5) SAT_TextWidget::setText(MOnText);
    //else SAT_TextWidget::setText(MOffText);
    drawText(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif




