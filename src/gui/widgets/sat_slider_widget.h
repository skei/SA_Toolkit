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
  SAT_Rect  MSliderBarOffset    = {0};

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

  virtual void drawSliderBar(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawSliderBar) {
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      mrect.shrink(MSliderBarOffset);

      double value = getValue();
      mrect.w *= value;

      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      painter->setFillColor(MSliderBarColor);
      painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
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




