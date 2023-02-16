#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

class SAT_Widget;

class SAT_WidgetListener {
public:
  virtual void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) {}
  virtual void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) {}
  virtual void do_widget_set_state(SAT_Widget* ASender, uint32_t AState) {}
  virtual void do_widget_set_cursor(SAT_Widget* ASender, uint32_t ACursor) {}
  virtual void do_widget_set_hint(SAT_Widget* ASender, const char* AHint) {}
};

//----------------------------------------------------------------------
#endif

