#ifndef sat_slider_widget_included
#define sat_slider_widget_included
//---------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_drag_value_widget.h"

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

  bool      MDrawSliderBar  = true;
  SAT_Color MSliderBarColor = SAT_LightOrange;

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

  virtual void drawSliderBar(SAT_PaintContext* AContext) {
    if (MDrawSliderBar) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      double x = rect.x;
      double y = rect.y;
      double w = getValue() * rect.w;
      double h = rect.h;
      //double range = getMaxRange() - getMinValue();
      //if (range > 0) w /= range;
      painter->setFillColor(MSliderBarColor);
      painter->fillRect(x,y,w,h);
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

