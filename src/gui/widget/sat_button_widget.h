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

  const char* MOffText            =  "Off";
  SAT_Color   MOffTextColor       = SAT_DarkerGrey;
  SAT_Color   MOffBackgroundColor = SAT_Grey;
  SAT_Color   MOffBorderColor     = SAT_Black;

  const char* MOnText             = "On";
  SAT_Color   MOnTextColor        = SAT_Black;
  SAT_Color   MOnBackgroundColor  = SAT_DarkGrey;
  SAT_Color   MOnBorderColor      = SAT_Black;

  bool        MMomentary          = true;

//------------------------------
public:
//------------------------------

  SAT_ButtonWidget(SAT_Rect ARect, bool AMomentary=false)
  : SAT_TextWidget(ARect) {
    setName("SAT_ButtonWidget");
    setHint("SAT_ButtonWidget");
    setCursor(SAT_CURSOR_FINGER);
    MMomentary = AMomentary;
    setFillBackground(true);
    setFillGradient(true);

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
      if (MMomentary) {
        setValue(1.0);
      }
      else { // momentary
        double value = getValue();
        if (value > 0.5) setValue(0.0);
        else setValue(1.0);
      }

//========== test tween

      SAT_Rect rect = getRect();
      double starts[4]  = {0,0,0,0};
      double ends[4]    = {0,0,0,100};
      SAT_TweenChain* chain = new SAT_TweenChain();
      SAT_TweenNode* node1 = new SAT_TweenNode(this,255,1.0,SAT_TWEEN_RECT,4,starts,ends,SAT_EASING_OUT_BOUNCE);
      chain->appendNode(node1);
      SAT_TweenNode* node2 = new SAT_TweenNode(this,255,2.0,SAT_TWEEN_RECT,4,ends,starts,SAT_EASING_IN_EXPO);
      chain->appendNode(node2);
      do_widget_start_tween(this,chain);

//==========

      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MMomentary) {
        setValue(0.0);
        do_widget_update(this);
        do_widget_redraw(this);
      }
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
    drawIndicators(AContext);
    drawBorder(AContext);

    // why not just:
    // SAT_TextWidget::on_widget_paint(AContext);

  }

};

//----------------------------------------------------------------------
#endif