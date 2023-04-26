#ifndef sat_dual_slider_included
#define sat_dual_slider_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_slider_widget.h"



//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DualSliderWidget
: public SAT_SliderWidget {
  
//------------------------------
private:
//------------------------------

  bool      MDrawValueBar     = true;
  bool      MDrawValues       = true;

  SAT_Color MValue1Color      = SAT_White;
  double    MValue1Size       = 12;
  uint32_t  MValue1Alignment  = SAT_TEXT_ALIGN_LEFT;
  SAT_Rect  MValue1Offset     = {};
  char      MValue1Text[256]  = {0};

  SAT_Color MValue2Color      = SAT_White;
  double    MValue2Size       = 12;
  uint32_t  MValue2Alignment  = SAT_TEXT_ALIGN_RIGHT;
  SAT_Rect  MValue2Offset     = {};
  char      MValue2Text[256]  = {0};

  SAT_Color MValueBarColor    = SAT_LightGrey;
  SAT_Rect  MValueBarOffset   = SAT_Rect(2,2,2,2);

//------------------------------
public:
//------------------------------

  SAT_DualSliderWidget(SAT_Rect ARect, const char* AText, double AValue1, double AValue2)
  : SAT_SliderWidget(ARect,AText,AValue1) {
    setName("SAT_DualSliderWidget");
    setValue(AValue2,1);
    
    setTextAlignment(SAT_TEXT_ALIGN_CENTER);
  }
  
  //----------

  virtual ~SAT_DualSliderWidget() {
  }
  
//------------------------------
public:
//------------------------------

  void drawSliderBar(SAT_PaintContext* AContext) override {
    SAT_Assert(AContext);
    if (MDrawValueBar) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      
      double v1 = getValue(0);
      SAT_Parameter* param = (SAT_Parameter*)getConnection(0);
      if (param) v1 = param->denormalizeValue(v1);
      
      double v2 = getValue(1);
      param = (SAT_Parameter*)getConnection(1);
      if (param) v2 = param->denormalizeValue(v2);
      
      SAT_Rect mrect = getRect();
      mrect.shrink(MValueBarOffset);
      //if ((mrect.w > 0.0) && (mrect.h > 0.0)) {

      double x = mrect.x  + (v1 * mrect.w);
      double y = mrect.y;
      double w = (v2 - v1) * mrect.w;
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
        
      SAT_Parameter* param = (SAT_Parameter*)getConnection(0);
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
        
      param = (SAT_Parameter*)getConnection(1);
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
  
//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    fillBackground(AContext);
    drawSliderBar(AContext);
    drawText(AContext);
    drawValues(AContext);
    drawHostIndicators(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }
  

};

//----------------------------------------------------------------------
#endif