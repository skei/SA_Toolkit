#ifndef sat_knob_widget_included
#define sat_knob_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_drag_value_widget.h"

//----------

class SAT_KnobWidget
: public SAT_DragValueWidget {

//------------------------------
private:
//------------------------------

  bool        MDrawKnobArc            = true;
  bool        MDrawKnobArcBackground  = true;
  SAT_Color   MKnobArcColor           = SAT_LightBlue1;
  SAT_Color   MKnobArcBackgroundColor = 0.4;
  double      MKnobArcThickness       = 8.0;

  bool        MDrawKnobNeedle         = false;
  SAT_Color   MKnobNeedleColor        = SAT_LighterGrey;
  double      MKnobNeedleStart        = 0.0;
  double      MKnobNeedleLength       = 1.0;
  double      MKnobNeedleThickness    = 3.0;

  bool        MDrawModulation         = true;
  SAT_Color   MModulationColor        = SAT_Color(1,1,1,0.25);

  bool        MDrawBipolar            = false;
  double      MBipolarCenter          = 0.5;

//------------------------------
public:
//------------------------------

  SAT_KnobWidget(SAT_Rect ARect)
  : SAT_DragValueWidget(ARect) {
    setName("SAT_KnobWidget");
    //setFillBackground(true);
    //setBackgroundColor(0.45);
    setDrawBorder(false);
    setTextSize(10);
    setTextAlignment(SAT_TEXT_ALIGN_BOTTOM);
    setValueTextSize(10);
    setValueTextAlignment(SAT_TEXT_ALIGN_CENTER);
  }

  //----------

  virtual ~SAT_KnobWidget() {
  }

//------------------------------
public:
//------------------------------


  // void setText(const char* AText)                { MText = AText; }
  // void setTextAlignment(uint32_t AAlignment)     { MTextAlignment = AAlignment; }

  void setDrawKnobArc(bool ADraw=true)              { MDrawKnobArc = ADraw; }
  void setDrawKnobArcBackground(bool ADraw=true)    { MDrawKnobArcBackground = ADraw; }
  void setKnobArcColor(SAT_Color AColor)            { MKnobArcColor = AColor; }
  void setKnobArcBackgroundColor(SAT_Color AColor)  { MKnobArcBackgroundColor = AColor; }
  void setKnobArcThickness(double AThickness)       { MKnobArcThickness = AThickness; }

  void setDrawKnobNeedle(bool ADraw=true)           { MDrawKnobNeedle = ADraw; }
  void setKnobNeedleColor(SAT_Color AColor)         { MKnobNeedleColor = AColor; }
  void setKnobNeedleStart(double AStart)            { MKnobNeedleStart = AStart; }
  void setKnobNeedleLength(double ALength)          { MKnobNeedleLength = ALength; }
  void setKnobNeedleThickness(double AThickness)    { MKnobNeedleThickness = AThickness; }

  virtual void setDrawBipolar(bool AState=true)       { MDrawBipolar = AState; }
  virtual void setBipolarCenter(double AValue)        { MBipolarCenter = AValue; }

  virtual bool isBipolar() { return MDrawBipolar; }


//------------------------------
public:
//------------------------------

  virtual void drawKnobArc(SAT_PaintContext* AContext) {
    if (MDrawKnobArc) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      
      rect.shrink(MKnobArcThickness * 0.5 * scale);

      double x = rect.x;
      double y = rect.y;
      double w = rect.w;
      double h = rect.h;
      double v = calcValue();
      double m = calcModulation();

      double w05 = (w * 0.5);
      double h05 = (h * 0.5);

      double cx = x + w05;
      double cy = y + h05;

      double r = w05;
      if (h05 < w05) r = h05;

      painter->setLineWidth(MKnobArcThickness * scale);

      if (MDrawKnobArcBackground) {
        double a1 = SAT_PI2 * (0.35);
        double a2 = SAT_PI2 * (0.35 + 0.8);
        painter->setDrawColor(MKnobArcBackgroundColor);
        painter->drawArc(cx,cy,r,a1,a2);
      }

      v *= 0.8;
      m *= 0.8;
      double b = MBipolarCenter * 0.8;

      //if (v > 0) {
        double a05  = SAT_PI2 * (b + 0.35);
        double a1 = 0.0;
        double a2 = 0.0;
        if (MDrawBipolar) {
          if (v < b) {
            a1   = SAT_PI2 * (v + 0.35);
            a2   = SAT_PI2 * (b + 0.35);
          }
          else if (v > b){
            a1   = SAT_PI2 * (b + 0.35);
            a2   = SAT_PI2 * (v + 0.35);
          }
        }
        else {
          a1   = SAT_PI2 * (0 + 0.35);
          a2   = SAT_PI2 * (v + 0.35);
        }
        painter->setDrawColor(MKnobArcColor);
        painter->drawArc(cx,cy,r,a1,a2);
      //}

      if (MDrawModulation) {
        double a1,a2;
        if (m > v) {
          a1 = SAT_PI2 * (v + 0.35);
          a2 = SAT_PI2 * (m + 0.35);
        }
        else {
          a1 = SAT_PI2 * (m + 0.35);
          a2 = SAT_PI2 * (v + 0.35);
        }    
        //SAT_PRINT("v %.3f m %.3f x %.3f w %.3f mx %.3f mw %.3f\n",v,m,x,w,mx,mw);
        painter->setDrawColor(MModulationColor);
        painter->drawArc(cx,cy,r,a1,a2);
      }

      if (MDrawKnobNeedle) {

        double a = SAT_PI2 * (v + 0.35);
        double rx = cos(a);
        double ry = sin(a);

        double len     = (r * MKnobNeedleLength) + (MKnobArcThickness * 0.5 * scale);
        double start = len * MKnobNeedleStart;
        double end   = len;

        double x1 = cx + (rx * start);
        double y1 = cy + (ry * start);
        double x2 = cx + (rx * end);
        double y2 = cy + (ry * end);

        painter->setLineWidth(MKnobNeedleThickness * scale);
        painter->setDrawColor(MKnobNeedleColor);
        painter->drawLine(x1,y1,x2,y2);
      }

    }
  }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    drawKnobArc(AContext);
    drawText(AContext);
    drawValueText(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif