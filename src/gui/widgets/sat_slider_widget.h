#ifndef sat_slider_widget_included
#define sat_slider_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "base/utils/sat_math.h"
#include "gui/widgets/sat_drag_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_SliderWidget
: public SAT_DragValueWidget {

//------------------------------
private:
//------------------------------

//------------------------------
protected:
//------------------------------

  bool      MDrawSliderBar      = true;
  SAT_Color MSliderBarColor     = SAT_Grey;
  uint32_t  MSliderBarDirection = SAT_DIRECTION_LEFT;
  SAT_Rect  MSliderBarOffset    = SAT_Rect(0,0,0,0);


  bool      MDrawModulation     = true;
  SAT_Color MModulationColor    = SAT_Color(1,1,1,0.25);
  SAT_Rect  MModulationOffset   = SAT_Rect(0,0,0,0);

  bool      MDrawSliderEdge     = true;
  double    MSliderEdgeWidth    = 2;
  SAT_Color MSliderEdgeColor    = SAT_LighterGrey;
  
  uint32_t  MDrawDirection      = SAT_DIRECTION_RIGHT;

//------------------------------
public:
//------------------------------

  SAT_SliderWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_DragValueWidget(ARect,AText,AValue) {
    setName("SAT_SliderWidget");
    //setNumValues(1);
  }

  virtual ~SAT_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawSliderBar(bool ADraw)           { MDrawSliderBar = ADraw; }
  virtual void setSliderBarColor(SAT_Color AColor)    { MSliderBarColor = AColor; }
  virtual void setSliderBarDirection(uint32_t  ADir)  { MSliderBarDirection = ADir; }
  virtual void setSliderBarOffset(SAT_Rect AOffset)   { MSliderBarOffset = AOffset; }
  virtual void setDrawSliderEdge(bool ADraw)          { MDrawSliderEdge = ADraw; }
  virtual void setSliderEdgeWidth(double AWidth)      { MSliderEdgeWidth = AWidth; }
  virtual void setSliderEdgeColor(SAT_Color AColor)   { MSliderEdgeColor = AColor; }

  virtual void setDrawModulation(bool ADraw)          { MDrawModulation = ADraw; }
  virtual void setModulationColor(SAT_Color AColor)   { MModulationColor = AColor; }

  virtual void setDrawDirection(uint32_t ADir)        { MDrawDirection = ADir; }

//------------------------------
public:
//------------------------------

  virtual void drawSliderBar(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    
    if (MDrawSliderBar) {

      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      SAT_Rect sliderbaroffset = MSliderBarOffset;
      sliderbaroffset.scale(S);
      mrect.shrink(sliderbaroffset);




      if ((MDrawDirection == SAT_DIRECTION_LEFT) || (MDrawDirection == SAT_DIRECTION_RIGHT)) {
        double sew05 = MSliderEdgeWidth * 0.5 * S;
        mrect.shrink(sew05,0,sew05,0);
        if (mrect.w <= 0.0) return;
        if (mrect.h <= 0.0) return;
        // bar
        painter->setFillColor(MSliderBarColor);
        SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
        double value = 0.0;
        if (param) value = param->getNormalizedValue();
        else value = getValue();
        
        if (isBipolar()) {
          double v1 = 0.0;
          double v2 = 0.0;
          double bpc = getBipolarCenter();
          if (value < bpc) {
            v1 = value;
            v2 = bpc;
          }
          else if (value > bpc) {
            v1 = bpc;
            v2 = value;
          }
          double x1 = mrect.x + (v1 * mrect.w);
          double w2 = (v2 - v1) * mrect.w;
          if (w2 > 0) painter->fillRect(x1,mrect.y,w2,mrect.h);
          
        }
        else { // ! bipolar
          double w2 = (mrect.w * value);
          if (w2 > 0) {
            painter->fillRect(mrect.x,mrect.y,w2,mrect.h);
          }
        } // bipolar
        
        // edge
        if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {
          double x = mrect.x + (value * mrect.w);
          x -= (MSliderEdgeWidth * 0.5 * S);
          painter->setFillColor(MSliderEdgeColor);
          painter->fillRect(x,mrect.y,MSliderEdgeWidth*S,mrect.h);
        } // edge
        
        // modulation
        double modulation = 0.0;//value + getModulation();
        if (param) modulation = param->getNormalizedModulation();
        else modulation = getModulation();
        modulation = SAT_Clamp(modulation + value,0,1);
        if (MDrawModulation) {
          //double S = getWindowScale();
          mrect = getRect(); // reset
          SAT_Rect modulationoffset = MModulationOffset;
          modulationoffset.scale(S);
          mrect.shrink(modulationoffset);
          double v1 = 0;
          double v2 = 0;
          if (modulation < value) {
            v1 = modulation;
            v2 = value;
          }
          else if (modulation > value) {
            v1 = value;
            v2 = modulation;
          }
          double x1 = mrect.x + (v1 * mrect.w);
          double w2 = (v2 - v1) * mrect.w;
          if (w2 > 0) {
            painter->setFillColor(MModulationColor);
            painter->fillRect(x1,mrect.y,w2,mrect.h);
          }
        }
      }



      
      // and the same thing, but vertically



      
      else if ((MDrawDirection == SAT_DIRECTION_UP) || (MDrawDirection == SAT_DIRECTION_DOWN)) {
        double sew05 = MSliderEdgeWidth * 0.5 * S;
        mrect.shrink(sew05,0,sew05,0);
        if (mrect.w <= 0.0) return;
        if (mrect.h <= 0.0) return;
        // bar
        painter->setFillColor(MSliderBarColor);
        SAT_Parameter* param = (SAT_Parameter*)getParameter(0);
        double value = 0.0;
        if (param) value = param->getNormalizedValue();
        else value = getValue();
        
        if (isBipolar()) {
//          double v1 = 0.0;
//          double v2 = 0.0;
//          double bpc = getBipolarCenter();
//          if (value < bpc) {
//            v1 = value;
//            v2 = bpc;
//          }
//          else if (value > bpc) {
//            v1 = bpc;
//            v2 = value;
//          }
//          double x1 = mrect.x + (v1 * mrect.w);
//          double w2 = (v2 - v1) * mrect.w;
//          if (w2 > 0) painter->fillRect(x1,mrect.y,w2,mrect.h);
//          
        }
        else { // ! bipolar

          double h2 = (mrect.h * value);
          if (h2 > 0) {
            painter->fillRect(mrect.x,mrect.y2() - h2,mrect.w,h2);
          }
          
        } // bipolar
        
        // edge
        if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {
          double y = mrect.y2() - (value * mrect.h);
          y -= (MSliderEdgeWidth * 0.5 * S);
          painter->setFillColor(MSliderEdgeColor);
          painter->fillRect(mrect.x,y,mrect.w,MSliderEdgeWidth*S);
        } // edge
        
//        // modulation
//        double modulation = 0.0;//value + getModulation();
//        if (param) modulation = param->getNormalizedModulation();
//        else modulation = getModulation();
//        modulation = SAT_Clamp(modulation + value,0,1);
//        if (MDrawModulation) {
//          //double S = getWindowScale();
//          mrect = getRect(); // reset
//          SAT_Rect modulationoffset = MModulationOffset;
//          modulationoffset.scale(S);
//          mrect.shrink(modulationoffset);
//          double v1 = 0;
//          double v2 = 0;
//          if (modulation < value) {
//            v1 = modulation;
//            v2 = value;
//          }
//          else if (modulation > value) {
//            v1 = value;
//            v2 = modulation;
//          }
//          double x1 = mrect.x + (v1 * mrect.w);
//          double w2 = (v2 - v1) * mrect.w;
//          if (w2 > 0) {
//            painter->setFillColor(MModulationColor);
//            painter->fillRect(x1,mrect.y,w2,mrect.h);
//          }
//        }
        
      }
      



      
      
      

    }
  }
  
  //

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    fillBackground(AContext);
    drawSliderBar(AContext);
    drawText(AContext);
    drawValue(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
    drawHostIndicators(AContext);
  }

  //void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //  SAT_DragValueWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
  //}

  //void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //  SAT_DragValueWidget::on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATime);
  //}

  //void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
  //  SAT_DragValueWidget::on_widget_mouse_move(AXpos,AYpos,AState,ATime);
  //}
  
};

//----------------------------------------------------------------------
#endif




