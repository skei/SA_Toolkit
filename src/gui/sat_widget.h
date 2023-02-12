#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "gui/sat_paint_context.h"
#include "gui/sat_paint_style.h"
#include "gui/sat_widget_listener.h"

  // normal, modal, disabled, mouse_capture, mouse_lock, key_capture


// do_widget_set_state

enum SAT_EWidgetStates {
  SAT_WIDGET_STATE_NORMAL = 0,
  SAT_WIDGET_STATE_MODAL,
};

// do_widget_update

enum SAT_EWidgetUpdateModes {
  SAT_WIDGET_UPDATE_VALUE = 0
};

// do_widget_redraw

enum SAT_EWidgetRedrawModes {
  SAT_WIDGET_REDRAW_FULL = 0,
  SAT_WIDGET_REDRAW_VALUE,
  SAT_WIDGET_REDRAW_MOD,
  SAT_WIDGET_REDRAW_HOVER,
  SAT_WIDGET_REDRAW_INTERACT
};

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget
: public SAT_WidgetListener {

//------------------------------
private:
//------------------------------

  SAT_WidgetListener* MListener   = nullptr;
  SAT_WidgetArray     MChildren   = {};
  uint32_t            MIndex      = 0;
  SAT_Rect            MRect       = {};
  SAT_PaintStyle*     MPaintStyle = nullptr;//SAT_DefaultPaintStyle;

  //uint32_t            MDirtyFlags = 0; // all, value,modulation, ..

//------------------------------
protected:
//------------------------------

  const char*         MHint   = "";
  uint32_t            MCursor = 0;


//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect) {
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

  void        setListener(SAT_WidgetListener* AListener)  { MListener = AListener; }
  void        setIndex(uint32_t AIndex)                   { MIndex = AIndex; }
  void        setRect(SAT_Rect ARect)                     { MRect = ARect; }

//void        setDirtyFlags(uint32_t AFlags)              { MDirtyFlags = AFlags; }
//void        setDirtyFlag(uint32_t AFlag)                { MDirtyFlags |= AFlag; }
//void        clearDirtyFlag(uint32_t AFlag)              { MDirtyFlags &= ~AFlag; }
//bool        hasDirtyFlag(uint32_t AFlag)                { return (MDirtyFlags & AFlag); }

  uint32_t    getIndex()                                  { return MIndex; }
  SAT_Rect    getRect()                                   { return MRect; }

//------------------------------
public:
//------------------------------

  // called from SAT_Editor.show()

  virtual void prepare(SAT_WidgetListener* AWindow, bool ARecursive=true) {
    uint32_t num = MChildren.size();
    if (ARecursive) {
      for (uint32_t i=0; i<num; i++) {
        MChildren[i]->prepare(AWindow,ARecursive);
      }
    }
  }

  //----------

  virtual void setPaintStyle(SAT_PaintStyle* AStyle, bool ARecursive=true) {
    uint32_t num = MChildren.size();
    if (ARecursive) {
      for (uint32_t i=0; i<num; i++) {
        MChildren[i]->setPaintStyle(AStyle,ARecursive);
      }
    }
  }

  //----------

  virtual void paintChildren(SAT_PaintContext* AContext) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Widget* child = MChildren[i];
      child->on_widget_paint(AContext);
    }
  }

  //----------

  virtual void setPos(double AXpos, double AYpos) {
    MRect.x = AXpos;
    MRect.y = AYpos;
  }

  //----------

  virtual void setSize(double AWidth, double AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
  }

//------------------------------
public: // widget
//------------------------------

  virtual void on_widget_move(double AXpos, double AYpos) {
  }

  virtual void on_widget_resize(double AWidth, double AHeight) {
  }

  virtual void on_widget_paint(SAT_PaintContext* AContext) {
    paintChildren(AContext);
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
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widget_update(ASender,AMode,AIndex);
  }

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widget_redraw(ASender,AMode,AIndex);
  }

  void do_widget_set_state(SAT_Widget* ASender, uint32_t AState) override {
    if (MListener) MListener->do_widget_set_state(ASender,AState);
  }

  void do_widget_set_cursor(SAT_Widget* ASender, uint32_t ACursor) override {
    if (MListener) MListener->do_widget_set_cursor(ASender,ACursor);
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    uint32_t index = MChildren.size();
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
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

