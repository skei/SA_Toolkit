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

  // void setText(const char* AText)             { MText = AText; }
  void setDrawKnobArc(bool ADraw=true)           { MDrawKnobArc = ADraw; }
  void setKnobArcColor(SAT_Color AColor)         { MKnobArcColor = AColor; }
  void setKnobArcThickness(double AThickness)    { MKnobArcThickness = AThickness; }
  // void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }

//------------------------------
public:
//------------------------------

  /*


      if (MDrawModulation) {
        double mx = 0.0;
        double mw = 0.0;
        if (m > v) {
          mx = x + (v * w);
          mw = w * (m - v);
        }
        else {
          mx = x + (m * w);
          mw = w * (v - m);
        }    
        //SAT_PRINT("v %.3f m %.3f x %.3f w %.3f mx %.3f mw %.3f\n",v,m,x,w,mx,mw);
        painter->setFillColor(MModulationColor);
        painter->fillRect(mx,y,mw,h);
      }
  */

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
      //double vw = (v * w);

      if (v > 0) {
        double a1 = SAT_PI2 * (0 + 0.35); //0.65;
        double a2 = SAT_PI2 * (v + 0.35); //-0.15 + (1 - v);
        painter->setDrawColor(MKnobArcColor);
        painter->drawArc(cx,cy,r,a1,a2);
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
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif