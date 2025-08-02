#ifndef sat_anim_test_widget_included
#define sat_anim_test_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_AnimTestWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  uint32_t  MNumCoords  = 4;
  SAT_Point MCoords[4]  = {};
  SAT_Point MSpeeds[4]  = {};

  double    MSplineWidth = 5.0;
  SAT_Color MSplineColor  = SAT_Black;

  SAT_Color MCircleColor  = SAT_LightGrey;
  double    MCircleSize = 10.0;
  double    MCircleLineWidth = 2.0;

  SAT_Color MCircle2Color  = SAT_LightYellow;
  double    MCircle2Size = 4.0;
  double    MCircle2LineWidth = 1.0;

  double    MLineWidth    = 1.0;
  SAT_Color MLineColor    = SAT_LightGrey;

  double    MLine2Width   = 0.5;
  SAT_Color MLine2Color   = SAT_DarkGrey;

//------------------------------
public:
//------------------------------

  SAT_AnimTestWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("sa_demo_animated_widget");
    setHint("sa_demo_animated_widget");

    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Options.wantHoverEvents = true;
    //Options.autoClip = true;

    Options.opaque = false;

    setBackgroundColor(SAT_Color(0.5, 0.6, 0.5, 0.4)); //0.05));

    setMovable(true);
    //setMovableOffset(SAT_Rect(0,0,0,0));
    //setMovableDirections(SAT_DIRECTION_ALL);

    setSizable(true);
    //setSizableBorder(SAT_Rect(5,5,5,5));
    //setSizableEdges(SAT_EDGE_ALL);
  
    //do_Widget_want_timer(this,true);

    MCoords[0] = { 0.0, 0.0 };
    MCoords[1] = { 0.0, 1.0 };
    MCoords[2] = { 0.5, 0.2 };
    MCoords[3] = { 0.5, 0.8 };

    MSpeeds[0] = { 0.11, 0.12 };
    MSpeeds[1] = { 0.05, 0.17 };
    MSpeeds[2] = { 0.23, 0.26 };
    MSpeeds[3] = { 0.15, 0.14 };


  }

  //----------

  virtual ~SAT_AnimTestWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_Widget_open(SAT_WidgetOwner* AOwner) final {
    do_Widget_want_timer(this,true);
  }

  //----------

  void on_Widget_close(SAT_WidgetOwner* AOwner) final {
    do_Widget_want_timer(this,false);
  }

  //----------

  // don't touch anything painting related here!
  // set signals, etc..

  // [TIMER THREAD]

  void on_Widget_timer(double ADelta) final {
    //SAT_PRINT("delta %f\n",ADelta);
    updateCoords(ADelta);
    do_Widget_redraw(this,0,SAT_WIDGET_REDRAW_SELF);
  }

  //----------

  void on_Widget_paint(SAT_PaintContext* AContext) final {
    //SAT_TRACE;
    //drawDropShadow(AContext);
    fillBackground(AContext);
    //paintChildren(AContext);
    drawSpline(AContext);
    //drawIndicators(AContext);
    drawBorder(AContext);    
  }


//------------------------------
public:
//------------------------------

  void updateCoords(double ADelta) {
    SAT_Rect rect = getRect();
    for (uint32_t i=0; i<MNumCoords; i++) {
      MCoords[i].x += (MSpeeds[i].x * ADelta);
      MCoords[i].y += (MSpeeds[i].y * ADelta);
      if (MCoords[i].x <= 0.0 ) { MCoords[i].x = 0.0; MSpeeds[i].x *= -1.0; }
      if (MCoords[i].x >= 1.0 ) { MCoords[i].x = 1.0; MSpeeds[i].x *= -1.0; }
      if (MCoords[i].y <= 0.0 ) { MCoords[i].y = 0.0; MSpeeds[i].y *= -1.0; }
      if (MCoords[i].y >= 1.0 ) { MCoords[i].y = 1.0; MSpeeds[i].y *= -1.0; }
    }
  }

  //----------

  void drawSpline(SAT_PaintContext* AContext) {
    SAT_Painter* painter = AContext->painter;
    SAT_Rect rect = getRect();

    painter->pushOverlappingClip(rect);

    double scale = getWindowScale();
    double x1  = rect.x + (rect.w * MCoords[0].x);
    double y1  = rect.y + (rect.h * MCoords[0].y);
    double x2  = rect.x + (rect.w * MCoords[1].x);
    double y2  = rect.y + (rect.h * MCoords[1].y);
    double c1x = rect.x + (rect.w * MCoords[2].x);
    double c1y = rect.y + (rect.h * MCoords[2].y);
    double c2x = rect.x + (rect.w * MCoords[3].x);
    double c2y = rect.y + (rect.h * MCoords[3].y);

    // lines

    painter->setDrawColor(MLineColor);
    painter->setLineWidth(MLineWidth * scale);
    painter->drawLine(x1,y1,c1x,c1y);
    painter->drawLine(c2x,c2y,x2,y2);

    painter->setDrawColor(MLine2Color);
    painter->setLineWidth(MLine2Width * scale);
    painter->drawLine(c1x,c1y,c2x,c2y);

    //painter->drawLine(x1,y1,x2,y2,c1x,c1y,c2x,c2y);

    // circles

    painter->setDrawColor(MCircleColor);
    painter->setLineWidth(MCircleLineWidth * scale);
    painter->drawCircle(x1,y1,MCircleSize * scale);
    painter->drawCircle(x2,y2,MCircleSize * scale);

    painter->setDrawColor(MCircle2Color);
    painter->setLineWidth(MCircle2LineWidth * scale);
    painter->drawCircle(c1x,c1y,MCircle2Size * scale);
    painter->drawCircle(c2x,c2y,MCircle2Size * scale);

    // bezier

    painter->setDrawColor(MSplineColor);
    painter->setLineWidth(MSplineWidth * scale);
    painter->drawCurveBezier(x1,y1,x2,y2,c1x,c1y,c2x,c2y);

    painter->popClip();

  }

};

//----------------------------------------------------------------------
#endif
