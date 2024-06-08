#ifndef sat_slider_widget_included
#define sat_slider_widget_included
//----------------------------------------------------------------------

#include "sat.h"
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
  SAT_Color MSliderBarColor     = SAT_DarkestGrey;
  uint32_t  MSliderBarDirection = SAT_DIRECTION_LEFT;
  SAT_Rect  MSliderBarOffset    = SAT_Rect(0,0,0,0);


  bool      MDrawModulation     = true;
  SAT_Color MModulationColor    = SAT_Color(1,0,0,0.4);
  SAT_Rect  MModulationOffset   = SAT_Rect(0,0,0,0);

  bool      MDrawSliderEdge     = true;
  double    MSliderEdgeWidth    = 2;
  SAT_Color MSliderEdgeColor    = SAT_LightestGrey;
  
  uint32_t  MDrawDirection      = SAT_DIRECTION_RIGHT;

  bool      MDrawIndicator      = false;
  double    MIndicatorValue     = 0.75;
  SAT_Color MIndicatorColor     = SAT_LightYellow;
  double    MIndicatorThickness = 1;
  SAT_Rect  MIndicatorOffset    = SAT_Rect(0,0,0,0);  


//------------------------------
public:
//------------------------------

  SAT_SliderWidget(SAT_Rect ARect, const char* AText, double AValue=0.0)
  : SAT_DragValueWidget(ARect,AText,AValue) {
    setName("SAT_SliderWidget");
    //setNumValues(1);
  }

  virtual ~SAT_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawSliderBar(bool ADraw)             { MDrawSliderBar = ADraw; }
  virtual void setSliderBarColor(SAT_Color AColor)      { MSliderBarColor = AColor; }
  virtual void setSliderBarDirection(uint32_t  ADir)    { MSliderBarDirection = ADir; }
  virtual void setSliderBarOffset(SAT_Rect AOffset)     { MSliderBarOffset = AOffset; }
  virtual void setDrawSliderEdge(bool ADraw)            { MDrawSliderEdge = ADraw; }
  virtual void setSliderEdgeWidth(double AWidth)        { MSliderEdgeWidth = AWidth; }
  virtual void setSliderEdgeColor(SAT_Color AColor)     { MSliderEdgeColor = AColor; }

  virtual void setDrawModulation(bool ADraw)            { MDrawModulation = ADraw; }
  virtual void setModulationColor(SAT_Color AColor)     { MModulationColor = AColor; }

  virtual void setDrawDirection(uint32_t ADir)          { MDrawDirection = ADir; }

  virtual void setDrawIndicator(bool ADraw)             { MDrawIndicator = ADraw; }
  virtual void setIndicatorValue(double AValue)         { MIndicatorValue = AValue; }
  virtual void setIndicatorColor(SAT_Color AColor)      { MIndicatorColor = AColor; }
  virtual void setIndicatorThickness(double AThickness) { MIndicatorThickness = AThickness; }
  virtual void setIndicatorOffset(SAT_Rect AOffset)     { MIndicatorOffset = AOffset; }

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

        SAT_Parameter* param1 = (SAT_Parameter*)getParameter();
        SAT_Parameter* param2 = (SAT_Parameter*)getParameter(1);

        double value1 = 0.0;
        if (param1) value1 = param1->getNormalizedValue();
        else value1 = getValue(0);

//

        if (getNumValues() == 2) {

          double value2 = 0.0;
          if (param2) value2 = param2->getNormalizedValue();
          else value2 = getValue(1);
          //else value2 = getSelectedValue();

//SAT_Print("value1 %.3f value2 %.3f\n",value1,value2);

          if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {

            double x1 = mrect.x + (value1 * mrect.w);
            double x2 = mrect.x + (value2 * mrect.w);
            double w = x2 - x1;

            x1 -= (MSliderEdgeWidth * 0.5 * S);
            x2 -= (MSliderEdgeWidth * 0.5 * S);

            // bar
            if (w > 0) {
              painter->fillRect(x1,mrect.y,w,mrect.h);
            }            

            // edge1
            painter->setFillColor(MSliderEdgeColor);
            painter->fillRect(x1,mrect.y,MSliderEdgeWidth*S,mrect.h);

            // edge2
            painter->setFillColor(MSliderEdgeColor);
            painter->fillRect(x2,mrect.y,MSliderEdgeWidth*S,mrect.h);

            // modulation

          }
        }

