#ifndef sat_text_widget_included
#define sat_text_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "gui/sat_window.h"
#include "gui/widgets/sat_movable_widget.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TextWidget
: public SAT_PanelWidget {
//: public SAT_MovableWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawText         = true;
  SAT_Color MTextColor        = SAT_White;
  double    MTextSize         = 10.0;
  uint32_t  MTextAlignment    = SAT_TEXT_ALIGN_CENTER;
  SAT_Rect  MTextOffset       = {};
  //char      MText[256]        = {0};
  char      MText[1024]       = {0};

//------------------------------
public:
//------------------------------

  SAT_TextWidget(SAT_Rect ARect, const char* AText)
  : SAT_PanelWidget(ARect) {
  //: SAT_MovableWidget(ARect) {
    setName("SAT_TextWidget");
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

  virtual void adjustTextWidth(SAT_Painter* APainter) {
    //SAT_Window* window = (SAT_Window*)getOwner();
    //if (window) {
    //  SAT_Painter* painter = window->getPainter();
    //  if (painter) {
        const char* text = getText();
        SAT_Print("text: %s\n",text);
        double bounds[4];
        if (APainter->getTextBounds(text,bounds)) {
          //textwidget->setWidth(bounds[2]);
          setWidth(bounds[2]);
        }
    //  }
    //}
  }
  
  //----------

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
      SAT_Parameter* param = (SAT_Parameter*)getConnection(0);
      if (param) {
        painter->drawTextBox(mrect,param->getName(),MTextAlignment);
      }
      else {
        painter->drawTextBox(mrect,MText,MTextAlignment);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawText(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif

