#ifndef sat_dual_slider_widget_included
#define sat_dual_slider_widget_included
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

//------------------------------
public:
//------------------------------

  SAT_DualSliderWidget(SAT_Rect ARect, const char* AText, double AValue1, double AValue2)
  : SAT_SliderWidget(ARect,AText,AValue1) {
    setName("SAT_DualSliderWidget");
    setNumValues(2);
    setValue(AValue2,1);
    setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    setDragDirection(SAT_DIRECTION_RIGHT);
    //strcpy(MText,AText);
    //setTextAlignment(SAT_TEXT_ALIGN_CENTER);
    setTextAlignment(SAT_TEXT_ALIGN_CENTER);
    
  }
  
  //----------

  virtual ~SAT_DualSliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValues(bool ADraw=true)                          { MDrawValues = ADraw; }
  virtual void setValueColors(SAT_Color AColor1, SAT_Color AColor2)    { MValue1Color = AColor1, MValue2Color = AColor2; }
  virtual void setValueSizes(double ASize1, double ASize2)             { MValue1Size = ASize1, MValue2Size = ASize2; }
  virtual void setValueAlignments(uint32_t AAlign1, uint32_t AAlign2)  { MValue1Alignment = AAlign1; MValue2Alignment = AAlign2; }
  virtual void setValueOffsets(SAT_Rect AOffset1, SAT_Rect AOffset2)   { MValue1Offset = AOffset1, MValue2Offset = AOffset2; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    fillBackground(AContext);
    drawSliderBar(AContext);
    drawText(AContext);
    drawValues(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
    drawHostIndicators(AContext);
  }

//------------------------------
public:
//------------------------------


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

  void drawSliderBar(SAT_PaintContext* AContext) override {
    
    //SAT_SliderWidget::drawSliderBar(AContext);

    // slider

    if (MDrawSliderBar) {
      
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();

      SAT_Rect sliderbaroffset = MSliderBarOffset;
      sliderbaroffset.scale(S);
      mrect.shrink(sliderbaroffset);

      double sew05 = MSliderEdgeWidth * 0.5 * S;
      mrect.shrink(sew05,0,sew05,0);

      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;

      // bar

      painter->setFillColor(MSliderBarColor);
      
      SAT_Parameter* param1 = (SAT_Parameter*)getParameter(0);
      SAT_Parameter* param2 = (SAT_Parameter*)getParameter(1);
      
      double v1 = 0.0;
      if (param1) v1 = param1->getNormalizedValue();
      else v1 = getValue(0);

      double v2 = 0.0;
      if (param2) v2 = param2->getNormalizedValue(); // v2 = 0
      else v2 = getValue(1);

      double x = mrect.x + (v1 * mrect.w);
      double y = mrect.y;
      double w = abs(v2 - v1) * mrect.w;
      double h = mrect.h;
      //SAT_Print("v1 %.2f v2 %.2f x %.1f y %.1f w %.1f h %.1f\n",v1,v2,x,y,w,h);
      if (w > 0) {
        painter->fillRect(x,y,w,h);
      }
        
      // edges

      if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {
        
        double x1 = mrect.x + (v1 * mrect.w);
        double x2 = mrect.x + (v2 * mrect.w);

        x1 -= (MSliderEdgeWidth * 0.5 * S);
        x2 -= (MSliderEdgeWidth * 0.5 * S);
        
        painter->setFillColor(MSliderEdgeColor);
        painter->fillRect(x1,mrect.y,MSliderEdgeWidth*S,mrect.h);
        painter->fillRect(x2,mrect.y,MSliderEdgeWidth*S,mrect.h);
      } // draw edge

      // modulation

      if (MDrawModulation) {
        
        //double S = getWindowScale();
        mrect = getRect(); // reset
        SAT_Rect modulationoffset = MModulationOffset;
        modulationoffset.scale(S);
        mrect.shrink(modulationoffset);
        
        // 1

        double m1 = 0.0;
        if (param1) m1 = param1->getNormalizedModulation();
        else m1 = getModulation(0);
        m1 = SAT_Clamp(m1 + v1,0,1);

        double m1v1 = 0;
        double m1v2 = 0;
        if (m1 < v1) {
          m1v1 = m1;
          m1v2 = v1;
        }
        else if (m1 > v1) {
          m1v1 = v1;
          m1v2 = m1;
        }
        double m1x = mrect.x + (m1v1 * mrect.w);
        double m1w = (m1v2 - m1v1) * mrect.w;

        if (m1w > 0) {
          painter->setFillColor(MModulationColor);
          painter->fillRect(m1x,mrect.y,m1w,mrect.h);
        }
        
        // 2

        double m2 = 0.0;
        if (param2) m2 = param2->getNormalizedModulation();
        else m2 = getModulation(1);
        m2 = SAT_Clamp(m2 + v2,0,1);

        double m2v1 = 0;
        double m2v2 = 0;
        if (m2 < v2) {
          m2v1 = m2;
          m2v2 = v2;
        }
        else if (m2 > v2) {
          m2v1 = v2;
          m2v2 = m2;
        }
        double m2x = mrect.x + (m2v1 * mrect.w);
        double m2w = (m2v2 - m2v1) * mrect.w;
        
        if (m2w > 0) {
          painter->setFillColor(MModulationColor);
          painter->fillRect(m2x,mrect.y,m2w,mrect.h);
        }

      } // draw mod

    } // draw bar

  }
  
  
};

//----------------------------------------------------------------------
#endif

