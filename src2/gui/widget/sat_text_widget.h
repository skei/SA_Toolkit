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

//------------------------------
public:
//------------------------------

  SAT_TextWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_TextWidget");
  }

  //----------

  virtual ~SAT_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  void setText(const char* AText)             { MText = AText; }

  void setDrawText(bool ADraw=true)           { MDrawText = ADraw; }
  void setTextColor(SAT_Color AColor)         { MTextColor = AColor; }
  void setTextSize(double ASize)              { MTextSize = ASize; }
  void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }

//------------------------------
public:
//------------------------------

  virtual void drawText(SAT_PaintContext* AContext) {
    if (MDrawText) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      painter->setTextColor(MTextColor);
      painter->setTextSize(MTextSize);
      //painter->drawText(rect.x,rect.y,MText);
      const char* text = "";
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        text = param->getName();
      }
      else {
        text = MText;
      }
      painter->drawTextBox(rect,text,MTextAlignment);
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    drawText(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif