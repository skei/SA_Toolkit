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

  bool      MDrawQuantized    = true;
  SAT_Color MQuantizedColor   = SAT_Color(0,0,0,0.5);
  double    MQuantizedWidth   = 1.0;

  bool      MDrawBipolar      = false;
  double    MBipolarCenter    = 0.5;

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
public:
//------------------------------

  virtual bool      isBipolar()         { return MDrawBipolar; }
  virtual double    getBipolarCenter()  { return MBipolarCenter; }

  //

  virtual void setDrawSliderBar(bool ADraw=true)    { MDrawSliderBar = ADraw; }
  virtual void setSliderBarColor(SAT_Color AColor)  { MSliderBarColor = AColor; }
  virtual void setDrawModulation(bool ADraw=true)   { MDrawModulation = ADraw; }
  virtual void setModulationColor(SAT_Color AColor) { MModulationColor = AColor; }
  virtual void setDrawQuantized(bool ADraw=true)    { MDrawQuantized = ADraw; }
  virtual void setQuantizedColor(SAT_Color AColor)  { MQuantizedColor = AColor; }
  virtual void setQuantizedWidth(double AWidth)     { MQuantizedWidth = AWidth; }

  virtual void setDrawBipolar(bool AState=true)     { MDrawBipolar = AState; }
  virtual void setBipolarCenter(double AValue)      { MBipolarCenter = AValue; }



//------------------------------
public:
//------------------------------

  virtual void drawQuantized(SAT_PaintContext* AContext) {
    if (MDrawQuantized) {
      SAT_Rect rect = getRect();
      double x = rect.x;
      double y = rect.y;
      double w = rect.w;
      double h = rect.h;
      //SAT_TRACE;
      if (getQuantize()) {
        uint32_t num = getQuantizeSteps();
        if (num > 2) {

          SAT_Painter* painter = AContext->painter;
          double scale = getWindowScale();

          painter->setDrawColor(MQuantizedColor);
          painter->setLineWidth(MQuantizedWidth*scale);

          double size = w / (num - 1);
          double pos  = x + size;
          for (uint32_t i=1; i<(num-1); i++) {
            painter->drawLine(pos,y,pos,y+h);
            pos += size;
          }

          painter->setLineWidth(1.0);

        }
      }
    }
  }

  //----------

  virtual void drawSliderBar(SAT_PaintContext* AContext) {
    if (MDrawSliderBar) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double scale = getWindowScale();
      double x = rect.x;
      double y = rect.y;
      double w = rect.w;
      double h = rect.h;
      double v = calcValue();
      double m = calcModulation();

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
    drawQuantized(AContext);
    drawValueBar(AContext,getDragValue());
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