//


        else { // numvalues != 2
        
          if (isBipolar()) {
            double v1 = 0.0;
            double v2 = 0.0;
            double bpc = getBipolarCenter();
            if (value1 < bpc) {
              v1 = value1;
              v2 = bpc;
            }
            else if (value1 > bpc) {
              v1 = bpc;
              v2 = value1;
            }
            double x1 = mrect.x + (v1 * mrect.w);
            double w2 = (v2 - v1) * mrect.w;
            if (w2 > 0) painter->fillRect(x1,mrect.y,w2,mrect.h);
            
          }

          else { // ! bipolar
            double w2 = (mrect.w * value1);
            if (w2 > 0) {
              painter->fillRect(mrect.x,mrect.y,w2,mrect.h);
            }
          } // bipolar
          
          // edge
          if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {
            double x = mrect.x + (value1 * mrect.w);
            x -= (MSliderEdgeWidth * 0.5 * S);
            painter->setFillColor(MSliderEdgeColor);
            painter->fillRect(x,mrect.y,MSliderEdgeWidth*S,mrect.h);
          } // edge

          // modulation

          double modulation1 = 0.0;//value1 + getModulation();
          if (param1) modulation1 = param1->getNormalizedModulation();
          else modulation1 = 0.0;//getModulation();
          modulation1 = SAT_Clamp(modulation1 + value1,0,1);

          if (MDrawModulation) {

            //double S = getWindowScale();
            mrect = getRect(); // reset
            SAT_Rect modulationoffset = MModulationOffset;
            modulationoffset.scale(S);
            mrect.shrink(modulationoffset);

            double v1 = 0;
            double v2 = 0;
            if (modulation1 < value1) {
              v1 = modulation1;
              v2 = value1;
            }
            else if (modulation1 > value1) {
              v1 = value1;
              v2 = modulation1;
            }
            double x1 = mrect.x + (v1 * mrect.w);
            double w2 = (v2 - v1) * mrect.w;
            if (w2 > 0) {
              painter->setFillColor(MModulationColor);
              painter->fillRect(x1,mrect.y,w2,mrect.h);
            }
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
        SAT_Parameter* param = (SAT_Parameter*)getParameter();
        double value1 = 0.0;
        if (param) value1 = param->getNormalizedValue();
        else value1 = getValue();
        
        if (isBipolar()) {
          // double v1 = 0.0;
          // double v2 = 0.0;
          // double bpc = getBipolarCenter();
          // if (value1 < bpc) {
          //   v1 = value1;
          //   v2 = bpc;
          // }
          // else if (value1 > bpc) {
          //   v1 = bpc;
          //   v2 = value1;
          // }
          // double x1 = mrect.x + (v1 * mrect.w);
          // double w2 = (v2 - v1) * mrect.w;
          // if (w2 > 0) painter->fillRect(x1,mrect.y,w2,mrect.h);
        }

        else { // ! bipolar
          double h2 = (mrect.h * value1);
          if (h2 > 0) {
            painter->fillRect(mrect.x,mrect.y2() - h2,mrect.w,h2);
          }
        } // bipolar
        
        // edge
        if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {
          double y = mrect.y2() - (value1 * mrect.h);
          y -= (MSliderEdgeWidth * 0.5 * S);
          painter->setFillColor(MSliderEdgeColor);
          painter->fillRect(mrect.x,y,mrect.w,MSliderEdgeWidth*S);
        } // edge
        
        // // modulation
        //
        // double modulation = 0.0;//value1 + getModulation();
        // if (param) modulation = param->getNormalizedModulation();
        // else modulation = getModulation();
        // modulation = SAT_Clamp(modulation + value1,0,1);
        // if (MDrawModulation) {
        //   //double S = getWindowScale();
        //   mrect = getRect(); // reset
        //   SAT_Rect modulationoffset = MModulationOffset;
        //   modulationoffset.scale(S);
        //   mrect.shrink(modulationoffset);
        //   double v1 = 0;
        //   double v2 = 0;
        //   if (modulation < value1) {
        //     v1 = modulation;
        //     v2 = value1;
        //   }
        //   else if (modulation > value1) {
        //     v1 = value1;
        //     v2 = modulation;
        //   }
        //   double x1 = mrect.x + (v1 * mrect.w);
        //   double w2 = (v2 - v1) * mrect.w;
        //   if (w2 > 0) {
        //     painter->setFillColor(MModulationColor);
        //     painter->fillRect(x1,mrect.y,w2,mrect.h);
        //   }
        // }
        
      }
    }
  }
  
  //----------

  virtual void drawIndicator(SAT_PaintContext* AContext) {
    if (MDrawIndicator) {
      if (MIndicatorValue >= 0.0) {
        double S = getWindowScale();
        SAT_Painter* painter = AContext->painter;
        SAT_Assert(painter);
        SAT_Rect mrect = getRect();
        SAT_Rect indicatoroffset = MIndicatorOffset;
        indicatoroffset.scale(S);
        mrect.shrink(indicatoroffset);

        if ((MDrawDirection == SAT_DIRECTION_LEFT) || (MDrawDirection == SAT_DIRECTION_RIGHT)) {
          if (isActive()) {
            double v = MIndicatorValue;
            double x1 = mrect.x + (mrect.w * v);
            double y1 = mrect.y;
            double x2 = x1;
            double y2 = mrect.y2();
            //SAT_Print("MIndicatorValue %.3f x1 %.3f y1 %.3f x2 %.3f y1 %.3f\n",v,x1,y1,x2,y2);
            painter->setLineWidth(MIndicatorThickness*S);
            painter->setDrawColor(MIndicatorColor);
            painter->drawLine(x1,y1,x2,y2);
            //painter->drawRect(x1,y1,5,5);
          }
        }

        else { // vertical
        }

      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_Print("%s : counter %i w/h %.2f,%.2f\n",getName(),AContext->counter,getWidth(),getHeight());
    //drawDropShadow(AContext);
    fillBackground(AContext);
    drawSliderBar(AContext);
    drawText(AContext);
    if (getNumValues() == 2) {
      drawValue(AContext,0);
      drawValue(AContext,1);
    }
    else {
      drawValue(AContext);
    }
    drawIndicator(AContext);
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



