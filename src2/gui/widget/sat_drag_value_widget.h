#ifndef sat_drag_value_widget_included
#define sat_drag_value_widget_included
//---------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_DragValueWidget
: public SAT_ValueWidget {

//------------------------------
private:
//------------------------------

  bool    MIsDragging   = false;
  double  MSensitivity  = (1.0 / 200.0);
  int32_t MClickedX     = 0;
  int32_t MClickedY     = 0;

//------------------------------
public:
//------------------------------

  SAT_DragValueWidget(SAT_Rect ARect)
  : SAT_ValueWidget(ARect) {
    setName("SAT_DragValueWidget");
    setCursor(SAT_CURSOR_ARROW_UP_DOWN);
  }

  //----------

  virtual ~SAT_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = true;
      MClickedX = AXpos;
      MClickedY = AYpos;
      do_widget_set_cursor(this,SAT_CURSOR_LOCK);
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MIsDragging = false;
      do_widget_set_cursor(this,SAT_CURSOR_UNLOCK);
    }
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    if (MIsDragging) {

      double deltax = AXpos - MClickedX;
      double deltay = AYpos - MClickedY;
      double delta  = (deltay * MSensitivity);

      double value = 0.0;

      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) {
        value = param->getNormalizedValue();
      }
      else {
        value = getValue();
      }

      // double value = 0.0;
      // SAT_Parameter* param = (SAT_Parameter*)getParameter();
      // if (param) value = param->getNormalizedValue();
      // else value = getValue();

      double new_value = value - delta;
      new_value = SAT_Clamp(new_value,0,1);
      
      if (new_value != value) {
        setValue(new_value);
        do_widget_update(this,SAT_WIDGET_UPDATE_VALUE);
        do_widget_redraw(this,SAT_WIDGET_REDRAW_PARAM);
      }
    }
  }

};

//---------------------------------------------------------------------
#endif

