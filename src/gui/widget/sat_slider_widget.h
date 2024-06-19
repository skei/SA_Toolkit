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
protected:
//------------------------------

  bool      MDrawSliderBar    = true;
  SAT_Color MSliderBarColor   = SAT_LightOrange;

  bool      MDrawModulation   = true;
  SAT_Color MModulationColor  = SAT_Color(1,1,1,0.25);

  bool      MDrawQuantized    = false;
  SAT_Color MQuantizedColor   = SAT_Color(0,0,0,0.5);
  double    MQuantizedWidth   = 1.0;

  bool      MDrawBipolar      = false;
  double    MBipolarCenter    = 0.5;

  uint32_t  MDrawDirection    = SAT_DIRECTION_RIGHT;


//------------------------------
public:
//------------------------------

  SAT_SliderWidget(SAT_Rect ARect)
  : SAT_DragValueWidget(ARect) {
    setName("SAT_SliderWidget");
    setHint("SAT_SliderWidget");
  }

  //----------

  virtual ~SAT_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual bool      isBipolar()         { return MDrawBipolar; }
  virtual double    getBipolarCenter()  { return MBipolarCenter; }
  virtual double    getDrawDirection()  { return MDrawDirection; }

  //

  virtual void setDrawSliderBar(bool ADraw=true)      { MDrawSliderBar = ADraw; }
  virtual void setSliderBarColor(SAT_Color AColor)    { MSliderBarColor = AColor; }
  virtual void setDrawModulation(bool ADraw=true)     { MDrawModulation = ADraw; }
  virtual void setModulationColor(SAT_Color AColor)   { MModulationColor = AColor; }
  virtual void setDrawQuantized(bool ADraw=true)      { MDrawQuantized = ADraw; }
  virtual void setQuantizedColor(SAT_Color AColor)    { MQuantizedColor = AColor; }
  virtual void setQuantizedWidth(double AWidth)       { MQuantizedWidth = AWidth; }

  virtual void setDrawBipolar(bool AState=true)       { MDrawBipolar = AState; }
  virtual void setBipolarCenter(double AValue)        { MBipolarCenter = AValue; }

  virtual void setDrawDirection(uint32_t ADirection)  { MDrawDirection = ADirection; }

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
      double vh = (v * h);

      if (MDrawBipolar) {
      }

      painter->setFillColor(MSliderBarColor);

      if (v > 0) {
        switch (MDrawDirection) {
          case SAT_DIRECTION_LEFT:
            painter->fillRect((x+w-vw),y,vw,h);
            break;
          case SAT_DIRECTION_RIGHT:
            painter->fillRect(x,y,vw,h);
            break;
          case SAT_DIRECTION_UP:
            painter->fillRect(x,(y+h-vh),w,vh);
            break;
          case SAT_DIRECTION_DOWN:
            painter->fillRect(x,y,w,vh);
            break;
        }

      }

      if (MDrawModulation && (getParameter() != nullptr)) {
        double mx = 0.0;
        double mw = 0.0;
        double my = 0.0;
        double mh = 0.0;

        painter->setFillColor(MModulationColor);

        switch (MDrawDirection) {
          case SAT_DIRECTION_LEFT:
            if (m > v) {
              mx = x + (v * w);
              mw = w * (m - v);
            }
            else {
              mx = x + (m * w);
              mw = w * (v - m);
            }    
            painter->fillRect(mx,y,mw,h);
            break;
          case SAT_DIRECTION_RIGHT:
            break;
          case SAT_DIRECTION_UP:
            break;
          case SAT_DIRECTION_DOWN:
            break;
        }

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
    drawIndicators(AContext);
    drawBorder(AContext);
  }

  // void on_widget_timer(double ADelta) {
  //   SAT_PRINT("delta %f\n",ADelta);
  // }

};

//---------------------------------------------------------------------
#endif

