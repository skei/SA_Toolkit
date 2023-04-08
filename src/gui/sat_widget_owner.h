#ifndef sat_widget_owner_included
#define sat_widget_owner_included
//----------------------------------------------------------------------

class SAT_Widget;

class SAT_WidgetOwner {
public:
  virtual double        do_widget_get_width()   { return 0; }
  virtual double        do_widget_get_height()  { return 0; } 
  virtual double        do_widget_get_scale()   { return 1.0; }
  virtual SAT_Painter*  do_widget_get_painter() { return 0; } 
};


//----------------------------------------------------------------------
#endif

