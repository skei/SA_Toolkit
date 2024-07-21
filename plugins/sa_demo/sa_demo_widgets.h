#ifndef sa_demo_widgets_included
#define sa_demo_widgets_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// root
//
//----------------------------------------------------------------------

class sa_demo_root_widget
: public SAT_RootWidget {

//------------------------------
public:
//------------------------------

  sa_demo_root_widget(SAT_WidgetListener* AListener/*, SAT_Rect ARect*/)
  : SAT_RootWidget(AListener/*,ARect*/) {
    setName("sa_demo_root_widget");
    setHint("sa_demo_root_widget");
  }

  //----------

  virtual ~sa_demo_root_widget() {
  }

//------------------------------
public:
//------------------------------

  // void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
  //   SAT_TRACE;
  //   SAT_RootWidget::do_widget_update(AWidget,AIndex,AMode);
  // }

};

//----------------------------------------------------------------------
//
// menu (button_grid)
//
//----------------------------------------------------------------------

class sa_demo_button_grid_widget
: public SAT_ButtonGridWidget {

//------------------------------
private:
//------------------------------

  SAT_PagesWidget* MPages = nullptr;

//------------------------------
public:
//------------------------------

  sa_demo_button_grid_widget(SAT_Rect ARect, uint32_t AColumns, uint32_t ARows, const char** ATexts=nullptr, SAT_PagesWidget* APages=nullptr)
  : SAT_ButtonGridWidget(ARect,AColumns,ARows,ATexts) {
    MPages = APages;
    setName("sa_demo_button_grid_widget");
    setHint("sa_demo_button_grid_widget");

  }

  //----------

  virtual ~sa_demo_button_grid_widget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setPages(SAT_PagesWidget* APages) {
    MPages = APages;
  }

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) override {
    SAT_ButtonGridWidget::on_clickCell(AX,AY,AB,AS);
    if (MPages) {
      MPages->selectPage(AY);
      MPages->realignChildren();
      MPages->do_widget_redraw(MPages);

    }
  }


};

//----------------------------------------------------------------------
//
// animated
//
//----------------------------------------------------------------------

class sa_demo_animated_widget
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

  sa_demo_animated_widget(SAT_Rect ARect)
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

    MSpeeds[0] = { 0.10, 0.10 };
    MSpeeds[1] = { 0.05, 0.17 };
    MSpeeds[2] = { 0.20, 0.20 };
    MSpeeds[3] = { 0.15, 0.15 };


  }

  //----------

  virtual ~sa_demo_animated_widget() {
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

  void on_widget_timer(double ADelta) final {
    updateCoords(ADelta);
    do_widget_redraw(this);
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) final {
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

