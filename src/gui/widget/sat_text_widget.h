#ifndef sat_text_widget_included
#define sat_text_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------

class SAT_TextWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  const char* MText           = "text";
  bool        MDrawText       = true;
  SAT_Color   MTextColor      = SAT_Black;
  double      MTextSize       = 12.0;
  uint32_t    MTextAlignment  = SAT_TEXT_ALIGN_CENTER;
  bool        MDrawParamText  = true;

  SAT_Rect    MTextOffset     = {0,0,0,0};

//------------------------------
public:
//------------------------------

  SAT_TextWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_TextWidget");
    setHint("SAT_TextWidget");
  }

  //----------

  SAT_TextWidget(SAT_Rect ARect, const char* AText)
  : SAT_VisualWidget(ARect) {
    setName("SAT_TextWidget");
    setHint("SAT_TextWidget");
    MText = AText;
  }

  //----------

  virtual ~SAT_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char* AText)             { MText = AText; }

  virtual void setDrawText(bool ADraw=true)           { MDrawText = ADraw; }
  virtual void setTextColor(SAT_Color AColor)         { MTextColor = AColor; }
  virtual void setTextSize(double ASize)              { MTextSize = ASize; }
  virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  virtual void setTextOffset(SAT_Rect AOffset)        { MTextOffset = AOffset; }

  virtual void setDrawParamText(bool AParam=true)     { MDrawParamText = AParam; }

  virtual const char* getText()     { return MText; }
  virtual double      getTextSize() { return MTextSize; }

//------------------------------
public:
//------------------------------

  virtual void drawText(SAT_PaintContext* AContext) {
    if (MDrawText) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();

      SAT_Rect ofs = MTextOffset;
      ofs.scale(scale);
      rect.shrink(ofs);

      painter->setTextColor(MTextColor);
      painter->setTextSize(MTextSize*scale);

      //painter->drawText(rect.x,rect.y,MText);

      const char* text = "";
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (MDrawParamText && param) {
        text = param->getName();
      }
      else {
        text = MText;
      }

      painter->drawTextBox(rect,text,MTextAlignment);
      //painter->drawText(rect.x,rect.y,text);

    }
  }

//------------------------------
public: // on_widget
//------------------------------

  // void on_widget_open(SAT_WidgetOwner* AOwner) override {
  //   SAT_Rect rect = getRect();
  //   SAT_Painter* painter = AOwner->on_widgetOwner_getPainter();
  //   double scale = getWindowScale();
  //   painter->setTextSize(MTextSize * scale);
  //   double bounds[4] = {0};
  //   double advance = painter->getTextBounds(MText,bounds);
  //   SAT_PRINT("bounds %.2f, %.2f, %.2f, %.2f\n",bounds[0],bounds[1],bounds[2],bounds[3]);
  //   double width = bounds[2] - bounds[0];
  //   //setWidth(width);
  //   setRectAndBase(rect);
  //   SAT_VisualWidget::on_widget_open(AOwner);
  // };

  // SAT_Rect on_widget_preAlign(SAT_Rect ARect) override {
  //   SAT_WidgetOwner* owner = getOwner();
  //   SAT_Painter* painter = owner->on_widgetOwner_getPainter();
  //   double scale = getWindowScale();
  //   painter->setTextSize(MTextSize * scale);
  //   double bounds[4] = {0};
  //   double width = painter->getTextBounds(MText,bounds);
  //   ARect.w = width;
  //   return ARect;
  // }

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    drawText(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif