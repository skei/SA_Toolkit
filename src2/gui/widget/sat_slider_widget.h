#ifndef sat_slider_widget_included
#define sat_slider_widget_included
//---------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_drag_value_widget.h"
#include "plugin/sat_parameter.h"

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

  bool      MDrawSliderBar    = true;
  SAT_Color MSliderBarColor   = SAT_LightOrange;

  bool      MDrawModulation   = true;
  SAT_Color MModulationColor  = SAT_Color(1,1,1,0.25);

//------------------------------
public:
//------------------------------

  SAT_SliderWidget(SAT_Rect ARect)
  : SAT_DragValueWidget(ARect) {
    setName("SAT_SliderWidget");
  }

  //----------

  virtual ~SAT_SliderWidget() {
  }

//------------------------------
private:
//------------------------------

  virtual void setDrawSliderBar(bool ADraw=true)    { MDrawSliderBar = ADraw; }
  virtual void setSliderBarColor(SAT_Color AColor)  { MSliderBarColor = AColor; }
  virtual void setDrawModulation(bool ADraw=true)   { MDrawModulation = ADraw; }
  virtual void setModulationColor(SAT_Color AColor) { MModulationColor = AColor; }

//------------------------------
private:
//------------------------------

  double calcValue() {
    double value = getValue();
    //SAT_PRINT("value %.3f\n",value);
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    //SAT_PRINT("param %p\n",param);
    if (param) {
      value = param->getNormalizedValue();
      //SAT_PRINT("normalized value %.3f\n",value);
    }
    return value;
  }

  //----------

  double calcModulation(double value) {
    double modulation = value + getModulation();
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {
      modulation = value + param->getNormalizedModulation();
      modulation = SAT_Clamp(modulation,0,1);
    }
    return modulation;
  }

//------------------------------
public:
//------------------------------

  virtual void drawSliderBar(SAT_PaintContext* AContext) {
    if (MDrawSliderBar) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double x = rect.x;
      double y = rect.y;
      double w = rect.w;
      double h = rect.h;
      double v = calcValue();
      double m = calcModulation(v);

      double vw = (v * w);
      if (v > 0) {
        painter->setFillColor(MSliderBarColor);
        painter->fillRect(x,y,vw,h);
      }

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
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  // void on_widget_open(SAT_WidgetOwner* AOwner) override {
  //   do_widget_want_timer(this);
  // }

  // void on_widget_close(SAT_WidgetOwner* AOwner) override {
  //   do_widget_want_timer(this,false);
  // }

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    drawSliderBar(AContext);
    drawText(AContext);
    drawValueText(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

  // void on_widget_timer(double ADelta) {
  //   SAT_PRINT("delta %f\n",ADelta);
  // }

};

//---------------------------------------------------------------------
#endif

