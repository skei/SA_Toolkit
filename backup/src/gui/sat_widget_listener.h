#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

// listener = usually SAT_Widget, or SAT_Window

class SAT_Widget;

class SAT_WidgetListener {
public:
  virtual void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0)     {}
  virtual void do_widgetListener_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0)     {}
  virtual void do_widgetListener_realign(SAT_Widget* ASender)                                       {}
  virtual void do_widgetListener_close(SAT_Widget* ASender)                                         {}
  virtual void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex, int32_t ASubIndex=-1)  {}
  virtual void do_widgetListener_set_cursor(SAT_Widget* ASender, int32_t ACursor)                   {}
  virtual void do_widgetListener_set_hint(SAT_Widget* ASender, uint32_t AMode, const char* AHint)   {}
  virtual void do_widgetListener_set_modal(SAT_Widget* ASender)                                     {}
  virtual void do_widgetListener_resized(SAT_Widget* ASender, double ADeltaX, double ADeltaY)       {}
  virtual void do_widgetListener_want_keys(SAT_Widget* ASender)                                     {}

};


//----------------------------------------------------------------------
#endif

