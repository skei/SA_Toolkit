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
  double    MTextSize         = 8.0;
  uint32_t  MTextAlignment    = SAT_TEXT_ALIGN_CENTER;
  SAT_Rect  MTextOffset       = {};
  //char      MText[256]        = {0};
  char      MText[1024]       = {0};
  
  bool      MAutoSize         = false;
  bool      MAutoTextSize     = false;
  bool      MDrawParamText    = false;
  
  SAT_Rect    MAutoSizeBorder = SAT_Rect(0,0,0,0);

  bool      MDrawTextDropShadow   = false;
  double    MTextDropShadowSize   = 4.0;        //2.0;
  SAT_Color MTextDropShadowColor  = SAT_Black;  //SAT_Color(0,0,0,0.75);
  SAT_Point MTextDropShadowOffset = {2,2};      //{2,2};

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
  
  virtual void setText(const char* AText) {
    //SAT_Print("%s\n",AText);
    strcpy(MText,AText);
  }
  
  virtual void setAutoSize(bool AAuto=true)       { MAutoSize = AAuto; }
  virtual void setAutoTextSize(bool AAuto=true)   { MAutoTextSize = AAuto; }

  virtual void setDrawTextDropShadow(bool ADraw=true)     { MDrawTextDropShadow = ADraw; }
  virtual void setTextDropShadowSize(double ASize)        { MTextDropShadowSize = ASize; }
  virtual void setTextDropShadowColor(SAT_Color AColor)   { MTextDropShadowColor  = AColor; }
  virtual void setTextDropShadowOffset(SAT_Point AOffset) { MTextDropShadowOffset = AOffset; }

  virtual const char* getText() { return MText; }
  virtual double  getTextSize() { return MTextSize; }

//------------------------------
public:
//------------------------------

//  void prepare(SAT_WidgetOwner* AOwner) override {
//    SAT_PRINT;
//    SAT_Window* window = (SAT_Window*)AOwner;
//    SAT_Assert(window);
//    SAT_Painter* painter = window->getPainter();
//    SAT_Assert(painter);
//
//    double S = window->getScale();
//    painter->setTextSize(MTextSize);
//    
//    double bounds[4];
//    double advance = painter->getTextBounds(MText,bounds);
//    SAT_Rect rect = getRect();
//    rect.w = advance;//bounds[2];
//    rect.h = bounds[3];
//    rect.grow(MAutoSizeBorder);
//    //setRect(rect);
//    setRect(rect);
//    //rect.scale(1.0 / S);
//    setBasisRect(rect);
//  }
  
  //----------

//  virtual void adjustTextWidth(SAT_Painter* APainter) {
//    //SAT_Window* window = (SAT_Window*)getOwner();
//    //if (window) {
//    //  SAT_Painter* painter = window->getPainter();
//    //  if (painter) {
//        const char* text = getText();
//        //SAT_Print("text: %s\n",text);
//        double bounds[4];
//        //if (APainter->getTextBounds(text,bounds)) {
//        //  //textwidget->setWidth(bounds[2]);
//        //  setWidth(bounds[2]);
//        //}
//        double advance = APainter->getTextBounds(text,bounds);
//        setWidth(advance);
//
//    //  }
//    //}
//  }
  
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
      double textsize = MTextSize * S;
      painter->setTextColor(MTextColor);
      painter->setTextSize(textsize); // try original..
      
      // if autosize is true, the text will be scaled up as much as
      // possible to fit inside the rect..

      if (MAutoTextSize) {
        double bounds[4];
        //if (painter->getTextBounds(MText,bounds)) {
        //  //SAT_Print("%.2f, %.2f - %.2f, %.2f\n",bounds[0],bounds[1],bounds[2],bounds[3]);
          painter->getTextBounds(MText,bounds);
          double width = bounds[2] - bounds[0];
          double height = bounds[3] - bounds[1];
          double wratio = 1.0;
          double hratio = 1.0;
          if (width > 0)  wratio = mrect.w / width;
          if (height > 0) hratio = mrect.h / height;
          if (wratio < hratio) textsize *= wratio;
          else textsize *= hratio;
          //SAT_Print("mrect.w %.2f mrect.h %.2f wratio %.2f hratio %.2f\n",mrect.w,mrect.h,wratio,hratio);
          painter->setTextSize(textsize);
        //}
      }
      
      char* text = MText;
      
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        // drop shadow
        //painter->drawTextBox(mrect,param->getName(),MTextAlignment);
        if (MDrawParamText) text = param->getName();
        SAT_Point p = painter->getTextPos(mrect,text,MTextAlignment);
        if (MDrawTextDropShadow) {
          painter->setFontBlur(MTextDropShadowSize);
          painter->setTextColor(MTextDropShadowColor);
          painter->drawText(p.x + MTextDropShadowOffset.x, p.y + MTextDropShadowOffset.y, text);
          painter->setFontBlur(0);
        }
        
        painter->setTextColor(MTextColor);
        painter->drawText(p.x,p.y,text);
        
      }
      else {
        // drop shadow
        //painter->drawTextBox(mrect,MText,MTextAlignment);
        SAT_Point p = painter->getTextPos(mrect,MText,MTextAlignment);
        if (MDrawTextDropShadow) {
          painter->setFontBlur(MTextDropShadowSize);
          painter->setTextColor(MTextDropShadowColor);
          painter->drawText(p.x + MTextDropShadowOffset.x, p.y + MTextDropShadowOffset.y, MText);
          painter->setFontBlur(0);
        }
        painter->setTextColor(MTextColor);
        painter->drawText(p.x,p.y,text);
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

