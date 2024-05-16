#ifndef sat_button_widget_included
#define sat_button_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_text_widget.h"

//----------

class SAT_ButtonWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  const char* MOffText            = "Off";
  SAT_Color   MOffTextColor       = SAT_Black;
  SAT_Color   MOffBackgroundColor = SAT_Grey;
  SAT_Color   MOffBorderColor     = SAT_Black;

  const char* MOnText            = "On";
  SAT_Color   MOnTextColor       = SAT_LightGrey;
  SAT_Color   MOnBackgroundColor = SAT_DarkGrey;
  SAT_Color   MOnBorderColor     = SAT_White;
  
//------------------------------
public:
//------------------------------

  SAT_ButtonWidget(SAT_Rect ARect)
  : SAT_TextWidget(ARect) {
    setName("SAT_ButtonWidget");
  }

  //----------

  virtual ~SAT_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------

  void setTexts(const char* AOff, const char* AOn) {
    MOffText = AOff;
    MOnText = AOn;
  }

  void setTextColors(SAT_Color AOff, SAT_Color AOn) {
    MOffTextColor = AOff;
    MOnTextColor = AOn;
  }

  // void setTextColor(SAT_Color AColor) { MTextColor = AColor; }
  // void setTextSize(double ASize)      { MTextSize = ASize; }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      double value = getValue();
      if (value > 0.5) {
        //SAT_PRINT("off\n");
        setValue(0.0);
      }
      else {
        //SAT_PRINT("on\n");
        setValue(1.0);
      }
      do_widget_update(this,SAT_WIDGET_UPDATE_VALUE);
      do_widget_redraw(this,SAT_WIDGET_REDRAW_PARAM);
    }
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    double value = getValue();
    if (value < 0.5) {
      setText(MOffText);
      setTextColor(MOffTextColor);
      setBackgroundColor(MOffBackgroundColor);
      setBorderColor(MOffBorderColor);
    }
    else {
      setText(MOnText);
      setTextColor(MOnTextColor);
      setBackgroundColor(MOnBackgroundColor);
      setBorderColor(MOnBorderColor);
    }

    fillBackground(AContext);
    drawText(AContext);
    paintChildren(AContext);
    drawBorder(AContext);

    // why not just:
    // SAT_TextWidget::on_widget_paint(AContext);

  }

};

//----------------------------------------------------------------------
#endif