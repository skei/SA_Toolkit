#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

class SAT_WidgetListener {
  
//------------------------------
public:
//------------------------------

  virtual void on_widgetListener_mouseClick() {}
  virtual void on_widgetListener_mouseRelease() {}
  virtual void on_widgetListener_mouseMove() {}
  virtual void on_widgetListener_keyPress() {}
  virtual void on_widgetListener_keyRelease() {}
  virtual void on_widgetListener_timer() {}
  virtual void on_widgetListener_paint() {}

};

//----------------------------------------------------------------------
#endif