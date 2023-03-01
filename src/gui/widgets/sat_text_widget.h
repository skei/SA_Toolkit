#ifndef sat_text_widget_included
#define sat_text_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TextWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawText         = true;
  SAT_Color MTextColor        = SAT_White;
  double    MTextSize         = 12;
  uint32_t  MTextAlignment    = SAT_TEXT_ALIGN_CENTER;
  SAT_Rect  MTextOffset       = {};
  char      MText[256]        = {0};

//------------------------------
public:
//------------------------------

  SAT_TextWidget(SAT_Rect ARect, const char* AText)
  : SAT_PanelWidget(ARect) {
    strcpy(MText,AText);
    setFillBackground(true);
  }

  virtual ~SAT_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw=true)       { MDrawText = ADraw; }
  virtual void setTextColor(SAT_Color AColor)     { MTextColor = AColor; }
  virtual void setTextSize(double ASize)          { MTextSize = ASize; }
  virtual void setTextAlignment(uint32_t AAlign)  { MTextAlignment = AAlign; }
  virtual void setTextOffset(SAT_Rect AOffset)    { MTextOffset = AOffset; }
  virtual void setText(const char* AText)         { strcpy(MText,AText); }

  virtual const char* getText() { return MText; }

//------------------------------
public:
//------------------------------

  virtual void drawText(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawText) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect textoffset = MTextOffset;
      textoffset.scale(S);
      mrect.shrink(textoffset);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      painter->setTextColor(MTextColor);
      painter->setTextSize(MTextSize*S);
      painter->drawTextBox(mrect,MText,MTextAlignment);
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    //paintChildren(AContext);
    drawText(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif

