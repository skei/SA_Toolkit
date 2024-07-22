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

    setBackgroundColor(SAT_Color(0.5, 0.5, 0.6, 0.5));

    setMovable(true);
    //setMovableOffset(SAT_Rect(0,0,0,0));
    //setMovableDirections(SAT_DIRECTION_ALL);

    setSizable(true);
    //setSizableBorder(SAT_Rect(5,5,5,5));
    //setSizableEdges(SAT_EDGE_ALL);
  
    //do_widget_want_timer(this,true);

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

  void on_widget_open(SAT_WidgetOwner* AOwner) final {
    do_widget_want_timer(this,true);
  }

  //----------

  void on_widget_close(SAT_WidgetOwner* AOwner) final {
    do_widget_want_timer(this,false);
  }

  //----------

  // can we call do_widget_redraw here?
  // [TIMER THREAD]

  void on_widget_timer(double ADelta) final {
    //SAT_PRINT("delta %f\n",ADelta);
    updateCoords(ADelta);

    // ugh.. this crashes!
    do_widget_redraw(this,0,SAT_WIDGET_REDRAW_TIMER);
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) final {
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
    double scale = getWindowScale();
    double x1  = rect.x + (rect.w * MCoords[0].x);
    double y1  = rect.y + (rect.h * MCoords[0].y);
    double x2  = rect.x + (rect.w * MCoords[1].x);
    double y2  = rect.y + (rect.h * MCoords[1].y);
    double c1x = rect.x + (rect.w * MCoords[2].x);
    double c1y = rect.y + (rect.h * MCoords[2].y);
    double c2x = rect.x + (rect.w * MCoords[3].x);
    double c2y = rect.y + (rect.h * MCoords[3].y);
    painter->setDrawColor(SAT_Black);
    painter->setLineWidth(5.0 * scale);
    painter->drawCurveBezier(x1,y1,x2,y2,c1x,c1y,c2x,c2y);
    painter->setDrawColor(SAT_Black);
    painter->setLineWidth(0.5 * scale);
    painter->drawLine(x1,y1,c1x,c1y);
    painter->drawLine(c1x,c1y,c2x,c2y);
    painter->drawLine(c2x,c2y,x2,y2);
    //painter->drawLine(x1,y1,x2,y2,c1x,c1y,c2x,c2y);

  }

};

//----------------------------------------------------------------------
#endif
