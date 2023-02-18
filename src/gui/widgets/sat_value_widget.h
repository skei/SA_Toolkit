#ifndef sat_value_widget_included
#define sat_value_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_text_widget.h"

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

  bool      MDrawValue      = true;
  SAT_Color MValueColor     = SAT_White;
  double    MValueSize      = 12;
  uint32_t  MValueAlignment = SAT_TEXT_ALIGN_RIGHT;
  SAT_Rect  MValueOffset    = {};
  char      MValueText[256] = {0};

//------------------------------
public:
//------------------------------

  SAT_ValueWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_TextWidget(ARect,AText) {
    setValue(AValue);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    setTextOffset(SAT_Rect(5,0,0,0));
    setValueOffset(SAT_Rect(0,0,5,0));
  }

  virtual ~SAT_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValue(bool ADraw=true)      { MDrawValue = ADraw; }
  virtual void setValueColor(SAT_Color AColor)    { MValueColor = AColor; }
  virtual void setValueSize(double ASize)         { MValueSize = ASize; }
  virtual void setValueAlignment(uint32_t AAlign) { MValueAlignment = AAlign; }
  virtual void setValueOffset(SAT_Rect AOffset)   { MValueOffset = AOffset; }

//------------------------------
public:
//------------------------------

  virtual void drawValue(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawValue) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect valueoffset = MValueOffset;
      valueoffset.scale(S);
      mrect.shrink(valueoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;

      sprintf(MValueText,"%.3f",getValue());
      painter->setTextColor(MValueColor);
      painter->setTextSize(MValueSize*S);
      painter->drawTextBox(mrect,MValueText,MValueAlignment);
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    fillBackground(AContext);
    drawText(AContext);
    drawValue(AContext);
    //paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif


