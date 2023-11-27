#ifndef sat_widget_listener_included
#define sat_widget_listener_included
//----------------------------------------------------------------------

// widget -> window

#include "sat.h"
//#include "gui/sat_widget.h"

class SAT_Widget;

//----------------------------------------------------------------------

class SAT_WidgetListener {
public:
  virtual void on_widgetListener_update(SAT_Widget* AWidget, uint32_t AMode=0) {}
  virtual void on_widgetListener_redraw(SAT_Widget* AWidget) {}
  virtual void on_widgetListener_realign(SAT_Widget* AFrAWidgetom) {}
  virtual void on_widgetListener_setCursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void on_widgetListener_setHint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void on_widgetListener_setModal(SAT_Widget* AWidget) {}
  virtual void on_widgetListener_captureKeys(SAT_Widget* ASender) {}
  virtual void on_widgetListener_close(SAT_Widget* ASender) {}
  virtual void on_widgetListener_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1) {}
  //virtual void on_widgetListener_resized(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) {}
};

//----------------------------------------------------------------------
#endif