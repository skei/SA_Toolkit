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

  bool      MDrawSliderBar      = true;
  SAT_Color MSliderBarColor     = SAT_LightBlue1;
  uint32_t  MSliderBarDirection = SAT_DIRECTION_LEFT;
  SAT_Rect  MSliderBarOffset    = SAT_Rect(0,0,0,0);

  bool      MDrawModulation     = true;
  SAT_Color MModulationColor    = SAT_LightBlue1;
  SAT_Rect  MModulationOffset   = SAT_Rect(0,0,0,0);

  bool      MDrawSliderEdge     = true;
  double    MSliderEdgeWidth    = 5;
  SAT_Color MSliderEdgeColor    = SAT_White;

  bool      MBipolar            = true;
  double    MBipolarCenter      = 0.5;

//------------------------------
public:
//------------------------------

  SAT_SliderWidget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_DragValueWidget(ARect,AText,AValue) {
  }

  virtual ~SAT_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  void setDrawSliderBar(bool ADraw)          { MDrawSliderBar = ADraw; }
  void setSliderBarColor(SAT_Color AColor)   { MSliderBarColor = AColor; }
  void setSliderBarDirection(uint32_t  ADir) { MSliderBarDirection = ADir; }
  void setSliderBarOffset(SAT_Rect AOffset)  { MSliderBarOffset = AOffset; }
  void setDrawSliderEdge(bool ADraw)         { MDrawSliderEdge = ADraw; }
  void setSliderEdgeWidth(double AWidth)     { MSliderEdgeWidth = AWidth; }
  void setSliderEdgeColor(SAT_Color AColor)  { MSliderEdgeColor = AColor; }
  void setBipolar(bool ABipolar)             { MBipolar = ABipolar; }
  void setBipolarCenter(double APos)         { MBipolarCenter = APos; }

  void setDrawModulation(bool ADraw)         { MDrawModulation = ADraw; }
  void setModulationColor(SAT_Color AColor)  { MModulationColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void drawSliderBar(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);

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
      double value = getValue();

      if (MBipolar) {
        double v1,v2;
        if (value < MBipolarCenter) {
          v1 = value;
          v2 = MBipolarCenter;
        }
        else if (value > MBipolarCenter) {
          v1 = MBipolarCenter;
          v2 = value;
        }
        double x1 = mrect.x + (v1 * mrect.w);
        double w2 = (v2 - v1) * mrect.w;
        if (w2 > 0) painter->fillRect(x1,mrect.y,w2,mrect.h);
      } // bipolar
      else {
        double w2 = (mrect.w * value);
        if (w2 > 0) {
          painter->fillRect(mrect.x,mrect.y,w2,mrect.h);
        }
      }

      // edge

      if (MDrawSliderEdge && (MSliderEdgeWidth > 0)) {
        double x = mrect.x + (value * mrect.w);
        x -= (MSliderEdgeWidth * 0.5 * S);
        painter->setFillColor(MSliderEdgeColor);
        painter->fillRect(x,mrect.y,MSliderEdgeWidth*S,mrect.h);
      } // edge

      // modulation

      double modulation = value + getModulation();
      modulation = SAT_Clamp(modulation,0,1);

      if (MDrawModulation) {
        //double S = getWindowScale();
        mrect = getRect(); // reset
        SAT_Rect modulationoffset = MModulationOffset;
        modulationoffset.scale(S);
        mrect.shrink(modulationoffset);

        double v1,v2;
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
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_PRINT;
    fillBackground(AContext);
    drawSliderBar(AContext);
    drawText(AContext);
    drawValue(AContext);
    //paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif




