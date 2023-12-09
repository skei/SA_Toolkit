#ifndef sat_base_widget_included
#define sat_base_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseWidget {

//------------------------------
public:
//------------------------------

  SAT_BaseWidget(SAT_Rect ARect) {}
  virtual ~SAT_BaseWidget() {}

//------------------------------
public: // parent -> child
//------------------------------

  virtual void on_widget_prepare() {}
  virtual void on_widget_cleanup() {}
  virtual void on_widget_move(double AXpos, double AYpos) {}
  virtual void on_widget_resize(double AWidth, double AHeight) {}
  virtual void on_widget_prepaint(SAT_PaintContext* AContext) {}
  virtual void on_widget_paint(SAT_PaintContext* AContext) {}
  virtual void on_widget_postpaint(SAT_PaintContext* AContext) {}
  virtual void on_widget_timer(uint32_t AId, double ADelta) {}
  virtual void on_widget_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATimestamp) {}
  virtual void on_widget_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATimestamp) {}
  virtual void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {}
  virtual void on_widget_mouse_dblclick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {}
  virtual void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {}
  virtual void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATimestamp) {}
  virtual void on_widget_mouse_enter(SAT_BaseWidget* AFrom, double AXpos, double AYpos, uint32_t ATimestamp) {}
  virtual void on_widget_mouse_leave(SAT_BaseWidget* ATo, double AXpos, double AYpos, uint32_t ATimestamp) {}
  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AValue) {}

//------------------------------
public: // child -> parent // -> widgetListener
//------------------------------

//  virtual void do_widget_update(SAT_BaseWidget* ASender, uint32_t AMode, uint32_t AIndex=0)   {}
//  virtual void do_widget_redraw(SAT_BaseWidget* ASender, uint32_t AMode, uint32_t AIndex=0)   {}
//  virtual void do_widget_realign(SAT_BaseWidget* ASender)                                     {}
//  virtual void do_widget_close(SAT_BaseWidget* ASender)                                       {}
//  virtual void do_widget_select(SAT_BaseWidget* ASender, int32_t AIndex)                      {}
//  virtual void do_widget_set_cursor(SAT_BaseWidget* ASender, int32_t ACursor)                 {}
//  virtual void do_widget_set_hint(SAT_BaseWidget* ASender, uint32_t AMode, const char* AHint) {}
//  virtual void do_widget_set_modal(SAT_BaseWidget* ASender)                                   {}
//  virtual void do_widget_resized(SAT_BaseWidget* ASender, double ADeltaX, double ADeltaY)     {}
//  virtual void do_widget_want_keys(SAT_BaseWidget* ASender)                                   {}

};

//----------------------------------------------------------------------
#endif
