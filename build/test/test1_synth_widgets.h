#ifndef test1_synth_widgets_included
#define test1_synth_widgets_included
//----------------------------------------------------------------------

class test1_synth_widget
: public SAT_MovableWidget {

//------------------------------
private:
//------------------------------

  //SAT_Window* MWindow = nullptr;
  
  double x1 = 0.50;
  double y1 = 0.50;
  double x2 = 0.50;
  double y2 = 0.50;
  double x3 = 0.50;
  double y3 = 0.50;
  double x4 = 0.50;
  double y4 = 0.50;

  double x1_add = -0.012;
  double y1_add = 0.083;
  double x2_add = 0.07;
  double y2_add = -0.025;
  double x3_add = 0.03;
  double y3_add = -0.067;
  double x4_add = -0.054;
  double y4_add = 0.039;

//------------------------------
public:
//------------------------------

  test1_synth_widget(SAT_Rect ARect)
  : SAT_MovableWidget(ARect) {
    SAT_PRINT;
    setBackgroundColor(SAT_Color(0.4,0.4,0.3,0.03));
  }
  
  //----------

  virtual ~test1_synth_widget() {
    SAT_PRINT;
  }
  
  //----------

  virtual void prepare(SAT_WidgetOwner* AOwner) {
    SAT_MovableWidget::prepare(AOwner);
    SAT_Window* window = (SAT_Window*)AOwner;
    window->registerTimerWidget(this);      // TODO: -> SAT_WidgetOwner
  }
  
  //----------

  void on_widget_timer(uint32_t AId, double ADelta) override {
    
    //SAT_Print("ADelta = %f\n",ADelta);
    
    x1 += (x1_add * ADelta);
    y1 += (y1_add * ADelta);
    x2 += (x2_add * ADelta);
    y2 += (y2_add * ADelta);
    x3 += (x3_add * ADelta);
    y3 += (y3_add * ADelta);
    x4 += (x4_add * ADelta);
    y4 += (y4_add * ADelta);
    
    if (x1 >= 1)  { x1 = 1; x1_add = -x1_add; }
    if (y1 >= 1)  { y1 = 1; y1_add = -y1_add; }
    if (x1 <  0)  { x1 = 0; x1_add = -x1_add; }
    if (y1 <  0)  { y1 = 0; y1_add = -y1_add; }
    
    if (x2 >= 1)  { x2 = 1; x2_add = -x2_add; }
    if (y2 >= 1)  { y2 = 1; y2_add = -y2_add; }
    if (x2 <  0)  { x2 = 0; x2_add = -x2_add; }
    if (y2 <  0)  { y2 = 0; y2_add = -y2_add; }
    
    if (x3 >= 1)  { x3 = 1; x3_add = -x3_add; }
    if (y3 >= 1)  { y3 = 1; y3_add = -y3_add; }
    if (x3 <  0)  { x3 = 0; x3_add = -x3_add; }
    if (y3 <  0)  { y3 = 0; y3_add = -y3_add; }
    
    if (x4 >= 1)  { x4 = 1; x4_add = -x4_add; }
    if (y4 >= 1)  { y4 = 1; y4_add = -y4_add; }
    if (x4 <  0)  { x4 = 0; x4_add = -x4_add; }
    if (y4 <  0)  { y4 = 0; y4_add = -y4_add; }
    
    //parentRedraw();
    do_widget_redraw(this,0);
  };

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    
    SAT_Painter* painter = AContext->painter;
    double S = getWindowScale();
    SAT_Rect R = getRect();
    R.shrink(S*2.0);
    
    drawDropShadow(AContext);
    fillBackground(AContext);
    
    //painter->setDrawColor(SAT_Color(0.4,0.4,0.4,0.1));
    painter->setDrawColor(SAT_Color(0,0,0,0.05));

    painter->setLineWidth(2.5 * S);
    
    double _x1 = R.x + (x1 * R.w);
    double _y1 = R.y + (y1 * R.h);
    double _x2 = R.x + (x2 * R.w);
    double _y2 = R.y + (y2 * R.h);
    double _x3 = R.x + (x3 * R.w);
    double _y3 = R.y + (y3 * R.h);
    double _x4 = R.x + (x4 * R.w);
    double _y4 = R.y + (y4 * R.h);
    
    //SAT_Print("%.f,%.f,%.f,%.f\n",R.x,R.y,R.w,R.h);
    painter->drawLine(_x1,_y1,_x2,_y2);
    painter->drawLine(_x2,_y2,_x3,_y3);
    painter->drawLine(_x3,_y3,_x4,_y4);
    painter->drawLine(_x4,_y4,_x1,_y1);
    //painter->drawCurveBezier(_x1,_y1,_x4,_y4,_x2,_y2,_x3,_y3);
    
    drawBorder(AContext);
    
  }

};

//----------------------------------------------------------------------
#endif
