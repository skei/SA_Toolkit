#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "gui/sat_paint_context.h"

enum SAT_EWidgetStates {
  SAT_WIDGET_STATE_NORMAL = 0,
  SAT_WIDGET_STATE_HOVER,
  SAT_WIDGET_STATE_INTERACT,
  SAT_WIDGET_STATE_MODAL,
  SAT_WIDGET_STATE_MOUSE_CAPTURE,
  SAT_WIDGET_STATE_KEY_CAPTURE,
  SAT_WIDGET_STATE_DISABLED,
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

class SAT_Widget {

//------------------------------
private:
//------------------------------

  SAT_Widget*     MParent   = nullptr;
  SAT_WidgetArray MChildren = {};
  uint32_t        MIndex    = 0;
  SAT_DRect       MRect     = {};
  uint32_t        MState    = SAT_WIDGET_STATE_NORMAL;

//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_DRect ARect) {
    MRect = ARect;
  }

  virtual ~SAT_Widget() {
    #ifndef SAT_NO_AUTODELETE
    deleteChildren();
    #endif
  }

//------------------------------
public:
//------------------------------

  void setIndex(uint32_t AIndex) { MIndex = AIndex; }
  void setRect(SAT_DRect ARect) { MRect = ARect; }

  uint32_t  getIndex()  { return MIndex; }
  SAT_DRect getRect()   { return MRect; }

//------------------------------
public:
//------------------------------

  void setPos(double AXpos, double AYpos) {
  }

  //----------

  void setSize(double AWidth, double AHeight) {
  }

//------------------------------
public: // runtime, downwards
//------------------------------

  virtual void on_widget_move(double AXpos, double AYpos) {
  }

  virtual void on_widget_resize(double AWidth, double AHeight) {
  }

  virtual void on_widget_paint(SAT_PaintContext* AContext) {
  }

  virtual void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) {
  }

  virtual void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) {
  }

//------------------------------
public: // runtime, upwards
//------------------------------

  virtual void do_widget_update(SAT_Widget* ASender, uint32_t AMode) {
    if (MParent) MParent->do_widget_update(ASender,AMode);
  }

  virtual void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode) {
    if (MParent) MParent->do_widget_redraw(ASender,AMode);
  }

  // normal, modal, disabled, mouse_capture, mouse_lock, key_capture

  virtual void do_widget_set_state(SAT_Widget* ASender, uint32_t AState, bool AEnable) {
    if (MParent) MParent->do_widget_set_state(ASender,AState,AEnable);
  }

  virtual void do_widget_set_cursor(SAT_Widget* ASender, uint32_t ACursor) {
    if (MParent) MParent->do_widget_set_cursor(ASender,ACursor);
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget) {
    uint32_t index = MChildren.size();
    AWidget->setIndex(index);
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  void deleteChildren() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) delete MChildren[i];
      MChildren[i] = nullptr;
    }
  }

  //----------

  SAT_Widget* findChild(double AXpos, double AYpos) {
    return nullptr; // this;
  }

};

//----------------------------------------------------------------------
#endif

