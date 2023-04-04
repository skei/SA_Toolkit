#ifndef sat_knob_widget_included
#define sat_knob_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_drag_value_widget.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_KnobWidget
: public SAT_DragValueWidget {

//------------------------------
private:
//------------------------------


//------------------------------
protected:
//------------------------------


  bool      MDrawArc                  = true;
  SAT_Color MArcBackColor             = SAT_DarkGrey;
  SAT_Color MArcValueColor            = SAT_LightGrey;
  SAT_Color MInteractiveArcValueColor = SAT_White;
  double    MArcThickness             = 8.0;

  double    MModArcOffset             = 2.0;// 13.0;
  double    MModArcThickness          = 4.0;
  //SAT_Color MModArcColor              = SAT_COLOR_LIGHT_RED;

  bool      MDrawMarker               = false;
  double    MMarkerValue              = 0.75;
  SAT_Color MMarkerColor              = SAT_White;
  double    MMarkerThickness          = 0.01;

//------------------------------
public:
//------------------------------

  SAT_KnobWidget(SAT_Rect ARect, const char* AText="%", double AValue=0.0)
  : SAT_DragValueWidget(ARect,AText,AValue) {
    setName("SAT_KnobWidget");

    setFillBackground(true);
    setDrawBorder(false);

    //setDrawValue(false);
    setDrawValue(true);
    setValueAlignment(SAT_TEXT_ALIGN_CENTER);
    setValueOffset(0);
    setValueSize(12);

    //setDrawText(false);
    setDrawText(true);
    setTextAlignment(SAT_TEXT_ALIGN_BOTTOM);
    setTextOffset(0);
    setTextSize(10);

    //setDrawIndicator(true);

  }

  //----------

  virtual ~SAT_KnobWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawArc(bool ADraw=true)                    { MDrawArc = ADraw; }
  virtual void setArcBackColor(SAT_Color AColor)              { MArcBackColor = AColor; }
  virtual void setArcValueColor(SAT_Color AColor)             { MArcValueColor = AColor; }
  virtual void setInteractiveArcValueColor(SAT_Color AColor)  { MInteractiveArcValueColor = AColor; }
  virtual void setArcThickness(double AThickness)             { MArcThickness = AThickness; }
  virtual void setModArcThickness(double AThickness)          { MModArcThickness = AThickness; }
  virtual void setModArcOffset(double AOffset)                { MModArcOffset = AOffset; }

//------------------------------
public:
//------------------------------

  virtual void drawArc(SAT_PaintContext* AContext) {

    if (MDrawArc) {

      //SAT_Window* window = (SAT_Window*)getOwnerWindow();
      //double S = window->getWindowScale();
      double S = getWindowScale();
      SAT_Rect mrect = getRect();
      SAT_Painter* painter = AContext->painter;
      double value = getValue();

      //SAT_Parameter* param = getParameter();
      SAT_Parameter* param = (SAT_Parameter*)getConnection();

      if (param) {
        value = param->normalizeValue(value);
      }

      double thickness  = MArcThickness * S;
      //double bthick = MArcThickness * S * 0.5;

      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double r  = (mrect.w - thickness)  * 0.5;
      //double br = (mrect.w - bthick) * 0.5;

      // draw background arc

      SAT_Color color = SAT_DarkerGrey;//SAT_Black;//MArcBackColor;
      //if (isDisabled()) color.blend(getDisabledColor(),getDisabledAlpha());
      painter->setDrawColor(color);
      painter->setLineWidth(thickness);

      double a1 = 0.35;
      double a2 = a1 + 0.8;
      painter->drawArc(cx,cy,r,a1*SAT_PI2,a2*SAT_PI2);

      // draw value arc

      painter->setLineWidth(thickness);

      if (isInteracting()) {
        SAT_Color color = MInteractiveArcValueColor;
        if (isDisabled()) color.blend(getDisabledColor(),getDisabledAlpha());
        painter->setDrawColor(color);
      }
      else {
        SAT_Color color = MArcValueColor;
        if (isDisabled()) color.blend(getDisabledColor(),getDisabledAlpha());
        painter->setDrawColor(color);
      }

      if (isBipolar()) {
        double bc = getBipolarCenter();
        if (value < bc) {
          double a1 = 0.35 + (value      * 0.8);
          double a2 = a1   + ((bc-value) * 0.8);
          painter->drawArc(cx,cy,r,a1*SAT_PI2,a2*SAT_PI2);
        }
        else if (value > bc) {
          double a1 = 0.35 + (bc         * 0.8);
          double a2 = a1   + ((value-bc) * 0.8);
          painter->drawArc(cx,cy,r,a1*SAT_PI2,a2*SAT_PI2);
        }
      }
      else {
        double a1 = 0.35 + (0.0   * 0.8);
        double a2 = a1   + (value * 0.8);
        painter->drawArc(cx,cy,r,a1*SAT_PI2,a2*SAT_PI2);

      }

      // draw modulation

  //    if (MDrawModulation) {
  //      if (!isDisabled()) {
  //
  //        painter->setLineWidth(MModArcThickness*S);
  //        painter->setDrawColor(MModArcColor);
  //
  //        double modulation = MModulations[0];
  //
  //        if (modulation < 0) {
  //          if ((value + modulation) < 0) modulation = - value;
  //          double aa1 = (0.35 + ((value)  * 0.8)) * SAT_PI2;
  //          double aa2 = (       ((-modulation) * 0.8)) * SAT_PI2;
  //          painter->drawArc(cx,cy,r - (MModArcOffset*S),aa1,aa2);
  //        }
  //        else if (modulation > 0) {
  //          if ((value + modulation) > 1) modulation = 1 - value;
  //          double aa1 = (0.35 + ((value+modulation)  * 0.8)) * SAT_PI2;
  //          double aa2 = (       ((modulation) * 0.8)) * SAT_PI2;
  //          painter->drawArc(cx,cy,r - (MModArcOffset*S),aa1,aa2);
  //        }
  //        //else {
  //        //}
  //
  //      }
  //    }

      // draw indicator

      if (MDrawMarker) {
        if (!isDisabled()) {
          if (MMarkerValue >= 0.0) {
            //double iv = MMarkerValue + (0.02 * 0.5);
            //double a1 = (0.35 + (iv * 0.8)) * SAT_PI2;
            //double a2 =  0.02 * SAT_PI2;

            double ms = MMarkerThickness * 0.5;
            double a1 = 0.35 + ((MMarkerValue - ms) * 0.8);
            double a2 = a1   + ((          ms + ms) * 0.8);

            painter->setLineWidth(thickness);
            color = MMarkerColor;
            painter->setDrawColor(color);
            painter->drawArc(cx,cy,r,a1*SAT_PI2,a2*SAT_PI2);
          }
        }
      }

    }
  }

  //----------

  void drawModulation(SAT_PaintContext* AContext) override {
    if (getDrawModulation()) {
//    //SAT_Window* window = (SAT_Window*)getOwnerWindow();
//    //double S = window->getWindowScale();
//    double S = getWindowScale();
//    SAT_Rect mrect = getRect();
//    SAT_Painter* painter = AContext->painter;
//
//    double value = getValue();
//    //SAT_Parameter* param = getParameter();
//    SAT_Parameter* param = (SAT_Parameter*)getConnection();
//    if (param) {
//      value = param->normalizeValue(value);
//    }
//
//    double thickness  = MArcThickness * S;
//    //double bthick = MArcThickness * S * 0.5;
//
//    double cx = mrect.x + (mrect.w * 0.5);
//    double cy = mrect.y + (mrect.h * 0.5);
//    double r  = (mrect.w - thickness)  * 0.5;
//    //double br = (mrect.w - bthick) * 0.5;
//
//    if (!isDisabled()) {
//
//      painter->setLineWidth(MModArcThickness*S);
//      //painter->setDrawColor(MModArcColor);
//      painter->setDrawColor(MModulationColor);
//
//      double modulation = MModulations[0];
//
//      if (modulation < 0) {
//        if ((value + modulation) < 0) modulation = - value;
//        double aa1 = (0.35 + ((value)  * 0.8)) * SAT_PI2;
//        double aa2 = (       ((-modulation) * 0.8)) * SAT_PI2;
//        painter->drawArc(cx,cy,r - (MModArcOffset*S),aa1,aa2);
//      }
//      else if (modulation > 0) {
//        if ((value + modulation) > 1) modulation = 1 - value;
//        double aa1 = (0.35 + ((value+modulation)  * 0.8)) * SAT_PI2;
//        double aa2 = (       ((modulation) * 0.8)) * SAT_PI2;
//        painter->drawArc(cx,cy,r - (MModArcOffset*S),aa1,aa2);
//      }
//      //else {
//      //}
//
//    }
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    drawArc(AContext);
    drawText(AContext);
    drawValue(AContext);
    drawModulation(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);

  }


};

//----------------------------------------------------------------------
#endif

