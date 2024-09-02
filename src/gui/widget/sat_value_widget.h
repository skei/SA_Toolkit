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

  SAT_Rect    MValueTextOffset                = {0,0,0,0};
  const char* MValueTextFormat                = "%.2f";


//------------------------------
public:
//------------------------------

  SAT_ValueWidget(SAT_Rect ARect, const char* AText="", double AValue=0.0)
  : SAT_TextWidget(ARect,AText) {
    setName("SAT_ValueWidget");
    setHint("SAT_ValueWidget");
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    setValue(AValue);
  }

  //----------

  virtual ~SAT_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValueText(bool ADraw=true)          { MDrawValueText = ADraw; }
  virtual void setValueTextColor(SAT_Color AColor)        { MValueTextColor = AColor; }
  virtual void setValueTextSize(double ASize)             { MValueTextSize = ASize; }
  virtual void setValueTextAlignment(uint32_t AAlignment) { MValueTextAlignment = AAlignment; }
  virtual void setValueTextOffset(SAT_Rect AOffset)       { MValueTextOffset = AOffset; }
  virtual void setValueTextFormat(const char* AFormat)    { MValueTextFormat = AFormat; }

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
        //SAT_PRINT("value %.3f text %s\n",value,text);
      }
      else {
        value = getValue();
        sprintf(MValueText,MValueTextFormat,value);
        text = MValueText;
      }
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();

      SAT_Rect offset = MValueTextOffset;
      offset.scale(scale);
      rect.shrink(offset);


      painter->setTextColor(MValueTextColor);
      painter->setTextSize(MValueTextSize*scale);
      //painter->drawText(rect.x,rect.y,MText);
      painter->drawTextBox(rect,text,MValueTextAlignment);
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  void on_Widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_TRACE;
    if (AButton == SAT_BUTTON_LEFT) {
      SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
      if (param) {
        double value = param->getDefaultValue();
        value = param->normalize(value);
        setValue(value);
        do_Widget_update(this);
        do_Widget_redraw(this);
      }
    }
  }

  //----------


  void on_Widget_paint(SAT_PaintContext* AContext) override {
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

