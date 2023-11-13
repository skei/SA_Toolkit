#ifndef sat_widget_owner_included
#define sat_widget_owner_included
//----------------------------------------------------------------------

// (aka SAT_Window)

class SAT_Widget;

class SAT_WidgetOwner {
public:
  virtual double        do_widgetOwner_get_width()      { return 0; }
  virtual double        do_widgetOwner_get_height()     { return 0; } 
  virtual double        do_widgetOwner_get_scale()      { return 1.0; }
  virtual SAT_Painter*  do_widgetOwner_get_painter()    { return 0; } 

//virtual void          do_widgetOwner_register_timer() { return 1.0; }

};


//----------------------------------------------------------------------
#endif

