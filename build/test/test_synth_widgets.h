#ifndef test1_synth_widgets_included
#define test1_synth_widgets_included
//----------------------------------------------------------------------

class test1_synth_widget
: public SAT_MovableWidget {

//------------------------------
private:
//------------------------------

  //SAT_Window* MWindow = nullptr;
  
  double x1 = 0.1;
  double y1 = 0.1;
  double x2 = 0.5;
  double y2 = 0.5;
  double x3 = 0.5;
  double y3 = 0.5;
  double x4 = 1.1;
  double y4 = 1.1;

  double x1_add =  0.077;
  double y1_add =  0.033;
  double x2_add = -0.107;
  double y2_add = -0.083;
  double x3_add =  0.091;
  double y3_add =  0.057;
  double x4_add = -0.051;
  double y4_add = -0.073;
  
  double delta_delta = 0;
  
//------------------------------
public:
//------------------------------

  test1_synth_widget(SAT_Rect ARect)
  : SAT_MovableWidget(ARect) {
    SAT_PRINT;
    //setBackgroundColor(SAT_Color(1,1,1,0.004));
    setBackgroundColor(SAT_DarkGrey);
    
    SAT_ButtonWidget* button = new SAT_ButtonWidget(SAT_Rect(10,10,50,15));
    button->setTextSize(8);
    button->setTextColor(SAT_White);
    button->setFillBackground(true);
    button->setBackgroundColor(SAT_DarkYellow);
    button->setDrawBorder(false);
    appendChildWidget(button);
    
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
    
    if (ADelta > 1) ADelta = 0;
    
//    SAT_Print("ADelta = %f\n",ADelta);
    
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
    do_widgetListener_redraw(this,0);
  };

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    
    SAT_Painter* painter = AContext->painter;
    double S = getWindowScale();
    
    SAT_Rect R = getRect();
    R.shrink(S*2.0);
    
    drawDropShadow(AContext);
    fillBackground(AContext);
    
    paintChildWidgets(AContext);
    
    double _x1 = R.x + (x1 * R.w);
    double _y1 = R.y + (y1 * R.h);
    double _x2 = R.x + (x2 * R.w);
    double _y2 = R.y + (y2 * R.h);
    double _x3 = R.x + (x3 * R.w);
    double _y3 = R.y + (y3 * R.h);
    double _x4 = R.x + (x4 * R.w);
    double _y4 = R.y + (y4 * R.h);
    
    painter->setLineWidth(4 * S);
    painter->setDrawColor(SAT_Color(0.2,0.2,0.2,1));
    painter->drawCurveBezier(_x1,_y1,_x4,_y4,_x2,_y2,_x3,_y3);

    painter->setFillColor(SAT_Color(1,1,0,0.5));
    painter->fillCircle(_x1,_y1,3*S);
    painter->fillCircle(_x4,_y4,3*S);

    painter->setFillColor(SAT_Color(0,1,1,0.5));
    painter->fillCircle(_x2,_y2,3*S);
    painter->fillCircle(_x3,_y3,3*S);
    
    painter->setLineWidth(1 * S);
    painter->setDrawColor(SAT_Color(1,1,1,0.2));
    painter->drawLine(_x1,_y1,_x2,_y2);
    painter->drawLine(_x3,_y3,_x4,_y4);
    //painter->setLineWidth(1 * S);
    //painter->setDrawColor(SAT_Color(1,1,1,0.2));
    painter->drawLine(_x2,_y2,_x3,_y3);
    //painter->drawLine(_x4,_y4,_x1,_y1);

    
    drawBorder(AContext);
    
  }

};

//----------------------------------------------------------------------
#endif
