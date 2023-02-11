#ifndef sat_window_listener_included
#define sat_window_listener_included
//----------------------------------------------------------------------

class SAT_Window;

class SAT_WindowListener {
public:
  virtual void do_window_listener_timer(SAT_Window* ASender) {}
  //virtual void do_window_listener_parameter(SAT_Window* ASender, uint32_t AIndex, sat_param_t AValue) {}
  //virtual void do_window_listener_resize(SAT_Window* ASender, uint32_t AWidth, uint32_t AHeight) {}
};

//----------------------------------------------------------------------
#endif

