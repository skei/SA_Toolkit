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
    setHint("SAT_ValueWidget");
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

  // double calcValue() {
  //   double value = getValue();
  //   //SAT_PRINT("value %.3f\n",value);
  //   SAT_Parameter* param = (SAT_Parameter*)getParameter();
  //   //SAT_PRINT("param %p\n",param);
  //   if (param) {
  //     value = param->getNormalizedValue();
  //     //SAT_PRINT("normalized value %.3f\n",value);
  //   }
  //   return value;
  // }

  //----------

  virtual void drawValueText(SAT_PaintContext* AContext) {
    if (MDrawValueText) {
      //SAT_TRACE;
      sat_param_t value = 0.0;
      const char* text = "";
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        value = param->getValue();
        text = param->valueToText(value);
      }
      else {
        value = getValue();
        sprintf(MValueText,"%.2f",value);
        text = MValueText;
      }
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      painter->setTextColor(MValueTextColor);
      painter->setTextSize(MValueTextSize*scale);
      //painter->drawText(rect.x,rect.y,MText);
      painter->drawTextBox(rect,text,MValueTextAlignment);
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
//    drawIndicators(AContext);
    drawBorder(AContext);
  }
};


//---------------------------------------------------------------------
#endif

