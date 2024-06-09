#ifndef sat_base_widget_included
#define sat_base_widget_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------

#include "sat.h"
#include "gui/painter/sat_paint_context.h"
#include "gui/widget/sat_widget_owner.h"

class SAT_Widget;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseWidget {

//------------------------------
public:
//------------------------------

  SAT_BaseWidget() {}
  virtual ~SAT_BaseWidget() {}

//------------------------------
public:
//------------------------------

  // virtual SAT_Widget* appendChild(SAT_Widget* AWidget) { return nullptr; }
  // virtual void        removeChild(SAT_Widget* AWidget) {}
  // virtual void        deleteChildren() {}
  // virtual uint32_t    getNumChildren() { return 0; }
  // virtual SAT_Widget* getChild(uint32_t AIndex) { return nullptr; }

  // virtual void        ownerWindowOpened(SAT_WidgetOwner* AOwner) {}
  // virtual void        closeOwnerWindow(SAT_WidgetOwner* AOwner) {}

  // virtual double      getWindowWidth() { return 0.0; }
  // virtual double      getWindowHeight() { return 0.0; }
  // virtual double      getWindowScale() { return 1.0; }

  // virtual SAT_Widget* findWidget(int32_t AXpos, int32_t AYpos) { return nullptr; }
  // virtual void        paintChildren(SAT_PaintContext* AContext/*, bool ARecursive=true*/) {}
  // virtual void        realignChildren(bool ARecursive=true) {}

//------------------------------
public:
//------------------------------

  virtual void on_widget_open(SAT_WidgetOwner* AOwner) {}
  virtual void on_widget_close(SAT_WidgetOwner* AOwner) {}
  virtual void on_widget_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_widget_resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void on_widget_preAlign() {}
  virtual void on_widget_postAlign() {}
  virtual void on_widget_realign() {}
  virtual void on_widget_paint(SAT_PaintContext* AContext) {}
  virtual void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_leave(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) {}
  virtual void on_widget_timer(double ADelta) {}

//------------------------------
public:
//------------------------------

  virtual void do_widget_update(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void do_widget_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) {}
  virtual void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) {}
  virtual void do_widget_notify(SAT_Widget* AWidget, int32_t AValue) {}
  virtual void do_widget_mouse_capture(SAT_Widget* AWidget, bool ACapture=true) {}
  virtual void do_widget_key_capture(SAT_Widget* AWidget, bool ACapture=true) {}
  virtual void do_widget_modal(SAT_Widget* AWidget, bool AModal=true) {}
  virtual void do_widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void do_widget_set_hint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void do_widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) {}
  //virtual void do_widget_start_tween(SAT_Widget* AWidget, ...) {}

};

//----------------------------------------------------------------------
#endif


  