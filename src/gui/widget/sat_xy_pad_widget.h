#ifndef sat_xy_pad_widget_included
#define sat_xy_pad_widget_included
//----------------------------------------------------------------------

#include "plugin/sat_parameter.h"
#include "gui/widget/sat_visual_widget.h"

//----------

class SAT_XYPadWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  SAT_Color MLineColor  = SAT_Black;
  double    MLineWidth  = 1.5;
  bool      MDragging   = false;

//------------------------------
public:
//------------------------------

  SAT_XYPadWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_XYPadWidget");
    setHint("SAT_XYPadWidget");
    setCursor(SAT_CURSOR_FINGER);
    setFillBackground(true);
    setBackgroundColor(0.45);
    setValue( 0.5, 0 );
    setValue( 0.5, 1 );
  }

  //----------

  virtual ~SAT_XYPadWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void drawXYPad(SAT_PaintContext* AContext) {
    //if (MDrawXYPad) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();

      painter->pushOverlappingClip(rect);

      double scale = getWindowScale();
      double x = rect.x;
      double y = rect.y;
      double w = rect.w;
      double h = rect.h;
      double v0 = getValue(0);
      double v1 = getValue(1);
      SAT_Parameter* param0 = (SAT_Parameter*)getParameter(0);
      if (param0) v0 = param0->getNormalizedValue();
      SAT_Parameter* param1 = (SAT_Parameter*)getParameter(1);
      if (param1) v1 = param1->getNormalizedValue();
      x = rect.x + (rect.w * v0);
      y = rect.y + (rect.h * v1);
      painter->setDrawColor(MLineColor);
      painter->setLineWidth(MLineWidth * scale);
      painter->drawLine(x,rect.y,x,rect.y2());
      painter->drawLine(rect.x,y,rect.x2(),y);

      double circle = 6.0 * scale;
      painter->drawCircle(x,y,circle);

      painter->popClip();

    //}
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawXYPad(AContext);
    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    MDragging = true;
    SAT_Rect rect = getRect();
    double x = (AXpos - rect.x) / rect.w;
    double y = (AYpos - rect.y) / rect.h;
    x = SAT_Clamp(x,0,1);
    y = SAT_Clamp(y,0,1);
    setValue(x,0);
    setValue(y,1);
    do_widget_update(this,0);
    do_widget_update(this,1);
    do_widget_redraw(this);
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    MDragging = false;
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (MDragging) {
      SAT_Rect rect = getRect();
      double x = (AXpos - rect.x) / rect.w;
      double y = (AYpos - rect.y) / rect.h;
      x = SAT_Clamp(x,0,1);
      y = SAT_Clamp(y,0,1);
      setValue(x,0);
      setValue(y,1);
      do_widget_update(this,0);
      do_widget_update(this,1);
      do_widget_redraw(this);
    }
  }

};

//----------------------------------------------------------------------
#endif