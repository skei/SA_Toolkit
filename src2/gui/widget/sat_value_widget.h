#ifndef sat_value_widget_included
#define sat_value_widget_included
//---------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ValueWidget
: public SAT_TextWidget {

//------------------------------
private:
//------------------------------

  char        MValueText[SAT_MAX_NAME_LENGTH] = {0};

  bool        MDrawValueText                  = true;
  SAT_Color   MValueTextColor                 = SAT_Black;
  double      MValueTextSize                  = 10.0;
  uint32_t    MValueTextAlignment             = SAT_TEXT_ALIGN_RIGHT;

//------------------------------
public:
//------------------------------

  SAT_ValueWidget(SAT_Rect ARect)
  : SAT_TextWidget(ARect) {
    setName("SAT_ValueWidget");
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
  }

  //----------

  virtual ~SAT_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  void setDrawValueText(bool ADraw=true)          { MDrawValueText = ADraw; }
  void setValueTextColor(SAT_Color AColor)        { MValueTextColor = AColor; }
  void setValueTextSize(double ASize)             { MValueTextSize = ASize; }
  void setValueTextAlignment(uint32_t AAlignment) { MValueTextAlignment = AAlignment; }

//------------------------------
public:
//------------------------------

  virtual void drawValueText(SAT_PaintContext* AContext) {
    if (MDrawValueText) {
      double value = getValue();
      sprintf(MValueText,"%.2f",value);
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      painter->setTextColor(MValueTextColor);
      painter->setTextSize(MValueTextSize);
      //painter->drawText(rect.x,rect.y,MText);
      painter->drawTextBox(rect,MValueText,MValueTextAlignment);
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    drawText(AContext);
    drawValueText(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }
};


//---------------------------------------------------------------------
#endif

