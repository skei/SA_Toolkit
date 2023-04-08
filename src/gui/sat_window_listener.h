#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------

class SAT_Window;

class SAT_WindowListener {
public:
  virtual void do_window_timer(SAT_Window* ASender, double AElapsed) {}
  //virtual void do_window_parameter(SAT_Window* ASender, uint32_t AIndex, sat_param_t AValue) {}
  //virtual void do_window_resize(SAT_Window* ASender, uint32_t AWidth, uint32_t AHeight) {}
  virtual void do_window_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) {}
  virtual void do_window_redraw_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) {}
  virtual void do_window_set_widget_state(SAT_Widget* ASender, uint32_t AState) {}
  virtual void do_window_set_cursor(SAT_Widget* ASender, uint32_t ACursor) {}
  virtual void do_window_set_hint(SAT_Widget* ASender, const char* AHint) {}
};

//----------------------------------------------------------------------
#endif

