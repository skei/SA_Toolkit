#ifndef sat_dual_slider_included
#define sat_dual_slider_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "gui/widgets/sat_slider_widget.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DualSliderWidget0
: public SAT_PanelWidget {
  
//------------------------------
private:
//------------------------------

  bool      MDrawValueBar     = true;
  bool      MDrawValues       = true;
  
  SAT_Color MValue1Color      = SAT_White;
  double    MValue1Size       = 9;
  uint32_t  MValue1Alignment  = SAT_TEXT_ALIGN_LEFT;
  SAT_Rect  MValue1Offset     = {};
  char      MValue1Text[256]  = {0};

  SAT_Color MValue2Color      = SAT_White;
  double    MValue2Size       = 9;
  uint32_t  MValue2Alignment  = SAT_TEXT_ALIGN_RIGHT;
  SAT_Rect  MValue2Offset     = {};
  char      MValue2Text[256]  = {0};

  SAT_Color MValueBarColor    = SAT_LightGrey;
  SAT_Rect  MValueBarOffset   = SAT_Rect(2,2,2,2);
  
  bool      MDrawText         = true;
  char      MText[256]        = {0};
  uint32_t  MTextAlignment    = SAT_TEXT_ALIGN_CENTER;
  SAT_Rect  MTextOffset       = {};
  SAT_Color MTextColor        = SAT_Black;
  double    MTextSize         = 10.0;
  
  bool      MDrawHoverEdge    = true;
  SAT_Color MEdgeColor        = SAT_Black;
  
  double    MHoverDistance    = 5;
  int32_t   MHoverEdge        = 0;
  
  bool      MIsDragging       = false;
  int32_t   MDragEdge         = 0;
  
  double    MPreviousXpos     = 0.0;
  double    MPreviousYpos     = 0.0;
  
//  bool      MPushOther = true;
//  bool      MStopOther = true;

//------------------------------
public:
//------------------------------

  SAT_DualSliderWidget0(SAT_Rect ARect, const char* AText, double AValue1, double AValue2)
  : SAT_PanelWidget(ARect) {
    setName("SAT_DualSliderWidget0");
    setValue(AValue1,0);
    setValue(AValue2,1);
    strcpy(MText,AText);
    //setTextAlignment(SAT_TEXT_ALIGN_CENTER);
    MTextAlignment = SAT_TEXT_ALIGN_CENTER;
  }
  
  //----------

  virtual ~SAT_DualSliderWidget0() {
  }
  
//------------------------------
public:
//------------------------------

  int32_t find_hover(double AXpos, double AYpos) {
    double S = getWindowScale();
    SAT_Rect mrect = getRect();
    
    if (!mrect.contains(AXpos,AYpos)) return 0;
    
    double hd = MHoverDistance * S;
    double range = mrect.w;
    
    double v1 = getValue(0);
    double x1 = mrect.x + (v1 * range);

    double v2 = getValue(1);
    double x2 = mrect.x + (v2 * range);

    double dist_x1 = abs(x1 - AXpos);
    double dist_x2 = abs(x2 - AXpos);
    
    int32_t hover = 0;
    if (AXpos < x1) {
      if (dist_x1 < hd) hover = 1;
      else hover = -1;
    }
    else if (AXpos >= x2) {
      if (dist_x2 < hd) hover = 2;
      else hover = -2;
    }
    else {
      // use smallest dist?
      if (dist_x1 < hd) hover = 1;
      else if (dist_x2 < hd) hover = 2;
      else hover = 3;
    }
    //SAT_Print("hover %i\n",hover);
    return hover;
  }
  
  //----------

  virtual void drawValueBar(SAT_PaintContext* AContext) {
    //double S = getWindowScale();
    SAT_Assert(AContext);
    if (MDrawValueBar) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      
      double v1 = getValue(0);
      SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
      if (param) v1 = param->denormalize(v1);
      
      double v2 = getValue(1);
      param = (SAT_Parameter*)getParameter(1);
      if (param) v2 = param->denormalize(v2);
      
      SAT_Rect mrect = getRect();
      mrect.shrink(MValueBarOffset);
      //if ((mrect.w > 0.0) && (mrect.h > 0.0)) {
        
      //mrect.x += 2;
      //mrect.w -= 4;

      double x = mrect.x  + (v1 * mrect.w) - (2*S);
      double y = mrect.y;
      double w = ((v2 - v1) * mrect.w) + (4*S);
      double h = mrect.h;
      
      painter->setFillColor(MValueBarColor);
      painter->fillRect(x,y,w,h);
    }
  }
  
  //----------
  
  virtual void drawValues(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawValues) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      
      // v1
      
      SAT_Rect valueoffset = MValue1Offset;
      valueoffset.scale(S);
      SAT_Rect mrect = getRect();
      mrect.shrink(valueoffset);
      //if ((mrect.w > 0.0) && (mrect.h > 0.0)) {
        
      SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
      if (param) {
        double pv = getValue(0);
        double dpv = param->denormalizeValue(pv);
        param->valueToText(dpv,MValue1Text,255);
      }
      else {
        double v = getValue(0);
        sprintf(MValue1Text,"%.2f",v);
      }
      
      painter->setTextColor(MValue1Color);
      painter->setTextSize(MValue1Size*S);
      painter->drawTextBox(mrect,MValue1Text,MValue1Alignment);
      
      // v2
      
      valueoffset = MValue2Offset;
      valueoffset.scale(S);
      mrect = getRect();
      mrect.shrink(valueoffset);
      //if ((mrect.w > 0.0) && (mrect.h > 0.0)) {
        
      param = (SAT_Parameter*)getParameter(1);
      if (param) {
        double pv = getValue(1);
        double dpv = param->denormalizeValue(pv);
        param->valueToText(dpv,MValue2Text,255);
      }
      else {
        double v = getValue(1);
        sprintf(MValue2Text,"%.2f",v);
      }
      
      painter->setTextColor(MValue2Color);
      painter->setTextSize(MValue2Size*S);
      painter->drawTextBox(mrect,MValue2Text,MValue2Alignment);
      
    }
  }
  
  //----------
  
  virtual void drawHoverEdge(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawHoverEdge) {
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      
      double S = getWindowScale();

      double v1 = getValue(0);
      SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
      if (param) v1 = param->denormalizeValue(v1);
      
      double v2 = getValue(1);
      param = (SAT_Parameter*)getParameter(1);
      if (param) v2 = param->denormalizeValue(v2);
      
      SAT_Rect mrect = getRect();
      mrect.shrink(MValueBarOffset);
      //if ((mrect.w > 0.0) && (mrect.h > 0.0)) {
  
      double x = mrect.x;
      double y = mrect.y;
      double w = 4*S;
      double h = mrect.h;
      
      painter->setFillColor(MEdgeColor);

      switch (MHoverEdge) {
        case 1: {
          x += (v1 * mrect.w) - (2*S);
          painter->fillRect(x,y,w,h);
          break;
        }
        case 2: {
          x += (v2 * mrect.w) - (2*S);
          painter->fillRect(x,y,w,h);
          break;
        }
        case 3: {
          x += (v1 * mrect.w);
          w = (v2 - v1) * mrect.w;
          painter->fillRect(x,y,w,h);
          break;
        }
      }
      
    }
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
      SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
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
    //SAT_PRINT;
    fillBackground(AContext);
    drawValueBar(AContext);
    drawHoverEdge(AContext);
    drawText(AContext);
    drawValues(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
    drawHostIndicators(AContext);
  }
  
  //----------
  
  void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MPreviousXpos = AXpos;
      MPreviousYpos = AYpos;
      switch (MHoverEdge) {
        case 1: { // left
          //SAT_Print("left\n");
          MIsDragging = true;
          MDragEdge = 1;
          break;
        }
        case 2: { // right
          //SAT_Print("right\n");
          MIsDragging = true;
          MDragEdge = 2;
          break;
        }
        case 3: { // center
          //SAT_Print("middle\n");
          MIsDragging = true;
          MDragEdge = 3;
          break;
        }
      }
    }
  }
  
  //----------
  
  void on_widget_mouseRelease(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MIsDragging) {
        MIsDragging = false;
        MDragEdge = 0;
        MHoverEdge = find_hover(AXpos,AYpos);
        //SAT_Print("MHoverEdge = %i\n",MHoverEdge);
      }
      MIsDragging = false;
    }
  }
  
  //----------
  
  void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    
    if (MIsDragging) {
      double v1 = getValue(0);
      double v2 = getValue(1);
      
      double sens = 0.001;//MDragSensitivity;
      if (AState & SAT_STATE_CTRL) sens *= 0.1;//MShiftSensitivity;
      double diff = AXpos - MPreviousXpos;

      //double value;
      //switch (MDragEdge) {
      //  case 1: v1 = getValue(0); break;
      //  case 2: v1 = getValue(1); break;
      //}
      
      switch (MDragEdge) {
        case 1: {
          v1 += (diff * sens);
          v1 = SAT_Clamp(v1,0,1);
          //SAT_Print("MDragEdge %i value: %f\n",MDragEdge,value);
          setValue(v1,0);
          do_widget_update(this,0,0);
          do_widget_redraw(this,0,0);
          break;
        }
        case 2: {
          v2 += (diff * sens);
          v2 = SAT_Clamp(v2,0,1);
          //SAT_Print("MDragEdge %i value: %f\n",MDragEdge,value);
          setValue(v2,1);
          do_widgetListener_update(this,0,1);
          do_widgetListener_redraw(this,0,1);
          break;
        }
        case 3: {
          v1 += (diff * sens);
          v1 = SAT_Clamp(v1,0,1);
          setValue(v1,0);
          v2 += (diff * sens);
          v2 = SAT_Clamp(v2,0,1);
          setValue(v2,1);
          do_widgetListener_update(this,0,0);
          //do_widgetListener_redraw(this,0,0);
          do_widgetListener_update(this,0,1);
          //do_widgetListener_redraw(this,0,1);
          do_widgetListener_redraw(this,0);
          break;
        }
      }
      
      
//      v1 += (diff * sens);
//      v1 = SAT_Clamp(v1,0,1);
//      
//      switch (MDragEdge) {
//        case 1:
//          //SAT_Print("MDragEdge %i value: %f\n",MDragEdge,value);
//          setValue(v1,0);
//          do_widgetListener_update(this,0,0);
//          do_widgetListener_redraw(this,0,0);
//          break;
//        case 2:
//          //SAT_Print("MDragEdge %i value: %f\n",MDragEdge,value);
//          setValue(v1,1);
//          do_widgetListener_update(this,0,1);
//          do_widgetListener_redraw(this,0,1);
//          break;
//        //case 3:
//        //  do_widgetListener_update(this,0,1);
//        //  do_widgetListener_redraw(this,0,1);
//        //  break;
//      }
      
      
      //SAT_Print("value %f\n",value); // ok.. 0..1
    
    }
    else {
      int32_t edge = find_hover(AXpos,AYpos);
      if (edge != MHoverEdge) {
        //SAT_Print("MHoverEdge = %i\n",edge);
        MHoverEdge = edge;
        do_widgetListener_redraw(this,0);
      }
    }
    
    //SAT_PanelWidget::on_widget_mouse_move(AXpos,AYpos,AState,ATime);
    
    //else {
    //  // check if hover left/right edge?
    //  MHoverLeftEdge = false;
    //  MHoverRightEdge = false;
    //  // if...
    //}
    
    MPreviousXpos = AXpos;
    MPreviousYpos = AYpos;

  }
  
  //----------
  
  //void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
  //}

  //----------
  
  void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos) override {
    //SAT_Print("MHoverEdge %i\n",MHoverEdge);
    if ((MHoverEdge != 0) && (!MIsDragging)) {
      MHoverEdge = 0;
      do_widget_redraw(this,0,0);
    }
  }

};

//----------------------------------------------------------------------
#endif